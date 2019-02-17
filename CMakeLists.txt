cmake_minimum_required(VERSION 3.0)
project(midi-moni)

string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/thirdparty/Win32")
find_package(GTest)
find_package(wxWidgets)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(midimonilib STATIC
  MainFrame.h
  MainFrame.cpp
  InputDevicePanel.h
  InputDevicePanel.cpp
)
target_include_directories(midimonilib
  PUBLIC ${wxWidgets_INCLUDE_DIRS}
)
target_link_libraries(midimonilib
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