#pragma once

#include "OpenGL/Texture.h"
#include <glm/glm.hpp>

#include "Core/Timestep.h"

namespace SuperBit {
	class Card
	{
	public:
		Card(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture> texture, float rotation = 0);

		void CalculateStep();

	public:
		std::shared_ptr<Texture> m_Texture;
		std::shared_ptr<Texture> m_FrontTexture;
		glm::vec3 m_Pos;
		float m_Rotation;
		glm::vec2 m_Size;
		
		// ANIMATION PURPOSES
		glm::vec3 m_FinalPos;
		float m_FinalRotation;

		float stepX;
		float stepY;
		float stepR;

		float nrOfSteps = 0;

		// ANIMATION PURPOSES

		int m_Indice = 0;
	};
}