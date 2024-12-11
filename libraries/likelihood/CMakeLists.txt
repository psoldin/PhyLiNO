set(files
    Fit.h
    Fit.cpp
        ParameterWrapper.h
        ParameterWrapper.cpp
    Likelihood.h
    DoubleChooz/Oscillator.cpp
    Definitions.h
    DoubleChooz/SpectrumBase.h
    DoubleChooz/Oscillator.h
    DoubleChooz/Oscillator.cpp
    DoubleChooz/DCBackground.h
    DoubleChooz/Calculate_Spectrum.h
    DoubleChooz/AccidentalBackground.cpp
    DoubleChooz/AccidentalBackground.h
    DoubleChooz/DCBackground.cpp
    DoubleChooz/SpectrumBase.h
    DoubleChooz/LithiumBackground.h
    DoubleChooz/LithiumBackground.cpp
    DoubleChooz/FastNBackground.cpp
    DoubleChooz/FastNBackground.h
    DoubleChooz/RangeOscillator.h
    DoubleChooz/ThreeFlavorOscillation.cpp
    DoubleChooz/ThreeFlavorOscillation.h
    DoubleChooz/DNCBackground.h
    DoubleChooz/DNCBackground.cpp
    DoubleChooz/ReactorSpectrum.cpp
    DoubleChooz/ReactorSpectrum.h
    DoubleChooz/EnergyCorrection.h
    DoubleChooz/EnergyCorrection.cpp
    DoubleChooz/ShapeCorrection.cpp
    DoubleChooz/ShapeCorrection.h
)

add_library(likelihood SHARED ${files})

target_include_directories(likelihood PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(likelihood PUBLIC utilities io)
target_include_directories(likelihood PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(likelihood PRIVATE ${ROOT_INCLUDE_DIR})

target_link_libraries(likelihood PUBLIC utilities ${Boost_LIBRARIES} io utilities ROOT::Core ROOT::Tree ROOT::TreePlayer)

set_target_properties(likelihood PROPERTIES LINKER_LANGUAGE CXX)
