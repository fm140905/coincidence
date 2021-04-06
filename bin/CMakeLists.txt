add_executable(main main.cpp)
target_link_libraries(main PUBLIC getInput loadEvents loadChannel loadCoincidences)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")