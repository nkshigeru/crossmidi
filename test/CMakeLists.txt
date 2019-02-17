add_executable(test_midi_moni
  test.cpp
)
target_link_libraries(test_midi_moni
  midimonilib
  GTest::GTest
)
