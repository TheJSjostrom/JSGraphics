#pragma once
#include <stdint.h>

namespace JSG {

	class VertexBuffer
	{
	public:
		~VertexBuffer();

		void Init(uint32_t size, float* vertices);

		void Bind();
		void Unbind();
	private:
		void Shutdown();
	private:
		uint32_t m_VertexBufferID = 0;
	};

	class IndexBuffer
	{
	public:
		~IndexBuffer();

		void Init(uint32_t size, uint32_t* indices);

		void Bind();
		void Unbind();
	private:
		void Shutdown();
	private:
		uint32_t m_IndexBufferID = 0;
	};

}