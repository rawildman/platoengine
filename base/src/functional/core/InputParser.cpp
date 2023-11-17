#define BOOST_PHOENIX_STL_TUPLE_H_  // Work-around for ODR violation in boost phoenix.
                                    // https://github.com/boostorg/phoenix/issues/111

#include "InputParser.hpp"

#include <fstream>
#include <streambuf>

#include "Exception.hpp"
#include "Plato_InputParser.hpp"

namespace Plato::Functional
{
Plato::PlatoInput parse_input(const std::string_view aInput)
{
    Plato::InputParser<std::string_view::const_iterator> tParser;
    Plato::PlatoInput tData;
    auto tIter = aInput.cbegin();
    const bool tParseResult = phrase_parse(tIter, aInput.cend(), tParser, boost::spirit::ascii::space, tData);
    if (!tParseResult || tIter != aInput.cend())
    {
        throw Plato::Functional::Exception("Could not parse input deck.");
    }
    return tData;
}

Plato::PlatoInput parse_input_from_file(const std::filesystem::path& aFileName)
{
    std::ifstream tInputStream(aFileName);
    const std::string tInputFileString((std::istreambuf_iterator<char>(tInputStream)),
                                       std::istreambuf_iterator<char>());
    return parse_input(tInputFileString);
}

}  // namespace Plato::Functional
