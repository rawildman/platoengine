/*
 * XMLGeneratorParseCriteria.cpp
 *
 *  Created on: Jun 23, 2020
 */

#include <algorithm>

#include "XMLGeneratorCriterionMetadata.hpp"
#include "XMLGeneratorParseCriteria.hpp"
#include "XMLGeneratorValidInputKeys.hpp"

namespace XMLGen
{

void ParseCriteria::setTags(XMLGen::Criterion& aCriterion)
{
    XMLGen::ValidCriterionParameterKeys tValidKeys;
    for(auto& tTag : mTags)
    {
        auto tLowerKey = XMLGen::to_lower(tTag.first);
        auto tItr = std::find(tValidKeys.mKeys.begin(), tValidKeys.mKeys.end(), tLowerKey);
        if(tItr == tValidKeys.mKeys.end())
        {
            THROWERR(std::string("Check Keyword: keyword '") + tTag.first + std::string("' is not supported"))
        }

        if(tTag.second.first.second.empty())
        {
            auto tDefaultValue = tTag.second.second;
            aCriterion.append(tTag.first, tDefaultValue);
        }
        else
        {
            auto tInputValue = tTag.second.first.second;
            aCriterion.append(tTag.first, tInputValue);
        }
    }
}

std::string check_criterion_type_key
(const std::string& aKeyword)
{
    XMLGen::ValidCriterionTypeKeys tValidKeys;
    auto tValue = tValidKeys.value(aKeyword);
    if(tValue.empty())
    {
        auto tMsg = std::string("Input criterion keyword '") + aKeyword + "' is not defined in the list of supported Plato criterion. Supported Plato criterion options are: ";
        const auto& tKeys = tValidKeys.list();
        for(const auto& tKey : tKeys)
        {
            size_t tIndex = &tKey - &tKeys[0];
            auto tDelimiter = (tIndex + 1u) == tKeys.size() ? ".\n" : ", ";
            tMsg = tMsg + tKey + tDelimiter;
        }
        THROWERR(tMsg)
    }
    return tValue;
}

void ParseCriteria::insertTag(std::string keyword, std::string defaultValue)
{
    mTags.insert({keyword, { { {keyword}, ""}, defaultValue } });
}

void ParseCriteria::allocate()
{
    mTags.clear();

    // core properties
    insertTag("type");
    insertTag("normalize", "false");
    insertTag("normalization_value", "1.0");
    insertTag("stress_limit");
    insertTag("scmm_initial_penalty");
    insertTag("scmm_penalty_expansion_multiplier");
    insertTag("scmm_constraint_exponent");
    insertTag("scmm_penalty_upper_bound");
    insertTag("scmm_stress_weight", "1.0");
    insertTag("scmm_mass_weight", "1.0");

    insertTag("stress_p_norm_exponent", "6.0");
    insertTag("mechanical_weighting_factor", "1.0");
    insertTag("thermal_weighting_factor", "1.0");
    insertTag("local_measure", "vonmises");
    insertTag("spatial_weighting_function", "1.0"); // function of x, y, and z
    insertTag("material_penalty_model", "simp");
    insertTag("material_penalty_exponent", "3.0");
    insertTag("minimum_ersatz_material_value");
    insertTag("criterion_ids");
    insertTag("criterion_weights");
    insertTag("location_names");
    insertTag("blocks");
    insertTag("conductivity_ratios");

    /* These are all related to stress-constrained mass minimization problems with Sierra/SD */
    insertTag("volume_misfit_target");
    insertTag("relative_stress_limit");
    insertTag("relaxed_stress_ramp_factor");
    insertTag("limit_power_min");
    insertTag("limit_power_max");
    insertTag("limit_power_feasible_bias");
    insertTag("limit_power_feasible_slope");
    insertTag("limit_power_infeasible_bias");
    insertTag("limit_power_infeasible_slope");
    insertTag("limit_reset_subfrequency");
    insertTag("limit_reset_count");
    insertTag("inequality_allowable_feasibility_upper");
    insertTag("inequality_feasibility_scale");
    insertTag("inequality_infeasibility_scale");
    insertTag("stress_inequality_power");
    insertTag("stress_favor_final");
    insertTag("stress_favor_updates");
    insertTag("volume_penalty_power");
    insertTag("volume_penalty_divisor");
    insertTag("volume_penalty_bias");
    insertTag("surface_area_sideset_id");

    // Sierra/SD modal objectives
    insertTag("num_modes_compute", "30");
    insertTag("modes_to_exclude");
    insertTag("eigen_solver_shift", "-1e6");
    insertTag("camp_solver_tol", "1e-6");
    insertTag("camp_max_iter", "1000");
    insertTag("shape_sideset");
    insertTag("ref_data_file");
    insertTag("match_nodesets");
}

void ParseCriteria::setModesToExclude(XMLGen::Criterion &aMetadata)
{
    auto tItr = mTags.find("modes_to_exclude");
    std::string tValues = tItr->second.first.second;
    if (tItr != mTags.end() && !tValues.empty()) {
        std::vector<std::string> tModes;
        char tValuesBuffer[10000];
        strcpy(tValuesBuffer, tValues.c_str());
        XMLGen::parse_tokens(tValuesBuffer, tModes);
        aMetadata.modesToExclude(tModes);
    }
}

void ParseCriteria::setMatchNodesetIDs(XMLGen::Criterion &aMetadata)
{
    auto tItr = mTags.find("match_nodesets");
    std::string tValues = tItr->second.first.second;
    if (tItr != mTags.end() && !tValues.empty())
    {
        std::vector<std::string> tNodesetIDs;
        char tValuesBuffer[10000];
        strcpy(tValuesBuffer, tValues.c_str());
        XMLGen::parse_tokens(tValuesBuffer, tNodesetIDs);
        aMetadata.setMatchNodesetIDs(tNodesetIDs);
    }
}


void ParseCriteria::setCriterionWeights(XMLGen::Criterion &aMetadata)
{
    if(aMetadata.type() == "composite")
    {
        auto tItr = mTags.find("criterion_weights");
        std::string tValues = tItr->second.first.second;
        if (tItr != mTags.end() && !tValues.empty())
        {
            std::vector<std::string> tWeights;
            char tValuesBuffer[10000];
            strcpy(tValuesBuffer, tValues.c_str());
            XMLGen::parse_tokens(tValuesBuffer, tWeights);
            aMetadata.criterionWeights(tWeights);
        }
        else
        {
            THROWERR("Criterion Weights ('criterion_weights' keyword) are not defined for composite criterion with criterion block id '" + aMetadata.id() + "'.");
        }
    }
}

void ParseCriteria::setCriterionIDs(XMLGen::Criterion &aMetadata)
{
    if(aMetadata.type() == "composite")
    {
        auto tItr = mTags.find("criterion_ids");
        std::string tValues = tItr->second.first.second;
        if (tItr != mTags.end() && !tValues.empty())
        {
            std::vector<std::string> tIDs;
            char tValuesBuffer[10000];
            strcpy(tValuesBuffer, tValues.c_str());
            XMLGen::parse_tokens(tValuesBuffer, tIDs);
            aMetadata.criterionIDs(tIDs);
        }
        else
        {
            THROWERR("Criterion IDs ('criterion_ids' keyword) are not defined for composite criterion with criterion block id '" + aMetadata.id() + "'.");
        }
    }
}

void ParseCriteria::setCriterionType(XMLGen::Criterion& aMetadata)
{
    auto tItr = mTags.find("type");
    if (tItr->second.first.second.empty())
    {
        THROWERR("Parse Criteria: Criterion type is not defined.")
    }
    else
    {
        auto tValidCriterionType = XMLGen::check_criterion_type_key(tItr->second.first.second);
        aMetadata.type(tValidCriterionType);
    }
}

void ParseCriteria::setMetadata(XMLGen::Criterion& aMetadata)
{
    this->setCriterionType(aMetadata);
    this->setCriterionIDs(aMetadata);
    this->setCriterionWeights(aMetadata);
    setModesToExclude(aMetadata);
    setMatchNodesetIDs(aMetadata);
    this->setTags(aMetadata);
}

void ParseCriteria::checkUniqueIDs()
{
    std::vector<std::string> tIDs;
    for(auto& tCriterion : mData)
    {
        tIDs.push_back(tCriterion.id());
    }

    if(!XMLGen::unique(tIDs))
    {
        THROWERR("Parse Criteria: Criterion block identification numbers, i.e. IDs, are not unique.  Criterion block IDs must be unique.")
    }
}

std::vector<XMLGen::Criterion> ParseCriteria::data() const
{
    return mData;
}

void ParseCriteria::parse(std::istream &aInputFile)
{
    mData.clear();
    this->allocate();
    constexpr int MAX_CHARS_PER_LINE = 10000;
    std::vector<char> tBuffer(MAX_CHARS_PER_LINE);
    while (!aInputFile.eof())
    {
        // read an entire line into memory
        std::vector<std::string> tTokens;
        aInputFile.getline(tBuffer.data(), MAX_CHARS_PER_LINE);
        XMLGen::parse_tokens(tBuffer.data(), tTokens);
        XMLGen::to_lower(tTokens);

        std::string tCriterionBlockID;
        if (XMLGen::parse_single_value(tTokens, { "begin", "criterion" }, tCriterionBlockID))
        {
            XMLGen::Criterion tMetadata;
            if(tCriterionBlockID.empty())
            {
                THROWERR(std::string("Parse Criteria: criterion identification number is empty. ")
                    + "A unique criterion identification number must be assigned to each criterion block.")
            }
            tMetadata.id(tCriterionBlockID);
            XMLGen::erase_tag_values(mTags);
            XMLGen::parse_input_metadata( { "end", "criterion" }, aInputFile, mTags);
            this->setMetadata(tMetadata);
            mData.push_back(tMetadata);
        }
    }
    this->checkUniqueIDs();
}

}
// namespace XMLGen
