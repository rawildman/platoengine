###############################################################################
# Sources:
###############################################################################
SET(PlatoXMLGenerator_UnitTester_SRCS XMLGenerator_UnitMain.cpp
                                      XMLGenerator_UnitTester.cpp
                                      Plato_Test_XMLGenerator.cpp
                                      XMLGeneratorSrom_UnitTester.cpp
                                      XMLGeneratorLaunch_UnitTester.cpp
                                      XMLGeneratorPlatoGemma_UnitTester.cpp
                                      XMLGeneratorDefineFile_UnitTester.cpp
                                      XMLGeneratorDakotaDriver_UnitTester.cpp
                                      XMLGeneratorParseMetaData_UnitTester.cpp
                                      XMLGeneratorInterfaceFile_UnitTester.cpp
                                      XMLGeneratorLaunchScript_UnitTester.cpp
                                      XMLGeneratorRandomInterfaceFile_UnitTester.cpp
                                      XMLGeneratorPlatoMainInputDeckFile_UnitTester.cpp
                                      XMLGeneratorPlatoMainOperationFile_UnitTester.cpp
                                      XMLGeneratorPlatoAnalyzeInputFile_UnitTester.cpp
                                      XMLGeneratorPlatoAnalyzeOperationsFile_UnitTester.cpp
                                      XMLGeneratorSierraSDInputFile_UnitTester.cpp
                                      XMLGeneratorXTKInputDeck_UnitTester.cpp
                                      XMLGeneratorXTKOperationFile_UnitTester.cpp
                                      XMLGeneratorFileObject_UnitTester.cpp
                                      XMLGeneratorPerformer_UnitTester.cpp
                                      XMLGeneratorSharedData_UnitTester.cpp
                                      XMLGeneratorStage_UnitTester.cpp
                                      XMLGeneratorOperation_UnitTester.cpp
                                      ../XMLGenerator.cpp
                                      ../XMLGeneratorProblem.cpp
                                      ../XMLGeneratorParseRun.cpp
                                      ../XMLGeneratorUtilities.cpp
                                      ../XMLGeneratorParseBlock.cpp
                                      ../XMLGeneratorParseLoads.cpp
                                      ../XMLGeneratorParseOutput.cpp
                                      ../XMLGeneratorRunMetadata.cpp
                                      ../XMLGeneratorDakotaProblem.cpp
                                      ../XMLGeneratorParseAssembly.cpp
                                      ../XMLGeneratorParseScenario.cpp
                                      ../XMLGeneratorParseServices.cpp
                                      ../XMLGeneratorParseMaterial.cpp
                                      ../XMLGeneratorParseCriteria.cpp
                                      ../XMLGeneratorOutputMetadata.cpp
                                      ../XMLGeneratorParseObjective.cpp
                                      ../XMLGeneratorOutputUtilities.cpp
                                      ../XMLGeneratorStagesUtilities.cpp
                                      ../XMLGeneratorParserUtilities.cpp
                                      ../XMLGeneratorParseConstraint.cpp
                                      ../XMLGeneratorServiceMetadata.cpp
                                      ../XMLGeneratorProblemUtilities.cpp
                                      ../XMLGeneratorServiceUtilities.cpp
                                      ../XMLGeneratorParseUncertainty.cpp
                                      ../XMLGeneratorScenarioMetadata.cpp
                                      ../XMLGeneratorSierraSDUtilities.cpp
                                      ../XMLGeneratorPlatoXTKInputFile.cpp
                                      ../XMLGeneratorConstraintMetadata.cpp
                                      ../XMLGeneratorSharedDataUtilities.cpp
                                      ../XMLGeneratorPerformersUtilities.cpp
                                      ../XMLGeneratorUncertaintyMetadata.cpp
                                      ../XMLGeneratorDefinesFileUtilities.cpp
                                      ../XMLGeneratorPlatoXTKOperationFile.cpp
                                      ../XMLGeneratorPlatoAnalyzeUtilities.cpp
                                      ../XMLGeneratorLaunchUtilities.cpp
                                      ../XMLGeneratorInterfaceFileUtilities.cpp
                                      ../XMLGeneratorDakotaInterfaceFileUtilities.cpp
                                      ../XMLGeneratorDakotaDriverInputFileUtilities.cpp
                                      ../XMLGeneratorGradBasedOptimizerOptions.cpp
                                      ../XMLGeneratorStagesOperationsUtilities.cpp
                                      ../XMLGeneratorMaterialFunctionInterface.cpp
                                      ../XMLGeneratorPlatoESPInputFileUtilities.cpp
                                      ../XMLGeneratorSierraSDInputDeckUtilities.cpp
                                      ../XMLGeneratorPlatoMainInputFileUtilities.cpp
                                      ../XMLGeneratorParseOptimizationParameters.cpp
                                      ../XMLGeneratorAnalyzeLoadFunctionInterface.cpp
                                      ../XMLGeneratorRandomInterfaceFileUtilities.cpp
                                      ../XMLGeneratorPlatoAnalyzeInputFileUtilities.cpp
                                      ../XMLGeneratorOptimizationParametersMetadata.cpp
                                      ../XMLGeneratorPlatoESPOperationsFileUtilities.cpp
                                      ../XMLGeneratorAnalyzePhysicsFunctionInterface.cpp
                                      ../XMLGeneratorPlatoMainOperationFileUtilities.cpp
                                      ../XMLGeneratorParseEssentialBoundaryCondition.cpp
                                      ../XMLGeneratorSierraSDOperationsFileUtilities.cpp
                                      ../XMLGeneratorAnalyzeLoadTagFunctionInterface.cpp
                                      ../XMLGeneratorParseMethodInputOptionsUtilities.cpp
                                      ../XMLGeneratorPostOptimizationRunFileUtilities.cpp
                                      ../XMLGeneratorAnalyzeAssemblyFunctionInterface.cpp
                                      ../XMLGeneratorPlatoAnalyzeOperationsFileUtilities.cpp
                                      ../XMLGeneratorAnalyzeEssentialBCFunctionInterface.cpp
                                      ../XMLGeneratorAnalyzeMaterialModelFunctionInterface.cpp
                                      ../XMLGeneratorAnalyzeEssentialBCTagFunctionInterface.cpp
                                      ../XMLGeneratorLaunchScriptUtilities.cpp
                                      ../XMLGeneratorPlatoMainConstraintValueOperationInterface.cpp
                                      ../XMLGeneratorPlatoMainConstraintGradientOperationInterface.cpp
                                      # ../PlatoAnalyzeInputDeckWriter.cpp
                                      # ../SalinasInputDeckWriter.cpp)
			              ../XMLGeneratorFileObject.cpp
			              ../XMLGeneratorOperation.cpp
                                      ../XMLGeneratorStage.cpp
			              ../XMLGeneratorPerformer.cpp
			              ../XMLGeneratorSharedData.cpp
			              ../XMLGeneratorGemmaProblem.cpp
                                      )

SET(PlatoXMLGenerator_UnitTester_HDRS XMLGenerator_UnitTester.hpp
                                      XMLGenerator_UnitTester_Tools.hpp)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wno-unused-variable -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable -Wno-write-strings")
endif()

set(PLATOUNIT_INCLUDES ${CMAKE_SOURCE_DIR}/base/src/tools ..)
INCLUDE_DIRECTORIES(${PLATOUNIT_INCLUDES})

set(PlatoXMLGenerator_UnitTester_LIBS GTest::GTest ${PLATO_LIBRARIES} ${Trilinos_LIBRARIES} PlatoParser )
add_executable(PlatoXMLGenerator_UnitTester ${PlatoXMLGenerator_UnitTester_SRCS})
target_link_libraries(PlatoXMLGenerator_UnitTester ${PlatoXMLGenerator_UnitTester_LIBS})
add_test(NAME PlatoXMLGenerator_UnitTester COMMAND ${CMAKE_BINARY_DIR}/base/src/input_generator/unittest/PlatoXMLGenerator_UnitTester)


if( CMAKE_INSTALL_PREFIX )
  install( TARGETS PlatoXMLGenerator_UnitTester DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
endif()
