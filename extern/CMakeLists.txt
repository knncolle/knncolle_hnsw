include(FetchContent)

FetchContent_Declare(
  knncolle
  GIT_REPOSITORY https://github.com/knncolle/knncolle
  GIT_TAG master # ^2.0.0
)

FetchContent_Declare(
  hnswlib
  GIT_REPOSITORY https://github.com/nmslib/hnswlib
  GIT_TAG master # ^0.8.0
)

FetchContent_MakeAvailable(knncolle)
FetchContent_MakeAvailable(hnswlib)
