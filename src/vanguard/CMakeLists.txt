add_library(core STATIC
    VanguardClient.cpp
)

SET_TARGET_PROPERTIES(core PROPERTIES LINK_FLAGS "/CLRTHREADATTRIBUTE:STA")


file(GLOB CLR_FILES
VanguardClient.cpp
)
set_source_files_properties(${CLR_FILES} PROPERTIES COMPILE_FLAGS "/clr /EHa /Zc:twoPhase-")
set_target_properties(core PROPERTIES VS_DOTNET_REFERENCES_COPY_LOCAL ON )
set_target_properties(core PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.1")
set_target_properties(core PROPERTIES VS_DOTNET_REFERENCE_CorruptCore "${PROJECT_SOURCE_DIR}/../../../RTCV/Build/CorruptCore.dll")
set_target_properties(core PROPERTIES VS_DOTNET_REFERENCE_NetCore "${PROJECT_SOURCE_DIR}/../../../RTCV/Build/NetCore.dll")
set_target_properties(core PROPERTIES VS_DOTNET_REFERENCE_RTCVCommon "${PROJECT_SOURCE_DIR}/../../../RTCV/Build/RTCV.Common.dll")
set_target_properties(core PROPERTIES VS_DOTNET_REFERENCE_Vanguard "${PROJECT_SOURCE_DIR}/../../../RTCV/Build/Vanguard.dll")
set_target_properties(core PROPERTIES VS_DOTNET_REFERENCES "System;System.Windows.Forms")
string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

if (ENABLE_FFMPEG_VIDEO_DUMPER)
    target_sources(core PRIVATE
        dumping/ffmpeg_backend.cpp
        dumping/ffmpeg_backend.h
    )
endif()

create_target_directory_groups(core)

target_link_libraries(core PUBLIC common PRIVATE audio_core network video_core)
target_link_libraries(core PUBLIC Boost::boost PRIVATE cryptopp fmt open_source_archives Boost::serialization)

if (ENABLE_WEB_SERVICE)
    get_directory_property(OPENSSL_LIBS
        DIRECTORY ${PROJECT_SOURCE_DIR}/externals/libressl
        DEFINITION OPENSSL_LIBS)

    target_compile_definitions(core PRIVATE -DENABLE_WEB_SERVICE -DCPPHTTPLIB_OPENSSL_SUPPORT)
    target_link_libraries(core PRIVATE web_service ${OPENSSL_LIBS} httplib lurlparser)
    if (ANDROID)
        target_link_libraries(core PRIVATE ifaddrs)
    endif()
endif()

if (ARCHITECTURE_x86_64)
    target_sources(core PRIVATE
        arm/dynarmic/arm_dynarmic.cpp
        arm/dynarmic/arm_dynarmic.h
        arm/dynarmic/arm_dynarmic_cp15.cpp
        arm/dynarmic/arm_dynarmic_cp15.h
    )
    target_link_libraries(core PRIVATE dynarmic)
endif()

if (ENABLE_FFMPEG_VIDEO_DUMPER)
    target_link_libraries(core PUBLIC FFmpeg::avcodec FFmpeg::avformat FFmpeg::swscale FFmpeg::swresample FFmpeg::avutil)
endif()
