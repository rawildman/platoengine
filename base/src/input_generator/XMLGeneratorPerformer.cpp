/*
 * XMLGeneratorPerformer.cpp
 *
 *  Created on: April 14, 2022
 */

#include "XMLGeneratorPerformer.hpp"
#include "XMLGeneratorUtilities.hpp"
#include "XMLGeneratorPlatoMainOperationFileUtilities.hpp"

namespace XMLGen
{
    XMLGeneratorPerformer::XMLGeneratorPerformer
    (const std::string& aPerformerID,
    const std::string& aName,
    const std::string& aCode) :
    mPerformerID(aPerformerID),
    mName(aName),
    mCode(aCode)
    {
    }

    void XMLGeneratorPerformer::write(pugi::xml_document& aDocument)
    {
        auto tPerformerNode = aDocument.append_child("Perfomer");
        addChild(tPerformerNode, "PerformerID", mPerformerID);
        addChild(tPerformerNode, "Name", mName);
        addChild(tPerformerNode, "Code", mCode);
    }

    std::string XMLGeneratorPerformer::name()
    {
        return mName;
    }
}