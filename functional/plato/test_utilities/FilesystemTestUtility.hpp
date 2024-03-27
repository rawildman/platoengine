#ifndef PLATO_TEST_UTILITIES_FILESYSTEMTESTUTILITY
#define PLATO_TEST_UTILITIES_FILESYSTEMTESTUTILITY

#include <string>
#include <vector>
namespace plato::test_utilities
{
void test_for_existence_and_delete(const std::vector<std::string>& aFilesToCheck);
}

#endif