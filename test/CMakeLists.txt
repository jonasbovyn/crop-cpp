cmake_minimum_required(VERSION 3.20)
# 'test' is the subproject name
project(crop_test)

set(CODE_DIR ../src/crop)
set(CODE_FILES
        ${CODE_DIR}/idp/utils/BitsetUtils.cpp
        ${CODE_DIR}/idp/utils/SubsetUtils.cpp
        ${CODE_DIR}/idp/EntryMetadata.cpp
        ${CODE_DIR}/idp/IdpQueryPlan.cpp
        ${CODE_DIR}/idp/IdpQueryOperator.cpp
        ${CODE_DIR}/idp/IdpFindPlan.cpp)
set(TEST_FILES
        idp/utils/SubsetUtilsTest.cpp
        idp/IdpQueryPlanTest.cpp
        idp/IdpFindPlanTest.cpp
        idp/utils/BitsetUtilsTest.cpp)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(lib/googletest/include)
include_directories(lib/googlemock/include)
include_directories(../src)

# 'test_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(test_run ${TEST_FILES} ${CODE_FILES})

target_link_libraries(test_run gtest gtest_main)
target_link_libraries(test_run gmock gmock_main)