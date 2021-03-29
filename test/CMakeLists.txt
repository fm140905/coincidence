add_executable(getInputTest getInputTest.cpp)
target_link_libraries(getInputTest PUBLIC gtest_main getInput)

add_executable(loadEventsTest loadEventsTest.cpp)
target_link_libraries(loadEventsTest PUBLIC gtest_main getInput loadEvents)

add_test(
    NAME getInputTest
    COMMAND getInputTest
)

add_test(
    NAME loadEventsTest
    COMMAND loadEventsTest
)