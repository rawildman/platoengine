#ifndef PLATO_FUNCTIONAL_VALIDATEDINPUTTYPEWRAPPER
#define PLATO_FUNCTIONAL_VALIDATEDINPUTTYPEWRAPPER

#include <utility>

namespace Plato::Functional::Validation
{
class ValidatedInput;
}

namespace Plato::Functional::Core
{

template <typename InputType>
class ValidatedInputTypeWrapper
{
   public:
    using RawInputType = InputType;

    const InputType& value() const
    {
        return mValue;
    }

   private:
    friend class Validation::ValidatedInput;
    ValidatedInputTypeWrapper(InputType aValue) : mValue(std::move(aValue)) {}
    InputType mValue;
};

}  // namespace Plato::Functional::Core

#endif