#include "Buffer.h"
#include "glad/glad.h"

namespace JSG {

	// VertexBuffer
	VertexBuffer::~VertexBuffer()
	{
		Shutdown();
	}

	void VertexBuffer::Init(uint32_t size, float* vertices)
	{
		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, reinterpret_cast<void*>(vertices), GL_STATIC_DRAW);
	}

	void VertexBuffer::Shutdown()
	{
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// IndexBuffer
	IndexBuffer::~IndexBuffer()
	{
		Shutdown();
	}

	void IndexBuffer::Init(uint32_t size, uint32_t* indices)
	{
		glCreateBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, reinterpret_cast<void*>(indices), GL_STATIC_DRAW);
	}

	void IndexBuffer::Shutdown()
	{
		glDeleteBuffers(1, &m_IndexBufferID);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}