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
	if(result)
	{
		System->Run();
	}

	//Shutdown and release
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}