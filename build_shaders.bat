@echo off
ECHO Building Shaders...

SET OutputPath=.\bin\shaders

IF NOT EXIST %OutputPath% MKDIR %OutputPath%
PUSHD %OutputPath%

SET SRC=..\..\assets\shaders

del *.spv

for /r %%i in (%SRC%\*.vert) do (
	echo %%~nxi
	%VULKAN_SDK%/bin/glslc %%i -fshader-stage=vertex -o %%~ni.vert.spv
)
for /r %%i in (%SRC%\*.frag) do (
	echo %%~nxi	
	%VULKAN_SDK%/bin/glslc %%i -fshader-stage=fragment -o %%~ni.frag.spv
)

popd
popd

:BUILD_DONE