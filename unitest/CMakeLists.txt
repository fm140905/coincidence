add_executable(getInputTest getInputTest.cpp)
target_link_libraries(getInputTest PUBLIC gtest_main getInput)
set_target_properties(getInputTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(
    NAME getInputTest
    COMMAND getInputTest
    )
    
add_executable(loadChannelTest loadChannelTest.cpp)
target_link_libraries(loadChannelTest PUBLIC gtest_main getInput loadEvents loadChannel)
set_target_properties(loadChannelTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(
    NAME loadChannelTest
    COMMAND loadChannelTest
)

add_executable(loadCoincidencesTest loadCoincidencesTest.cpp)
target_link_libraries(loadCoincidencesTest PUBLIC gtest_main getInput loadEvents loadChannel loadCoincidences)
set_target_properties(loadCoincidencesTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(
    NAME loadCoincidencesTest
    COMMAND loadCoincidencesTest
)