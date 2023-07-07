configure_file(${CMAKE_SOURCE_DIR}/share/esp/unittest/testHarness.py
               ${CMAKE_BINARY_DIR}/share/esp/unittest/testHarness.py COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/share/esp/ESPtools.py
               ${CMAKE_BINARY_DIR}/share/esp/unittest/ESPtools.py COPYONLY)

enable_testing()
add_test(NAME PythonTests COMMAND python ${CMAKE_BINARY_DIR}/share/esp/unittest/testHarness.py)
