cmake_minimum_required(VERSION 3.0)
project(midi-moni)
enable_testing()

string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/thirdparty/Win32")
find_package(GTest)
find_package(wxWidgets)

add_library(rtmidi STATIC
  thirdparty/rtmidi/RtMidi.h
  thirdparty/rtmidi/RtMidi.cpp
)
target_compile_definitions(rtmidi
  PUBLIC __WINDOWS_MM__
)
target_include_directories(rtmidi
  PUBLIC thirdparty/rtmidi
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(midimonilib STATIC
  InputDeviceLogFrame.h
  InputDeviceLogFrame.cpp
  InputDevicePanel.h
  InputDevicePanel.cpp
  MainFrame.h
  MainFrame.cpp
)
target_include_directories(midimonilib
  PUBLIC ${wxWidgets_INCLUDE_DIRS}
)
target_link_libraries(midimonilib
  rtmidi
  ${wxWidgets_LIBRARIES}
)
add_executable(MIDI-moni WIN32
  App.h
  App.cpp
)
target_link_libraries(MIDI-moni
  midimonilib
)

add_subdirectory(test)