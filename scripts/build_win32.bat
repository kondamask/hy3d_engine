@echo off

ctime -begin hy3d_engine_build_time.ctm

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

REM COMPILER OPTIONS
SET CL_FLAGS=-MTd -WL -nologo -fp:fast -fp:except- -Gm- -EHsc -Zo -Oi -FC -Zi -GS-
SET DEFINES=-D_CRT_SECURE_NO_WARNINGS
SET INCLUDES=/I %SRC%
SET WARNINGS=-W4 -wd4100 -wd4458 -wd4505 -wd4201
SET APP_NAME=hy3d_engine
SET COMPILER_OPTIONS=%CL_FLAGS% %DEFINES% %INCLUDES% %WARNINGS% -Fe%APP_NAME%

REM LINKER OPTIONS
SET LINK_FLAGS=-incremental:no -opt:ref
SET LIBRARIES=user32.lib gdi32.lib
SET LINKER_OPTIONS=%LINK_FLAGS% %LIBRARIES%

DEL *.pdb > NUL 2> NUL

REM BUILD WINDOWS RESOURCE
REM rc /fo win32_hy3d.res /nologo %SRC%\platform\win32\resource.rc
SET COMPILER_OPTIONS=%COMPILER_OPTIONS% win32_hy3d.res

REM SET EXPOTED_FUNCS=-EXPORT:EngineInitialize -EXPORT:EngineUpdateAndRender -EXPORT:EngineDestroy
REM cl /I %VULKAN_SDK%\Include %COMPILER_FLAGS% ..\src\engine_platform.cpp -Fmengine_platform.map -LD -link -incremental:no -opt:ref -PDB:engine_platform_%RANDOM%.pdb %EXPOTED_FUNCS%

cl %COMPILER_OPTIONS% %SRC%\build.cpp -link %LINKER_OPTIONS%
POPD

ECHO ----------------------------------------------------------------------------

:EOF

ctime -end hy3d_engine_build_time.ctm