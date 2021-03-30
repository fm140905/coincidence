add_executable(getInputTest getInputTest.cpp)
target_link_libraries(getInputTest PUBLIC gtest_main getInput)

add_test(
    NAME getInputTest
    COMMAND getInputTest
    )
    
add_executable(loadChannelTest loadChannelTest.cpp)
target_link_libraries(loadChannelTest PUBLIC gtest_main getInput loadEvents loadChannel)

add_test(
    NAME loadChannelTest
    COMMAND loadChannelTest
)

add_executable(loadCoincidencesTest loadCoincidencesTest.cpp)
target_link_libraries(loadCoincidencesTest PUBLIC gtest_main getInput loadEvents loadChannel loadCoincidences)

add_test(
    NAME loadCoincidencesTest
    COMMAND loadCoincidencesTest
)