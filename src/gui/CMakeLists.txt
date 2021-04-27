#---Create a shared library with geneated dictionary
add_library(gui SHARED gui.cpp)
ROOT_GENERATE_DICTIONARY(G__gui ../../include/gui.h 
                         LINKDEF ../../include/guiLinkDef.h
                         MODULE gui)
target_link_libraries(gui PUBLIC getInput ROOT::Gui)

#---Create a main program using the library
add_executable(demo demo.cpp)
target_link_libraries(demo PUBLIC gui ROOT::Gui)
