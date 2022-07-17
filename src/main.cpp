#include "core/application.h"

using namespace HY3D;

int main()
{
	application_config config = {640, 480, "HY3D Engine"};
	if (ApplicationInitialize(&config))
	{
		if (!ApplicationRun())
		{
			LOG_ERROR("Application stopped running unexpectedly");
		}
	}
	return 0;
}