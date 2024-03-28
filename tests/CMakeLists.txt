add_executable(enclosed_mutex_tests test.cpp)
target_link_libraries(enclosed_mutex_tests enclosed_mutex_lib GTest::GTest GTest::Main)
gtest_discover_tests(enclosed_mutex_tests)
