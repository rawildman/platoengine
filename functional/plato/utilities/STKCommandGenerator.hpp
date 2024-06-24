#ifndef PLATO_UTILITIES_STKCOMMANDGENERATOR
#define PLATO_UTILITIES_STKCOMMANDGENERATOR

#include <optional>
#include <string>

#include "Vector3.hpp"

namespace plato::utilities
{
/// @brief Simple enum class to indicate whether the STK command should generate hexs or tets. Very limited scope of
/// this enum class.
enum class STKCommandElementType
{
    Hex,
    Tet
};

using STKCommandBounds = Coordinate;

/// @brief Struct to bundle the number of elements in a STK generate command
struct STKCommandNumberOfElements
{
    unsigned int mX = 1u;
    unsigned int mY = 1u;
    unsigned int mZ = 1u;
};

struct STKNodeSetSideSetIdentifiers
{
    bool mLowerX = false;
    bool mUpperX = false;
    bool mLowerY = false;
    bool mUpperY = false;
    bool mLowerZ = false;
    bool mUpperZ = false;
};

/// @brief Struct to bundle the common operations needed to work with STK "generate" command
struct STKCommandGenerator
{
    STKCommandNumberOfElements mElements = {1u, 1u, 1u};
    STKCommandBounds mLowerBounds = {0, 0, 0};
    STKCommandBounds mUpperBounds = {1, 1, 1};
    STKCommandElementType mType = STKCommandElementType::Hex;
    STKNodeSetSideSetIdentifiers mNodeset{};
    STKNodeSetSideSetIdentifiers mSideset{};
    int mPrecision = 8;

    /// @brief Take the struct data and return a string with the STK "generate" command
    [[nodiscard]] std::string toString() const;

    /// @brief Take the struct data and determine the volume of the brick
    [[nodiscard]] double volume() const;

    /// @brief Take the struct data and determine the total number of elements
    [[nodiscard]] unsigned int numberOfElements() const;

    /// @brief Take the struct data and determine the total number of nodes
    [[nodiscard]] unsigned int numberOfNodes() const;

    /// @brief Take the struct data and return a string with the STK "nodeset" command
    [[nodiscard]] std::string nodesetString() const;

    /// @brief Take the struct data and return a string with the STK "sideset" command
    [[nodiscard]] std::string sidesetString() const;
};

namespace detail
{
/// @brief Take the struct of nodeset sideset identifiers and return lower and upper case x,y,zs for STK
[[nodiscard]] std::string xyz_boundary_string(const STKNodeSetSideSetIdentifiers& aSTKNodeSetSideSetIdentifiers);

}  // namespace detail

}  // namespace plato::utilities

#endif
