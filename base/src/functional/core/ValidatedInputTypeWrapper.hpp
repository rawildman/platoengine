#ifndef PLATO_FUNCTIONAL_VALIDATEDINPUTTYPEWRAPPER
#define PLATO_FUNCTIONAL_VALIDATEDINPUTTYPEWRAPPER

#include <utility>

namespace plato::functional::main::library
{
class ValidatedInput;
}

namespace plato::functional::core
{
template <typename InputType>
class ValidatedInputTypeWrapper
{
   public:
    using RawInputType = InputType;

    const InputType& rawInput() const { return mRawInput; }

   private:
    friend class plato::functional::main::library::ValidatedInput;
    ValidatedInputTypeWrapper(InputType aRawInput) : mRawInput(std::move(aRawInput)) {}
    InputType mRawInput;
};

}  // namespace plato::functional::core

#endif
