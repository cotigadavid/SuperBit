#pragma once

#include "Core.h"

namespace SuperBit {
	
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int mousebuttoncode);
		
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();


	private:
		static Input* s_Instance;
	};
}