add_executable(getInputTest getInputTest.cpp)
target_link_libraries(getInputTest PUBLIC gtest_main getInput)

add_test(
    NAME getInputTest
    COMMAND getInputTest
)