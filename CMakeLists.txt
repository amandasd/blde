cmake_minimum_required( VERSION 2.8 )

project( blde )

if (PROFILING)
   message(STATUS "Enabling time profiling")
   add_definitions(-DPROFILING)
endif ()

if (NATIVE)
   message(STATUS "Enabling OpenCL native functions")
   add_definitions(-DNATIVE)
endif ()

add_subdirectory( src )
