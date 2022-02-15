#pragma once

#include "Core.h"
#include "Window/Window.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Texture.h"

#include "Core/Timestep.h"

#include "ProjectionData.h"

#include "Game/Card.h"

#include "Game/Person.h"

namespace SuperBit {


	class Application
	{
	public:

		Application();
		~Application();

		void Run();
		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);


		inline static Application& Get() { return *s_Instance; }

		void Close() { m_Running = false; }

		Window& GetWindow() { return *m_Window; }
		ImGuiLayer* GetImGuiLayer() { return m_ImguiLayer; }

		void OnResize(float width, float height);

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	public:

		Person* Player;
		Person* Player2 = nullptr;
		Person* Dealer;

		Person* CurrentPlayer = nullptr;

		std::vector<int> pachet;

		int nr_carti_pachet = 52;
		bool frec[53] = { 0 };

		int m_Money = 100;

		Timestep endedTime;
		Timestep timestep;

		bool GameOver = false;
		bool TurnOver = false;

		std::string message;

	private:
		ProjectionData m_ProjectionData;
		ImGuiLayer* m_ImguiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0;

		ShaderLibrary m_ShaderLibrary;

		static Application* s_Instance;

		std::shared_ptr<Texture> background;
		std::shared_ptr<Texture> logo;
		std::shared_ptr<Texture> back;
	};
}