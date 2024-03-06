add_library(bst bst.hpp)

add_subdirectory(iterator)

set_target_properties(bst PROPERTIES LINKER_LANGUAGE CXX)