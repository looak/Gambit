// GambitConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\GambitEngine\GambitEngine.h"


int main()
{
	if (GambitEngine::UCI::HelloWorld())
	{
		return 0;
	}

    return 1;
}

