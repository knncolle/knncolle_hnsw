#ifndef KNNCOLLE_HNSW_HPP
#define KNNCOLLE_HNSW_HPP

#include <vector>
#include <type_traits>
#include <queue>
#include <algorithm>
#include <memory>

#include "knncolle/knncolle.hpp"
#include "hnswlib/hnswalg.h"

#include "distances.hpp"

/**
 * @file knncolle_hnsw.hpp
 *
 * @brief Approximate nearest neighbor search with HNSW.
 */

/**
 * @namespace knncolle_hnsw
 * @brief knncolle bindings for HNSW search.
 */
namespace knncolle_hnsw {

/**
 * @brief Options for `HnswBuilder` and `HnswPrebuilt`.
 *
 * This can also be created via using the `HnswBuilder::Options` definition,
 * which ensures consistency of template parameters with `HnswBuilder`.
 *
 * @tparam Dim_ Integer type for the number of dimensions.
 * For the `HnswBuilder` constructor, this should be equal to `Matrix_::dimension_type`.
 * @tparam InternalData_ Floating point type for the HNSW index.
 */
template<typename Dim_ = int, typename InternalData_ = float>
struct HnswOptions {
    /**
     * Number of bidirectional links for each node.
     * This is equivalent to the `M` parameter in the underlying **hnswlib** library, 
     * see [here](https://github.com/nmslib/hnswlib/blob/master/ALGO_PARAMS.md#construction-parameters) for details.
     */
    int num_links = 16;

    /**
     * Size of the dynamic list of nearest neighbors during index construction.
     * This controls the trade-off between indexing time and accuracy and is equivalent to the `ef_construct` parameter in the underlying **hnswlib** library,
     * see [here](https://github.com/nmslib/hnswlib/blob/master/ALGO_PARAMS.md#construction-parameters) for details.
     */
    int ef_construction = 200;

    /**
     * Size of the dynamic list of nearest neighbors during searching.
     * This controls the trade-off between search speed and accuracy and is equivalent to the `ef` parameter in the underlying **hnswlib** library,
     * see [here](https://github.com/nmslib/hnswlib/blob/master/ALGO_PARAMS.md#search-parameters) for details.
     */
    int ef_search = 10;

    /**
     * Choice of distance metric to be used during HNSW index construction and search.
     */
    DistanceOptions<Dim_, InternalData_> distance_options;
};

template<typename Dim_, typename Index_, typename Float_, typename InternalData_>
class HnswPrebuilt;

/**
 * @brief Searcher on an Hnsw index.
 *
 * Instances of this class are usually constructed using `HnswPrebuilt::initialize()`.
 *
 * @tparam Dim_ Integer type for the number of dimensions.
 * @tparam Index_ Integer type for the indices.
 * @tparam Float_ Floating point type for the query data and output distances.
 * @tparam InternalData_ Floating point type for the internal data in the HNSW index.
 */
template<typename Dim_, typename Index_, typename Float_, typename InternalData_>
class HnswSearcher : public knncolle::Searcher<Index_, Float_> {
private:
    const HnswPrebuilt<Dim_, Index_, Float_, InternalData_>* my_parent;

    std::priority_queue<std::pair<InternalData_, hnswlib::labeltype> > my_queue;
    std::vector<InternalData_> my_buffer;

    static constexpr bool same_internal = std::is_same<Float_, InternalData_>::value;

public:
    /**
     * @cond
     */
    HnswSearcher(const HnswPrebuilt<Dim_, Index_, Float_, InternalData_>* parent) : my_parent(parent) {
        if constexpr(!same_internal) {
            my_buffer.resize(my_parent->my_dim);
        }
    }
    /**
     * @endcond
     */

public:
    void search(Index_ i, Index_ k, std::vector<Index_>* output_indices, std::vector<Float_>* output_distances) {
        my_buffer = my_parent->my_index.template getDataByLabel<InternalData_>(i);
        Index_ kp1 = k + 1;
        my_queue = my_parent->my_index.searchKnn(my_buffer.data(), kp1); // +1, as it forgets to discard 'self'.

        if (output_indices) {
            output_indices->clear();
            output_indices->reserve(kp1);
        }
        if (output_distances) {
            output_distances->clear();
            output_distances->reserve(kp1);
        }

        bool self_found = false;
        hnswlib::labeltype icopy = i;
        while (!my_queue.empty()) {
            const auto& top = my_queue.top();
            if (!self_found && top.second == icopy) {
                self_found = true;
            } else {
                if (output_indices) {
                    output_indices->push_back(top.second);
                }
                if (output_distances) {
                    output_distances->push_back(top.first);
                }
            }
            my_queue.pop();
        }

        if (output_indices) {
            std::reverse(output_indices->begin(), output_indices->end());
        }
        if (output_distances) {
            std::reverse(output_distances->begin(), output_distances->end());
        }

        // Just in case we're full of ties at duplicate points, such that 'c'
        // is not in the set.  Note that, if self_found=false, we must have at
        // least 'K+2' points for 'c' to not be detected as its own neighbor.
        // Thus there is no need to worry whether we are popping off a non-'c'
        // element and then returning fewer elements than expected.
        if (!self_found) {
            if (output_indices) {
                output_indices->pop_back();
            }
            if (output_distances) {
                output_distances->pop_back();
            }
        }

        if (output_distances && my_parent->my_normalize) {
            for (auto& d : *output_distances) {
                d = my_parent->my_normalize(d);
            }
        }
    }

private:
    void search_raw(const InternalData_* query, Index_ k, std::vector<Index_>* output_indices, std::vector<Float_>* output_distances) {
        k = std::min(k, my_parent->my_obs);
        my_queue = my_parent->my_index.searchKnn(query, k); 

        if (output_indices) {
            output_indices->resize(k);
        }
        if (output_distances) {
            output_distances->resize(k);
        }

        size_t position = k;
        while (!my_queue.empty()) {
            const auto& top = my_queue.top();
            --position;
            if (output_indices) {
                (*output_indices)[position] = top.second;
            }
            if (output_distances) {
                (*output_distances)[position] = top.first;
            }
            my_queue.pop();
        }

        if (output_distances && my_parent->my_normalize) {
            for (auto& d : *output_distances) {
                d = my_parent->my_normalize(d);
            }
        }
    }

public:
    void search(const Float_* query, Index_ k, std::vector<Index_>* output_indices, std::vector<Float_>* output_distances) {
        if constexpr(same_internal) {
            my_queue = my_parent->my_index.searchKnn(query, k);
            search_raw(query, k, output_indices, output_distances);
        } else {
            std::copy_n(query, my_parent->my_dim, my_buffer.begin());
            search_raw(my_buffer.data(), k, output_indices, output_distances);
        }
    }
};

/**
 * @brief Prebuilt index for an Hnsw search.
 *
 * Instances of this class are usually constructed using `HnswBuilder::build_raw()`.
 * The `initialize()` method will create an instance of the `HnswSearcher` class.
 *
 * @tparam Dim_ Integer type for the number of dimensions.
 * For the output of `HnswBuilder::build_raw()`, this is set to `Matrix_::dimension_type`.
 * @tparam Index_ Integer type for the indices.
 * For the output of `HnswBuilder::build_raw()`, this is set to `Matrix_::index_type`.
 * @tparam Float_ Floating point type for the query data and output distances.
 * @tparam InternalData_ Floating point type for the internal data in the HNSW index.
 */
template<typename Dim_, typename Index_, typename Float_, typename InternalData_>
class HnswPrebuilt : public knncolle::Prebuilt<Dim_, Index_, Float_> {
public:
    /**
     * @cond
     */
    template<class Matrix_>
    HnswPrebuilt(const Matrix_& data, const HnswOptions<Dim_, InternalData_>& options) :
        my_dim(data.num_dimensions()),
        my_obs(data.num_observations()),
        my_space([&]() {
            if (options.distance_options.create) {
                return options.distance_options.create(my_dim);
            } else if constexpr(std::is_same<InternalData_, float>::value) {
                return static_cast<hnswlib::SpaceInterface<InternalData_>*>(new hnswlib::L2Space(my_dim));
            } else {
                return static_cast<hnswlib::SpaceInterface<InternalData_>*>(new SquaredEuclideanDistance<InternalData_>(my_dim));
            }
        }()),
        my_normalize([&]() {
            if (options.distance_options.normalize) {
                return options.distance_options.normalize;
            } else if (options.distance_options.create) {
                return std::function<InternalData_(InternalData_)>();
            } else {
                return std::function<InternalData_(InternalData_)>([](InternalData_ x) -> InternalData_ { return std::sqrt(x); });
            }
        }()),
        my_index(my_space.get(), my_obs, options.num_links, options.ef_construction)
    {
        typedef typename Matrix_::data_type Data_;
        auto work = data.create_workspace();
        if constexpr(std::is_same<Data_, InternalData_>::value) {
            for (Index_ i = 0; i < my_obs; ++i) {
                auto ptr = data.get_observation(work);
                my_index.addPoint(ptr, i);
            }
        } else {
            std::vector<InternalData_> incoming(my_dim);
            for (Index_ i = 0; i < my_obs; ++i) {
                auto ptr = data.get_observation(work);
                std::copy_n(ptr, my_dim, incoming.begin());
                my_index.addPoint(incoming.data(), i);
            }
        }

        my_index.setEf(options.ef_search);
        return;
    }
    /**
     * @endcond
     */

private:
    Dim_ my_dim;
    Index_ my_obs;

    // The following must be a pointer for polymorphism, but also so that
    // references to the object in my_index are still valid after copying.
    std::shared_ptr<hnswlib::SpaceInterface<InternalData_> > my_space;

    std::function<InternalData_(InternalData_)> my_normalize;
    hnswlib::HierarchicalNSW<InternalData_> my_index;

    friend class HnswSearcher<Dim_, Index_, Float_, InternalData_>;

public:
    Dim_ num_dimensions() const {
        return my_dim;
    }

    Index_ num_observations() const {
        return my_obs;
    }

    std::unique_ptr<knncolle::Searcher<Index_, Float_> > initialize() const {
        return std::make_unique<HnswSearcher<Dim_, Index_, Float_, InternalData_> >(this);
    }
};

/**
 * @brief Perform an approximate nearest neighbor search with HNSW.
 *
 * In the HNSW algorithm (Malkov and Yashunin, 2016), each point is a node in a "nagivable small world" graph.
 * The nearest neighbor search proceeds by starting at a node and walking through the graph to obtain closer neighbors to a given query point.
 * Nagivable small world graphs are used to maintain connectivity across the data set by creating links between distant points.
 * This speeds up the search by ensuring that the algorithm does not need to take many small steps to move from one cluster to another.
 * The HNSW algorithm extends this idea by using a hierarchy of such graphs containing links of different lengths,
 * which avoids wasting time on small steps in the early stages of the search where the current node position is far from the query.
 *
 * The `build_raw()` method will create an instance of the `HnswPrebuilt` class.
 *
 * @see
 * Malkov YA, Yashunin DA (2016).
 * Efficient and robust approximate nearest neighbor search using Hierarchical Navigable Small World graphs.
 * _arXiv_.
 * https://arxiv.org/abs/1603.09320
 *
 * @tparam Matrix_ Matrix-like object satisfying the `knncolle::MockMatrix` interface.
 * @tparam Float_ Floating point type for the query data and output distances.
 * @tparam InternalData_ Floating point type for the internal data in HNSW index.
 * This defaults to a `float` instead of a `double` to sacrifice some accuracy for performance.
 */
template<
    class Matrix_ = knncolle::SimpleMatrix<int, int, double>, 
    typename Float_ = double, 
    typename InternalData_ = float>
class HnswBuilder : public knncolle::Builder<Matrix_, Float_> {
public:
    /**
     * Convenient name for the `HnswOptions` class that ensures consistent template parametrization.
     */
    typedef HnswOptions<typename Matrix_::dimension_type, InternalData_> Options;

private:
    Options my_options;

public:
    /**
     * @param options Further options for HNSW index construction and searching.
     */
    HnswBuilder(Options options) : my_options(std::move(options)) {}

    /**
     * Default constructor.
     */
    HnswBuilder() = default;

    /**
     * @return Options for HNSW, to be modified prior to calling `build_raw()` and friends.
     */
    Options& get_options() {
        return my_options;
    }

public:
    knncolle::Prebuilt<typename Matrix_::dimension_type, typename Matrix_::index_type, Float_>* build_raw(const Matrix_& data) const {
        return new HnswPrebuilt<typename Matrix_::dimension_type, typename Matrix_::index_type, Float_, InternalData_>(data, my_options);
    }
};

}

#endif
