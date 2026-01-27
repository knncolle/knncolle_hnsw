# knncolle bindings for HNSW

![Unit tests](https://github.com/knncolle/knncolle_hnsw/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/knncolle/knncolle_hnsw/actions/workflows/doxygenate.yaml/badge.svg)
[![Codecov](https://codecov.io/gh/knncolle/knncolle_hnsw/branch/master/graph/badge.svg)](https://codecov.io/gh/knncolle/knncolle_hnsw)

## Overview

The **knncolle_hnsw** library provides [**knncolle**](https://github.com/knncolle/knncolle) bindings to 
the [HNSW](https://github.com/nmslib/hnswlib) library for approximate nearest neighbors search.
This allows users to use HNSW searches in any framework that accepts **knncolle** interfaces, sacrificing neighbor search accuracy for greater speed.
For most applications involving large datasets, this is an acceptable trade-off.

## Quick start

Instances of the various `knncolle_hnsw::Hnsw*` classes can be used anywhere that accepts the corresponding **knncolle** interface.
For example:

```cpp
#include "knncolle_hnsw/knncolle_hnsw.hpp"

int ndim = 10;
int nobs = 10000;
std::vector<double> matrix(ndim * nobs); // column-major ndim * nobs matrix.

// Wrap our data in a light SimpleMatrix.
knncolle::SimpleMatrix<int, double> mat(ndim, nobs, matrix.data());

// Build a HNSW index, defaulting to Euclidean distances.
auto euclid_config = knncolle_hnsw::configure_euclidean_distance<double>();
knncolle_hnsw::HnswBuilder<int, double, double> h_builder(euclid_config);
auto h_index = h_builder.build_unique(mat);

// Find 10 (approximate) nearest neighbors of every element.
auto results = knncolle::find_nearest_neighbors(*h_index, 10); 
```

Check out the [reference documentation](https://knncolle.github.io/knncolle_hnsw/) for more details.

## Customizing the search

We can also customize the construction of the `HnswBuilder` by passing in options or specifying a different distance metric:

```cpp
knncolle_hnsw::HnswOptions h_opts;
h_opts.num_links = 100;
knncolle_hnsw::HnswBuilder<int, double, double> h_builder2(
    knncolle_hnsw::newManhattanDistanceConfig(),
    h_opts
);
```

We could also modify the builder after construction, which will affect all subsequently constructed indices:

```cpp
auto& more_opt = h_builder.get_options()
more_opt.ef_construction = 250;
```

Advanced users can configure the template parameters to use more suitable types for their applications.
A hypothetical configuration is shown below with (mostly made up) reasons:

```cpp
typedef knncolle_annoy::HnswBuilder<
    // The type for the observation indices - perhaps int isn't big enough to
    // hold all the indices for a large dataset, so we'll use size_t.
    size_t,

    // The type for the input data, maybe we're dealing with small counts.
    uint8_t,

    // The type for the distances, maybe we'll use floats to save space.
    float,

    // The type of matrix that the AnnoyBuilder takes as input: forcing it
    // to be a SimpleMatrix to enable devirtualization.
    knncolle::SimpleMatrix<size_t, uint8_t>,

    // The type of data in the HNSW index, using floats for performance.
    float
> MyHnswBuilder;
```

## Building projects 

### CMake with `FetchContent`

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  knncolle_hnsw
  GIT_REPOSITORY https://github.com/knncolle/knncolle_hnsw
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(knncolle_hnsw)
```

Then you can link to **knncolle_hnsw** to make the headers available during compilation:

```cmake
# For executables:
target_link_libraries(myexe knncolle::knncolle_hnsw)

# For libaries
target_link_libraries(mylib INTERFACE knncolle::knncolle_hnsw)
```

By default, this will use `FetchContent` to fetch all external dependencies.
Applications are advised to pin the versions of each dependency for stability - see [`extern/CMakeLists.txt`](extern/CMakeLists.txt) for suggested versions.
If you want to install them manually, use `-DKNNCOLLE_HNSW_FETCH_EXTERN=OFF`.

### CMake with `find_package()`

```cmake
find_package(knncolle_knncolle_hnsw CONFIG REQUIRED)
target_link_libraries(mylib INTERFACE knncolle::knncolle_hnsw)
```

To install the library, use:

```sh
mkdir build && cd build
cmake .. -DKNNCOLLE_HNSW_TESTS=OFF
cmake --build . --target install
```

Again, this will automatically acquire all its dependencies, see recommendations above.

### Manual

If you're not using CMake, the simple approach is to just copy the files in `include/` - either directly or with Git submodules - and include their path during compilation with, e.g., GCC's `-I`.
This will also require the external dependencies listed in [`extern/CMakeLists.txt`](extern/CMakeLists.txt). 

## Note on vectorization

HNSW will attempt to perform manual vectorization based on SSE and/or AVX instructions.
This may result in differences in the results across machines due to changes in numeric precision across architectures with varying support for SSE/AVX intrinsics.
For the most part, such differences can be avoided by consistently compiling for the "near-lowest common denominator" (such as the typical `x86-64` default for GCC and Clang) 
and ensuring that the more specific instruction subsets like SSE3 and AVX are not enabled (which are typically off by default anyway).
Nonetheless, if reproducibility across architectures is important, it can be achieved at the cost of some speed by defining the `NO_MANUAL_VECTORIZATION` macro,
which will instruct Hnsw to disable its vectorized optimizations.
