# Do "out-of-source" build so that the cmake generated files don't clutter the project directories
Set-Location -Path "..\Build"

cmake.exe -G "MinGW Makefiles" .. 

mingw32-make.exe

Set-Location -Path (Split-Path -Path $PSCommandPath)