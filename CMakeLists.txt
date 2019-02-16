cmake_minimum_required(VERSION 3.0)
project(midi-moni)

string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

set(wxWidgets_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/thirdparty/Win32")
find_package(wxWidgets)
message(STATUS ${wxWidgets_INCLUDE_DIRS})
message(STATUS ${wxWidgets_LIBRARIES})
message(STATUS ${wxWidgets_DEFINITIONS})
message(STATUS ${wxWidgets_DEFINITIONS_DEBUG})

add_executable(MIDI-moni WIN32
  App.h
  App.cpp
  MainFrame.h
  MainFrame.cpp
  InputDevicePanel.h
  InputDevicePanel.cpp
)
target_include_directories(MIDI-moni
  PRIVATE ${wxWidgets_INCLUDE_DIRS}
)
target_link_libraries(MIDI-moni
  ${wxWidgets_LIBRARIES}
)