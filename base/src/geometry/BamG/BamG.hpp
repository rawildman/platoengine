#include <vector>
#include <map>


// add new element types here
//
#define BAMG_COMPUTE(function) {\
          if( matches(aSpec.meshType, "hex8") ) { return Hex8::function(aSpec); } \
          else \
          if( matches(aSpec.meshType, "tet4") ) { return Tet4::function(aSpec); } \
          else \
          if( matches(aSpec.meshType, "quad4") ) { return Quad4::function(aSpec); } \
          else \
          if( matches(aSpec.meshType, "tri3") ) { return Tri3::function(aSpec); } \
          else \
          if( matches(aSpec.meshType, "bar2") ) { return Bar2::function(aSpec); } \
          std::stringstream tError; \
          tError << "Unknown element type '" << aSpec.meshType << "'"; \
          throw std::logic_error(tError.str()); \
        }


namespace BamG
{
    using Uint = unsigned int;
    using Real = double;

    template <typename T>
    using ArrayT = typename std::vector<T>;

    using Array = ArrayT<Real>;
    using IArray = ArrayT<Uint>;

    template <typename T>
    using Array2DT = typename BamG::ArrayT<BamG::ArrayT<T>>;
  
    using Array2D = Array2DT<Real>;
    using IArray2D = Array2DT<Uint>;

    using IArrayMap = std::map<std::string, IArray>;

    enum Dim : Array::size_type { X=0, Y=1, Z=2 };

    struct SideSet
    {
        IArray elements;
        IArray faces;
    };
    using SideSetMap = std::map<std::string, SideSet>;

    struct MeshSpec
    {
        std::string fileName = "BamG_mesh.exo";
        std::string meshType;

        Uint numX = 1;
        Uint numY = 1;
        Uint numZ = 1;
        Real dimX = 1;
        Real dimY = 1;
        Real dimZ = 1;
    };

    struct MeshData
    {
        Array2D coordinates;
        IArray connectivity;
        IArrayMap nodeSets;
        SideSetMap sideSets;
        Uint numNPE = 0;

    };

    void generate(const MeshSpec & aSpec);
    void writeMesh(MeshData & aMeshData, const MeshSpec & aSpec);


    // The five functions below define the concept for an element type.
    // To add another element type, implement these five functions in
    // a namespace, and add the namespace to the BAMG_COMPUTE macro.

    Array2D generateCoords(const MeshSpec & aSpec);

    IArray generateConnectivity(const MeshSpec & aSpec);

    IArrayMap generateNodeSets(const MeshSpec & aSpec);

    SideSetMap generateSideSets(const MeshSpec & aSpec);

    Uint getNumNPE(const MeshSpec & aSpec);

    namespace Hex8
    {
        /******************************************************************************//**
         * \brief Generate Hex8 mesh coordinates
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The coordinates are generated in a 3D structured cartesian grid with Z as the
         * fastest changing index.  The minimum corner is at the origin.
        **********************************************************************************/
        Array2D generateCoords(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Generate Hex8 mesh connectivity
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The element indices are generated in a 3D structured cartesian grid with Z as the
         * fastest changing index.
        **********************************************************************************/
        IArray generateConnectivity(const MeshSpec & aSpec);

        IArrayMap generateNodeSets(const MeshSpec & aSpec);

        SideSetMap generateSideSets(const MeshSpec & aSpec);

        IArray getNodes(const MeshSpec& aSpec,
                        Uint aFromX, Uint aFromY, Uint aFromZ,
                        Uint aToX, Uint aToY, Uint aToZ);

        SideSet getSides(const MeshSpec& aSpec, std::string aFace,
                         Uint aFromX, Uint aFromY, Uint aFromZ,
                         Uint aNumX, Uint aNumY, Uint aNumZ);

        Uint getNumNPE(const MeshSpec & aSpec);

        int indexMap(int i, int j, int k, int I, int J, int K);
    }

    namespace Quad4
    {
        /******************************************************************************//**
         * \brief Generate Quad4 mesh coordinates
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The coordinates are generated in a 2D structured cartesian grid with Y as the
         * fastest changing index.  The minimum corner is at the origin.
        **********************************************************************************/
        Array2D generateCoords(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Generate Quad4 mesh connectivity
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The element indices are generated in a 2D structured cartesian grid with Y as the
         * fastest changing index.
        **********************************************************************************/
        IArray generateConnectivity(const MeshSpec & aSpec);

        IArrayMap generateNodeSets(const MeshSpec & aSpec);

        SideSetMap generateSideSets(const MeshSpec & aSpec);

        IArray getNodes(const MeshSpec& aSpec,
                        Uint aFromX, Uint aFromY,
                        Uint aToX, Uint aToY);

        SideSet getSides(const MeshSpec& aSpec, std::string aFace,
                         Uint aFromX, Uint aFromY,
                         Uint aNumX, Uint aNumY);

        Uint getNumNPE(const MeshSpec & aSpec);

        int indexMap(int i, int j, int I, int J);
    }

    namespace Bar2
    {
        /******************************************************************************//**
         * \brief Generate Bar2 mesh coordinates
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The coordinates are generated in a 1D structured cartesian grid. The minimum
         * corner is at the origin.
        **********************************************************************************/
        Array2D generateCoords(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Generate Bar2 mesh connectivity
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The element indices are generated in a 1D structured cartesian grid.
        **********************************************************************************/
        IArray generateConnectivity(const MeshSpec & aSpec);

        IArrayMap generateNodeSets(const MeshSpec & aSpec);

        SideSetMap generateSideSets(const MeshSpec & aSpec);

        IArray getNodes(const MeshSpec& aSpec, Uint aFromX, Uint aToX);

        Uint getNumNPE(const MeshSpec & aSpec);

        int indexMap(int i, int I);
    }

    namespace Tet4
    {
        /******************************************************************************//**
         * \brief Generate Tet4 coordinates
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The Hex8 implementation is used to compute the Tet4 grid.
        **********************************************************************************/
        Array2D generateCoords(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Generate Tet4 mesh connectivity
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The element indices are computed by first generating a Hex8 mesh then applying
         * a hex-to-tet stencil.
        **********************************************************************************/
        IArray generateConnectivity(const MeshSpec & aSpec);

        IArrayMap generateNodeSets(const MeshSpec & aSpec);

        SideSetMap generateSideSets(const MeshSpec & aSpec);

        Uint getNumNPE(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Create a Tet4 mesh connectivity from a Hex8 mesh connectivity
         * \param [in] aHex8Conn Hex8 connectivity
        **********************************************************************************/
        IArray fromHex8(const IArray& aHex8Conn);

        /******************************************************************************//**
         * \brief Create Tet4 side sets from Hex8 side sets
         * \param [in] aHex8SideSets Hex8 side sets
        **********************************************************************************/
        SideSetMap fromHex8(const SideSetMap& aHex8SideSets);
    }

    namespace Tri3
    {
        /******************************************************************************//**
         * \brief Generate Tri3 coordinates
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The Quad4 implementation is used to compute the Tri3 grid.
        **********************************************************************************/
        Array2D generateCoords(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Generate Tri3 mesh connectivity
         * \param [in] aSpec MeshSpec with mesh definition
         *
         * The element indices are computed by first generating a Quad4 mesh then applying
         * a quad-to-tri stencil.
        **********************************************************************************/
        IArray generateConnectivity(const MeshSpec & aSpec);

        IArrayMap generateNodeSets(const MeshSpec & aSpec);

        SideSetMap generateSideSets(const MeshSpec & aSpec);

        Uint getNumNPE(const MeshSpec & aSpec);

        /******************************************************************************//**
         * \brief Create a Tri3 mesh connectivity from a Quad4 mesh connectivity
         * \param [in] aQuad4Conn Quad4 connectivity
        **********************************************************************************/
        IArray fromQuad4(const IArray& aQuad4Conn);

        /******************************************************************************//**
         * \brief Create Tri3 side sets from Quad4 side sets
         * \param [in] aQuad4SideSets Quad4 side sets
        **********************************************************************************/
        SideSetMap fromQuad4(const SideSetMap& aQuad4SideSets);
    }

    bool matches(std::string tStrA, std::string tStrB);


} // end namespace BamG
