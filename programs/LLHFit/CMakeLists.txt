set(files
       LLHFit.C
)

add_executable(LLHFit ${files})

target_link_libraries(LLHFit LINK_PUBLIC ${ROOT_LIBRARIES} likelihood utilities io ROOT::RooFit ROOT::RooStats ROOT::RooFitCore)
set_target_properties(LLHFit PROPERTIES LINKER_LANGUAGE CXX)
