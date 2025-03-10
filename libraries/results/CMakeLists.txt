set(files
        profile_likelihood_scan.h
        perform_fit.h
        write_results.h
        write_results.cpp
)

add_library(results SHARED ${files})

target_include_directories(results PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(results PUBLIC utilities ${Boost_LIBRARIES} io utilities likelihood nlohmann_json::nlohmann_json)

set_target_properties(results PROPERTIES LINKER_LANGUAGE CXX)