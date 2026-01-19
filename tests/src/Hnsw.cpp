#include <gtest/gtest.h>
#include "knncolle_hnsw/knncolle_hnsw.hpp"

#include <random>
#include <vector>

class TestCore {
protected:
    inline static int nobs, ndim;
    inline static std::vector<double> data;
    inline static std::tuple<int, int> last_params;

protected:
    static void assemble(const std::tuple<int, int>& param) {
        if (param == last_params) {
            return;
        }
        last_params = param;

        nobs = std::get<0>(param);
        ndim = std::get<1>(param);

        std::mt19937_64 rng(nobs * 10 + ndim);
        std::normal_distribution distr;

        data.resize(nobs * ndim);
        for (auto& d : data) {
            d = distr(rng);
        }
    }

    template<class It_, class Rng_>
    static void fill_random(It_ start, It_ end, Rng_& eng) {
        std::normal_distribution distr;
        while (start != end) {
            *start = distr(eng);
            ++start;
        }
    }

protected:
    static void sanity_checks(const std::vector<int>& indices, const std::vector<double>& distances) {
        EXPECT_TRUE(std::is_sorted(distances.begin(), distances.end())); // sorted by increasing distance.

        auto sorted = indices;
        std::sort(sorted.begin(), sorted.end());
        EXPECT_TRUE(std::adjacent_find(sorted.begin(), sorted.end()) == sorted.end()); // all neighbors are unique.
    }

    static void sanity_checks(const std::vector<int>& indices, const std::vector<double>& distances, int k, int self) { // for finding by index
        EXPECT_EQ(indices.size(), distances.size());
        EXPECT_EQ(indices.size(), std::min(k, nobs - 1));

        for (const auto& ix : indices) { // self is not in there.
            EXPECT_TRUE(ix != self);
        }

        sanity_checks(indices, distances);
    }
};

class HnswTest : public TestCore, public ::testing::TestWithParam<std::tuple<std::tuple<int, int>, int> > {
protected:
    void SetUp() {
        assemble(std::get<0>(GetParam()));
    }
};

TEST_P(HnswTest, FindEuclidean) {
    int k = std::get<1>(GetParam());    
    auto euconfig = knncolle_hnsw::makeEuclideanDistanceConfig();
    knncolle::EuclideanDistance<double, double> eudist; 

    knncolle::SimpleMatrix<int, double> mat(ndim, nobs, data.data());
    knncolle_hnsw::HnswBuilder<int, double, double> builder(euconfig);
    auto bptr = builder.build_unique(mat);
    EXPECT_EQ(bptr->num_dimensions(), ndim);
    EXPECT_EQ(bptr->num_observations(), nobs);
    auto bsptr = bptr->initialize();

    // Trying with a different interface type such that Data_ == HnswData_ == float.
    std::vector<float> fdata(data.begin(), data.end());
    knncolle::SimpleMatrix<int, float> mat2(ndim, nobs, fdata.data());
    knncolle_hnsw::HnswBuilder<int, float, double> builder2(euconfig);
    auto bptr2 = builder2.build_unique(mat2);
    auto bsptr2 = bptr2->initialize();

    std::vector<int> ires, ires0, ires2;
    std::vector<double> dres, dres0, dres2;

    for (int x = 0; x < nobs; ++x) {
        bsptr->search(x, k, &ires, &dres);
        sanity_checks(ires, dres, k, x);

        // Checking that NULLs work.
        bsptr->search(x, k, NULL, &dres0);
        EXPECT_EQ(dres, dres0);
        bsptr->search(x, k, &ires0, NULL);
        EXPECT_EQ(ires, ires0);

        // Checking the distance to the most distant neighbor. This needs to be a little
        // gentle w.r.t. tolerances due to differences in precision.
        {
            auto furthest = ires.back();
            auto current = data.data() + x * ndim;
            auto ptr = data.data() + furthest * ndim;
            auto expected = eudist.raw(ndim, current, ptr);
            EXPECT_LT(std::abs(eudist.normalize(expected) - dres.back()), 0.0001);
        }

        // Checking the different types.
        bsptr2->search(x, k, &ires2, &dres2);
        EXPECT_EQ(ires, ires2);
        EXPECT_EQ(dres, dres2);
    }
}

TEST_P(HnswTest, FindManhattan) {
    int k = std::get<1>(GetParam());    
    auto manconfig = knncolle_hnsw::makeManhattanDistanceConfig();
    knncolle::ManhattanDistance<double, double> mandist; 

    knncolle::SimpleMatrix<int, double> mat(ndim, nobs, data.data());
    knncolle_hnsw::HnswBuilder<int, double, double> builder(manconfig);
    auto bptr = builder.build_shared(mat); // making a shared pointer for some variety.
    auto bsptr = bptr->initialize();

    std::vector<int> ires;
    std::vector<double> dres;

    for (int x = 0; x < nobs; ++x) {
        bsptr->search(x, k, &ires, &dres);
        sanity_checks(ires, dres, k, x);

        // Checking the distance to the most distant neighbor. This needs to be a little
        // gentle w.r.t. tolerances due to differences in precision.
        {
            auto furthest = ires.back();
            auto current = data.data() + x * ndim;
            auto ptr = data.data() + furthest * ndim;
            auto expected = mandist.raw(ndim, current, ptr);
            EXPECT_LT(std::abs(mandist.normalize(expected) - dres.back()), 0.0001);
        }
    }
}

TEST_P(HnswTest, QueryEuclidean) {
    int k = std::get<1>(GetParam());    
    auto euconfig = knncolle_hnsw::makeEuclideanDistanceConfig();

    knncolle::SimpleMatrix<int, double> mat(ndim, nobs, data.data());
    knncolle_hnsw::HnswBuilder<int, double, double> builder(euconfig);
    auto bptr = builder.build_known_unique(mat); // test coverage for known overrides.
    auto bsptr = bptr->initialize();

    // Trying with a different type.
    std::vector<float> fdata(data.begin(), data.end());
    knncolle::SimpleMatrix<int, float> mat2(ndim, nobs, fdata.data());
    knncolle_hnsw::HnswBuilder<int, float, double> builder2(euconfig);
    auto bptr2 = builder2.build_unique(mat2);
    auto bsptr2 = bptr2->initialize();

    std::vector<int> ires, ires0, ires2;
    std::vector<double> dres, dres0, dres2;

    std::mt19937_64 rng(ndim * 10 + nobs - k);
    std::vector<double> query(ndim);
    std::vector<float> fquery(ndim);

    for (int x = 0; x < nobs; ++x) {
        fill_random(query.begin(), query.end(), rng);

        bsptr->search(query.data(), k, &ires, &dres);
        EXPECT_EQ(ires.size(), std::min(k, nobs));
        EXPECT_EQ(ires.size(), dres.size());
        sanity_checks(ires, dres);

        // Checking that NULLs work.
        bsptr->search(query.data(), k, NULL, &dres0);
        EXPECT_EQ(dres, dres0);
        bsptr->search(query.data(), k, &ires0, NULL);
        EXPECT_EQ(ires, ires0);

        // Checking the different types.
        std::copy(query.begin(), query.end(), fquery.begin());
        bsptr2->search(fquery.data(), k, &ires2, &dres2);
        EXPECT_EQ(ires, ires2);
        EXPECT_EQ(dres, dres2);
    }
}

INSTANTIATE_TEST_SUITE_P(
    Hnsw,
    HnswTest,
    ::testing::Combine(
        ::testing::Combine(
            ::testing::Values(10, 500), // number of observations
            ::testing::Values(5, 20) // number of dimensions
        ),
        ::testing::Values(3, 10, 20) // number of neighbors (one is greater than # observations, to test correct limiting)
    )
);

TEST(Hnsw, Constructor) {
    auto euconfig = knncolle_hnsw::makeEuclideanDistanceConfig<float>();

    knncolle_hnsw::HnswBuilder<int, double, double> def(euconfig);
    auto def_opt = def.get_options();
    EXPECT_NE(def_opt.num_links, 10000);

    // Checking that this is respected in the overloaded constructor.
    def_opt.num_links = 1000;
    knncolle_hnsw::HnswBuilder<int, double, double> mutant(euconfig, def_opt);
    EXPECT_EQ(mutant.get_options().num_links, 1000);

    // Checking that we throw errors correctly if the configuration has no distance.
    knncolle_hnsw::DistanceConfig<float> emptydist;
    std::string msg;
    try {
        knncolle_hnsw::HnswBuilder<int, double, double> emptyobj(emptydist);
    } catch (std::exception& e) {
        msg = e.what();
    }
    EXPECT_TRUE(msg.find("not provided") != std::string::npos);
}

class HnswMiscTest : public TestCore, public ::testing::Test {
protected:
    void SetUp() {
        assemble({ 100, 5 });
    }
};

TEST_F(HnswMiscTest, EuclideanDouble) {
    knncolle::EuclideanDistance<double, double> eudist;
    auto euconfig = knncolle_hnsw::makeEuclideanDistanceConfig<double>(); // using HnswData_ = double to check that we dispatch correctly to a SquaredEuclideanDistance.

    knncolle::SimpleMatrix<int, double> mat(ndim, nobs, data.data());
    knncolle_hnsw::HnswBuilder<int, double, double, knncolle::SimpleMatrix<int, double>, double> builder(std::move(euconfig));
    auto bptr = builder.build_known_shared(mat); // test coverage for known overrides.
    auto bsptr = bptr->initialize();

    std::vector<int> ires;
    std::vector<double> dres;
    for (int x = 0; x < nobs; ++x) {
        bsptr->search(x, 10, &ires, &dres);

        // Checking the distance to the most distant neighbor. Here we can afford to have some
        // lower tolerances because everything's computed with doubles.
        auto furthest = ires.back();
        auto current = data.data() + x * ndim;
        auto ptr = data.data() + furthest * ndim;
        auto expected = eudist.raw(ndim, current, ptr);
        EXPECT_LT(std::abs(eudist.normalize(expected) - dres.back()), 0.000001);
    }
}

TEST_F(HnswMiscTest, EuclideanNormalize) {
    // Checking that the normalization option is respected.
    auto distconfig = knncolle_hnsw::makeEuclideanDistanceConfig<float>();
    distconfig.normalize = [](float x) -> float { return x + 1; };

    knncolle::SimpleMatrix<int, double> mat(ndim, nobs, data.data());
    knncolle_hnsw::HnswBuilder<int, double, double> builder(std::move(distconfig)); 
    auto bptr = builder.build_unique(mat);
    auto bsptr = bptr->initialize();

    knncolle::EuclideanDistance<double, double> eudist;

    std::vector<int> ires;
    std::vector<double> dres;
    for (int x = 0; x < nobs; ++x) {
        bsptr->search(x, 10, &ires, &dres);

        // Checking the distance to the most distant neighbor; using some more gentle tolerances again.
        auto furthest = ires.back();
        auto current = data.data() + x * ndim;
        auto ptr = data.data() + furthest * ndim;
        auto expected = eudist.raw(ndim, current, ptr);
        EXPECT_LT(std::abs((expected + 1) - dres.back()), 0.0001);
    }
}

TEST(Hnsw, Duplicates) {
    // Checking that the neighbor identification works correctly when there are
    // so many duplicates that an observation doesn't get reported by HNSW in
    // its own list of neighbors.
    int ndim = 5;
    int nobs = 100;
    std::vector<double> data(ndim * nobs);
    knncolle::SimpleMatrix<int, double> mat(ndim, nobs, data.data());

    knncolle_hnsw::HnswBuilder<int, double, double> builder(knncolle_hnsw::makeEuclideanDistanceConfig()); 
    auto bptr = builder.build_unique(mat);
    auto bsptr = bptr->initialize();

    std::vector<int> ires, ires0;
    std::vector<double> dres, dres0;

    for (int x = 0; x < nobs; ++x) {
        bsptr->search(x, 10, &ires, &dres);
        EXPECT_EQ(ires.size(), 10);
        for (const auto& ix : ires) { // self is not in there.
            EXPECT_TRUE(ix != x);
        }

        EXPECT_EQ(dres.back(), 0);
        EXPECT_EQ(dres.front(), 0);

        // Same behavior with NULLs.
        bsptr->search(x, 10, NULL, &dres0);
        EXPECT_EQ(dres, dres0);
        bsptr->search(x, 10, &ires0, NULL);
        EXPECT_EQ(ires, ires0);
    }
}
