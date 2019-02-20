add_executable(test_midi_moni
  test.cpp
)
target_link_libraries(test_midi_moni
  GTest::GTest
)

add_test(
  NAME test_midi_moni
  COMMAND $<TARGET_FILE:test_midi_moni>
)
