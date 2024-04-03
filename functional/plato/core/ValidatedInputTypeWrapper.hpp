#ifndef PLATO_CORE_VALIDATEDINPUTTYPEWRAPPER
#define PLATO_CORE_VALIDATEDINPUTTYPEWRAPPER

#include <utility>

namespace plato::process_manager::library
{
class ValidatedInput;
}

namespace plato::core
{
template <typename InputType>
class ValidatedInputTypeWrapper
{
   public:
    using RawInputType = InputType;

    const InputType& rawInput() const { return mRawInput; }

   private:
    friend class plato::process_manager::library::ValidatedInput;
    ValidatedInputTypeWrapper(InputType aRawInput) : mRawInput(std::move(aRawInput)) {}
    InputType mRawInput;
};

}  // namespace plato::core

#endif
