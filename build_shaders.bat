@echo off
ECHO Building Shaders...

SET OutputPath=.\bin\shaders

IF NOT EXIST %OutputPath% MKDIR %OutputPath%
PUSHD %OutputPath%

SET SRC=..\..\assets\shaders

SET ARGS=--target-env=vulkan1.2 -O -o %%~nxi.spv

IF EXIST *.spv DEL *.spv

for /r %%i in (%SRC%\*.*) do (
	echo %%~nxi
	%VULKAN_SDK%/bin/glslc %%i %ARGS%
)

popd
popd

:BUILD_DONE