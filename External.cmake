
######################################################################
# EXTERNAL LIBRARIES
######################################################################
if (NOT EXTERNAL_INSTALL_LOCATION)
	set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)
endif()
if (NOT IS_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
	file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
endif()

option(BUILD_OFFLINE "Build offline" OFF)

# If offline you can set -DBUILD_OFFLINE=ON so it does not try to update stuff
if (${BUILD_OFFLINE})
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                 PROPERTY EP_UPDATE_DISCONNECTED 1)
endif()


include(ExternalProject)
# External include directory
include_directories(${EXTERNAL_INSTALL_LOCATION})

ExternalProject_Add(callable
  GIT_REPOSITORY https://github.com/sth/callable.hpp.git 
  SOURCE_DIR ${EXTERNAL_INSTALL_LOCATION}/callable
  BUILD_COMMAND ""
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND ""
)


