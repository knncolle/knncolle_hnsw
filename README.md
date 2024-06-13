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
#include "knncolle/knncolle_hnsw.hpp"

// Wrap our data in a light SimpleMatrix.
knncolle::SimpleMatrix<int, int, double> mat(ndim, nobs, matrix.data());

// Build a HNSW index. 
knncolle_hnsw::HnswBuilder<> an_builder;
auto an_index = an_builder.build_unique(mat);

// Find 10 (approximate) nearest neighbors of every element.
auto results = knncolle::find_nearest_neighbors(*an_index, 10); 
```

We could alternate between exact and approximate searches at run-time:

```cpp
std::unique_ptr<knncolle::Prebuilt<int, int, double> > ptr;
if (use_exact) {
    knncolle::KmknnBuilder<> kbuilder;
    ptr = kbuilder.build_unique(mat);
} else {
    knncolle::HnswBuilder<> abuilder;
    ptr = abuilder.build_unique(mat);
}
```

We can also customize the construction of the `HnswBuilder` by passing in options:

```cpp
auto& an_opts = an_builder.get_options();
an_opts.num_links = 100;
an_opts.distance_options.create = [](int dim) -> hnswlib::SpaceInterface<float>* {
    return new knncolle_hnsw::ManhattanDistance<float>(dim);
};
knncolle_hnsw::HnswBuilder<> an_builder2(an_opts);
```

Check out the [reference documentation](https://knncolle.github.io/knncolle_hnsw/) for more details.

## Building projects 

### CMake with `FetchContent`

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  knncolle
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

By default, this will use `FetchContent` to fetch all external dependencies.
If you want to install them manually, use `-DKNNCOLLE_HNSW_FETCH_EXTERN=OFF`.
See [`extern/CMakeLists.txt`](extern/CMakeLists.txt) to find compatible versions of each dependency.

### Manual

If you're not using CMake, the simple approach is to just copy the files in `include/` - either directly or with Git submodules - and include their path during compilation with, e.g., GCC's `-I`.
This requires the external dependencies listed in [`extern/CMakeLists.txt`](extern/CMakeLists.txt), which also need to be made available during compilation.

## Note on vectorization

HNSW will attempt to perform manual vectorization based on SSE and/or AVX instructions.
This may result in differences in the results across machines due to changes in numeric precision across architectures with varying support for SSE/AVX intrinsics.
For the most part, such differences can be avoided by consistently compiling for the "near-lowest common denominator" (such as the typical `x86-64` default for GCC and Clang) 
and ensuring that the more specific instruction subsets like SSE3 and AVX are not enabled (which are typically off by default anyway).
Nonetheless, if reproducibility across architectures is important, it can be achieved at the cost of some speed by defining the `NO_MANUAL_VECTORIZATION` macro,
which will instruct Hnsw to disable its vectorized optimizations.
