@echo off
cls

:: ***********************************************************************
   rem Batch file for compiling Raylib and/or Raygui applications (modifique el compiler)
:: ***********************************************************************

:Initialization
	echo ^> Initializing Variables
	echo ----------------------------
	SET "RAYLIB_DIR=C:\raylib"
	SET "CURRENT_DIR=%cd%"
	SET "CC=g++"	

	SET "INPUT_FILE=%1"
	SET "OUTPUT_FILE=%2"
	SET "INCLUDE_FILES="

	SET "COMPILER=%RAYLIB_DIR%\w64devkit\w64devkit.exe"
	SET "CFLAGS=%RAYLIB_DIR%\raylib\src\raylib.rc.data -std=c++11 -Wall"
	SET "LDFLAGS=-lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 "
	SET "EXTRAFLAGS="

	IF /I "%3"=="Release" SET EXTRAFLAGS=%EXTRAFLAGS% -b

:Main
	echo(
	echo ^> Removing Previous Build
	echo ----------------------------
	IF EXIST "%1.exe" del /F "%1.exe"

	echo(
	echo ^> Compiling Program
	echo ----------------------------
	%CC% -o "%OUTPUT_FILE%" "%INPUT_FILE%" %INCLUDE_FILES% %CFLAGS%  %LDFLAGS% %EXTRAFLAGS%