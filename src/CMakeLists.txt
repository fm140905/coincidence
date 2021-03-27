# add_executable(RootSimpleExample SimpleExample.cpp)
# target_link_libraries(RootSimpleExample PUBLIC ROOT::Physics)

add_library(getInput STATIC getInput.cpp)
target_link_libraries(getInput PUBLIC ROOT::Physics)