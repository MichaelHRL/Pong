$sfmlBinPath = "C:\ThirdParty\SFML-2.5.0\bin"
$sfmlIncludePath = "C:\ThirdParty\SFML-2.5.0\include"
$sourceFiles = Get-ChildItem -Path *.cpp

if (-not ($Env:Path.Contains($sfmlBinPath)))
{
  $Env:Path += $sfmlBinPath + ";"
}

& g++ -L($sfmlBinPath) -I($sfmlIncludePath) -lsfml-graphics-2 -lsfml-window-2 -lsfml-system-2 $sourceFiles -o App