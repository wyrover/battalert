@echo off
::
:: Author     : Jean-Charles Lefebvre
:: Created On : 2013-01-10 17:32:56
::
:: $Id$
::

set MINGWMAKE=mingw32-make
"%MINGWMAKE%" --version > nul 2>&1
if "%ERRORLEVEL%"=="0" goto __GO

set MINGWMAKE=c:\mingw\bin\mingw32-make
"%MINGWMAKE%" --version > nul 2>&1
if "%ERRORLEVEL%"=="0" goto __GO

echo ** ERROR: Failed to find mingw32-make!
pause
goto :EOF

:__GO
"%MINGWMAKE%" -f Makefile %*
