#ifndef PLATO_FUNCTIONAL_VALIDATEDINPUT
#define PLATO_FUNCTIONAL_VALIDATEDINPUT

#include "GeometryRegistration.hpp"
#include "Plato_InputBlocks.hpp"
#include "ValidatedInputTypeWrapper.hpp"

namespace Plato::Functional::Validation
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

    [[nodiscard]] auto geometry() const -> Core::ValidatedInputTypeWrapper<GeometryFactory::GeometryInput>;
    [[nodiscard]] auto objectives() const -> Core::ValidatedInputTypeWrapper<std::vector<Plato::objective>>;
    [[nodiscard]] auto constraints() const -> Core::ValidatedInputTypeWrapper<std::vector<Plato::constraint>>;
    [[nodiscard]] auto optimizationParameters() const -> Core::ValidatedInputTypeWrapper<Plato::optimization_parameters>;

   private:
    Plato::PlatoInput mInput;
};

ValidatedInput make_validated_input(Plato::PlatoInput aInput);

}  // namespace Plato::Functional::Validation

#endif