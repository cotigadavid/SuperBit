#include "sbpch.h"

#include "Core/Application.h"

int main(int argc, char** argv)
{
	SuperBit::Log::Init();

	auto app = new SuperBit::Application();
	app->Run();
	delete app;
}