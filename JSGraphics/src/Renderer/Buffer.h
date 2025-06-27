#pragma once
#include <stdint.h>

namespace JSG {

	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size, float* vertices);
		~VertexBuffer();

		void Bind();
		void Unbind();
	private:
		uint32_t m_VertexBufferID;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t size, uint32_t* indices);
		~IndexBuffer();

		void Bind();
		void Unbind();
	private:
		uint32_t m_IndexBufferID;
	};

}