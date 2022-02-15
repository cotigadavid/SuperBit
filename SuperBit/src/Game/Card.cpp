#include "sbpch.h"
#include "Card.h"

#include "Core/Application.h"

namespace SuperBit {

	Card::Card(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture> texture, float rotation)
		: m_Pos(pos), m_Size(size), m_Texture(texture), m_Rotation(rotation)
	{
		m_FinalPos = m_Pos;
		m_FinalRotation = rotation;
	}

	void Card::CalculateStep()
	{
		stepX = (m_FinalPos.x - m_Pos.x) / 0.3;
		stepY = (m_FinalPos.y - m_Pos.y) / 0.3;
		stepR = (m_FinalRotation - m_Rotation) / 0.3;

		nrOfSteps = (m_FinalPos.x - m_Pos.x) / (stepX * 0.016);
	}
}