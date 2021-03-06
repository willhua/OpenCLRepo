# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

include_directories(common/)
include_directories(TestModules/)
include_directories(qualcomm/)
add_library(libOpenCL SHARED IMPORTED)
set_target_properties(libOpenCL PROPERTIES IMPORTED_LOCATION  ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libOpenCL.so)


include_directories(F:/opencv_401/build_arm_v7_v8/install_NOopencl_NOneon_NOtbb_WITHnonfree/sdk/native/jni/include)
set(OpenCV_DIR F:/opencv_401/build_arm_v7_v8/install_NOopencl_NOneon_NOtbb_WITHnonfree/sdk/native/jni)
find_package(OpenCV REQUIRED)


aux_source_directory(qualcomm/util QCOM)


add_library( # Sets the name of the library.
        native-lib

        SHARED

        native-lib.cpp)



target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        android
        log
        ${OpenCV_LIBS}
        QcomOpenCLUtils
        ion
        fisheyeCamera
        remap
        )


#################build common
add_library(common SHARED common/cl_utils.cpp common/FileUtils.cpp)
target_link_libraries(common android log)


#################build libion
add_library(ion SHARED ion/ion.c)
target_link_libraries(ion log)

#################build libQcomOpenCLUtils
aux_source_directory(qualcomm/utils QcomOpenCLUtilsSrc)
add_library(QcomOpenCLUtils SHARED ${QcomOpenCLUtilsSrc})
target_link_libraries(QcomOpenCLUtils log libOpenCL ion)


#################fisheyeCamera
add_library(fisheyeCamera SHARED TestModules/FisheyeCamera/fisheye.cpp)
target_link_libraries(fisheyeCamera ${OpenCV_LIBS} common)

################remap
add_library(remap SHARED TestModules/Remap/remaper.cpp)
target_link_libraries(remap log libOpenCL QcomOpenCLUtils common)