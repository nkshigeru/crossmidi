include(ExternalProject)
ExternalProject_Add(googletest
  URL https://github.com/google/googletest/archive/release-1.8.1.tar.gz
  URL_HASH SHA1=152b849610d91a9dfa1401293f43230c2e0c33f8
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  BUILD_BYPRODUCTS
    "<INSTALL_DIR>/lib/gtestd.lib"
    "<INSTALL_DIR>/lib/gtest.lib"
)

ExternalProject_Get_Property(googletest INSTALL_DIR)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
add_library(GTest::GTest STATIC IMPORTED)
if(MSVC)
  set_target_properties(GTest::GTest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
    IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/gtestd.lib"
    IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/gtest.lib"
  )
endif()
if(APPLE)
  set_target_properties(GTest::GTest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
    IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/libgtestd.a"
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libgtest.a"
  )
endif()
add_dependencies(GTest::GTest googletest)
