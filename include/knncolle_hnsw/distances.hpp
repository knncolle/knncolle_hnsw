#ifndef KNNCOLLE_HNSW_DISTANCES_HPP
#define KNNCOLLE_HNSW_DISTANCES_HPP

#include <cmath>

/**
 * @file distances.hpp
 * @brief Distance classes for HNSW.
 */

namespace knncolle_hnsw {

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
    ManhattanDistance(size_t dim) : my_data_size(num_dim * sizeof(InternalData_)), my_dim(ndim) {}

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
 * @brief Euclidean distance with single-precision floats. 
 */
class EuclideanFloatDistnace : public hnswlib::L2Space {
public:
    /**
     * @param dim Number of dimensions.
     */
    EuclideanFloatDistance(size_t dim) : hnswlib::L2Space(ndim) {}

    /**
     * @param raw Squared distance.
     * @return Euclidean distance.
     */
    static float normalize(float raw) {
        return std::sqrt(raw);
    }
};

}

#endif
