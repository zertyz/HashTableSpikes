# test this CMakeLists.txt in a standalone fashion as:
#     TYPE="Release"; rm -fr $TYPE; mkdir $TYPE; cd $TYPE; cmake -G Ninja -DCMAKE_BUILD_TYPE=$TYPE ..; cmake --build .; ctest; cd ..; ls -l "$TYPE/AlgorithmComplexityAndReentrancyAnalysisSpikes"
# The rules here should be used when importing this library into other projects

cmake_minimum_required (VERSION 3.9)
project                (HashTableSpikes VERSION 2018.10.10)

set(CMAKE_CXX_STANDARD 17)


#file(GLOB_RECURSE SOURCE_FILES  cpp/*.h cpp/*.cpp)
set(SOURCE_FILES cpp/HashTableSpikes.cpp)
message("Building executable './${CMAKE_BUILD_TYPE}/${PROJECT_NAME}' with: ${SOURCE_FILES}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# imported mutua libraries
##########################
# mutua libraries have the inclues inside the "cpp/" directory

# SplitRun dependency
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

foreach (_referencedLib AlgorithmComplexityAndReentrancyAnalysis CppUtils SplitRun)
	set(_referencedLib_SOURCES "../../${_referencedLib}/cpp/")
	message("    --> searching library '${_referencedLib}' in '~/.cmake/packages/${_referencedLib}' with headers in '${_referencedLib_SOURCES}'")
	if (NOT TARGET ${_referencedLib})
		find_package(${_referencedLib} CONFIG REQUIRED)
		add_library (${_referencedLib} STATIC IMPORTED GLOBAL)
		message("        (built in '${${_referencedLib}_DIR}')")
	else()
		add_library(mutua::${_referencedLib} ALIAS ${_referencedLib})
		message("        (defined by the parent CMakeLists.txt)")
	endif()
	include_directories("${_referencedLib_SOURCES}")
	target_link_libraries(${PROJECT_NAME} PRIVATE mutua::${_referencedLib})
endforeach()


# EASTL library
###############
set(EASTL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp/EASTL)
include_directories (${EASTL_ROOT_DIR}/include/)
include_directories (${EASTL_ROOT_DIR}/test/packages/EAAssert/include)
include_directories (${EASTL_ROOT_DIR}/test/packages/EABase/include/Common)
include_directories (${EASTL_ROOT_DIR}/test/packages/EAMain/include)
include_directories (${EASTL_ROOT_DIR}/test/packages/EAStdC/include)
include_directories (${EASTL_ROOT_DIR}/test/packages/EATest/include)
include_directories (${EASTL_ROOT_DIR}/test/packages/EAThread/include)
set(EASTL_LIBRARY debug ${EASTL_ROOT_DIR}/build/Debug/EASTL.lib optimized ${EASTL_ROOT_DIR}/Release/libEASTL.a)
add_custom_target(NatVis SOURCES ${EASTL_ROOT_DIR}/doc/EASTL.natvis)
target_link_libraries(${PROJECT_NAME} PRIVATE ${EASTL_LIBRARY})