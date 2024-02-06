#ifndef PLATO_FUNCTIONAL_EXCEPTION
#define PLATO_FUNCTIONAL_EXCEPTION

#include <stdexcept>

namespace plato::functional::utilities
{
class Exception : public std::runtime_error
{
   public:
    explicit Exception(const std::string& what) : std::runtime_error(what) {}
};
}  // namespace plato::functional::utilities
#endif
