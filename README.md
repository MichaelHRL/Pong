# Dependencies
* [SFML (>= 2.5.0)](https://www.sfml-dev.org/)
* C++ compiler (>= C++11) (I used GCC 7.3.0 via [MSYS2](https://www.msys2.org/))
* [PowerShell (>= 5.1.17134.165)](https://github.com/PowerShell/PowerShell#get-powershell)
* [CMake (>= 3.12.1)](https://cmake.org/)

_The version numbers are the ones that I've used, earlier versions may be sufficient but I have not tested them._

# Note
Change the `SFMLPath` and any subsequent variables (both within `CMakeLists.txt`) so that they hold the correct paths to the SFML headeres and libraries.

# Build
Make sure you're in the Scripts folder and then enter `.\Build.ps1` in the PowerShell console. Make sure the execution policy is set to allow downloaded scripts to run. Also make sure that the paths to the SFML binaries and headers are correct.

# Run
Make sure you're in the Scripts folder and then enter `.\Run.ps1` in the PowerShell console. Make sure the execution policy is set to allow downloaded scripts to run. Also make sure that the paths to the SFML binaries and headers are correct.
