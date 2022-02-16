#include "sbpch.h"

#include "Application.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Renderer2D.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "TempGui/imgui.h"

#include "TempGui/imgui_impl_glfw.h"
#include "TempGui/imgui_impl_opengl3.h"

namespace SuperBit {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_ProjectionData(0.0f, 1000.0f, 0.0f, 562.0f)
	{
		srand(time(NULL));

		//memset(frec, 0, sizeof(frec));

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImguiLayer = new ImGuiLayer();
		m_ImguiLayer->OnAttach();

		background = std::make_shared<Texture>("../assets/textures/background2.png");
		logo = std::make_shared<Texture>("../assets/textures/1.png");
		back = std::make_shared<Texture>("../assets/textures2/back_red.png");

		Chip1 = std::make_shared<Texture>("../assets/textures/RedChip.png");
		Chip5 = std::make_shared<Texture>("../assets/textures/YellowChip.png");
		Chip20 = std::make_shared<Texture>("../assets/textures/BlueChip.png");
		Chip100 = std::make_shared<Texture>("../assets/textures/GreenChip.png");
		Chip500 = std::make_shared<Texture>("../assets/textures/PurpleChip.png");

		Player = new Person(0);
		Dealer = new Person(1);

		while (nr_carti_pachet > 0)
		{
			int x = rand() % 52;
			if (frec[x] < 2)
			{
				frec[x]++;
				nr_carti_pachet--;
				SB_ERROR("{0}", x);
				pachet.push_back(x);
			}
		}

		Player->m_Position = { 485, 0 };

		CurrentPlayer = Player;
	}

	Application::~Application()
	{
		
	}
	
	void Application::Run()
	{
		while (m_Running)
		{
			Renderer::SetClearColor({ 1.0f, 0.1f, 1.0f, 1 });
			Renderer::Clear();

			float time = (float)glfwGetTime();
			timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			SB_INFO("{0}", m_GameRunning);

			Render();

			if (m_GameRunning)
			{
				if (gameStartedTime == -1)
					gameStartedTime = time;

				if (gameStartedTime != -1)
				{
					if (time - gameStartedTime > 2.5 && Dealer->m_NrOfCards == 0)
						Dealer->AddCard();
					if (time - gameStartedTime > 3.5 && Dealer->m_NrOfCards == 1)
						Dealer->AddHiddenCard();

					if (time - gameStartedTime > 2 && Player->m_NrOfCards == 0 && !Player2)
						Player->AddCard();
					if (time - gameStartedTime > 3 && Player->m_NrOfCards == 1 && !Player2)
						Player->AddCard();
				}

				if (CurrentPlayer->m_Sum > 21 && CurrentPlayer->m_NrOfAces)
				{
					CurrentPlayer->m_Sum -= 11;
					CurrentPlayer->m_NrOfAces--;
				}

				if (Dealer->m_Sum > 21 && Dealer->m_NrOfAces)
				{
					Dealer->m_Sum -= 11;
					Dealer->m_NrOfAces--;
				}

				if (CurrentPlayer->m_Sum > 21 && !CurrentPlayer->GameOver)
				{
					endedTime = time;
					m_Money -= CurrentPlayer->m_Bet;
					CurrentPlayer->m_Message = "Bust";
					CurrentPlayer->GameOver = true;
					CurrentPlayer->TurnOver = true;
				}

				if (CurrentPlayer->GameOver || CurrentPlayer->TurnOver)
				{
					if (Player2)
					{
						if (!Player2->GameOver && !Player2->TurnOver)
						{
							CurrentPlayer = Player2;
						}
					}
				}

				if (CurrentPlayer->GameOver)
				{
					if (Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_FrontTexture != nullptr)
					{
						Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_Texture = Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_FrontTexture;
						Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_FrontTexture = nullptr;
						Dealer->m_Sum += Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_Indice;
					}
				}

				if (CurrentPlayer->TurnOver)
				{
					if (Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_FrontTexture != nullptr)
					{
						Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_Texture = Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_FrontTexture;
						Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_FrontTexture = nullptr;
						Dealer->m_Sum += Dealer->m_Cards[Dealer->m_Cards.size() - 1].m_Indice;
					}

					if (Player2 && Dealer->m_Sum < 17)
					{
						if (!Player->GameOver || !Player2->GameOver)
						{
							if (time - endedTime > 2)
							{
								Dealer->AddCard();
								endedTime = time;
							}
						}
					}
					else
					{
						if (!Player->GameOver && Dealer->m_Sum < 17)
						{
							if (time - endedTime > 2)
							{
								Dealer->AddCard();
								endedTime = time;
							}
						}
					}
				}

				if (!Player->GameOver && CurrentPlayer->TurnOver && Dealer->m_Sum >= 17)
				{
					Player->GameOver = true;
					if (Dealer->m_Sum < Player->m_Sum || Dealer->m_Sum > 21)
					{
						Player->m_Message = "You Win";
						m_Money += Player->m_Bet;
					}
					else if (Dealer->m_Sum > Player->m_Sum)
					{
						Player->m_Message = "You Lose";
						m_Money -= Player->m_Bet;
					}
					else if (Dealer->m_Sum == Player->m_Sum)
						Player->m_Message = "Push";
				}

				if (Player2)
				{
					if (!Player2->GameOver && CurrentPlayer->TurnOver && Dealer->m_Sum >= 17)
					{
						Player2->GameOver = true;
						if (Dealer->m_Sum < Player2->m_Sum || Dealer->m_Sum > 21)
						{
							Player2->m_Message = "You Win";
							m_Money += Player2->m_Bet;
						}
						else if (Dealer->m_Sum > Player2->m_Sum)
						{
							Player2->m_Message = "You Lose";
							m_Money -= Player2->m_Bet;
						}
						else if (Dealer->m_Sum == Player2->m_Sum)
							Player2->m_Message = "Push";
					}
				}
			}

			if (Player2)
			{
				if (Player->GameOver && Player2->GameOver && gameEndedTime == -1)
				{
					gameEndedTime = time;
				}
			}
			else if (Player->GameOver && gameEndedTime == -1)
			{
				gameEndedTime = time;
			}

			if (gameEndedTime != -1 && time - gameEndedTime > 7)
			{
				EndGame();
			}

			m_ImguiLayer->Begin();
			if (m_GameRunning)
				this->OnUpdateGameRunning(timestep);
			else
				this->OnUpdateGamePaused(timestep);
			
			m_ImguiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnUpdateGameRunning(Timestep ts)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowSize(ImVec2(1000, 562));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		bool open = true;
		ImGui::Begin("Hello", &open, window_flags);

		ImGui::PushFont(io.Fonts->Fonts[0]);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.7f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		ImGui::SetCursorPos({ 50, ImGuiLayer::CursorCenteredVertically(50) + 200 });
		if (ImGui::Button("Hit", { 150, 75 }) && !CurrentPlayer->GameOver && !CurrentPlayer->TurnOver)
		{
			CurrentPlayer->AddCard();
		}

		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.5f, 0.0f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.0f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		ImGui::SetCursorPos({ 800, ImGuiLayer::CursorCenteredVertically(50) + 200 });
		if (ImGui::Button("Stand", { 150, 75 }))
		{
			//app.Dealer->AddCard();
			CurrentPlayer->TurnOver = true;
		}

		if (Player->GameOver)
		{
			ImGui::SetCursorPos({ Player->m_Position.x - 60, ImGuiLayer::CursorCenteredVertically(0) });
			ImGui::Text(Player->m_Message.c_str());
		}

		if (Player2)
		{
			if (Player2->GameOver)
			{
				ImGui::SetCursorPos({ Player2->m_Position.x - 60, ImGuiLayer::CursorCenteredVertically(0) });
				ImGui::Text(Player2->m_Message.c_str());
			}
		}

		ImGui::SetCursorPos({ 730, 23 });
		std::string money = "Bank: " + std::to_string(m_Money);
		ImGui::Text(money.c_str());

		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::PushFont(io.Fonts->Fonts[2]);
		ImGui::SetCursorPos({ ImGuiLayer::CursorCenteredHorizontally(125), ImGuiLayer::CursorCenteredVertically(35) - 20 });
		if (ImGui::Button("Double", { 125, 35 }) && ((!Player2 && CurrentPlayer->m_NrOfCards == 2) || CurrentPlayer->m_NrOfCards == 1))
		{
			CurrentPlayer->AddCard();
			CurrentPlayer->TurnOver = true;
			CurrentPlayer->m_Bet *= 2;
		}

		if (ImGui::Button("Split", { 100, 100 }) /*&& Player->m_NrOfCards == 2 && Player->m_Cards[0].m_Indice == Player->m_Cards[1].m_Indice*/)
		{
			Player2 = new Person(0);
			Player2->m_Cards.push_back(Player->m_Cards[Player->m_Cards.size() - 1]);
			Player->m_Cards.pop_back();

			Card& card = Player2->m_Cards[Player2->m_Cards.size() - 1];
			//card.m_Pos.x = 400;
			//card.m_FinalPos.x = 400;

			Player->m_Sum -= card.m_Indice;
			Player2->m_Sum += card.m_Indice;

			Player->m_NrOfCards--;
			Player2->m_NrOfCards++;

			if (card.m_Indice == 11)
			{
				Player->m_NrOfAces--;
				Player2->m_NrOfAces++;
			}

			Player->m_Position = { 400, 0 };
			Player2->m_Position = { 600, 0 };

			for (auto& card : Player->m_Cards)
			{
				card.m_FinalPos.x = 400;
				card.CalculateStep();
			}

			for (auto& card : Player2->m_Cards)
			{
				card.m_FinalPos.x = 600;
				card.CalculateStep();
			}
		}

		if (Player2)
		{
			if (!Player2->GameOver)
			{
				ImGui::SetCursorPos({ Player2->m_Position.x - 20, ImGuiLayer::CursorCenteredVertically(25) + 30 });
				std::string PlayerSum2 = std::to_string(Player2->m_Sum);
				ImGui::Text(PlayerSum2.c_str());
			}
		}

		if (!Player->GameOver)
		{ 
			ImGui::SetCursorPos({ Player->m_Position.x, ImGuiLayer::CursorCenteredVertically(25) + 30});
			std::string PlayerSum = std::to_string(Player->m_Sum);
			ImGui::Text(PlayerSum.c_str());
		}

		ImGui::SetCursorPos({ ImGuiLayer::CursorCenteredHorizontally(50), 0 });
		std::string DealerSum = std::to_string(Dealer->m_Sum);
		ImGui::Text(DealerSum.c_str());

		ImGui::PopStyleVar();
		ImGui::PopFont();
		ImGui::End();
	}

	void Application::OnUpdateGamePaused(Timestep ts)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowSize(ImVec2(1000, 562));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		bool open = true;
		ImGui::Begin("Hello", &open, window_flags);

		ImGui::PushFont(io.Fonts->Fonts[0]);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.7f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		ImGui::SetCursorPos({ ImGuiLayer::CursorCenteredHorizontally(100), ImGuiLayer::CursorCenteredVertically(50) });

		if (ImGui::Button("Play", { 100, 50 }))
		{
			StartGame();
		}

		ImGui::PopFont();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 1.0f, 0.0f, 0.0f });

		ImGui::SetCursorPosX(330);
		ImGui::ImageButton((void*)Chip1->GetRendererID(), ImVec2(50, 50));
		ImGui::SameLine(); ImGui::ImageButton((void*)Chip5->GetRendererID(), ImVec2(50, 50));
		ImGui::SameLine(); ImGui::ImageButton((void*)Chip20->GetRendererID(), ImVec2(50, 50));
		ImGui::SameLine(); ImGui::ImageButton((void*)Chip100->GetRendererID(), ImVec2(50, 50));
		ImGui::SameLine(); ImGui::ImageButton((void*)Chip500->GetRendererID(), ImVec2(50, 50));

		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		//dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

	}

	void Application::StartGame()
	{
		m_GameRunning = true;
		gameEndedTime = -1;
	}

	void Application::Render()
	{
		Renderer2D::BeginScene(m_ProjectionData);

		Renderer2D::DrawQuad({ m_Window->GetWidth() / 2, m_Window->GetHeight() / 2, 0 }, { m_Window->GetWidth(), m_Window->GetHeight() }, background);

		Player->m_Cards.size();

		for (auto& card : Player->m_Cards)
		{
			Renderer2D::DrawQuad(card);
			if (card.nrOfSteps > 0)
			{
				card.m_Pos.x += card.stepX * 0.016;
				card.m_Pos.y += card.stepY * 0.016;
				card.m_Rotation += card.stepR * 0.016;
				card.nrOfSteps--;
			}
		}

		if (Player2)
		{
			for (auto& card : Player2->m_Cards)
			{
				Renderer2D::DrawQuad(card);
				if (card.nrOfSteps > 0)
				{
					card.m_Pos.x += card.stepX * 0.016;
					card.m_Pos.y += card.stepY * 0.016;
					card.m_Rotation += card.stepR * 0.016;
					card.nrOfSteps--;
				}

			}
		}

		for (auto& card : Dealer->m_Cards)
		{
			Renderer2D::DrawQuad(card);
			if (card.nrOfSteps > 0)
			{
				card.m_Pos.x += card.stepX * 0.016;
				card.m_Pos.y += card.stepY * 0.016;
				card.m_Rotation += card.stepR * 0.016;
				card.nrOfSteps--;
			}
		}

		for (int i = 0; i <= 5; ++i)
		{
			Renderer2D::DrawQuad({ 845 - i * 5, 400, 0 + 0.990 + i * 0.001 }, { 96, 150 }, back);
		}

		Renderer2D::DrawQuad({ 181, 517, 1 }, { 666 / 3, 375 / 3 }, logo);
		Renderer2D::EndScene();
	}

	void Application::EndGame()
	{
		m_GameRunning = false;
		gameStartedTime = -1;

		delete Player2;
		delete Player;
		delete Dealer;

		Player = new Person(0);
		Dealer = new Person(1);

		Player->m_Position = { 485, 0 };

		CurrentPlayer = Player;
	}

	void Application::OnResize(float width, float height)
	{
		glViewport(0, 0, width, height);

		float aspectRatio = width / height;
		m_ProjectionData.SetProjection(0, aspectRatio * 720, 0, 720);
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		
		OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}

}