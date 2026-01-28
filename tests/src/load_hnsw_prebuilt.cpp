#include <gtest/gtest.h>
#include "knncolle_hnsw/load_hnsw_prebuilt.hpp"

#include <vector>
#include <filesystem>
#include <string>

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

TEST_F(HnswLoadPrebuiltTest, Custom) {
    knncolle_hnsw::custom_save_for_hnsw_data<float>() = [](const std::string& prefix) -> void {
        knncolle::quick_save(prefix + "WHEE", "stuff", 5);
    };
    knncolle_hnsw::custom_save_for_hnsw_distance<float>() = [](const std::string& prefix, const hnswlib::SpaceInterface<float>*) -> void {
        knncolle::quick_save(prefix + "FOO", "bar", 3);
    };
    knncolle_hnsw::custom_save_for_hnsw_normalize<double>() = [](const std::string& prefix, const std::function<double(double)>&) -> void {
        knncolle::quick_save(prefix + "YAY", "blah", 4);
    };

    // First we try with no custom distance/normalization.
    {
        knncolle_hnsw::HnswBuilder<int, double, double> ab(knncolle_hnsw::makeEuclideanDistanceConfig());
        auto aptr = ab.build_unique(knncolle::SimpleMatrix<int, double>(ndim, nobs, data.data()));

        const auto prefix = (savedir / "custom_").string();
        aptr->save(prefix);

        // Custom function is only respected for the data.
        EXPECT_EQ(knncolle::quick_load_as_string(prefix + "WHEE"), "stuff");
        EXPECT_FALSE(std::filesystem::exists(prefix + "FOO"));
        EXPECT_FALSE(std::filesystem::exists(prefix + "YAY"));

        // Everything else is still fine.
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

    // Now we add a custom distance/normalization.
    {
        // Mocking up a distance class. 
        class CustomEuclideanDistance final : public hnswlib::SpaceInterface<float> {
        private:
            std::size_t my_data_size;
            std::size_t my_dim;

        public:
            CustomEuclideanDistance(std::size_t dim) : my_data_size(dim * sizeof(float)), my_dim(dim) {}

        public:
            std::size_t get_data_size() { return my_data_size; }
            hnswlib::DISTFUNC<float> get_dist_func() { return L2; }
            void * get_dist_func_param() { return &my_dim; }

        private:
            static float L2(const void *pVect1v, const void *pVect2v, const void *qty_ptr) {
                const float* pVect1 = static_cast<const float*>(pVect1v);
                const float* pVect2 = static_cast<const float*>(pVect2v);
                std::size_t qty = *((size_t *) qty_ptr);
                float res = 0;
                for (; qty > 0; --qty, ++pVect1, ++pVect2) {
                    auto delta = *pVect1 - *pVect2;
                    res += delta * delta;
                }
                return res;
            }
        };

        knncolle_hnsw::DistanceConfig<double, float> config;
        config.create = [&](std::size_t n) -> hnswlib::SpaceInterface<float>* {
            return new CustomEuclideanDistance(n);
        };
        config.normalize_method = knncolle_hnsw::DistanceNormalizeMethod::CUSTOM;
        config.custom_normalize = [](double x) -> double {
            return std::sqrt(x);
        };

        knncolle_hnsw::HnswBuilder<int, double, double> ab(std::move(config));
        auto aptr = ab.build_unique(knncolle::SimpleMatrix<int, double>(ndim, nobs, data.data()));
        const auto prefix = (savedir / "custom2_").string();
        aptr->save(prefix);

        // If we load without setting the custom loaders, it crashes.
        {
            std::string errmsg;
            try {
                knncolle::load_prebuilt_shared<int, double, double>(prefix);
            } catch (std::exception& e){
                errmsg = e.what();
            }
            EXPECT_TRUE(errmsg.find("unknown distance") != std::string::npos);
        }

        knncolle_hnsw::custom_load_for_hnsw_distance<float>() = [](const std::string& prefix, std::size_t ndim) -> hnswlib::SpaceInterface<float>* {
            auto payload = knncolle::quick_load_as_string(prefix + "FOO");
            EXPECT_EQ(payload, "bar"); 
            return new CustomEuclideanDistance(ndim);
        };

        {
            std::string errmsg;
            try {
                knncolle::load_prebuilt_shared<int, double, double>(prefix);
            } catch (std::exception& e){
                errmsg = e.what();
            }
            EXPECT_TRUE(errmsg.find("unknown normalization") != std::string::npos) << errmsg;
        }

        knncolle_hnsw::custom_load_for_hnsw_normalize<double>() = [&](const std::string& prefix) -> std::function<double(double)> {
            auto payload = knncolle::quick_load_as_string(prefix + "YAY");
            EXPECT_EQ(payload, "blah"); 
            return std::function<double(double)>([](double x) -> double { return std::sqrt(x); });
        };

        // Now we can load it and everything is still fine.
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

    knncolle_hnsw::custom_save_for_hnsw_data<float>() = nullptr;
    knncolle_hnsw::custom_save_for_hnsw_distance<float>() = nullptr;
    knncolle_hnsw::custom_save_for_hnsw_normalize<double>() = nullptr;
    knncolle_hnsw::custom_load_for_hnsw_distance<float>() = nullptr;
    knncolle_hnsw::custom_load_for_hnsw_normalize<double>() = nullptr;
}
