cmake_minimum_required(VERSION 2.8)
project( DisplayImage )

MESSAGE ("\n==> Finding 3rd Party Packages")
find_package( OpenCV REQUIRED )

SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W" )
SET( CMAKE_C_FLAGS_RELEASE "-O3" )
SET( CMAKE_C_FLAGS_DEBUG "-g3" )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=c++11" )
SET( CMAKE_CXX_FLAGS_RELEASE "-O3" )
SET( CMAKE_CXX_FLAGS_DEBUG "-g3" )

SET( BASE_DIR src )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY bin )

# Make include directories available for all..
INCLUDE_DIRECTORIES( 
	${OpenCV_INCLUDE_DIR}
	${BASE_DIR}/../lib/
	${BASE_DIR}/include/
	${BASE_DIR}/coding/
	${BASE_DIR}/jpeg-ls/
	${BASE_DIR}/stages/
)

add_library(bstream ${BASE_DIR}/../lib/bstream.cpp)

add_library(extmath ${BASE_DIR}/../lib/extmath.cpp)

add_library(progress ${BASE_DIR}/../lib/progress.cpp)

add_library(golomb ${BASE_DIR}/coding/golomb.cpp)
target_link_libraries(golomb extmath)

## YUV

add_library(yuv-convert ${BASE_DIR}/yuv/convert.cpp)
target_link_libraries(yuv-convert ${OpenCV_LIBS})

add_library(yuv-info ${BASE_DIR}/yuv/info.cpp)

## JPEG-LS (Stage 2)

add_library(jpeg-ls-gradient ${BASE_DIR}/jpeg-ls/gradient.cpp)
target_link_libraries(jpeg-ls-gradient ${OpenCV_LIBS})

add_library(jpeg-ls-matrix ${BASE_DIR}/jpeg-ls/matrix.cpp)
target_link_libraries(jpeg-ls-matrix ${OpenCV_LIBS})

add_library(jpeg-ls-mode ${BASE_DIR}/jpeg-ls/matrix.cpp)
target_link_libraries(jpeg-ls-mode ${OpenCV_LIBS})

add_library(jpeg-ls-predictor ${BASE_DIR}/jpeg-ls/predictor.cpp)
target_link_libraries(jpeg-ls-predictor ${OpenCV_LIBS})

add_library(jpeg-ls-rle ${BASE_DIR}/jpeg-ls/rle.cpp)
target_link_libraries(jpeg-ls-rle ${OpenCV_LIBS})

add_library(stage2 ${BASE_DIR}/stages/stage2.cpp)
target_link_libraries(stage2 jpeg-ls-gradient jpeg-ls-matrix jpeg-ls-mode jpeg-ls-predictor jpeg-ls-rle yuv-info golomb bstream progress ${OpenCV_LIBS})

## Motion Compensation (Stage 3)

add_library(motion-blocks ${BASE_DIR}/motion/blocks.cpp)
target_link_libraries(motion-blocks ${OpenCV_LIBS})

add_library(motion-matrix ${BASE_DIR}/motion/matrix.cpp)
target_link_libraries(motion-matrix ${OpenCV_LIBS})

add_library(stage3 ${BASE_DIR}/stages/stage3.cpp)
target_link_libraries(stage3 bstream golomb motion-blocks motion-matrix ${OpenCV_LIBS})

## 

#add_executable(camplayer src/camplayer.cpp)
#target_link_libraries(camplayer ${OpenCV_LIBS})

add_executable(vplayer src/vplayer.cpp)
target_link_libraries(vplayer yuv-convert yuv-info ${OpenCV_LIBS})

add_executable(vz src/vz.cpp)
target_link_libraries(vz stage2 yuv-convert yuv-info progress ${OpenCV_LIBS})
