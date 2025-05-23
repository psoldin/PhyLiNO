option(MPI "MPI" ${_MPI})

if(NOT ${MPI})
 message(STATUS "MPI has been DISABLED!")
else()
	#message(STATUS "Searching for MPI")
  	## search for mpiicpc/mpiicc first
  	#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  	#  find_program(MPIICPC mpiicpc)
  	#  if(NOT "${MPIICPC}" STREQUAL "MPIICPC-NOTFOUND")
  	#    set(MPI_CXX_COMPILER ${MPIICPC})
  	#  endif()
  	#  find_program(MPIICC mpiicc)
  	#  if(NOT "${MPIICC}" STREQUAL "MPIICC-NOTFOUND")
  	#    set(MPI_C_COMPILER ${MPIICC})
  	#  endif()
  	#endif()
  	#FIND_PACKAGE(MPI)
  	#if (MPI_FOUND)
  	#  message(STATUS "Searching for MPI - found in ${MPI_LIBRARY}")
  	#  add_definitions(-DENABLE_MPI)
  	#  # check if MPI is intelmpi
  	#  if("$ENV{MPI_NAME}" STREQUAL "intelmpi")
  	#    add_definitions(-DMPICH_IGNORE_CXX_SEEK)
  	#  endif()
  	#endif()
  	#if (MPI_CXX_FOUND)
  	#  set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
  	#  set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS})
  	#  set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_CXX_LINK_FLAGS})
  	#  include_directories(${MPI_CXX_INCLUDE_PATH})
  	#endif()
  	#if (MPI_C_FOUND)
  	#  set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
  	#  set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_C_COMPILE_FLAGS})
  	#  set(CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS} ${MPI_C_LINK_FLAGS})
  	#  include_directories(${MPI_C_INCLUDE_PATH})
  	#endif()
  find_package(MPI REQUIRED)
  set(CMAKE_CXX_FLAG "${CMAKE_CXX_FLAG} ${MPI_COMPILE_FLAGS}")
  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS}")
  add_definitions(-DENABLE_MPI)
endif()
