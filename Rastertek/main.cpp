#include "Systemclass.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	//Create system obj
	System = new SystemClass();
	if (!System)
	{
		return 0;
	}

	//Initialize System
	result = System->Initialize();
	while(result == true)
	{
		int newmode = System->Run();

		if (newmode == -1)
		{
			result = false;
		}
		if(newmode == 1)
		{
			System->Shutdown();
			System = new SystemClass();
			result = System->Initialize();
			newmode = 0;
		}
	}

	//Shutdown and release
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}