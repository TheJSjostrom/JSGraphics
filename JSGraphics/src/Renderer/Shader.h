#pragma once
#include <string>
#include "glm/glm.hpp"

namespace JSG {

	class Shader
	{
	public:
		Shader() = default;
		Shader(const std::string& vertexScr, const std::string& fragSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void SetFloat(const std::string& name, float value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
	private:
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_ShaderID;
	};

}
