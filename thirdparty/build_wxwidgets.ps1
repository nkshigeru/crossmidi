$library_name = "wxWidgets"
$generator = "Visual Studio 15 2017"
$platform = "Win32"
$source_dir = Join-Path $PSScriptRoot $library_name
$build_dir = Join-Path $PSScriptRoot "build\$platform\$library_name"
$target_dir = Join-Path $PSScriptRoot $platform

& cmake -H"$source_dir" -B"$build_dir" `
    -G"$generator" `
    -DCMAKE_GENERATOR_PLATFORM="$platform" `
    -DCMAKE_INSTALL_PREFIX="$target_dir" `
    -DCMAKE_C_FLAGS_DEBUG="/MTd" `
    -DCMAKE_CXX_FLAGS_DEBUG="/MTd" `
    -DCMAKE_C_FLAGS_RELEASE="/MT" `
    -DCMAKE_CXX_FLAGS_RELEASE="/MT" `
    -DwxBUILD_SHARED=OFF `
    
& cmake --build "$build_dir" --target INSTALL --config Debug
& cmake --build "$build_dir" --target INSTALL --config Release
