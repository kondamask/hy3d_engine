@echo off

ECHO -------------------------------------------------------------------------------------

ctime -begin hy3d_engine_build_time.ctm

@REM -------------------------------------------------------------------------------------

SET AppName=hy3d_engine

@REM Build Mode --------------------------------------------------------------------------

IF [%1] == [] (
	ECHO Enter build mode: Debug or Release
	ECHO Example: build Debug
	GOTO :EOF
)

IF %1 == Debug (
	SET ModeFlags=-MTd -Od -Zi -Zo
    SET OutputPath=.\bin\Debug
	ECHO Debug Mode
)

IF %1 == Release (
	SET ModeFlags=-O2
    SET OutputPath=.\bin\Release
	ECHO Release Mode
	COLOR 0A
)

IF NOT EXIST %OutputPath% MKDIR %OutputPath%
PUSHD %OutputPath%
SET SRC=..\..\src

@REM Common Flags ------------------------------------------------------------------------

SET CompilerFlags=-EHsc -FC -fp:except- -fp:fast -Gm- -GS- -nologo -Oi -WL %ModeFlags%
SET Defines=-D_CRT_SECURE_NO_WARNINGS  -DHY3D_EXPORT
SET Includes=/I%SRC%
SET Warnings=-W4 -wd4100 -wd4458 -wd4505 -wd4201

SET LinkerFlags=-incremental:no -opt:ref
SET LIBs=user32.lib gdi32.lib
SET OBJs=all.obj

SET CommonCompilerOptions=%CompilerFlags% %Defines% %Includes% %Warnings%
SET CommonLinkerOptions=%LinkerFlags% %LIBs% %OBJs%

@REM Build Targets -----------------------------------------------------------------------

IF [%2] == [] (
	ECHO Building All...
	GOTO :BUILD_ALL
)

IF %2 == Engine (
	ECHO Building Engine...
	GOTO :BUILD_ENGINE
)

@REM -------------------------------------------------------------------------------------
:BUILD_ALL
DEL *.pdb > NUL 2> NUL

@REM All Cpps ----------------------------------------------------------------------------
cl	/c ^
	%CommonCompilerOptions% ^
	%SRC%\all.cpp

@REM Windows Resource --------------------------------------------------------------------
rc	/nologo ^
	/fo win32_hy3d.res ^
	%SRC%\platform\win32\resource.rc

@REM App Entry  --------------------------------------------------------------------------
cl	%CommonCompilerOptions% ^
	%SRC%\main.cpp win32_hy3d.res ^
	-Fe%AppName% ^
	-link ^
	%CommonLinkerOptions%

:BUILD_ENGINE
@REM Engine
cl	-LD ^
	%CommonCompilerOptions% ^
	%SRC%\engine\engine.cpp ^
	-link ^
	%CommonLinkerOptions% ^
	-PDB:engine_%RANDOM%.pdb

POPD

ECHO -------------------------------------------------------------------------------------

:EOF

ctime -end hy3d_engine_build_time.ctm