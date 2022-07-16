@echo off

IF [%1] == [] (
	echo Enter build mode: Debug or Release
	echo Example: build Debug
	goto :EOF
)

IF %1 == Debug (
	SET MODE=-DHY3D_DEBUG=1 -DVULKAN_VALIDATION_LAYERS_ON=1 -Od
    SET OUTPUT_PATH=.\bin\Debug
	ECHO Building Debug
)
IF %1 == Release (
	SET MODE=-DHY3D_DEBUG=0 -DVULKAN_VALIDATION_LAYERS_ON=0 -O2
    SET OUTPUT_PATH=.\bin\Release
	ECHO Building Release
)
IF NOT EXIST %OUTPUT_PATH% MKDIR %OUTPUT_PATH%
PUSHD %OUTPUT_PATH%

SET SRC=.\..\..\src

SET EXE_NAME=hy3d_engine
SET INCLUDE_FOLDERS=/I %SRC%

SET COMPILER_FLAGS=%INCLUDE_FOLDERS% %MODE% -MTd -WL -nologo -fp:fast -fp:except- -Gm- -EHsc -Zo -Oi -W4 -wd4100 -wd4458 -wd4505 -wd4201 -FC -Zi -GS- -D_CRT_SECURE_NO_WARNINGS
SET LIBS=user32.lib gdi32.lib 
SET LINKER_FLAGS=-incremental:no -opt:ref %LIBS%

REM BUILD WINDOWS RESOURCE
rc /fo win32_hy3d.res /nologo %SRC%\win32\resource.rc

DEL *.pdb > NUL 2> NUL

REM SET EXPOTED_FUNCS=-EXPORT:EngineInitialize -EXPORT:EngineUpdateAndRender -EXPORT:EngineDestroy
REM cl /I %VULKAN_SDK%\Include %COMPILER_FLAGS% ..\src\engine_platform.cpp -Fmengine_platform.map -LD -link -incremental:no -opt:ref -PDB:engine_platform_%RANDOM%.pdb %EXPOTED_FUNCS%

cl %COMPILER_FLAGS% %SRC%\build.cpp  win32_hy3d.res -Fe%EXE_NAME% -link %LINKER_FLAGS%
POPD

ECHO .
ECHO Done

:EOF