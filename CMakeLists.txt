cmake_minimum_required(VERSION 3.3)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(PROJECT_NAME cppbc)
project(${PROJECT_NAME})

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(AFTER ${PROJECT_ROOT})

set(BC7ENC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/bc7enc)
include_directories(AFTER ${BC7ENC_ROOT})

########################################################################
# Sources
set(HEADERS
    ${PROJECT_ROOT}/cppbc.h
    ${BC7ENC_ROOT}/bc7enc.h
    ${BC7ENC_ROOT}/bc7decomp.h
    ${BC7ENC_ROOT}/lodepng.h)

set(SOURCES
    ${PROJECT_ROOT}/cppbc.cpp
    ${BC7ENC_ROOT}/bc7enc.c
    ${BC7ENC_ROOT}/bc7decomp.cpp
    ${BC7ENC_ROOT}/lodepng.cpp)

source_group("include" FILES ${HEADERS})
source_group("src" FILES ${SOURCES} "${PROJECT_ROOT}/main.cpp")

set(FILES ${HEADERS} ${SOURCES} "${PROJECT_ROOT}/main.cpp")

set(OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

add_executable(${PROJECT_NAME} ${FILES})

if(MSVC)
    set(DEFAULT_CXX_FLAGS "/DWIN32 /D_WINDOWS /D_MBCS /W4 /WX- /nologo /fp:precise /arch:AVX /Zc:wchar_t /TP /Gd /std:c++20 /std:c11")
    if(MSVC_VERSION VERSION_LESS_EQUAL "1900")
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /Zc:__cplusplus /std:c++latest")
    else()
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /Zc:__cplusplus /std:c++20")
    endif()
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 /Gy /GR /GS /Gm- /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Oi /GL /GR /DNDEBUG /EHsc-")
    target_link_libraries(${PROJECT_NAME})

elseif(UNIX)
    set(DEFAULT_CXX_FLAGS "-Wall -O2 -std=c++20 -std=gnu++20 -march=x86-64-v3 -fno-exceptions")
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
    target_link_libraries(${PROJECT_NAME})
elseif(APPLE)
elseif(APPLE)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME_DEBUG "${PROJECT_NAME}" OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

