#ifndef KNNCOLLE_HNSW_DISTANCES_HPP
#define KNNCOLLE_HNSW_DISTANCES_HPP

#include <cmath>
#include <functional>

/**
 * @file distances.hpp
 * @brief Distance classes for HNSW.
 */

namespace knncolle_hnsw {

/**
 * @brief Distance options for the HNSW index.
 *
 * @tparam Dim_ Integer type for the number of dimensions.
 * @tparam InternalData_ Floating point type for the HNSW index.
 */
template<typename Dim_, typename InternalData_>
struct DistanceOptions {
    /**
     * Create a `hnswlib::SpaceInterface` object, given the number of dimensions.
     * If not provided, this defaults to `hnswlib::L2Space` if `InternalData_ = float`,
     * otherwise it defaults to `SquaredEuclideanDistance`.
     */
    std::function<hnswlib::SpaceInterface<InternalData_>*(Dim_)> create;

    /**
     * Normalization function to convert distance measures from `hnswlib::SpaceInterface::get_dist_func()` into actual distances.
     * If not provided and `create` is also provided, this defaults to a no-op.
     * If not provided and `create` is not provided, this defaults to the square root function (i.e., to convert the L2 norm to a Euclidean distance).
     */
    std::function<InternalData_(InternalData_)> normalize;
};

/**
 * @brief Manhattan distance. 
 *
 * @tparam InternalData_ Type of data in the HNSW index, usually floating-point.
 */
template<typename InternalData_>
class ManhattanDistance : public hnswlib::SpaceInterface<InternalData_> {
private:
    size_t my_data_size;
    size_t my_dim;

public:
    /**
     * @param dim Number of dimensions over which to compute the distance.
     */
    ManhattanDistance(size_t dim) : my_data_size(dim * sizeof(InternalData_)), my_dim(dim) {}

    /**
     * @cond
     */
public:
    size_t get_data_size() {
        return my_data_size;
    }

    hnswlib::DISTFUNC<InternalData_> get_dist_func() {
        return L1;
    }

    void * get_dist_func_param() {
        return &my_dim;
    }

private:
    static InternalData_ L1(const void *pVect1v, const void *pVect2v, const void *qty_ptr) {
        const InternalData_* pVect1 = static_cast<const InternalData_*>(pVect1v);
        const InternalData_* pVect2 = static_cast<const InternalData_*>(pVect2v);
        size_t qty = *((size_t *) qty_ptr);
        InternalData_ res = 0;
        for (; qty > 0; --qty, ++pVect1, ++pVect2) {
            res += std::abs(*pVect1 - *pVect2);
        }
        return res;
    }
    /**
     * @endcond
     */
};

/**
 * @brief Squared Euclidean distance. 
 *
 * @tparam InternalData_ Type of data in the HNSW index, usually floating-point.
 */
template<typename InternalData_>
class SquaredEuclideanDistance : public hnswlib::SpaceInterface<InternalData_> {
private:
    size_t my_data_size;
    size_t my_dim;

public:
    /**
     * @param dim Number of dimensions over which to compute the distance.
     */
    SquaredEuclideanDistance(size_t dim) : my_data_size(dim * sizeof(InternalData_)), my_dim(dim) {}

    /**
     * @cond
     */
public:
    size_t get_data_size() {
        return my_data_size;
    }

    hnswlib::DISTFUNC<InternalData_> get_dist_func() {
        return L2;
    }

    void * get_dist_func_param() {
        return &my_dim;
    }

private:
    static InternalData_ L2(const void *pVect1v, const void *pVect2v, const void *qty_ptr) {
        const InternalData_* pVect1 = static_cast<const InternalData_*>(pVect1v);
        const InternalData_* pVect2 = static_cast<const InternalData_*>(pVect2v);
        size_t qty = *((size_t *) qty_ptr);
        InternalData_ res = 0;
        for (; qty > 0; --qty, ++pVect1, ++pVect2) {
            auto delta = *pVect1 - *pVect2;
            res += delta * delta;
        }
        return res;
    }
    /**
     * @endcond
     */
};

}

#endif
