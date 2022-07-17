@echo off

ctime -begin hy3d_engine_build_time.ctm

REM Check if build mode is specified
IF [%1] == [] (
	echo Enter build mode: Debug or Release
	echo Example: build Debug
	goto :EOF
)

REM Set debug build options
IF %1 == Debug (
	SET ModeFlags=-MTd -Od -Zi -Zo
    SET OutputPath=.\bin\Debug
	ECHO Building Debug
)
REM Set release build options
IF %1 == Release (
	SET ModeFlags=-O2
    SET OutputPath=.\bin\Release
	ECHO Building Release
)
IF NOT EXIST %OutputPath% MKDIR %OutputPath%
PUSHD %OutputPath%

SET SRC=.\..\..\src

REM COMPILER OPTIONS
SET CFlags=-EHsc -FC -fp:except- -fp:fast -Gm- -GS- -nologo -Oi -WL %ModeFlags%
SET Defines=-D_CRT_SECURE_NO_WARNINGS
SET Includes=/I %SRC%
SET Warnings=-W4 -wd4100 -wd4458 -wd4505 -wd4201
SET AppName=hy3d_engine
SET COptions=%CFlags% %Defines% %Includes% %Warnings% %MODE% -Fe%AppName%

REM LINKER OPTIONS
SET LFlags=-incremental:no -opt:ref
SET Libraries=user32.lib gdi32.lib
SET LOptions=%LFlags% %Libraries%

DEL *.pdb > NUL 2> NUL

REM BUILD WINDOWS RESOURCE
REM rc /fo win32_hy3d.res /nologo %SRC%\platform\win32\resource.rc
SET COptions=%COptions% win32_hy3d.res

REM SET EXPOTED_FUNCS=-EXPORT:EngineInitialize -EXPORT:EngineUpdateAndRender -EXPORT:EngineDestroy
REM cl /I %VULKAN_SDK%\Include %COMPILER_FLAGS% ..\src\engine_platform.cpp -Fmengine_platform.map -LD -link -incremental:no -opt:ref -PDB:engine_platform_%RANDOM%.pdb %EXPOTED_FUNCS%

cl %COptions% %SRC%\build.cpp -link %LOptions%
POPD

ECHO ----------------------------------------------------------------------------

:EOF

ctime -end hy3d_engine_build_time.ctm