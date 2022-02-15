#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/Timestep.h"

namespace SuperBit {

	class ImGuiLayer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);
		void OnAttach();
		void OnDetach();
		void OnImGuiRender();

		void Begin();
		void End();

		static float CursorCenteredHorizontally(int size);
		static float CursorCenteredVertically(int size);

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}