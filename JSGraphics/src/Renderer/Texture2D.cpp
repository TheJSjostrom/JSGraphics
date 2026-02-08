#include "Texture2D.h"
#include "stb_image.h"
#include <print>
#include "glad/glad.h"
#include "glm/glm.hpp"
namespace JSG {

	Texture2D::Texture2D(const std::string& path) :
		m_Path(path)
	{
		int32_t width, height, colorChannels;
		uint8_t* textureData = stbi_load(path.c_str(), &width, &height, &colorChannels, 0);
		
		if (!textureData)
		{
			std::println("Error. Failed to load image!");
		}

		m_Width = width;
		m_Height = height;
		m_ColorChannels = colorChannels;

		uint32_t internalFormat = 0, textureDataFormat = 0;
		switch (colorChannels)
		{
		case 3:
			internalFormat = GL_RGB8;
			textureDataFormat = GL_RGB;
			break;
		case 4:
			internalFormat = GL_RGBA8;
			textureDataFormat = GL_RGBA;
			break;
		default:
			std::println("Error. Unsupported channel count {}", colorChannels);
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, width, height);
		
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, textureDataFormat, GL_UNSIGNED_BYTE, textureData);

		stbi_image_free(textureData);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	
	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}
}