#include "STKCommandGenerator.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace plato::utilities
{
std::string STKCommandGenerator::toString() const
{
    std::stringstream tStringStream;
    tStringStream << "generated:";
    tStringStream << std::setprecision(mPrecision);
    tStringStream << mElements.mX << "x" << mElements.mY << "x" << mElements.mZ;
    tStringStream << "|bbox:" << mLowerBounds.x << "," << mLowerBounds.y << "," << mLowerBounds.z << ",";
    tStringStream << mUpperBounds.x << "," << mUpperBounds.y << "," << mUpperBounds.z;
    if (mType == STKCommandElementType::Tet)
    {
        tStringStream << "|tets";
    }
    tStringStream << nodesetString();
    tStringStream << sidesetString();
    return tStringStream.str();
}

double STKCommandGenerator::volume() const
{
    return (mUpperBounds.x - mLowerBounds.x) * (mUpperBounds.y - mLowerBounds.y) * (mUpperBounds.z - mLowerBounds.z);
}

unsigned int STKCommandGenerator::numberOfElements() const
{
    const unsigned int tHexTotalElements = mElements.mX * mElements.mY * mElements.mZ;
    constexpr unsigned int tTetsPerHex = 6;

    if (mType == STKCommandElementType::Hex)
    {
        return tHexTotalElements;
    }
    else
    {
        return tHexTotalElements * tTetsPerHex;
    }
}

unsigned int STKCommandGenerator::numberOfNodes() const
{
    return (mElements.mX + 1) * (mElements.mY + 1) * (mElements.mZ + 1);
}

std::string STKCommandGenerator::nodesetString() const
{
    const std::string tString = detail::xyz_boundary_string(mNodeset);
    if (!tString.empty())
    {
        return "|nodeset:" + tString;
    }
    return {};
}

std::string STKCommandGenerator::sidesetString() const
{
    const std::string tString = detail::xyz_boundary_string(mSideset);
    if (!tString.empty())
    {
        return "|sideset:" + tString;
    }
    return {};
}

namespace detail
{
std::string xyz_boundary_string(const STKNodeSetSideSetIdentifiers& aSTKNodeSetSideSetIdentifiers)
{
    return std::string(aSTKNodeSetSideSetIdentifiers.mLowerX ? "x" : "") +
           std::string(aSTKNodeSetSideSetIdentifiers.mUpperX ? "X" : "") +
           std::string(aSTKNodeSetSideSetIdentifiers.mLowerY ? "y" : "") +
           std::string(aSTKNodeSetSideSetIdentifiers.mUpperY ? "Y" : "") +
           std::string(aSTKNodeSetSideSetIdentifiers.mLowerZ ? "z" : "") +
           std::string(aSTKNodeSetSideSetIdentifiers.mUpperZ ? "Z" : "");
}

}  // namespace detail

}  // namespace plato::utilities
