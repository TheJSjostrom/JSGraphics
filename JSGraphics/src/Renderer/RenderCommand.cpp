#include "RenderCommand.h"
#include "glad/glad.h"

namespace JSG {

	void RenderCommand::Init()
	{

	}

	void RenderCommand::Shutdown()
	{

	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::SetClearDepth(float depth)
	{
		glClearDepth(depth);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}