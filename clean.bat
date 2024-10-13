@echo off
setlocal

if exist builds rmdir builds /q /s
if exist intermediate rmdir intermediate /q /s

set DEMO_FOLDER=MFC_CEF_DEMO
if exist %DEMO_FOLDER%\include (
	for %%f in (%DEMO_FOLDER%\include\*.h) do del %%f
	for /d %%d in (%DEMO_FOLDER%\include\*) do rmdir %%d /q /s
)
if exist %DEMO_FOLDER%\lib\Win32\Debug (
	for %%f in (%DEMO_FOLDER%\lib\Win32\Debug\*.lib) do del %%f
	for %%f in (%DEMO_FOLDER%\lib\Win32\Debug\*.pdb) do del %%f
)
if exist %DEMO_FOLDER%\lib\Win32\Release (
	for %%f in (%DEMO_FOLDER%\lib\Win32\Release\*.lib) do del %%f
	for %%f in (%DEMO_FOLDER%\lib\Win32\Release\*.pdb) do del %%f
)

if exist cef_binary\Win32\builds rmdir cef_binary\Win32\builds /q /s

endlocal
@echo on
