#include "Shader.h"


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
	: ShaderProgramID(0), debugShader(false)
{
	VertexShaderText = ReadFile(vertexShaderPath);
	FragmentShaderText = ReadFile(fragmentShaderPath);
	ShaderProgramID = CompileShaders(VertexShaderText, FragmentShaderText);
}


void Shader::DebugOn()
{
	debugShader = true;
}

void Shader::DebugOff()
{
	debugShader = true;
};

GLuint Shader::GetAttribLocation(std::string shaderAttribute)
{
	int id = glGetAttribLocation(GetShaderProgramID(), shaderAttribute.c_str());
	if (id < 0 && debugShader)
	{
		fprintf(stderr, "Error finding attrib '%s' in shader program id: '%i'\n", shaderAttribute.c_str(), ShaderProgramID);
	}
	return id;
}

std::string Shader::GetVertexShaderText()
{
	return VertexShaderText;
}
std::string Shader::GetFragmentShaderText()
{
	return FragmentShaderText;
}

GLuint Shader::GetShaderProgramID()
{
	return ShaderProgramID;
}

void Shader::Use()
{
	glUseProgram(ShaderProgramID);
}

GLuint Shader::GetUniformLocation(const char* uniform)
{
	int uniformId = glGetUniformLocation(ShaderProgramID, uniform);
	if (uniformId < 0 && debugShader)
	{
		fprintf(stderr, "Error finding uniform '%s' in shader program id: '%i'\n", uniform, ShaderProgramID);
	}
	return uniformId;
}

void Shader::SetUniform1f(const char* uniform, float value)
{
	Use();
	int uniformId = GetUniformLocation(uniform);
	if (uniformId < 0 && debugShader)
	{
		fprintf(stderr, "Error setting uniform '%s' in shader program id: '%i'\n", uniform, ShaderProgramID);
	}
	glUniform1f(GetUniformLocation(uniform), value);
}

GLuint Shader::GetUniformMatrix4fv(const char* mat)
{
	int uniformId = glGetUniformLocation(ShaderProgramID, mat);
	if (uniformId < 0 && debugShader)
	{
		fprintf(stderr, "Error finding uniform matrix '%s' in shader program id: '%i'\n", mat, ShaderProgramID);
	}
	return uniformId;
}

void Shader::SetUniformMatrix44fv(const char* mat, glm::mat4x4* matrix)
{
	Use();
	int id = GetUniformMatrix4fv(mat);
	if (id < 0 && debugShader) {
		fprintf(stderr, "Unable to set uniform matrix '%s' in shader program id: '%i'\n", mat, ShaderProgramID);
	}
	//void glUniformMatrix4fv(	GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	glUniformMatrix4fv(id, 1, GL_FALSE, &matrix[0][0][0]);
}

void Shader::SetUniformMatrix4fv(const char* mat, glm::mat4* matrix)
{
	Use();
	int id = GetUniformMatrix4fv(mat);
	if (id < 0 && debugShader) {
		fprintf(stderr, "Unable to set uniform matrix '%s' in shader program id: '%i'\n", mat, ShaderProgramID);
	}
	//void glUniformMatrix4fv(	GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	glUniformMatrix4fv(id, 1, GL_FALSE, &matrix[0][0][0]);
}

GLuint Shader::GetUniformVec3(const char* vec)
{
	Use();
	int id = glGetUniformLocation(ShaderProgramID, vec);
	if (id < 0 && debugShader) {
		fprintf(stderr, "Unable to set vec3 '%s' in shader program id: '%i'\n", vec, ShaderProgramID);
	}

	return id;
}

void Shader::SetUniformVec3(const char* vec, glm::vec3 vector)
{
	Use();
	int id = GetUniformMatrix4fv(vec);
	if (id < 0 && debugShader) {
		fprintf(stderr, "Unable to set vec3 '%s' in shader program id: '%i'\n", vec, ShaderProgramID);
	}
	glUniform3f(id, vector.x, vector.y, vector.z);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
