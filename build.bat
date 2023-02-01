@echo off

rem Compile some C Code with the least amount of fuzz, based on a build.bat file originally by Ruben Osorio. 

rem Locating the compiler Executable
rem -------------------------------------------------------------------------------------------------------------------------------

rem Getting access to the compiler
rem Trying a bunch of locations that I've had on my machines before, it is what it is :)
WHERE cl
IF %ERRORLEVEL% NEQ 0 (
IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
  call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" (
  call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64 >nul
) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
  call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
  call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
) ELSE IF EXIST "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
  call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
))

rem Compiler Configuration
rem -------------------------------------------------------------------------------------------------------------------------------

rem Reference Link: https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170
rem /TC means interpret all files as C code
rem /GL activates whole program optimization
rem /W4 Activates all warnings
rem /Gy Enables function level linking
rem /Zi Activates complete debugging information
rem /O2/d Maximize Speed / Disable optimizations
rem /Zc:inline Removes unreferenced data or functions that are COMDATs (not sure what that really means)
rem /WX Treats all warnings as errors
rem /Gd Specifies the __cdecl calling convention
rem /Oi Substitute function calls with intrinsicts if it will helps
rem /MD Use the multithreaded versin of the runtime library
rem /GS- Disable buffer overrun protections
rem /Fd Renames the generated PDBfile
rem /Fo Sets the .obj file path directory

set DISABLE_THESE_WARNINGS=/wd4996
set DEFINES=/D "WIN32" /D "NDEBUG" /D "_CONSOLE"
set COMPILER_FLAGS=/TC /GL /W4 /Gy /Zi /Od /Zc:inline /fp:fast /WX /Gd /Oi /MD /GS- %DEFINES% %DISABLE_THESE_WARNINGS% /Fd"build/vc.pdb" /Fo"build/unity.obj"
set BUILD_FOLDER="build/"
set BUILD_TARGET_NAME="AVM.exe"

rem Compiling The Code
rem -------------------------------------------------------------------------------------------------------------------------------
if not exist build mkdir build 

cl %COMPILER_FLAGS% /Fe%BUILD_FOLDER%%BUILD_TARGET_NAME% ./src/unity.c

rem Moving intermediate files around
rem -------------------------------------------------------------------------------------------------------------------------------