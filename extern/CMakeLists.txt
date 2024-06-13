include(FetchContent)

FetchContent_Declare(
  hnswlib
  GIT_REPOSITORY https://github.com/nmslib/hnswlib
  GIT_TAG master # ^0.8.0
)

FetchContent_MakeAvailable(hnswlib)
