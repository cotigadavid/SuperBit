#pragma once

#include "Card.h"
#include "Core/Timestep.h"

namespace SuperBit
{
	class Person
	{
	public:
		Person(int type) : m_Type(type) {};

		void AddCard();
		void AddHiddenCard();
		void AddCard(const glm::vec3& finalPosition, bool hidden = false);

	public:
		int m_Bet = 10;
		int m_Type = -1;              // 0 - Player / 1 - Dealer
		int m_NrOfCards = 0;
		int m_NrOfAces = 0;
		int m_Sum = 0;
		std::vector<Card> m_Cards;
		std::string m_Message;

		glm::vec2 m_Position;

		bool GameOver = false;
		bool TurnOver = false;
	};
}