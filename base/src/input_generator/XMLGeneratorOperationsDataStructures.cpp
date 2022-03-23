/*
 * XMLGeneratorOperationsDataStructures.cpp
 *
 *  Created on: March 21, 2022
 */

#include "XMLG_Macros.hpp"
#include "XMLGeneratorParserUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

/*******************************************************************/
/************************ OperationArgument ************************/
/*******************************************************************/

void OperationArgument::set(const std::string& aKey, const std::string& aValue)
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    mData[tLowerKey] = aValue;
}

const std::string& OperationArgument::get(const std::string& aKey) const
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    return mData.find(tLowerKey)->second;
}

/*******************************************************************/
/************************ OperationMetaData ************************/
/*******************************************************************/

void OperationMetaData::append(const std::string& aKey, const std::string& aValue)
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    mData[tLowerKey].push_back(aValue);
}

void OperationMetaData::set(const std::string& aKey, const std::vector<std::string>& aValues)
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    mData[tLowerKey] = aValues;
}

void OperationMetaData::set(const std::unordered_map<std::string, std::vector<std::string>>& aData)
{
    mData = aData;
}

const std::vector<std::string>& OperationMetaData::get(const std::string& aKey) const
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    auto tItr = mData.find(tLowerKey);
    if(tItr == mData.end())
    {
        THROWERR(std::string("Did not find key '") + aKey + "' in map.")
    }
    return tItr->second;
}

bool OperationMetaData::is_defined(const std::string& aKey) const
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    auto tItr = mData.find(tLowerKey);
    if(tItr == mData.end())
    {
        return false;
    }
    return true;
}

const std::string& OperationMetaData::front(const std::string& aKey) const
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    if(!this->is_defined(tLowerKey))
    {
        THROWERR(std::string("Did not find key '") + aKey + "' in map.")
    }
    return mData.find(tLowerKey)->second.front();
}

std::string OperationMetaData::back(const std::string& aKey) const
{
    auto tLowerKey = XMLGen::to_lower(aKey);
    if(!this->is_defined(tLowerKey))
    {
        THROWERR(std::string("Did not find key '") + aKey + "' in map.")
    }
    return mData.find(tLowerKey)->second.back();
}

std::vector<std::string> OperationMetaData::keys() const
{
    std::vector<std::string> tKeys;
    for(auto tPair : mData)
    {
        tKeys.push_back(tPair.first);
    }
    return tKeys;
}

}
// namespace XMLGen