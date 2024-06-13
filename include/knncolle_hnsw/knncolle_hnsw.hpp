#ifndef KNNCOLLE_HNSW_HPP
#define KNNCOLLE_HNSW_HPP

#include <vector>
#include <type_traits>

#include "knncolle/knncolle.hpp"
#include "hnswlib/hnswalg.h"

#include "distances.hpp"

/**
 * @file knncolle_hnsw.hpp
 *
 * @brief Approximate nearest neighbor search with HNSW.
 */

namespace knncolle_hnsw {

/**
 * @brief Options for `HnswBuilder()` and `HnswPrebuilt()`.
 */
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
};

template<class Distance_, typename Dim_, typename Index_, typename Float_, typename InternalIndex_, typename InternalData_>
class HnswPrebuilt;

/**
 * @brief Searcher on an Hnsw index.
 *
 * Instances of this class are usually constructed using `HnswPrebuilt::initialize()`.
 *
 * @tparam Distance_ Subclass of a `hnswlib::SpaceInterface<InternalData_>`, see comments in `HnswBuilder`.
 * @tparam Dim_ Integer type for the number of dimensions.
 * @tparam Index_ Integer type for the indices.
 * @tparam Float_ Floating point type for the query data and output distances.
 * @tparam InternalIndex_ Integer type for the internal indices in Hnsw.
 * @tparam InternalData_ Floating point type for the internal data in Hnsw.
 */
template<class Distance_, typename Dim_, typename Index_, typename Float_, typename InternalData_>
class HnswSearcher : public knncolle::Searcher<Index_, Float_> {
private:
    const HnswPrebuilt<Distance_, Dim_, Index_, Float_, InternalIndex_, InternalData_>* my_parent;

    std::priority_queue<std::pair<InternalData_, hnswlib::labeltype> > my_queue;
    std::vector<InternalData_> my_buffer;

    static constexpr bool same_internal = std::is_same<Float_, InternalData_>::value;

public:
    /**
     * @cond
     */
    HnswSearcher(const HnswPrebuilt<Distance_, Dim_, Index_, Float_, InternalIndex_, InternalData_>* parent) : my_parent(parent) {
        if constexpr(!same_internal) {
            my_buffer.resize(my_parent->my_dim);
        }
    }
    /**
     * @endcond
     */

private:
    // Doing a little SFINAE to check if a normalize() method is available.
    template<class Test_, typename = int>
    struct has_normalize {
        static constexpr bool value = false;
    };

    template<class Test_>
    struct has_normalize<Test_, decltype((void) Test_::normalize(0), 0)> {
        static constexpr bool value = true;
    };

public:
    void search(Index_ i, Index_ k, std::vector<Index_>* output_indices, std::vector<Float_>* output_distances) {
        my_buffer = my_parent->my_index.getDataByLabel<InternalData_>(i);
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
        while (!my_queue.empty()) {
            const auto& top = my_queue.top();
            if (!self_found && top.second == i) {
                self_found = true;
            } else {
                if (output_indices) {
                    output_indices->push_back(top.second);
                }
                if (output_distances) {
                    output_distances->push_back([&](){
                        if constexpr(has_normalize<Distance_>::value) {
                            return Distance_::normalize(top.first);
                        } else {
                            return top.first;
                        }
                    }());
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
    }

private:
    void search_raw(const InternalData_* query, Index_ k, std::vector<Index_>* output_indices, std::vector<Float_>* output_distances) {
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
                (*output_distances)[position] = [&](){
                    if constexpr(has_normalize<Distance_>::value) {
                        return Distance_::normalize(top.first);
                    } else {
                        return top.first;
                    }
                }();
            }
            my_queue.pop();
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
 * @tparam Distance_ Subclass of a `hnswlib::SpaceInterface<InternalData_>`, see comments in `HnswBuilder`.
 * @tparam Dim_ Integer type for the number of dimensions.
 * For the output of `HnswBuilder::build_raw()`, this is set to `Matrix_::dimension_type`.
 * @tparam Index_ Integer type for the indices.
 * For the output of `HnswBuilder::build_raw()`, this is set to `Matrix_::index_type`.
 * @tparam Float_ Floating point type for the query data and output distances.
 * @tparam InternalIndex_ Integer type for the internal indices in Hnsw.
 * @tparam InternalData_ Floating point type for the internal data in Hnsw.
 */
template<class Distance_, typename Dim_, typename Index_, typename Float_, typename InternalIndex_, typename InternalData_>
class HnswPrebuilt : public knncolle::Prebuilt<Dim_, Index_, Float_> {
public:
    /**
     * @cond
     */
    template<class Matrix_>
    HnswPrebuilt(const Matrix_& data, const HnswOptions& options) :
        my_dim(data.num_dimensions()),
        my_obs(data.num_observations()),
        my_space(my_dim), 
        my_index(&my_space, my_obs, options.num_links, options.ef_construction),
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
    Distance_ my_distance;
    hnswlib::HierarchicalNSW<InternalData_> my_index;

    friend class HnswSearcher<Distance_, Dim_, Index_, Float_, InternalIndex_, InternalData_>;

public:
    Dim_ num_dimensions() const {
        return my_dim;
    }

    Index_ num_observations() const {
        return my_obs;
    }

    std::unique_ptr<knncolle::Searcher<Index_, Float_> > initialize() const {
        return std::make_unique<HnswSearcher<Distance_, Dim_, Index_, Float_, InternalIndex_, InternalData_> >(this);
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
 * @tparam Distance_ Subclass of a `hnswlib::SpaceInterface<InternalData_>` to compute the distance between vectors.
 * The constructor of this subclass should accept a single argument containing the number of dimensions.
 * The class may also implement a static `normalize()` method to convert relative distances from `get_dist_func()` into actual distances;
 * if no such method is available, it is assumed that the reported distances are already absolute.
 * @tparam Matrix_ Matrix-like object satisfying the `knncolle::MockMatrix` interface.
 * @tparam Float_ Floating point type for the query data and output distances.
 * @tparam InternalIndex_ Integer type for the internal indices in Hnsw.
 * @tparam InternalData_ Floating point type for the internal data in Hnsw.
 * This defaults to a `float` instead of a `double` to sacrifice some accuracy for performance.
 */
template<
    class Distance_ = EuclideanFloatDistance,
    class Matrix_ = knncolle::SimpleMatrix<int, int, double>, 
    typename Float_ = double, 
    typename InternalIndex_ = typename Matrix_::index_type, 
    typename InternalData_ = float>
class HnswBuilder : public knncolle::Builder<Matrix_, Float_> {
private:
    HnswOptions my_options;

public:
    /**
     * @param options Further options for Hnsw index construction and searching.
     */
    HnswBuilder(HnswOptions options) : my_options(std::move(options)) {}

    /**
     * Default constructor.
     */
    HnswBuilder() = default;

public:
    knncolle::Prebuilt<typename Matrix_::dimension_type, typename Matrix_::index_type, Float_>* build_raw(const Matrix_& data) const {
        return new HnswPrebuilt<Distance_, typename Matrix_::dimension_type, typename Matrix_::index_type, Float_, InternalIndex_, InternalData_>(data, my_options);
    }
};

}

#endif
