@echo off

IF [%1] == [] (
	echo Enter build mode: Debug or Release
	echo Example: build Debug
	goto :eof
)

IF %1 == Debug (
	set MODE=-DHY3D_DEBUG=1 -DVULKAN_VALIDATION_LAYERS_ON=1 -Od
    set OUTPUT_PATH=.\bin\Debug
)
IF %1 == Release (
	SET MODE=-DHY3D_DEBUG=0 -DVULKAN_VALIDATION_LAYERS_ON=0 -O2
    set OUTPUT_PATH=.\bin\Release
)
if not exist %OUTPUT_PATH% mkdir %OUTPUT_PATH%
pushd %OUTPUT_PATH%

set SRC=.\..\..\src

set EXE_NAME=hy3d_engine
set INCLUDE_FOLDERS=/I %SRC%

set COMPILER_FLAGS=%INCLUDE_FOLDERS% %MODE% -MTd -WL -nologo -fp:fast -fp:except- -Gm- -EHsc -Zo -Oi -W4 -wd4100 -wd4458 -wd4505 -wd4201 -FC -Zi -GS- -D_CRT_SECURE_NO_WARNINGS
set LIBS=user32.lib gdi32.lib 
set LINKER_FLAGS=-incremental:no -opt:ref %LIBS%

rem Build Windows Resource
rc /fo win32_hy3d.res /nologo %SRC%\win32\resource.rc

del *.pdb > NUL 2> NUL

REM set EXPOTED_FUNCS=-EXPORT:EngineInitialize -EXPORT:EngineUpdateAndRender -EXPORT:EngineDestroy
REM cl /I %VULKAN_SDK%\Include %COMPILER_FLAGS% ..\src\engine_platform.cpp -Fmengine_platform.map -LD -link -incremental:no -opt:ref -PDB:engine_platform_%RANDOM%.pdb %EXPOTED_FUNCS%

cl %COMPILER_FLAGS% %SRC%\main.cpp  win32_hy3d.res -Fe%EXE_NAME% -link %LINKER_FLAGS%
popd


:eof