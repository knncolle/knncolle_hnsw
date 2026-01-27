#include <gtest/gtest.h>
#include "knncolle_hnsw/load_hnsw_prebuilt.hpp"

#include <vector>
#include <filesystem>

#include "TestCore.h"

class HnswLoadPrebuiltTest : public TestCore, public ::testing::Test {
protected:
    inline static std::filesystem::path savedir;

    static void SetUpTestSuite() {
        assemble({ 50, 5 });

        savedir = "save-prebuilt-tests";
        std::filesystem::remove_all(savedir);
        std::filesystem::create_directory(savedir);

        auto& reg = knncolle::load_prebuilt_registry<int, double, double>();
        reg[knncolle_hnsw::save_name] = [](const std::string& prefix) -> knncolle::Prebuilt<int, double, double>* {
            auto scanned = knncolle_hnsw::load_hnsw_prebuilt_types(prefix);
            assert(scanned.data == knncolle::NumericType::FLOAT);
            return knncolle_hnsw::load_hnsw_prebuilt<int, double, double, float>(prefix);
        };
    }
};

TEST_F(HnswLoadPrebuiltTest, Euclidean) {
    knncolle_hnsw::HnswBuilder<int, double, double> ab(knncolle_hnsw::makeEuclideanDistanceConfig());
    auto aptr = ab.build_unique(knncolle::SimpleMatrix<int, double>(ndim, nobs, data.data()));

    const auto prefix = (savedir / "euclidean_").string();
    aptr->save(prefix);

    auto reloaded = knncolle::load_prebuilt_shared<int, double, double>(prefix);
    std::vector<int> output_i, output_i2;
    std::vector<double> output_d, output_d2;

    auto searcher = aptr->initialize();
    auto researcher = reloaded->initialize();
    for (int x = 0; x < nobs; ++x) {
        searcher->search(x, 5, &output_i, &output_d);
        researcher->search(x, 5, &output_i2, &output_d2);
        EXPECT_EQ(output_i, output_i2);
        EXPECT_EQ(output_d, output_d2);
    }
}

TEST_F(HnswLoadPrebuiltTest, Manhattan) {
    knncolle_hnsw::HnswBuilder<int, double, double> ab(knncolle_hnsw::makeManhattanDistanceConfig());
    auto aptr = ab.build_unique(knncolle::SimpleMatrix<int, double>(ndim, nobs, data.data()));

    const auto prefix = (savedir / "manhattan_").string();
    aptr->save(prefix);

    auto reloaded = knncolle::load_prebuilt_shared<int, double, double>(prefix);
    std::vector<int> output_i, output_i2;
    std::vector<double> output_d, output_d2;

    auto searcher = aptr->initialize();
    auto researcher = reloaded->initialize();
    for (int x = 0; x < nobs; ++x) {
        searcher->search(x, 5, &output_i, &output_d);
        researcher->search(x, 5, &output_i2, &output_d2);
        EXPECT_EQ(output_i, output_i2);
        EXPECT_EQ(output_d, output_d2);
    }
}
