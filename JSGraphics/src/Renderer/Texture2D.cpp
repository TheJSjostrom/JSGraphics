#include "Texture2D.h"
#include "stb_image.h"
#include <print>
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace JSG {

	namespace Utils {

		static ImageData LoadImage(const std::string& path)
		{
			int32_t width, height, channels;
			uint8_t* textureData = stbi_load(path.c_str(), &width, &height, &channels, 0);

			return { textureData, width, height, channels };
		}

		static ImageFormat DetermineFormats(int32_t channels)
		{
			uint32_t internalFormat = 0, textureDataFormat = 0;
			switch (channels)
			{
			case 3:
				internalFormat = GL_RGB8;
				textureDataFormat = GL_RGB;
				break;
			case 4:
				internalFormat = GL_RGBA8;
				textureDataFormat = GL_RGBA;
				break;
			}

			return { internalFormat, textureDataFormat };
		}

	}

	Texture2D::Texture2D(const std::string& path) :
		m_Path(path),
		m_TextureID(0)
	{
		stbi_set_flip_vertically_on_load(1);

		const ImageData imageData = Utils::LoadImage(path);
		if (!imageData.IsLoaded())
		{
			std::println("Error. Failed to load image!");
			return;
		}

		const ImageFormat imageFormat = Utils::DetermineFormats(imageData.Channels);
		if (!imageFormat.IsSupported())
		{
			std::println("Format is not supported");
			stbi_image_free(imageData.Data);
			return;
		}

		m_Spec.Width = imageData.Width;
		m_Spec.Height = imageData.Height;
		m_Spec.ColorChannels = imageData.Channels;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, imageFormat.InternalFormat, imageData.Width, imageData.Height);
		
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Spec.Width, m_Spec.Height, imageFormat.DataFormat, GL_UNSIGNED_BYTE, imageData.Data);

		stbi_image_free(imageData.Data);
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