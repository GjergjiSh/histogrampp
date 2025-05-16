include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(histogram_tests
    histogram_tests.cpp
)

target_link_libraries(histogram_tests PRIVATE histogram gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(histogram_tests)