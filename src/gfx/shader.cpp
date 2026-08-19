#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <string>


namespace gfx
{
	// =====================
	// Construction/Destruction
	// =====================

	Shader::Shader(const filepath& vertShader, const filepath& fragShader, const filepath& geomShader)
	try
	{
		std::string vertCode{}, fragCode{}, geomCode{};

		std::ifstream vSFile{}, fSFile{}, gSFile{};

		vSFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fSFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gSFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// - Read Content from Files

		// Open Files
		vSFile.open(vertShader);
		fSFile.open(fragShader);
		if (!geomShader.empty())
			gSFile.open(geomShader);

		// Move Streams
		std::stringstream vSStream{}, fSStream{}, gSStream{};

		vSStream << vSFile.rdbuf();
		fSStream << fSFile.rdbuf();
		if (!geomShader.empty())
			gSStream << gSFile.rdbuf();

		// CLose Files
		vSFile.close();
		fSFile.close();
		if (!geomShader.empty())
			gSFile.close();

		// Get Actual Code
		vertCode = vSStream.str();
		fragCode = fSStream.str();
		if (!geomShader.empty())
			geomCode = gSStream.str();

		// Compile/Link shaders

		const char* vCode{ vertCode.c_str() };
		const char* fCode{ fragCode.c_str() };
		const char* gCode{ geomCode.c_str() };

		// Create
		GLuint v_ID{ glCreateShader(GL_VERTEX_SHADER) }, f_ID{ glCreateShader(GL_FRAGMENT_SHADER) }, g_ID{};
		if (!geomShader.empty())
			g_ID = glCreateShader(GL_GEOMETRY_SHADER);

		glShaderSource(v_ID, 1, &vCode, nullptr);
		glShaderSource(f_ID, 1, &fCode, nullptr);
		if (!geomShader.empty())
			glShaderSource(g_ID, 1, &gCode, nullptr);


		// Compile
		compile(v_ID, "vertex");
		compile(f_ID, "fragment");
		if (!geomShader.empty())
			compile(g_ID, "geometry");


		// Link Shaders 
		m_id = glCreateProgram();

		glAttachShader(m_id, v_ID);
		glAttachShader(m_id, f_ID);
		if (!geomShader.empty())
			glAttachShader(m_id, g_ID);

		link(m_id);


		// Delete
		glDeleteShader(v_ID);
		glDeleteShader(f_ID);
		if (!geomShader.empty())
			glDeleteShader(g_ID);
	}
	catch (...)
	{
		throw;
	}


	Shader::Shader(Shader&& other) noexcept
		: m_id{ other.m_id }
	{
		other.m_id = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this == &other)
			return *this;

		m_id = other.m_id;
		other.m_id = 0;

		return *this;
	}


	Shader::~Shader() noexcept
	{
		glDeleteProgram(m_id);
	}


	// =====================
	// Actors
	// =====================

	void Shader::bind() const noexcept
	{
		glUseProgram(m_id);
	}

	void Shader::unbind() const noexcept
	{
		glUseProgram(0);
	}


	// =====================
	// Getters
	// =====================

	GLuint Shader::ID() const noexcept
	{
		return m_id;
	}

	std::int32_t Shader::getUniformLocation(std::string_view name) const noexcept
	{
		return glGetUniformLocation(m_id, name.data());
	}


	// =====================
	// Setters
	// =====================

	void Shader::setValue(std::string_view name, float value) const noexcept
	{
		glUniform1f(glGetUniformLocation(m_id, name.data()), value);
	}

	void Shader::setValue(std::string_view name, const v2f32& value) const noexcept
	{
		glUniform2fv(glGetUniformLocation(m_id, name.data()), 1, value.data_ptr());
	}

	void Shader::setValue(std::string_view name, const v3f32& value) const noexcept
	{
		glUniform3fv(glGetUniformLocation(m_id, name.data()), 1, value.data_ptr());
	}

	void Shader::setValue(std::string_view name, const v4f32& value) const noexcept
	{
		glUniform4fv(glGetUniformLocation(m_id, name.data()), 1, value.data_ptr());
	}

	void Shader::setValue(std::string_view name, const m3f32& value) const noexcept
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name.data()), 1, true, value.data_ptr());
	}

	void Shader::setValue(std::string_view name, const m4f32& value) const noexcept
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, false, value.data_ptr());
	}

	void Shader::setValueLocation(GLint location, const m4f32& value) const noexcept
	{
		glUniformMatrix4fv(location, 1, false, value.data_ptr());
	}


	// =====================
	// Private
	// =====================

	void Shader::compile(GLuint s_id, std::string_view name)
	{
		glCompileShader(s_id);

		GLint success;
		GLchar infoLog[1024];

		glGetShaderiv(s_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(s_id, 1024, nullptr, infoLog);

			throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED::" + std::string{name.data()} + '\n' + infoLog + '\n');
		}
	}

	void Shader::link(GLuint s_id)
	{
		glLinkProgram(s_id);

		GLint success;
		GLchar infoLog[1024];

		glGetProgramiv(s_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(s_id, 1024, nullptr, infoLog);

			throw std::runtime_error(std::string{ "ERROR::SHADER::LINKAGE_FAILED::Shader_Program::" } + infoLog + '\n');
		}
	}

}