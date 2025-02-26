enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(rbtree_tests
    testing.cpp
    ../include/rbtree.hpp
)

target_link_libraries(rbtree_tests
    Tree_lib
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(rbtree_tests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

gtest_discover_tests(rbtree_tests)