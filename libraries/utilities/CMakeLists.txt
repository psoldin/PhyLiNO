set(files
    FuzzyCompare.h
    )

add_library(utilities SHARED ${files})

target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(utilities PROPERTIES LINKER_LANGUAGE CXX)
