#ifndef PLATO_TEST_UTLITIES_TESTCONTEXT
#define PLATO_TEST_UTLITIES_TESTCONTEXT

#include <string>

/// Convenience macro for instantiating a TestContext object. The file and line are added automatically.
#define TEST_CONTEXT(MESSAGE) \
    plato::test_utilities::TestContext { __FILE__, __LINE__, (MESSAGE) }

namespace plato::test_utilities
{
/// @brief A helper for adding additional information to test failures.
///
/// This is meant to be used in common test functions that include a gtest test assertion macro.
/// One issue with functions that use test macros is that when a failure occurs, the failing line is
/// within the function. This can obscure the source of the failure, especially when one test
/// calls multiple functions that contain test assertions.
///
/// This is meant to be streamed into the test assertion as:
/// @code{.cpp}
/// void test_things(const TestContext& aTestContext) {
///   // Do stuff
///   EXPECT_EQ(thing1, thing2) << aTestContext;
/// }
/// @endcode
/// This should then be used with the `TEST_CONTEXT` macro as:
/// @code{.cpp}
/// test_things(TEST_CONTEXT("Descriptive error message"));
/// @endcode
/// or directly with the file and line numbers (`__FILE__` and `__LINE__`) as:
/// @code{.cpp}
/// test_things(TestContext{__FILE__, __LINE__, "Descriptive error message"});
/// @endcode
struct TestContext
{
    std::string mFileName;
    unsigned int mLineNumber;
    std::string mMessage;
};

template <typename OutStream>
OutStream& operator<<(OutStream& aOutStream, const TestContext& aTestContext)
{
    aOutStream << "Source file: " << aTestContext.mFileName << ":" << aTestContext.mLineNumber << ": "
               << aTestContext.mMessage;
    return aOutStream;
}

}  // namespace plato::test_utilities

#endif
