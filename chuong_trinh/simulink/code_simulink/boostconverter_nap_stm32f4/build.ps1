Set-Location "D:\DO AN TOT NGIEP\code_simulink\boostconverter_nap_stm32f4"
Remove-Item waijung_hwdrvlib.o -ErrorAction SilentlyContinue
cmd /c boostconverter_nap.bat
Write-Host "`nBuild completed!" -ForegroundColor Green
