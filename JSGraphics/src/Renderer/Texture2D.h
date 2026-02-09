#pragma once
#include <string>

namespace JSG {

	struct ImageData
	{
		uint8_t* Data = nullptr;
		int32_t Width = 0;
		int32_t Height = 0;
		int32_t Channels = 0;
	};

	struct ImageFormat
	{
		uint32_t InternalFormat = 0;
		uint32_t DataFormat = 0;
	};

	class Texture2D 
	{
	public:
		Texture2D(const std::string& path);
		~Texture2D();

		void Bind(uint32_t slot = 0) const;
	private:
		ImageData LoadImage(const std::string& path) const;
		ImageFormat DetermineFormats(int32_t channels) const;
	private:
		std::string m_Path;

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_ColorChannels;
		uint32_t m_TextureID;
	};

}