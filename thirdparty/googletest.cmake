if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(POSTFIX d)
endif()

include(ExternalProject)
ExternalProject_Add(googletest
  URL https://github.com/google/googletest/archive/release-1.8.1.tar.gz
  URL_HASH SHA1=152b849610d91a9dfa1401293f43230c2e0c33f8
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  BUILD_BYPRODUCTS
    "<INSTALL_DIR>/lib/gtest${POSTFIX}.lib"
)

ExternalProject_Get_Property(googletest INSTALL_DIR)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
add_library(GTest::GTest STATIC IMPORTED)
set_target_properties(GTest::GTest PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
  IMPORTED_LOCATION "${INSTALL_DIR}/lib/gtest${POSTFIX}.lib"
)
add_dependencies(GTest::GTest googletest)
