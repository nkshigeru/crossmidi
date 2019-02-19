add_executable(MIDI-moni WIN32
  App.h
  App.cpp
  InputDeviceLogFrame.h
  InputDeviceLogFrame.cpp
  InputDevicePanel.h
  InputDevicePanel.cpp
  MainFrame.h
  MainFrame.cpp
)
target_link_libraries(MIDI-moni
  rtmidi
  wxWidgets::core
)
