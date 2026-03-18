@echo off
cd /d "D:\DO AN TOT NGIEP\code_simulink\boostconverter_nap_stm32f4"
echo Deleting old object file...
del /Q waijung_hwdrvlib.o 2>nul
echo Building...
call boostconverter_nap.bat
echo.
echo ============================================
echo BUILD COMPLETED!
echo ============================================
echo File ready: boostconverter_nap.bin
pause
