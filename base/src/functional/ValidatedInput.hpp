#ifndef PLATO_FUNCTIONAL_VALIDATEDINPUT
#define PLATO_FUNCTIONAL_VALIDATEDINPUT

#include "Plato_InputBlocks.hpp"

namespace Plato::Functional::Core
{
class ValidatedInput;

struct Key
{
    friend ValidatedInput make_validated_input(Plato::PlatoInput input);

   private:
    Key() {}
    Key(const Key&) {}
};

class ValidatedInput
{
   public:
    ValidatedInput(Plato::PlatoInput aInput, Key);
    Plato::PlatoInput value() const;

   private:
    Plato::PlatoInput mInput;
};

ValidatedInput make_validated_input(Plato::PlatoInput aInput);

}  // namespace Plato::Functional::Core

#endif