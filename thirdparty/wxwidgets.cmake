include(ExternalProject)
if(MSVC)
  set(PLATFORM_CMAKE_ARGS 
    -DCMAKE_C_FLAGS_DEBUG="/MTd"
    -DCMAKE_CXX_FLAGS_DEBUG="/MTd"
    -DCMAKE_C_FLAGS_RELEASE="/MT"
    -DCMAKE_CXX_FLAGS_RELEASE="/MT"
  )
endif()
if(APPLE)
  set(PLATFORM_CMAKE_ARGS 
    -DCMAKE_CXX_FLAGS="-stdlib=libc++"
  )
endif()
ExternalProject_Add(wxWidgets
  URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxWidgets-3.1.2.tar.bz2 
  URL_HASH SHA1=29cbbba946d2a7b4d28ca1db12315810cc8de74d
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    ${PLATFORM_CMAKE_ARGS}
    -DwxBUILD_SHARED=OFF
  BUILD_BYPRODUCTS
    "<INSTALL_DIR>/lib/vc_lib/wxbase31ud.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxbase31u.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxmsw31ud_core.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxmsw31u_core.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxpngd.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxpng.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxzlibd.lib"
    "<INSTALL_DIR>/lib/vc_lib/wxzlib.lib"
)

ExternalProject_Get_Property(wxWidgets INSTALL_DIR)
if(MSVC)
  file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
  file(MAKE_DIRECTORY ${INSTALL_DIR}/lib/vc_lib/mswud)
  file(MAKE_DIRECTORY ${INSTALL_DIR}/lib/vc_lib/mswu)
  set(wxWidgets_INCLUDE_DIRS
    ${INSTALL_DIR}/include
    $<$<CONFIG:Debug>:${INSTALL_DIR}/lib/vc_lib/mswud>
    $<$<CONFIG:Release>:${INSTALL_DIR}/lib/vc_lib/mswu>
  )
endif()
if(APPLE)
  set(wxWidgets_INCLUDE_DIRS
    ${INSTALL_DIR}/include/wx-3.1
    ${INSTALL_DIR}/lib/wx/include/osx_cocoa-unicode-static-3.1
  )
  foreach(V ${wxWidgets_INCLUDE_DIRS})
    file(MAKE_DIRECTORY ${V})
  endforeach()
endif()
add_library(wxWidgets::wxzlib STATIC IMPORTED)
if(MSVC)
  set_target_properties(wxWidgets::wxzlib PROPERTIES
    IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/vc_lib/wxzlibd.lib"
    IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/vc_lib/wxzlib.lib"
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
    IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/vc_lib/wxbase31ud.lib"
    IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/vc_lib/wxbase31u.lib"
  )
  add_library(wxWidgets::core STATIC IMPORTED)
  set_target_properties(wxWidgets::core PROPERTIES
    IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/vc_lib/wxmsw31ud_core.lib"
    IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/vc_lib/wxmsw31u_core.lib"
    INTERFACE_LINK_LIBRARIES "wxWidgets::base;wxWidgets::wxpng;Comctl32.lib;Rpcrt4.lib"
  )
endif()
if(APPLE)
  add_library(wxWidgets::wxpng STATIC IMPORTED)
  set_target_properties(wxWidgets::wxpng PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libwxpng-3.1.a"
    INTERFACE_LINK_LIBRARIES "libz.dylib"
  )
  add_library(wxWidgets::base STATIC IMPORTED)
  set_target_properties(wxWidgets::base PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${wxWidgets_INCLUDE_DIRS}"
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libwx_baseu-3.1.a"
    INTERFACE_COMPILE_DEFINITIONS "__WXMAC__;__WXOSX__;__WXOSX_COCOA__"
  )
  add_library(wxWidgets::core STATIC IMPORTED)
  set_target_properties(wxWidgets::core PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libwx_osx_cocoau_core-3.1.a"
    INTERFACE_LINK_LIBRARIES "wxWidgets::base;wxWidgets::wxpng;-framework Carbon;-framework Cocoa;-framework CoreFoundation;-framework IOKit"
  )
endif()
add_dependencies(wxWidgets::base wxWidgets)
