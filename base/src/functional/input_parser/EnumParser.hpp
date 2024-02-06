#ifndef PLATO_ENUMPARSER_HPP
#define PLATO_ENUMPARSER_HPP

#include <boost/spirit/include/qi.hpp>

#include "EnumTable.hpp"

namespace plato::functional::input_parser
{
template <typename Enum>
auto make_enum_symbols(const utilities::EnumTable<Enum>& aTable) -> boost::spirit::qi::symbols<char, Enum>
{
    boost::spirit::qi::symbols<char, Enum> tSymbols;
    for (const auto& entry : aTable)
    {
        tSymbols.add(entry.second, entry.first);
    }
    return tSymbols;
}

}  // namespace plato::functional::input_parser

#endif
