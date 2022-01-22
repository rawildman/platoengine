#include "BamG.hpp"

#include <Teuchos_CommandLineProcessor.hpp>

int main(int argc, char** argv)
{
  const bool tThrowExceptions = false;
  const bool tRecogniseAllOptions = true;
  const bool tAddOutputSetupOptions = false;

  Teuchos::CommandLineProcessor My_CLP(tThrowExceptions, tRecogniseAllOptions, tAddOutputSetupOptions);

  My_CLP.setDocString( "BamG (Basic Mesh Generator) generates box meshes of various element types.\n" );

  std::string       tElementType = "hex8";
  const std::string tElementTypeDoc = "Element type (hex8 or tet4).";
  My_CLP.setOption("element-type", &tElementType, tElementTypeDoc.c_str());

  std::string       tOutputFile = "bamg.exo";
  const std::string tOutputFileDoc = "Name of the output mesh file.";
  My_CLP.setOption("output-file", &tOutputFile, tOutputFileDoc.c_str());

  int               tNumX = 1;
  const std::string tNumXDoc = "Number of element in X direction.";
  My_CLP.setOption("numX", &tNumX, tNumXDoc.c_str());

  int               tNumY = 1;
  const std::string tNumYDoc = "Number of element in Y direction.";
  My_CLP.setOption("numY", &tNumY, tNumYDoc.c_str());

  int               tNumZ = 1;
  const std::string tNumZDoc = "Number of element in Z direction.";
  My_CLP.setOption("numZ", &tNumZ, tNumZDoc.c_str());

  float             tDimX = 1.0;
  const std::string tDimXDoc = "Box width in X.";
  My_CLP.setOption("dimX", &tDimX, tDimXDoc.c_str());

  float             tDimY = 1.0;
  const std::string tDimYDoc = "Box width in Y.";
  My_CLP.setOption("dimY", &tDimY, tDimYDoc.c_str());

  float             tDimZ = 1.0;
  const std::string tDimZDoc = "Box width in Z.";
  My_CLP.setOption("dimZ", &tDimZ, tDimZDoc.c_str());

  Teuchos::CommandLineProcessor::EParseCommandLineReturn parseReturn = My_CLP.parse(argc, argv);

  if (parseReturn != Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL)
  {
      std::cerr << "Command line processing failed." << std::endl;
      exit(-1);
  }
  else
  if (parseReturn == Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL)
  {
      BamG::MeshSpec tMeshSpec;
      tMeshSpec.meshType = tElementType;
      tMeshSpec.fileName = tOutputFile;
      tMeshSpec.numX = tNumX;
      tMeshSpec.numY = tNumY;
      tMeshSpec.numZ = tNumZ;
      tMeshSpec.dimX = tDimX;
      tMeshSpec.dimY = tDimY;
      tMeshSpec.dimZ = tDimZ;

      BamG::generate(tMeshSpec);

      exit(0);
  }
  else
  {
      std::cerr << "Encountered unknown error." << std::endl;
      exit(-1);
  }

  return 0;
}
