cmake_minimum_required(VERSION 2.8)

### Specification of build type (Release or Debug) - uncomment the wanted type.
SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

### Specification of the build execution depending on the host operating system
IF (${APPLE})	# if the kernel is Darwin's
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -I/usr/local/include -L/usr/local/lib")
ELSE ()			# if the kernel is not Darwin's
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
ENDIF ()

### Specification of build flags, according to the chosen build type
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

### Specification of BASE_DIR variable
SET (BASE_DIR ${CMAKE_SOURCE_DIR} )

### Setting output directory (of built binaries) in the same level as BASE_DIR, within a bin/tools directory
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../../bin/tools)

### Add a new and own-made library
ADD_LIBRARY(own_libraries  ${BASE_DIR}/../../lib/utils.cpp)

### Setting multiple directories with project's inclusions, such as include and lib directories
INCLUDE_DIRECTORIES(${BASE_DIR}/../../include/ ${BASE_DIR}/../../lib/)

### Marking wavhist.cpp final binary as wavhist
add_executable (wavhist wavhist.cpp)

### Targeting libraries to further link with wavcp executable
target_link_libraries (wavhist sndfile boost_program_options boost_filesystem boost_iostreams boost_system own_libraries)

