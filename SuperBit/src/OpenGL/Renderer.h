#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Shader.h"

namespace SuperBit {

	class Renderer
	{
	public:
		static void Init();
		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform);

		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
	};
}