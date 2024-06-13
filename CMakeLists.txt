cmake_minimum_required(VERSION 3.14)

project(knncolle_hnsw
    VERSION 0.1.0
    DESCRIPTION "knncolle bindings for HNSW search"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Library
add_library(knncolle_hnsw INTERFACE)
add_library(knncolle::knncolle_hnsw ALIAS knncolle_hnsw)

target_include_directories(knncolle_hnsw INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/knncolle_hnsw>)
target_compile_features(knncolle_hnsw INTERFACE cxx_std_17)

# Dependencies
option(KNNCOLLE_HNSW_FETCH_EXTERN "Automatically fetch knncolle_hnsw's external dependencies." ON)
if(KNNCOLLE_HNSW_FETCH_EXTERN)
    add_subdirectory(extern)
else()
    find_package(knncolle_knncolle 2.0.0 CONFIG REQUIRED)
    find_package(hnswlib CONFIG REQUIRED)
endif()

target_link_libraries(knncolle_hnsw INTERFACE knncolle::knncolle Annoy::Annoy)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(KNNCOLLE_HNSW_TESTS "Build knncolle_hnsw's test suite." ON)
else()
    option(KNNCOLLE_HNSW_TESTS "Build knncolle_hnsw's test suite." OFF)
endif()

if(KNNCOLLE_HNSW_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif() 
endif()

# Install
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/knncolle_hnsw)

install(TARGETS knncolle_hnsw
    EXPORT knncolle_hnswTargets)

install(EXPORT knncolle_hnswTargets
    FILE knncolle_knncolle_hnswTargets.cmake
    NAMESPACE knncolle::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/knncolle_knncolle_hnsw)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/knncolle_knncolle_hnswConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/knncolle_knncolle_hnsw)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/knncolle_knncolle_hnswConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/knncolle_knncolle_hnswConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/knncolle_knncolle_hnswConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/knncolle_knncolle_hnsw)
