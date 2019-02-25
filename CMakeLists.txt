cmake_minimum_required(VERSION 3.)
project(opt)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(Path)
include(Compiler)
include(External)

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



