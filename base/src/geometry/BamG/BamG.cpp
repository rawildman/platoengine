#include "BamG.hpp"

#include <exodusII.h>

#include <locale>
#include <sstream>
#include <cassert>
#include <exception>
#include <cstring>


namespace BamG
{
    void generate(const MeshSpec & aSpec)
    {
        MeshData tMeshData;

        tMeshData.coordinates = generateCoords(aSpec);

        tMeshData.connectivity = generateConnectivity(aSpec);

        tMeshData.nodeSets = generateNodeSets(aSpec);

        tMeshData.sideSets = generateSideSets(aSpec);

        tMeshData.numNPE = getNumNPE(aSpec);

        writeMesh( tMeshData, aSpec );
    }

    void
    writeMesh(
              MeshData & aMeshData,
        const MeshSpec & aMeshSpec
    )
    {
        // create mesh file
        int CPU_word_size = 8;
        int IO_word_size = 8;
        auto tFileID = ex_create(aMeshSpec.fileName.c_str(), EX_CLOBBER, &CPU_word_size, &IO_word_size);

        // initialize mesh data
        std::string tTitle("BamG mesh");
        int tNumDim = aMeshData.coordinates.size();
        int tNumNodes = aMeshData.coordinates[Dim::X].size();
        int tNumElems = aMeshData.connectivity.size() / aMeshData.numNPE;
        int tNumBlocks = 1;
        int tNumNodeSets = aMeshData.nodeSets.size();
        int tNumSideSets = aMeshData.sideSets.size();
        if( ex_put_init(tFileID, tTitle.c_str(), tNumDim, tNumNodes, tNumElems, tNumBlocks, tNumNodeSets, tNumSideSets) )
        {
            throw std::logic_error("Error initializing exodus mesh");
        }

        // put node set data
        char** tNodeSetNames = new char*[aMeshData.nodeSets.size()];

        int tNodeSetIndex = 0;
        for( const auto& tEntry : aMeshData.nodeSets )
        {
            const auto& tNodes = tEntry.second;

            int tNodeSetID = tNodeSetIndex + 1;
            if( ex_put_set_param(tFileID, EX_NODE_SET, tNodeSetID, tNodes.size(), /*numDists=*/0 ) )
            {
                throw std::logic_error("Error writing node set parameters");
            }
            IArray tNodesCopy(tNodes);
            for( auto& tNode : tNodesCopy ) { tNode += 1; } // exodus numbering is one-based
            if( ex_put_set(tFileID, EX_NODE_SET, tNodeSetID, tNodesCopy.data(), NULL) )
            {
                throw std::logic_error("Error writing node set data");
            }

            const auto& tName  = tEntry.first;
            tNodeSetNames[tNodeSetIndex] = new char[tName.size()+1];
            std::strcpy(tNodeSetNames[tNodeSetIndex], tName.c_str());

            tNodeSetIndex++;
        }
        if( tNumNodeSets )
        {
            if( ex_put_names(tFileID, EX_NODE_SET, tNodeSetNames) )
            {
                throw std::logic_error("Error writing node set names");
            }
        }
        for( int tNodeSetIndex=0; tNodeSetIndex<tNumNodeSets; tNodeSetIndex++ )
        {
            delete [] tNodeSetNames[tNodeSetIndex];
        }
        delete [] tNodeSetNames;

        // put side set data
        char** tSideSetNames = new char*[aMeshData.sideSets.size()];

        int tSideSetIndex = 0;
        for( const auto& tEntry : aMeshData.sideSets )
        {
            const auto& tSideSet = tEntry.second;

            int tSideSetID = tSideSetIndex + 1;
            if( ex_put_set_param(tFileID, EX_SIDE_SET, tSideSetID, tSideSet.elements.size(), /*numDists=*/0 ) )
            {
                throw std::logic_error("Error writing side set parameters");
            }
            IArray tElementsCopy(tSideSet.elements);
            for( auto& tElement : tElementsCopy ) { tElement += 1; } // exodus numbering is one-based
            if( ex_put_set(tFileID, EX_SIDE_SET, tSideSetID, tElementsCopy.data(), tSideSet.faces.data()) )
            {
                throw std::logic_error("Error writing side set data");
            }

            const auto& tName  = tEntry.first;
            tSideSetNames[tSideSetIndex] = new char[tName.size()+1];
            std::strcpy(tSideSetNames[tSideSetIndex], tName.c_str());

            tSideSetIndex++;
        }
        if( tNumSideSets )
        {
            if( ex_put_names(tFileID, EX_SIDE_SET, tSideSetNames) )
            {
                throw std::logic_error("Error writing side set names");
            }
        }
        for( int tSideSetIndex=0; tSideSetIndex<tNumSideSets; tSideSetIndex++ )
        {
            delete [] tSideSetNames[tSideSetIndex];
        }
        delete [] tSideSetNames;


        // put block data
        if( ex_put_block( tFileID, EX_ELEM_BLOCK, /*block_id=*/1, aMeshSpec.meshType.c_str(), tNumElems, aMeshData.numNPE, 0, 0, 0 ) )
        {
            throw std::logic_error("Error writing element block parameters");
        }
        IArray tConnectCopy(aMeshData.connectivity);
        for( auto& tConnect : tConnectCopy ) tConnect += 1; // exodus numbering is one-based
        if( ex_put_conn( tFileID, EX_ELEM_BLOCK, /*block_id=*/1, tConnectCopy.data(), 0, 0) )
        {
            throw std::logic_error("Error writing element block connectivity");
        }
        std::string tBlockName("body");
        char** tBlockNames = new char*[1];
        tBlockNames[0] = new char[tBlockName.size()+1];
        std::strcpy(tBlockNames[0], tBlockName.c_str());
        if( ex_put_names(tFileID, EX_ELEM_BLOCK, tBlockNames) )
        {
            throw std::logic_error("Error writing node set names");
        }
        delete [] tBlockNames[0];
        delete [] tBlockNames;

        // put coordinate data
        Array::value_type *x, *y, *z;
        if( aMeshData.coordinates.size() > 0 ) x = aMeshData.coordinates[Dim::X].data();
        if( aMeshData.coordinates.size() > 1 ) y = aMeshData.coordinates[Dim::Y].data();
        if( aMeshData.coordinates.size() > 2 ) z = aMeshData.coordinates[Dim::Z].data();
        if( ex_put_coord(tFileID, x, y, z) )
        {
            throw std::logic_error("Error writing coordinated data");
        }

        // close
        ex_close(tFileID);
    }

    namespace Hex8
    {
        const Uint cNumDims = 3;
        const Uint cNumNPE  = 8;

        Uint getNumNPE(const MeshSpec & aSpec) { return cNumNPE; }

        int indexMap(int i, int j, int k, int I, int J, int K)
        {
            assert( i >= 0 && i < I );
            assert( j >= 0 && j < J );
            assert( k >= 0 && k < K );
            return K*J*i + K*j + k;
        }

        Array2D
        generateCoords(const MeshSpec & aSpec)
        {
            auto tNumPoints = (aSpec.numX+1)*(aSpec.numY+1)*(aSpec.numZ+1);
            Array2D tCoords(Hex8::cNumDims, Array(tNumPoints));

            Array::size_type tLocalNodeIndex = 0;
            for( decltype(aSpec.numX) tIndexI=0; tIndexI<=aSpec.numX; tIndexI++ )
              for( decltype(aSpec.numY) tIndexJ=0; tIndexJ<=aSpec.numY; tIndexJ++ )
                for( decltype(aSpec.numZ) tIndexK=0; tIndexK<=aSpec.numZ; tIndexK++ )
                {
                    tCoords[BamG::Dim::X][tLocalNodeIndex] = tIndexI*aSpec.dimX / aSpec.numX;
                    tCoords[BamG::Dim::Y][tLocalNodeIndex] = tIndexJ*aSpec.dimY / aSpec.numY;
                    tCoords[BamG::Dim::Z][tLocalNodeIndex] = tIndexK*aSpec.dimZ / aSpec.numZ;
                    tLocalNodeIndex++;
                }
            return tCoords;
        }

        IArray
        generateConnectivity(const MeshSpec & aSpec)
        {
            auto tNumElems = (aSpec.numX)*(aSpec.numY)*(aSpec.numZ);
            IArray tConnect(Hex8::cNumNPE*tNumElems);
            for( decltype(aSpec.numX) tIndexI=0; tIndexI<aSpec.numX; tIndexI++)
              for( decltype(aSpec.numY) tIndexJ=0; tIndexJ<aSpec.numY; tIndexJ++)
                for( decltype(aSpec.numZ) tIndexK=0; tIndexK<aSpec.numZ; tIndexK++)
                {
                    int tElIndex = indexMap(tIndexI, tIndexJ, tIndexK, aSpec.numX, aSpec.numY, aSpec.numZ);
                    tConnect[tElIndex*cNumNPE+0] = indexMap(tIndexI,   tIndexJ,   tIndexK,   aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+1] = indexMap(tIndexI+1, tIndexJ,   tIndexK,   aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+2] = indexMap(tIndexI+1, tIndexJ+1, tIndexK,   aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+3] = indexMap(tIndexI,   tIndexJ+1, tIndexK,   aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+4] = indexMap(tIndexI,   tIndexJ,   tIndexK+1, aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+5] = indexMap(tIndexI+1, tIndexJ,   tIndexK+1, aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+6] = indexMap(tIndexI+1, tIndexJ+1, tIndexK+1, aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                    tConnect[tElIndex*cNumNPE+7] = indexMap(tIndexI,   tIndexJ+1, tIndexK+1, aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);
                }
            return tConnect;
        }

        IArray
        getNodes(const MeshSpec& aSpec, Uint aFromX, Uint aFromY, Uint aFromZ, Uint aToX, Uint aToY, Uint aToZ)
        { 
            IArray tNodes((aToX-aFromX+1)*(aToY-aFromY+1)*(aToZ-aFromZ+1));
            Uint tIndex = 0;
            for( Uint tIndexI=aFromX; tIndexI<=aToX; tIndexI++ )
              for( Uint tIndexJ=aFromY; tIndexJ<=aToY; tIndexJ++ )
                for( Uint tIndexK=aFromZ; tIndexK<=aToZ; tIndexK++ )
                  tNodes[tIndex++] = indexMap(tIndexI, tIndexJ, tIndexK, aSpec.numX+1, aSpec.numY+1, aSpec.numZ+1);

            return tNodes;
        }

        IArrayMap
        generateNodeSets(const MeshSpec & aSpec)
        {
            IArrayMap tNodeSets;

            auto nX = aSpec.numX;
            auto nY = aSpec.numY;
            auto nZ = aSpec.numZ;

            tNodeSets["x-"] = getNodes(aSpec, 0,  0,  0,  0,  nY, nZ);
            tNodeSets["x+"] = getNodes(aSpec, nX, 0,  0,  nX, nY, nZ);
            tNodeSets["y-"] = getNodes(aSpec, 0,  0,  0,  nX, 0,  nZ);
            tNodeSets["y+"] = getNodes(aSpec, 0,  nY, 0,  nX, nY, nZ);
            tNodeSets["z-"] = getNodes(aSpec, 0,  0,  0,  nX, nY, 0 );
            tNodeSets["z+"] = getNodes(aSpec, 0,  0,  nZ, nX, nY, nZ);

            tNodeSets["y-z-"] = getNodes(aSpec, 0, 0,  0,  nX, 0,  0 );
            tNodeSets["y-z+"] = getNodes(aSpec, 0, 0,  nZ, nX, 0,  nZ);
            tNodeSets["y+z-"] = getNodes(aSpec, 0, nY, 0,  nX, nY, 0 );
            tNodeSets["y+z+"] = getNodes(aSpec, 0, nY, nZ, nX, nY, nZ);

            tNodeSets["x-z-"] = getNodes(aSpec, 0,  0, 0,  0,  nY, 0 );
            tNodeSets["x-z+"] = getNodes(aSpec, 0,  0, nZ, 0,  nY, nZ);
            tNodeSets["x+z-"] = getNodes(aSpec, nX, 0, 0,  nX, nY, 0 );
            tNodeSets["x+z+"] = getNodes(aSpec, nX, 0, nZ, nX, nY, nZ);

            tNodeSets["x-y-"] = getNodes(aSpec, 0,  0,  0,  0, 0,  nZ);
            tNodeSets["x-y+"] = getNodes(aSpec, 0,  nY, 0,  0, nY, nZ);
            tNodeSets["x+y-"] = getNodes(aSpec, nX, 0,  0, nX, 0,  nZ);
            tNodeSets["x+y+"] = getNodes(aSpec, nX, nY, 0, nX, nY, nZ);

            tNodeSets["x-y-z-"] = getNodes(aSpec, 0,  0,  0,  0,  0,  0 );
            tNodeSets["x-y-z+"] = getNodes(aSpec, 0,  0,  nZ, 0,  0,  nZ);
            tNodeSets["x-y+z-"] = getNodes(aSpec, 0,  nY,  0, 0,  nY, 0 );
            tNodeSets["x-y+z+"] = getNodes(aSpec, 0,  nY, nZ, 0,  nY, nZ);
            tNodeSets["x+y-z-"] = getNodes(aSpec, nX, 0,  0,  nX, 0,  0 );
            tNodeSets["x+y-z+"] = getNodes(aSpec, nX, 0,  nZ, nX, 0,  nZ);
            tNodeSets["x+y+z-"] = getNodes(aSpec, nX, nY,  0, nX, nY, 0 );
            tNodeSets["x+y+z+"] = getNodes(aSpec, nX, nY, nZ, nX, nY, nZ);

            return tNodeSets;
        }

        SideSet
        getSides(const MeshSpec& aSpec, std::string face, Uint aFromX, Uint aFromY, Uint aFromZ, Uint aNumX, Uint aNumY, Uint aNumZ)
        { 
            SideSet tSideSet;

            std::map<std::string, Uint> faceMap{{"y-",1},{"x+",2},{"y+",3},{"x-",4},{"z-",5},{"z+",6}};

            auto tNumFaces = aNumX*aNumY*aNumZ;

            tSideSet.elements = IArray(tNumFaces);
            tSideSet.faces = IArray(tNumFaces, faceMap[face]);

            Uint tIndex = 0;
            for( Uint tIndexI=aFromX; tIndexI<(aFromX+aNumX); tIndexI++ )
              for( Uint tIndexJ=aFromY; tIndexJ<(aFromY+aNumY); tIndexJ++ )
                for( Uint tIndexK=aFromZ; tIndexK<(aFromZ+aNumZ); tIndexK++ )
                  tSideSet.elements[tIndex++] = indexMap(tIndexI, tIndexJ, tIndexK, aSpec.numX, aSpec.numY, aSpec.numZ);

            return tSideSet;
        }

        SideSetMap
        generateSideSets(const MeshSpec & aSpec)
        {
            SideSetMap tSideSets;

            auto nX = aSpec.numX;
            auto nY = aSpec.numY;
            auto nZ = aSpec.numZ;
            
            tSideSets["x-"] = getSides(aSpec, "x-", 0,    0,    0,    1,  nY, nZ);
            tSideSets["x+"] = getSides(aSpec, "x+", nX-1, 0,    0,    1,  nY, nZ);
            tSideSets["y-"] = getSides(aSpec, "y-", 0,    0,    0,    nX, 1,  nZ);
            tSideSets["y+"] = getSides(aSpec, "y+", 0,    nY-1, 0,    nX, 1,  nZ);
            tSideSets["z-"] = getSides(aSpec, "z-", 0,    0,    0,    nX, nY, 1 );
            tSideSets["z+"] = getSides(aSpec, "z+", 0,    0,    nZ-1, nX, nY, 1 );

            return tSideSets;
        }

    } // end namespace Hex8

    namespace Quad4
    {
        const Uint cNumDims = 2;
        const Uint cNumNPE  = 4;

        Uint getNumNPE(const MeshSpec & aSpec) { return cNumNPE; }

        int indexMap(int i, int j, int I, int J)
        {
            assert( i >= 0 && i < I );
            assert( j >= 0 && j < J );
            return J*i + j;
        }

        Array2D
        generateCoords(const MeshSpec & aSpec)
        {
            auto tNumPoints = (aSpec.numX+1)*(aSpec.numY+1);
            Array2D tCoords(Quad4::cNumDims, Array(tNumPoints));

            Array::size_type tLocalNodeIndex = 0;
            for( decltype(aSpec.numX) tIndexI=0; tIndexI<=aSpec.numX; tIndexI++ )
              for( decltype(aSpec.numY) tIndexJ=0; tIndexJ<=aSpec.numY; tIndexJ++ )
                {
                    tCoords[BamG::Dim::X][tLocalNodeIndex] = tIndexI*aSpec.dimX / aSpec.numX;
                    tCoords[BamG::Dim::Y][tLocalNodeIndex] = tIndexJ*aSpec.dimY / aSpec.numY;
                    tLocalNodeIndex++;
                }
            return tCoords;
        }

        IArray
        generateConnectivity(const MeshSpec & aSpec)
        {
            auto tNumElems = (aSpec.numX)*(aSpec.numY);
            IArray tConnect(Quad4::cNumNPE*tNumElems);
            for( decltype(aSpec.numX) tIndexI=0; tIndexI<aSpec.numX; tIndexI++)
              for( decltype(aSpec.numY) tIndexJ=0; tIndexJ<aSpec.numY; tIndexJ++)
                {
                    int tElIndex = indexMap(tIndexI, tIndexJ, aSpec.numX, aSpec.numY);
                    tConnect[tElIndex*cNumNPE+0] = indexMap(tIndexI,   tIndexJ,   aSpec.numX+1, aSpec.numY+1);
                    tConnect[tElIndex*cNumNPE+1] = indexMap(tIndexI+1, tIndexJ,   aSpec.numX+1, aSpec.numY+1);
                    tConnect[tElIndex*cNumNPE+2] = indexMap(tIndexI+1, tIndexJ+1, aSpec.numX+1, aSpec.numY+1);
                    tConnect[tElIndex*cNumNPE+3] = indexMap(tIndexI,   tIndexJ+1, aSpec.numX+1, aSpec.numY+1);
                }
            return tConnect;
        }

        IArray
        getNodes(const MeshSpec& aSpec, Uint aFromX, Uint aFromY, Uint aToX, Uint aToY)
        { 
            IArray tNodes((aToX-aFromX+1)*(aToY-aFromY+1));
            Uint tIndex = 0;
            for( Uint tIndexI=aFromX; tIndexI<=aToX; tIndexI++ )
              for( Uint tIndexJ=aFromY; tIndexJ<=aToY; tIndexJ++ )
                tNodes[tIndex++] = indexMap(tIndexI, tIndexJ, aSpec.numX+1, aSpec.numY+1);

            return tNodes;
        }

        IArrayMap
        generateNodeSets(const MeshSpec & aSpec)
        {
            IArrayMap tNodeSets;

            auto nX = aSpec.numX;
            auto nY = aSpec.numY;

            tNodeSets["x-"] = getNodes(aSpec, 0,  0,  0,  nY);
            tNodeSets["x+"] = getNodes(aSpec, nX, 0,  nX, nY);
            tNodeSets["y-"] = getNodes(aSpec, 0,  0,  nX, 0 );
            tNodeSets["y+"] = getNodes(aSpec, 0,  nY, nX, nY);

            tNodeSets["x-y-"] = getNodes(aSpec, 0,  0,  0,  0 );
            tNodeSets["x-y+"] = getNodes(aSpec, 0,  nY, 0,  nY);
            tNodeSets["x+y-"] = getNodes(aSpec, nX, 0,  nX, 0 );
            tNodeSets["x+y+"] = getNodes(aSpec, nX, nY, nX, nY);

            return tNodeSets;
        }

        SideSet
        getSides(const MeshSpec& aSpec, std::string face, Uint aFromX, Uint aFromY, Uint aNumX, Uint aNumY)
        { 
            SideSet tSideSet;

            std::map<std::string, Uint> faceMap{{"y-",1},{"x+",2},{"y+",3},{"x-",4}};

            auto tNumFaces = aNumX*aNumY;

            tSideSet.elements = IArray(tNumFaces);
            tSideSet.faces = IArray(tNumFaces, faceMap[face]);

            Uint tIndex = 0;
            for( Uint tIndexI=aFromX; tIndexI<(aFromX+aNumX); tIndexI++ )
              for( Uint tIndexJ=aFromY; tIndexJ<(aFromY+aNumY); tIndexJ++ )
                tSideSet.elements[tIndex++] = indexMap(tIndexI, tIndexJ, aSpec.numX, aSpec.numY);

            return tSideSet;
        }

        SideSetMap
        generateSideSets(const MeshSpec & aSpec)
        {
            SideSetMap tSideSets;

            auto nX = aSpec.numX;
            auto nY = aSpec.numY;
            
            tSideSets["x-"] = getSides(aSpec, "x-", 0,    0,    1,  nY);
            tSideSets["x+"] = getSides(aSpec, "x+", nX-1, 0,    1,  nY);
            tSideSets["y-"] = getSides(aSpec, "y-", 0,    0,    nX, 1 );
            tSideSets["y+"] = getSides(aSpec, "y+", 0,    nY-1, nX, 1 );

            return tSideSets;
        }

    } // end namespace Quad4

    namespace Bar2
    {
        const Uint cNumDims = 1;
        const Uint cNumNPE  = 2;

        Uint getNumNPE(const MeshSpec & aSpec) { return cNumNPE; }

        int indexMap(int i, int I)
        {
            assert( i >= 0 && i < I );
            return i;
        }

        Array2D
        generateCoords(const MeshSpec & aSpec)
        {
            auto tNumPoints = (aSpec.numX+1);
            Array2D tCoords(Bar2::cNumDims, Array(tNumPoints));

            Array::size_type tLocalNodeIndex = 0;
            for( decltype(aSpec.numX) tIndexI=0; tIndexI<=aSpec.numX; tIndexI++ )
                {
                    tCoords[BamG::Dim::X][tLocalNodeIndex] = tIndexI*aSpec.dimX / aSpec.numX;
                    tLocalNodeIndex++;
                }
            return tCoords;
        }

        IArray
        generateConnectivity(const MeshSpec & aSpec)
        {
            auto tNumElems = (aSpec.numX);
            IArray tConnect(Bar2::cNumNPE*tNumElems);
            for( decltype(aSpec.numX) tIndexI=0; tIndexI<aSpec.numX; tIndexI++)
            {
                int tElIndex = indexMap(tIndexI, aSpec.numX);
                tConnect[tElIndex*cNumNPE+0] = indexMap(tIndexI,   aSpec.numX+1);
                tConnect[tElIndex*cNumNPE+1] = indexMap(tIndexI+1, aSpec.numX+1);
            }
            return tConnect;
        }

        IArray
        getNodes(const MeshSpec& aSpec, Uint aFromX, Uint aToX)
        { 
            IArray tNodes(aToX-aFromX+1);
            Uint tIndex = 0;
            for( Uint tIndexI=aFromX; tIndexI<=aToX; tIndexI++ )
              tNodes[tIndex++] = indexMap(tIndexI, aSpec.numX+1);

            return tNodes;
        }

        IArrayMap
        generateNodeSets(const MeshSpec & aSpec)
        {
            IArrayMap tNodeSets;

            auto nX = aSpec.numX;

            tNodeSets["x-"] = getNodes(aSpec, 0,  0 );
            tNodeSets["x+"] = getNodes(aSpec, nX, nX);

            return tNodeSets;
        }

        SideSetMap
        generateSideSets(const MeshSpec & aSpec)
        {
            SideSetMap tSideSets;
            return tSideSets;
        }

    } // end namespace Quad4

    namespace Tet4
    {
        const Uint cNumNPE  = 4;
        const Uint cTetPerHex = 6;

        Uint getNumNPE(const MeshSpec & aSpec) { return cNumNPE; }

        Array2D
        generateCoords(const MeshSpec & aSpec)
        {
            return Hex8::generateCoords(aSpec);
        }
        IArray
        generateConnectivity(const MeshSpec & aSpec)
        {
            auto tHex8Conn = Hex8::generateConnectivity(aSpec);

            return Tet4::fromHex8(tHex8Conn);
        }

        IArray
        fromHex8(const IArray & aHex8Conn)
        {
            const IArray2D cH2T = {{0,2,3,6},{0,3,7,6},{0,7,4,6},{0,4,5,6},{0,5,1,6},{0,1,2,6}};

            auto tNumHex8Elems = aHex8Conn.size()/Hex8::cNumNPE;
            auto tNumTet4Elems = tNumHex8Elems*cTetPerHex;

            IArray tTet4Conn(tNumTet4Elems*Tet4::cNumNPE);

            Uint iTotalTets = 0;
            for( decltype(tNumHex8Elems) iHex=0; iHex<tNumHex8Elems; iHex++ )
            {
                for( Uint iLocalTet=0; iLocalTet<cTetPerHex; iLocalTet++ )
                {
                    for( Uint iLocalVert=0; iLocalVert<Tet4::cNumNPE; iLocalVert++ )
                    {
                        tTet4Conn[iTotalTets*Tet4::cNumNPE+iLocalVert] = aHex8Conn[iHex*Hex8::cNumNPE+cH2T[iLocalTet][iLocalVert]];
                    }
                    iTotalTets++;
                }
            }
            return tTet4Conn;
        }
        IArrayMap
        generateNodeSets(const MeshSpec & aSpec)
        {
            return Hex8::generateNodeSets(aSpec);
        }

        SideSetMap
        fromHex8(const SideSetMap & aHex8SideSets)
        {
            SideSetMap tSideSets;

            std::map<Uint, const IArray2D> cH2T =
            {
              {1,{{3,4},{4,4}}},
              {2,{{4,2},{5,2}}},
              {3,{{0,2},{1,2}}},
              {4,{{1,4},{2,4}}},
              {5,{{0,4},{5,4}}},
              {6,{{2,2},{3,2}}}
            };

            for( auto& tEntry : aHex8SideSets )
            {
                SideSet tTet4SideSet;
                auto& tTet4Elems = tTet4SideSet.elements;
                auto& tTet4Faces = tTet4SideSet.faces;

                auto& tName = tEntry.first;
                auto& tSideSet = tEntry.second;

                auto& tHex8Elems = tSideSet.elements;
                auto& tHex8Faces = tSideSet.faces;

                auto tNumSides = tHex8Elems.size();
                for( decltype(tNumSides) iSide=0; iSide<tNumSides; iSide++ )
                {
                    auto tHex8Elem = tHex8Elems[iSide];
                    auto tHex8Face = tHex8Faces[iSide];

                    const auto& tTet4Sides = cH2T[tHex8Face];
                    for( auto tTet4Side : tTet4Sides )
                    {
                        tTet4Elems.push_back(tHex8Elem*cTetPerHex +  tTet4Side[0]);
                        tTet4Faces.push_back(tTet4Side[1]);
                    }
                }
                tSideSets[tName] = tTet4SideSet;
            }

            return tSideSets;
        }

        SideSetMap
        generateSideSets(const MeshSpec & aSpec)
        {
            SideSetMap tSideSets = Hex8::generateSideSets(aSpec);

            return Tet4::fromHex8(tSideSets);
        }

    } // end namespace Tet4

    namespace Tri3
    {
        const Uint cNumNPE  = 3;
        const Uint cTetPerHex = 2;

        Uint getNumNPE(const MeshSpec & aSpec) { return cNumNPE; }

        Array2D
        generateCoords(const MeshSpec & aSpec)
        {
            return Quad4::generateCoords(aSpec);
        }
        IArray
        generateConnectivity(const MeshSpec & aSpec)
        {
            auto tQuad4Conn = Quad4::generateConnectivity(aSpec);

            return Tri3::fromQuad4(tQuad4Conn);
        }

        IArray
        fromQuad4(const IArray & aQuad4Conn)
        {
            const IArray2D cQ2T = {{0,1,2},{0,2,3}};

            auto tNumQuad4Elems = aQuad4Conn.size()/Quad4::cNumNPE;
            auto tNumTri3Elems = tNumQuad4Elems*cTetPerHex;

            IArray tTri3Conn(tNumTri3Elems*Tri3::cNumNPE);

            Uint iTotalTets = 0;
            for( decltype(tNumQuad4Elems) iHex=0; iHex<tNumQuad4Elems; iHex++ )
            {
                for( Uint iLocalTet=0; iLocalTet<cTetPerHex; iLocalTet++ )
                {
                    for( Uint iLocalVert=0; iLocalVert<Tri3::cNumNPE; iLocalVert++ )
                    {
                        tTri3Conn[iTotalTets*Tri3::cNumNPE+iLocalVert] = aQuad4Conn[iHex*Quad4::cNumNPE+cQ2T[iLocalTet][iLocalVert]];
                    }
                    iTotalTets++;
                }
            }
            return tTri3Conn;
        }
        IArrayMap
        generateNodeSets(const MeshSpec & aSpec)
        {
            return Quad4::generateNodeSets(aSpec);
        }

        SideSetMap
        fromQuad4(const SideSetMap & aQuad4SideSets)
        {
            SideSetMap tSideSets;

            std::map<Uint, const IArray2D> cQ2T =
            {
              {1,{{0,1}}},
              {2,{{0,2}}},
              {3,{{1,2}}},
              {4,{{1,3}}}
            };

            for( auto& tEntry : aQuad4SideSets )
            {
                SideSet tTri3SideSet;
                auto& tTri3Elems = tTri3SideSet.elements;
                auto& tTri3Faces = tTri3SideSet.faces;

                auto& tName = tEntry.first;
                auto& tSideSet = tEntry.second;

                auto& tQuad4Elems = tSideSet.elements;
                auto& tQuad4Faces = tSideSet.faces;

                auto tNumSides = tQuad4Elems.size();
                for( decltype(tNumSides) iSide=0; iSide<tNumSides; iSide++ )
                {
                    auto tQuad4Elem = tQuad4Elems[iSide];
                    auto tQuad4Face = tQuad4Faces[iSide];

                    const auto& tTri3Sides = cQ2T[tQuad4Face];
                    for( auto tTri3Side : tTri3Sides )
                    {
                        tTri3Elems.push_back(tQuad4Elem*cTetPerHex +  tTri3Side[0]);
                        tTri3Faces.push_back(tTri3Side[1]);
                    }
                }
                tSideSets[tName] = tTri3SideSet;
            }

            return tSideSets;
        }

        SideSetMap
        generateSideSets(const MeshSpec & aSpec)
        {
            SideSetMap tSideSets = Quad4::generateSideSets(aSpec);

            return Tri3::fromQuad4(tSideSets);
        }

    } // end namespace Tri3

    Uint
    getNumNPE(const MeshSpec & aSpec) { BAMG_COMPUTE(getNumNPE); }

    Array2D
    generateCoords(const MeshSpec & aSpec) { BAMG_COMPUTE(generateCoords); }

    IArray
    generateConnectivity(const MeshSpec & aSpec) { BAMG_COMPUTE(generateConnectivity); }

    IArrayMap
    generateNodeSets(const MeshSpec & aSpec) { BAMG_COMPUTE(generateNodeSets); }

    SideSetMap
    generateSideSets(const MeshSpec & aSpec) { BAMG_COMPUTE(generateSideSets); }

    bool matches(std::string tStrA, std::string tStrB)
    {
        if( tStrA.size() != tStrB.size() ) return false;

        std::locale tLocale;
        std::ostringstream tOutput;
        bool tMatches = true;
        auto tLengthA = tStrA.size();
        for( decltype(tLengthA) iChar=0; iChar<tLengthA; iChar++ )
        {
            tMatches = tMatches && (std::tolower(tStrA[iChar],tLocale) == std::tolower(tStrB[iChar],tLocale));
        }
        return tMatches;
    }
}
