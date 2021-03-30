# add_executable(RootSimpleExample SimpleExample.cpp)
# target_link_libraries(RootSimpleExample PUBLIC ROOT::Physics)

add_library(getInput STATIC getInput.cpp)
target_link_libraries(getInput PUBLIC ROOT::Physics)

add_library(loadEvents STATIC loadEvents.cpp)
target_link_libraries(loadEvents PUBLIC getInput ROOT::Gpad)
# target_link_libraries(loadEvents PUBLIC getInput ROOT::Core ROOT::Gpad 
                    #   ROOT::Graf3d ROOT::Graf ROOT::Hist ROOT::Imt 
                    #   ROOT::MathCore ROOT::Matrix ROOT::MultiProc 
                    #   ROOT::Net ROOT::Physics ROOT::Postscript 
                    #   ROOT::RIO ROOT::ROOTDataFrame ROOT::ROOTVecOps 
                    #   ROOT::Rint ROOT::Thread ROOT::TreePlayer ROOT::Tree)

add_library(loadChannel STATIC loadChannel.cpp)
target_link_libraries(loadChannel PUBLIC getInput loadEvents ROOT::Gpad)

add_library(loadCoincidences STATIC loadCoincidences.cpp)
target_link_libraries(loadCoincidences PUBLIC getInput loadEvents loadChannel ROOT::Gpad)