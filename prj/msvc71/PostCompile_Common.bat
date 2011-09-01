@echo off

:: constants
set dir_root=..\..
set dir_bin=%dir_root%\bin
set dir_data=%dir_root%\data
set dir_dep=%dir_root%\dep
set dir_lib=%dir_root%\lib
set dir_obj=%dir_root%\obj

:: create required directories
if not exist "%dir_bin%" mkdir "%dir_bin%"
::if not exist "%dir_lib%" mkdir "%dir_lib%"

:: parse parameters
if "%1"=="" (set buildtype=debug) else (set buildtype=%1)
