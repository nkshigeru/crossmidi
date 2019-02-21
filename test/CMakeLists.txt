add_executable(test_crossmidi
  main.cpp
  test_midi_message.cpp
)
target_link_libraries(test_crossmidi
  GTest::GTest
)

add_test(
  NAME test_crossmidi
  COMMAND $<TARGET_FILE:test_crossmidi>
)
