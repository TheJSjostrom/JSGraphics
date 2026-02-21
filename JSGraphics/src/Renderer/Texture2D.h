#pragma once
#include <string>

namespace JSG {

	struct ImageData
	{
		uint8_t* Data = nullptr;
		int32_t Width = 0;
		int32_t Height = 0;
		int32_t Channels = 0;

		bool IsLoaded() const { return Data != nullptr; }
	};

	struct ImageFormat
	{
		uint32_t InternalFormat = 0;
		uint32_t DataFormat = 0;

		bool IsSupported() const { return InternalFormat != 0 && DataFormat != 0; }
	};

	class Texture2D
	{
	public:
		Texture2D(const std::string& path);
		~Texture2D();

		void Bind(uint32_t slot = 0) const;
	private:
		struct TextureSpecification
		{
			uint32_t Width = 0;
			uint32_t Height = 0;
			uint32_t ColorChannels = 0;
		};

		TextureSpecification m_Spec;
		std::string m_Path;
		uint32_t m_TextureID;
	};

}