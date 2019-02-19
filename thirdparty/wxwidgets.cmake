if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(POSTFIX d)
endif()

include(ExternalProject)
ExternalProject_Add(wxWidgets
  URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxWidgets-3.1.2.tar.bz2 
  URL_HASH SHA1=29cbbba946d2a7b4d28ca1db12315810cc8de74d
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_C_FLAGS_DEBUG="/MTd"
    -DCMAKE_CXX_FLAGS_DEBUG="/MTd"
    -DCMAKE_C_FLAGS_RELEASE="/MT"
    -DCMAKE_CXX_FLAGS_RELEASE="/MT"
    -DwxBUILD_SHARED=OFF
  BUILD_BYPRODUCTS
    "<INSTALL_DIR>/lib/vc_lib/wxbase31u${POSTFIX}.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxmsw31u${POSTFIX}_core.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxpng${POSTFIX}.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxzlib${POSTFIX}.lib"
)

ExternalProject_Get_Property(wxWidgets INSTALL_DIR)
set(wxWidgets_INCLUDE_DIRS
  ${INSTALL_DIR}/include
  ${INSTALL_DIR}/lib/vc_lib/mswu${POSTFIX})
foreach(V ${wxWidgets_INCLUDE_DIRS})
  file(MAKE_DIRECTORY ${V})
endforeach()
add_library(wxWidgets::wxzlib STATIC IMPORTED)
set_target_properties(wxWidgets::wxzlib PROPERTIES
  IMPORTED_LOCATION "${INSTALL_DIR}/lib/vc_lib/wxzlib${POSTFIX}.lib"
)
add_library(wxWidgets::wxpng STATIC IMPORTED)
set_target_properties(wxWidgets::wxpng PROPERTIES
  IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/vc_lib/wxpngd.lib"
  IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/vc_lib/wxpng.lib"
  INTERFACE_LINK_LIBRARIES "wxWidgets::wxzlib"
)
add_library(wxWidgets::base STATIC IMPORTED)
set_target_properties(wxWidgets::base PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${wxWidgets_INCLUDE_DIRS}"
  IMPORTED_LOCATION "${INSTALL_DIR}/lib/vc_lib/wxbase31u${POSTFIX}.lib"
)
add_library(wxWidgets::core STATIC IMPORTED)
set_target_properties(wxWidgets::core PROPERTIES
  IMPORTED_LOCATION "${INSTALL_DIR}/lib/vc_lib/wxmsw31u${POSTFIX}_core.lib"
  INTERFACE_LINK_LIBRARIES "wxWidgets::base;wxWidgets::wxpng;Comctl32.lib;Rpcrt4.lib"
)
add_dependencies(wxWidgets::base wxWidgets)
