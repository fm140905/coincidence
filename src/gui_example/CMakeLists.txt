#---Create a shared library with geneated dictionary
add_library(example SHARED example.cpp)
ROOT_GENERATE_DICTIONARY(G__example ../../include/example.h 
                         LINKDEF ../../include/exampleLinkDef.h
                         MODULE example)
target_link_libraries(example PUBLIC ROOT::Gui)

#---Create a main program using the library
add_executable(demo demo.cpp)
target_link_libraries(demo PUBLIC example ROOT::Gui)