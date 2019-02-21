add_library(rtmidi STATIC
  ${CMAKE_CURRENT_LIST_DIR}/rtmidi/RtMidi.h
  ${CMAKE_CURRENT_LIST_DIR}/rtmidi/RtMidi.cpp
)
target_include_directories(rtmidi
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/rtmidi
)
if(MSVC)
  target_compile_definitions(rtmidi
    PUBLIC __WINDOWS_MM__
  )
  target_link_libraries(rtmidi
    Winmm.lib
  )
endif()
if(APPLE)
  target_compile_definitions(rtmidi
    PUBLIC __MACOSX_CORE__
  )
  target_link_libraries(rtmidi
    "-framework CoreAudio"
    "-framework CoreMIDI"
  )
endif()