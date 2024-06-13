include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/a7f443b80b105f940225332ed3c31f2790092f47.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Avoid installing GoogleTest when installing this project.
option(INSTALL_GTEST "Enable installation of googletest." OFF)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    libtest
    src/Hnsw.cpp
)

target_link_libraries(
    libtest
    gtest_main
    knncolle_hnsw
)

target_compile_options(libtest PRIVATE -Wall -Wextra -Wpedantic)

set(CODE_COVERAGE "Enable coverage testing" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(libtest PRIVATE -O0 -g --coverage)
    target_link_options(libtest PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(libtest)
