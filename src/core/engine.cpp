#include "engine.h"

#include <stdio.h>

namespace HY3D
{
	EngineInitializeSignature(EngineInitialize)
	{
		printf("Engine Initialized\n");
	}

	EngineUpdateSignature(EngineUpdate)
	{
	}

	EngineRenderSignature(EngineRender)
	{
	}

	EngineTerminateSignature(EngineTerminate)
	{
		printf("Engine Terminated\n");
	}
}