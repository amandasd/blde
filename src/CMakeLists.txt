#############################################################################
## Find the optional OpenMP libraries
#############################################################################
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#############################################################################
## Find the required POCO C++ include dir and libraries
#############################################################################
# Accepts both 'cmake -DPoco_DIR=...' (has preference) and 'export Poco_DIR=...'
if(NOT DEFINED Poco_DIR AND DEFINED ENV{Poco_DIR})
   set(Poco_DIR "$ENV{Poco_DIR}")
endif()

if(DEFINED Poco_DIR)
   if(NOT EXISTS "${Poco_DIR}")
      message(FATAL_ERROR "Poco_DIR is defined but it does not exist: ${Poco_DIR}")
   endif()

   # Try to find the path where the header Poco.h is found
   find_path(Poco_INCLUDE_DIR Poco/Poco.h PATHS "${Poco_DIR}" "${Poco_DIR}/Foundation/include")
   # Try to find the POCO C++ libraries
   find_package(Poco PATHS "${Poco_DIR}" COMPONENTS Foundation Net Util REQUIRED)
else()
   # Try to find the path where the header Poco.h is found
   find_path(Poco_INCLUDE_DIR Poco/Poco.h)
   # Try to find the POCO C++ libraries
   find_library(POCO_FOUNDATION PocoFoundation)
   find_library(POCO_NET PocoNet)
   find_library(POCO_UTIL PocoUtil)
   set(Poco_LIBRARIES ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL})
endif()

if(NOT Poco_LIBRARIES)
   message(FATAL_ERROR "Poco libraries could not be found!")
endif()
#############################################################################

# Tell CMake to use the include directory found above
include_directories( ${Poco_INCLUDE_DIR} )
#############################################################################

add_subdirectory(kernels)
add_subdirectory(util)

add_library(blde blde.cc)

add_executable(main main.cc)
target_link_libraries(main blde kernels util ${OPENCL_LIBRARIES} ${Poco_LIBRARIES} pthread )
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
