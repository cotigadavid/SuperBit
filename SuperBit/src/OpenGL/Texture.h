#pragma once

#include <string>

#include <glad/glad.h>

namespace SuperBit {

	class Texture
	{
	public:
		Texture(uint32_t width, uint32_t height);
		Texture(const std::string& path);
		~Texture();

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }
		unsigned int GetRendererID() const { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size);

		void Bind(unsigned int slot = 0) const;

		virtual bool operator==(const Texture& other) const 
		{
			return m_RendererID == other.m_RendererID;
		}

	private:
		std::string m_Path;
		unsigned int m_Width, m_Height;
		unsigned int m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}