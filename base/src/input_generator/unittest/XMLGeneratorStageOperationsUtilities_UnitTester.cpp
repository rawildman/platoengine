/*
 * XMLGeneratorStageOperationsUtilities_UnitTester.cpp
 *
 *  Created on: Jul 6, 2022
 */

#include <gtest/gtest.h>

#include "XMLGenerator_UnitTester_Tools.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorStagesOperationsUtilities.hpp"

namespace PlatoTestXMLGenerator
{

TEST(PlatoTestXMLGenerator, StageOperation_get_objective_value_operation_name_SierraTF)
{
    XMLGen::Service tService;
    tService.id("2");
    tService.code("sierra_tf");
    std::string tString = XMLGen::get_objective_value_operation_name(tService);
    ASSERT_TRUE(tString == "Compute Criterion");
}

TEST(PlatoTestXMLGenerator, StageOperation_get_objective_value_operation_output_name_SierraTF)
{
    XMLGen::Service tService;
    tService.id("2");
    tService.code("sierra_tf");
    std::string tString = XMLGen::get_objective_value_operation_output_name(tService);
    ASSERT_TRUE(tString == "Criterion");
}

TEST(PlatoTestXMLGenerator, StageOperation_get_objective_gradient_operation_name_SierraTF)
{
    XMLGen::Service tService;
    tService.id("2");
    tService.code("sierra_tf");
    std::string tString = XMLGen::get_objective_gradient_operation_name(tService);
    ASSERT_TRUE(tString == "Compute Criterion Gradient");
}

TEST(PlatoTestXMLGenerator, StageOperation_append_compute_objective_sensitivity_operation_SierraTF)
{
    pugi::xml_document tDocument;
    XMLGen::append_compute_objective_sensitivity_operation("sierra_tf", "shared_data_name", tDocument);
    tDocument.save_file("xml.txt", " ");

    auto tReadData = XMLGen::read_data_from_file("xml.txt");
    std::string tGoldString = std::string("<?xmlversion=\"1.0\"?><Operation><Name>") +
        std::string("ComputeCriterionGradientwrtCADParameters</Name>") +
        std::string("<PerformerName>sierra_tf</PerformerName><Forvar=\"I\"in=\"Parameters\">") +
        std::string("<Input><ArgumentName>ParameterSensitivity{I}</ArgumentName>") +
        std::string("<SharedDataName>ParameterSensitivity{I}</SharedDataName>") +
        std::string("</Input></For><Output><ArgumentName>CriterionGradientwrtCADParameters") +
        std::string("</ArgumentName><SharedDataName>shared_data_name</SharedDataName>") +
        std::string("</Output></Operation>");

    ASSERT_STREQ(tGoldString.c_str(), tReadData.str().c_str());
    Plato::system("rm xml.txt");
}

}
// namespace PlatoTestXMLGenerator
