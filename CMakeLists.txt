cmake_minimum_required(VERSION 3.0)
project(CrossMIDI)
enable_testing()

if(MSVC)
  string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()
if(APPLE)
  list(APPEND CMAKE_CXX_FLAGS "-stdlib=libc++")
  set(CMAKE_CXX_STANDARD 11)
endif()

include(thirdparty/googletest.cmake)
include(thirdparty/rtmidi.cmake)
include(thirdparty/wxwidgets.cmake)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(src/midi-chrono)
add_subdirectory(src/midi-moni)
add_subdirectory(test)