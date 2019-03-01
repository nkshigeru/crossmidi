add_executable(MIDI-chrono WIN32
  App.h
  App.cpp
  BPMPanel.h
  BPMPanel.cpp
  Chrono.h
  Chrono.cpp
  ChronoFrame.h
  ChronoFrame.cpp
  DigitPanel.h
  DigitPanel.cpp
  TickPanel.h
  TickPanel.cpp
  TimeCodePanel.h
  TimeCodePanel.cpp
)
target_link_libraries(MIDI-chrono
  rtmidi
  wxWidgets::core
)
install(TARGETS MIDI-chrono
  RUNTIME DESTINATION .
)
