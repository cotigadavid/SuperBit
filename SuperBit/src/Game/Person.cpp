#include "sbpch.h"
#include "Person.h"

#include "Core/Application.h"

namespace SuperBit {

	void Person::AddCard()
	{
		int add = Application::Get().Player->m_NrOfCards + m_NrOfCards;
		if (Application::Get().Player2)
			add += Application::Get().Player2->m_NrOfCards;

		if (m_Type == 0)
			AddCard({ (m_Position.x + m_NrOfCards * 20) * Application::Get().m_Ratio, 150 * Application::Get().m_Ratio, 0.5 + 0.005 * add });
		else
			AddCard({ (m_Position.x + m_NrOfCards * 20) * Application::Get().m_Ratio, 450 * Application::Get().m_Ratio, 0.5 + 0.005 * add });
	}

	void Person::AddHiddenCard()
	{
		int add = Application::Get().Player->m_NrOfCards + m_NrOfCards;
		if (Application::Get().Player2)
			add += Application::Get().Player2->m_NrOfCards;

		if (m_Type == 0)
			AddCard({ (m_Position.x + m_NrOfCards * 20) * Application::Get().m_Ratio, 150 * Application::Get().m_Ratio, 0.5 + 0.005 * add }, true);
		else
			AddCard({ (m_Position.x + m_NrOfCards * 20) * Application::Get().m_Ratio, 450 * Application::Get().m_Ratio, 0.5 + 0.005 * add }, true);
	}


	void Person::AddCard(const glm::vec3& finalPosition, bool hidden)
	{
		int indice = Application::Get().pachet[Application::Get().pachet.size() - 1];
		Application::Get().pachet.pop_back();

		std::string indiceString = std::to_string(indice);
		Card tempCard({ 845 * Application::Get().m_Ratio, 400 * Application::Get().m_Ratio, finalPosition.z }, { 96 * Application::Get().m_Ratio, 150 * Application::Get().m_Ratio }, std::make_shared<Texture>("../assets/textures2/" + indiceString + ".png"));
				
		if (hidden)
		{
			tempCard.m_Texture = std::make_shared<Texture>("../assets/textures2/back_red.png");
			tempCard.m_FrontTexture = std::make_shared<Texture>("../assets/textures2/" + indiceString + ".png");
		}

		tempCard.m_FinalPos = finalPosition;

		tempCard.m_FinalRotation = 0;

		tempCard.CalculateStep();
		m_NrOfCards++;

		if (indice % 13)
		{
			if (!hidden)
				m_Sum += std::min(indice % 13 + 1, 10);
			tempCard.m_Indice = std::min(indice % 13 + 1, 10);
		}
		else
		{
			m_NrOfAces++;
			if (!hidden)
				m_Sum += 11;
			tempCard.m_Indice = 11;
		}

		m_Cards.push_back(tempCard);
	}
}