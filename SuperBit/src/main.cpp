//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "sbpch.h"

#include "Core/Application.h"
//#include <Windows.h>

int main()
{
	//FreeConsole();
	SuperBit::Log::Init();

	auto app = new SuperBit::Application();
	app->Run();
	delete app;
}