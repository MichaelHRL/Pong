$sfmlBinPath = "C:\ThirdParty\SFML-2.5.0\bin"

if (-not ($Env:Path.Contains($sfmlBinPath)))
{
  $Env:Path += $sfmlBinPath + ";"
}

& .\App.exe