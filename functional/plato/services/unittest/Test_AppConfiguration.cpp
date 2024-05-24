#include <gtest/gtest.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <filesystem>
#include <fstream>

#include "plato/services/AppConfiguration.hpp"
#include "plato/test_utilities/TestContext.hpp"

namespace plato::services
{
namespace
{
template <typename Archive, typename Serializable>
void save(const Serializable& tS, const std::filesystem::path& aFilename)
{
    auto tOutFileStream = std::ofstream{aFilename};
    Archive tOutputArchive(tOutFileStream, boost::archive::no_header | boost::archive::no_tracking);
    tOutputArchive << boost::serialization::make_nvp("Serializable", tS);
}

template <typename Archive, typename Serializable>
void load(Serializable& tS, const std::filesystem::path& aFilename)
{
    auto tInFileStream = std::ifstream{aFilename};
    Archive tInputArchive(tInFileStream, 1);
    tInputArchive >> boost::serialization::make_nvp("Serializable", tS);
}

template <typename Serializable>
void testSerializeRoundTrip(const Serializable& aSerializable, const test_utilities::TestContext& aTestContext)
{
    const auto tFilename = std::filesystem::path{"out.config"};
    save<boost::archive::text_oarchive>(aSerializable, tFilename);
    auto tRoundTripResult = Serializable{};
    load<boost::archive::text_iarchive>(tRoundTripResult, tFilename);

    EXPECT_EQ(aSerializable, tRoundTripResult) << aTestContext;

    std::filesystem::remove(tFilename);
}

}  // namespace

TEST(AppConfiguration, Serialization)
{
    const auto tAppConfiguration = services::AppConfiguration{/*.name=*/"test-app", /*.lib_name=*/"libtest.so"};
    testSerializeRoundTrip(tAppConfiguration, TEST_CONTEXT("App configuration"));
}
}  // namespace plato::services
