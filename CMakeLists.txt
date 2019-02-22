cmake_minimum_required(VERSION 3.0.2)
project(opt)

###################################################################################
# COMPILER FLAGS
###################################################################################

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# Select flags.
if ( ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") )
    set(CMAKE_CXX_FLAGS         "-std=c++17 -fconcepts -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -mtune=native")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
       set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Dsrandom=srand -Drandom=rand -D_USE_MATH_DEFINES")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message("Using Visual Studio, are you sure?")
endif()

message(STATUS "Compiler  = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "System    = ${CMAKE_SYSTEM_NAME}")
message(STATUS "Prefix    = ${CMAKE_PREFIX_PATH}")
message(STATUS "Flags     = ${CMAKE_CXX_FLAGS}")
message(STATUS "Build     = ${CMAKE_BUILD_TYPE}")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
   message(STATUS "R.Flags   = ${CMAKE_CXX_FLAGS_RELEASE}")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
   message(STATUS "D.Flags   = ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   message(STATUS "D.Flags   = ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

####################################################################
# PATHS
####################################################################
if (NOT EXECUTABLE_OUTPUT_PATH)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
endif()
if (NOT LIBRARY_OUTPUT_DIRECTORY)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
endif()
if (NOT EXTERNAL_INSTALL_LOCATION)
	set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)
endif()


if (NOT IS_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
	file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()
if (NOT IS_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
	file(MAKE_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endif()
if (NOT IS_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()
if (NOT IS_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
	file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
endif()




##################################################################################
# EXTERNAL LIBRARIES
##################################################################################
option(BUILD_OFFLINE "Build offline" OFF)

# If offline you can set -DBUILD_OFFLINE=ON so it does not try to update stuff
if (${BUILD_OFFLINE})
    set_property(DIRECTORY ${opt_SOURCE_DIR}
                 PROPERTY EP_UPDATE_DISCONNECTED 1)
endif()

include(ExternalProject)
# callable_traits (useful for parameter-type deduction for general stuff)

ExternalProject_Add(callable
  GIT_REPOSITORY https://github.com/sth/callable.hpp.git 
  SOURCE_DIR ${EXTERNAL_INSTALL_LOCATION}/callable
  BUILD_COMMAND ""
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND ""
)

##################################################################################
# INCLUDE
##################################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# External include directory
include_directories(${EXTERNAL_INSTALL_LOCATION})

##########################################################################################
# USEFUL FUNCTIONS
##########################################################################################
function(add_subdirectories_and_compile directory)
	file(GLOB children ${directory}/*)
	get_filename_component(dir ${directory} NAME)
	foreach(fullchild ${children})
		get_filename_component(child ${fullchild} NAME)
		if(IS_DIRECTORY ${fullchild})
			if (EXISTS ${fullchild}/${child}.cc)
				add_executable(${dir}-${child} ${fullchild}/${child}.cc)
				add_dependencies(${dir}-${child} callable)
			else()
				add_subdirectory(${fullchild})
			endif()
		endif()
	endforeach()
endfunction(add_subdirectories_and_compile)

#############################################################################################
# TARGETS
#############################################################################################

add_subdirectories_and_compile(main/test)
add_subdirectories_and_compile(main/doc)



