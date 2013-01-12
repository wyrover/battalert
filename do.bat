@echo off
::
:: Author     : Jean-Charles Lefebvre
:: Created On : 2013-01-10 17:32:56
::
:: $Id$
::

mingw32-make --version > nul 2>&1
if "%ERRORLEVEL%"=="0" goto __GO

c:\mingw\bin\mingw32-make --version > nul 2>&1
if "%ERRORLEVEL%"=="0" (
  set PATH=c:\mingw\bin;%PATH%
  goto __GO
)

echo ** ERROR: Failed to find mingw32-make!
pause
goto :EOF

:__GO
mingw32-make -f Makefile %*
exit /b %ERRORLEVEL%
