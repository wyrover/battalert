@echo off
call PostCompile_Common.bat %*


:: example : publish executable
if exist "%dir_obj%\BattAlert\%buildtype%\*.exe" (
  copy /Y "%dir_obj%\BattAlert\%buildtype%\*.exe" "%dir_bin%"
)

:: example : publish library
if exist "%dir_obj%\BattAlert\%buildtype%\*.lib" (
  copy /Y "%dir_obj%\BattAlert\%buildtype%\*.lib" "%dir_lib%"
)

:: publish nCore dependencies if needed
if "%buildtype%"=="dll_debug"   copy /Y "%dir_dep%\nCore\sdk\msvc71\lib\nCoreD.dll" "%dir_bin%"
if "%buildtype%"=="dll_release" copy /Y "%dir_dep%\nCore\sdk\msvc71\lib\nCore.dll" "%dir_bin%"

:: remove log files
if exist "%dir_bin%\*.log*" (
  del /F /Q "%dir_bin%\*.log*"
)
