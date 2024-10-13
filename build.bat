@echo off
setlocal

rem Generate the Visual Studio solution.
set PLATFORM=Win32
set RELATIVE_PATH=cef_binary\%PLATFORM%
cd %RELATIVE_PATH%
if not exist builds mkdir builds
cd builds
rem Change the value of CMAKE to the actual location of your CMake program.
set CMAKE="C:\Program Files\CMake\bin\cmake.exe"
%CMAKE% -G "Visual Studio 17" -A %PLATFORM% -S ..
cd ..\..\..

rem Build the VS solution, so that libcef_dll_wrapper will be built.
rem Change the value of MSBUILD to the actual location of your MSBuild.
set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBuild.exe"
%MSBUILD% %RELATIVE_PATH%\builds\cef.sln -p:Configuration=Debug
%MSBUILD% %RELATIVE_PATH%\builds\cef.sln -p:Configuration=Release

set LIBRARY_NAME=libcef_dll_wrapper
set DEBUG_LIB="%RELATIVE_PATH%\builds\%LIBRARY_NAME%\Debug\%LIBRARY_NAME%.lib"
set DEBUG_SYMBOL="%RELATIVE_PATH%\builds\%LIBRARY_NAME%\Debug\%LIBRARY_NAME%.pdb"
set RELEASE_LIB="%RELATIVE_PATH%\builds\%LIBRARY_NAME%\Release\%LIBRARY_NAME%.lib"
set RELEASE_SYMBOL="%RELATIVE_PATH%\builds\%LIBRARY_NAME%\Release\%LIBRARY_NAME%.pdb"
set DEMO_FOLDER=MFC_CEF_Demo

rem Copy library files to application solution folders
if exist %DEBUG_LIB% copy %DEBUG_LIB%    "%DEMO_FOLDER%\lib\%PLATFORM%\Debug"
if not exist %DEBUG_LIB% echo %DEBUG_LIB% is not available.
if exist %DEBUG_SYMBOL% copy %DEBUG_SYMBOL%    "%DEMO_FOLDER%\lib\%PLATFORM%\Debug"
if not exist %DEBUG_SYMBOL% echo %DEBUG_SYMBOL% is not available.
if exist "%RELATIVE_PATH%\Debug" copy "%RELATIVE_PATH%\Debug\*.lib"  "%DEMO_FOLDER%\lib\%PLATFORM%\Debug"
if not exist "%RELATIVE_PATH%\Debug" echo Debug libraries are not available.
if exist %RELEASE_LIB% copy %RELEASE_LIB%    "%DEMO_FOLDER%\lib\%PLATFORM%\Release"
if not exist %RELEASE_LIB% echo %RELEASE_LIB% is not available.
if exist %RELEASE_SYMBOL% copy %RELEASE_SYMBOL%    "%DEMO_FOLDER%\lib\%PLATFORM%\Release"
if not exist %RELEASE_SYMBOL% echo %RELEASE_SYMBOL% is not available.
if exist "%RELATIVE_PATH%\Release" copy "%RELATIVE_PATH%\Release\*.lib"  "%DEMO_FOLDER%\lib\%PLATFORM%\Release"
if not exist "%RELATIVE_PATH%\Release" echo Release libraries are not available.

rem Copy include files to application solution include folder
if exist "%RELATIVE_PATH%\include" xcopy "%RELATIVE_PATH%\include" "%DEMO_FOLDER%\include" /e /y
if not exist "%RELATIVE_PATH%\include" echo Include files are not available.

rem build the application
%MSBUILD% MFC_CEF.sln -p:Configuration=Debug -p:Platform=x86
%MSBUILD% MFC_CEF.sln -p:Configuration=Release -p:Platform=x86

rem copy all necessary binary and resource files
if exist "%RELATIVE_PATH%\Debug" (
	copy "%RELATIVE_PATH%\Debug\*.bin"  "builds\%PLATFORM%\Debug"
	copy "%RELATIVE_PATH%\Debug\*.dll"  "builds\%PLATFORM%\Debug"
	copy "%RELATIVE_PATH%\Debug\*.json" "builds\%PLATFORM%\Debug"
)
if not exist "%RELATIVE_PATH%\Debug" echo Debug binaries are not available.

if exist "%RELATIVE_PATH%\Release" (
	copy "%RELATIVE_PATH%\Release\*.bin"  "builds\%PLATFORM%\Release"
	copy "%RELATIVE_PATH%\Release\*.dll"  "builds\%PLATFORM%\Release"
	copy "%RELATIVE_PATH%\Release\*.json" "builds\%PLATFORM%\Release"
)
if not exist "%RELATIVE_PATH%\Release" echo Release binaries are not available.

if exist "%RELATIVE_PATH%\Resources" (
	xcopy "%RELATIVE_PATH%\Resources" "builds\%PLATFORM%\Debug"   /e /y
	xcopy "%RELATIVE_PATH%\Resources" "builds\%PLATFORM%\Release" /e /y
)
if not exist "%RELATIVE_PATH%\Resources" echo Resource files are not available.

endlocal
@echo on
