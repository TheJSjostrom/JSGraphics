#pragma once
#include <string>

namespace JSG {

	class Texture2D 
	{
	public:
		Texture2D(const std::string& path);

		~Texture2D();

		void Bind(uint32_t slot = 0) const;
	private:
		std::string m_Path;

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_ColorChannels;
		uint32_t m_TextureID;
	};

}