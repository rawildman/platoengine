/*
 * XMLGeneratorSharedData.cpp
 *
 *  Created on: April 14, 2022
 */

#include "XMLGeneratorSharedData.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"

namespace XMLGen
{
    XMLGeneratorSharedData::XMLGeneratorSharedData
    (const std::string& aName,
    const std::string& aSize,
    const std::string& aOwnerName,
    const std::vector<std::string>& aUserName) :
    mName(aName),
    mSize(aSize),
    mOwnerName(aOwnerName),
    mUserName(aUserName)
    {}
    void XMLGeneratorSharedData::write(pugi::xml_document& aDocument)
    {
        auto tSharedDataNode = aDocument.append_child("SharedData");
        addChild(tSharedDataNode, "Name", mName);
        addChild(tSharedDataNode, "Type", "Scalar");
        addChild(tSharedDataNode, "Layout", "Global");
        addChild(tSharedDataNode, "Size", mSize);
        addChild(tSharedDataNode, "OwnerName", mOwnerName);
        for(unsigned int iUserName = 0 ; iUserName < mUserName.size(); iUserName++)
            addChild(tSharedDataNode, "UserName", mUserName[iUserName]);
    }
    std::string XMLGeneratorSharedData::name()
    {
        return mName;
    }
}