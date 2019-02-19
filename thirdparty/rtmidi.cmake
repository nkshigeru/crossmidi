add_library(rtmidi STATIC
  ${CMAKE_CURRENT_LIST_DIR}/rtmidi/RtMidi.h
  ${CMAKE_CURRENT_LIST_DIR}/rtmidi/RtMidi.cpp
)
target_compile_definitions(rtmidi
  PUBLIC __WINDOWS_MM__
)
target_include_directories(rtmidi
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/rtmidi
)
target_link_libraries(rtmidi
  Winmm.lib
)
