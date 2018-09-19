New-Item -ItemType Directory -Name Release

$sfmlBinPath = "C:\ThirdParty\SFML-2.5.0\bin"
Copy-Item -Path $sfmlBinPath\sfml-graphics-2.dll -Destination .\Release
Copy-Item -Path $sfmlBinPath\sfml-window-2.dll -Destination .\Release
Copy-Item -Path $sfmlBinPath\sfml-system-2.dll -Destination .\Release
Copy-Item -Path .\App.exe -Destination .\Release

if (Test-Path -Path .\Release.zip)
{
  Remove-Item -Path .\Release.zip 
}

Compress-Archive -Path .\Release\* -DestinationPath .\Release.zip

Remove-Item -Path .\Release -Recurse