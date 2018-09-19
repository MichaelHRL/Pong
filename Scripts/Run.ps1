$sfmlBinPath = "C:\ThirdParty\SFML-2.5.0\bin"

if (-not ($Env:Path.Contains($sfmlBinPath)))
{
  $Env:Path += $sfmlBinPath + ";"
}

Set-Location -Path "..\Build"

.\PongApplication.exe

Set-Location -Path (Split-Path -Path $PSCommandPath)