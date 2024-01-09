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

    const InputType& rawInput() const
    {
        return mRawInput;
    }

   private:
    friend class Validation::ValidatedInput;
    ValidatedInputTypeWrapper(InputType aRawInput) : mRawInput(std::move(aRawInput)) {}
    InputType mRawInput;
};

}  // namespace Plato::Functional::Core

#endif