@echo off

ctime -begin hy3d_engine_build_time.ctm

SET AppName=hy3d_engine

@REM Build Mode -------------------------------------------------------------------------

IF [%1] == [] (
	ECHO Enter build mode: Debug or Release
	ECHO Example: build Debug
	GOTO :BUILD_DONE
)

IF %1 == Debug (
	SET DLLMode=-LDd
	SET ModeFlags=-MTd -Od -Zi -Zo
    SET OutputPath=.\bin\Debug
	ECHO Debug Mode
)

IF %1 == Release (
	SET DLLMode=-LD
	SET ModeFlags=-O2
    SET OutputPath=.\bin\Release
	ECHO Release Mode
	COLOR 0A
)

@REM ------------------------------------------------------------------------------------

IF NOT EXIST %OutputPath% MKDIR %OutputPath%
PUSHD %OutputPath%
SET SRC=..\..\src

@REM Common Flags -----------------------------------------------------------------------

SET CompilerFlags=-EHsc -FC -fp:except- -fp:fast -Gm- -GS- -nologo -Oi -WL %ModeFlags%
SET Defines=-D_CRT_SECURE_NO_WARNINGS -DHY3D_EXPORT
SET Includes=/I%SRC%
SET Warnings=-W4 -wd4100 -wd4458 -wd4505 -wd4201

SET LinkerFlags=-incremental:no -opt:ref
SET LIBs=user32.lib gdi32.lib
SET OBJs=all.obj

SET CommonCompilerOptions=%CompilerFlags% %Defines% %Includes% %Warnings%
SET CommonLinkerOptions=%LinkerFlags% %LIBs% %OBJs%

REM Build Tools
cl %CommonCompilerOptions% %SRC%\tools\api_binder.cpp -link %LinkerFlags%

@REM Build Targets ----------------------------------------------------------------------

IF [%2] == [] (
	api_binder.exe %SRC%\engine\engine.cpp engine_api %SRC%\engine
	api_binder.exe %SRC%\renderer\vulkan\vulkan_renderer.cpp renderer_api %SRC%\renderer
	ECHO Building All...
	SET BUILD_SINGLE=0
	GOTO :BUILD_ALL
)
IF %2 == Engine (
	api_binder.exe %SRC%\engine\engine.cpp engine_api %SRC%\engine
	ECHO Building Engine...
	SET BUILD_SINGLE=1
	GOTO :BUILD_ENGINE
)
IF %2 == Renderer (
	api_binder.exe %SRC%\renderer\vulkan\vulkan_renderer.cpp renderer_api %SRC%\renderer
	ECHO Building Renderer...
	SET BUILD_SINGLE=1
	GOTO :BUILD_RENDERER
)

@REM ------------------------------------------------------------------------------------
:BUILD_ALL
DEL *.pdb > NUL 2> NUL

@REM All Cpps ---------------------------------------------------------------------------
cl	/c ^
	%CommonCompilerOptions% ^
	%SRC%\all.cpp

@REM Windows Resource -------------------------------------------------------------------
rc	/nologo ^
	/fo win32_hy3d.res ^
	%SRC%\platform\win32\resource.rc

@REM App Entry --------------------------------------------------------------------------
cl	-Fe%AppName% ^
	%CommonCompilerOptions% ^
	%SRC%\main.cpp win32_hy3d.res ^
-link ^
	%CommonLinkerOptions%

:BUILD_RENDERER
@REM Renderer  ---------------------------------------------------------------------------
cl	%DLLMode% ^
	/I%VULKAN_SDK%\Include ^
	/Fevulkan_renderer ^
	%CommonCompilerOptions% ^
	%SRC%\renderer\vulkan\vulkan_renderer.cpp ^
-link ^
	-PDB:vulkan_renderer_%RANDOM%.pdb ^
	%CommonLinkerOptions%
IF %BUILD_SINGLE% == 1 (
	GOTO :BUILD_DONE
)

:BUILD_ENGINE
@REM Engine -----------------------------------------------------------------------------
cl	%DLLMode% ^
	%CommonCompilerOptions% ^
	%SRC%\engine\engine.cpp ^
-link ^
	-PDB:engine_%RANDOM%.pdb ^
	%CommonLinkerOptions%
IF %BUILD_SINGLE% == 1 (
	GOTO :BUILD_DONE
)

:BUILD_DONE
POPD
ECHO -------------------------------------------------
ctime -end hy3d_engine_build_time.ctm