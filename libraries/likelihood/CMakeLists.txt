set(files
    Fitter.h
    Fitter.cpp
    DCParameterWrapper.h
    DCParameterWrapper.cpp
    Likelihood.h
    Likelihood.cpp
    Oscillator.cpp
    Oscillator.h
    DoubleChooz/BackgroundBase.h
)

add_library(likelihood SHARED ${files})

target_include_directories(likelihood PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(likelihood PUBLIC utilities io)
target_include_directories(likelihood PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(likelihood PRIVATE ${ROOT_INCLUDE_DIR})

target_link_libraries(likelihood PUBLIC utilities ${Boost_LIBRARIES} io utilities ROOT::Core ROOT::Tree ROOT::TreePlayer)

set_target_properties(likelihood PROPERTIES LINKER_LANGUAGE CXX)