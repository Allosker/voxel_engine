#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <print>


namespace gfx
{
	// =====================
	// Construction/Destruction
	// =====================

	Shader::Shader(const filepath& vertShader, const filepath& fragShader, const filepath& geomShader)
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

		init(vertCode, fragCode, geomCode);
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

	GLuint Shader::id() const noexcept
	{
		return m_id;
	}

	GLuint Shader::get_uni_loc(std::string_view name) const noexcept
	{
		return glGetUniformLocation(m_id, name.data());
	}


	// =====================
	// Setters
	// =====================

	void Shader::set_value(std::string_view name, float value) const noexcept
	{
		glUniform1f(glGetUniformLocation(m_id, name.data()), value);
	}

	void Shader::set_value(std::string_view name, const v2f32& value) const noexcept
	{
		glUniform2fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
	}

	void Shader::set_value(std::string_view name, const v3f32& value) const noexcept
	{
		glUniform3fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
	}

	void Shader::set_value(std::string_view name, const v4f32& value) const noexcept
	{
		glUniform4fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
	}

	void Shader::set_value(std::string_view name, const m3f32& value) const noexcept
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name.data()), 1, false, glm::value_ptr(value));
	}

	void Shader::set_value(std::string_view name, const m4f32& value) const noexcept
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, false, glm::value_ptr(value));
	}

	void Shader::set_value_loc(GLint location, const m4f32& value) const noexcept
	{
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
	}


	const gfx::GlobalUniformBlockDefinition* Shader::FindGlobalUniformBlockDefinition(StringHash id)
	{
		const auto it = g_globalBlockDefinitions.find(id);
		if (it == g_globalBlockDefinitions.end())
		{
			return nullptr;
		}

		return &it->second;
	}

	// =====================
	// Private
	// =====================

	void Shader::compile(GLuint s_id, std::string_view name)
	{
		glCompileShader(s_id);

		GLint success;

		glGetShaderiv(s_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(s_id, 1024, nullptr, infoLog);

			std::println("ERROR::SHADER::COMPILATION_FAILED::{}\nInfoLog:\n{}\n", std::string{name.data()}, infoLog);
			throw;
		}
	}

	void Shader::link(GLuint s_id)
	{
		glLinkProgram(s_id);

		GLint success;
		
		glGetProgramiv(s_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(s_id, 1024, nullptr, infoLog);

			std::println("ERROR::SHADER::LINKAGE_FAILED::Shader_Program::InfoLog:\n{}\n", infoLog);
			throw;
		}
	}

	void Shader::init(std::string_view vert, std::string_view frag, std::string_view geom)
	{
		// Compile/Link shaders

		const char* vCode{ vert.data() };
		const char* fCode{ frag.data() };
		const char* gCode{ geom.data() };

		// Create
		GLuint v_ID{ glCreateShader(GL_VERTEX_SHADER) }, f_ID{ glCreateShader(GL_FRAGMENT_SHADER) }, g_ID{};
		if (!geom.empty())
			g_ID = glCreateShader(GL_GEOMETRY_SHADER);

		glShaderSource(v_ID, 1, &vCode, nullptr);
		glShaderSource(f_ID, 1, &fCode, nullptr);
		if (!geom.empty())
			glShaderSource(g_ID, 1, &gCode, nullptr);


		// Compile
		compile(v_ID, "vertex");
		compile(f_ID, "fragment");
		if (!geom.empty())
			compile(g_ID, "geometry");


		glAttachShader(m_id, v_ID);
		glAttachShader(m_id, f_ID);
		if (!geom.empty())
			glAttachShader(m_id, g_ID);

		link(m_id);


		// Delete
		glDeleteShader(v_ID);
		glDeleteShader(f_ID);
		if (!geom.empty())
			glDeleteShader(g_ID);


		GLint numBlocks{};
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);

		GLint maxBlockNameLength{};
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxBlockNameLength);
		std::vector<GLchar> blockNameBuffer(maxBlockNameLength);

		std::vector<int32_t> blockIndexRealIndex;
		std::vector<UniformDefinitions*> blockIndexToDefinitions;

		m_blockDefinitions.reserve(numBlocks);
		for (GLint blockIndex{}; blockIndex < numBlocks; ++blockIndex)
		{
			GLsizei length;
			glGetActiveUniformBlockName(m_id, blockIndex, maxBlockNameLength, &length, blockNameBuffer.data());
			UniformBlockDefinition blockDef;

			GLint blockSize{};
			glGetActiveUniformBlockiv(m_id, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
			blockDef.totalSize = static_cast<size_t>(blockSize);

			//blockDef.index = blockIndex;
			blockDef.name = std::string_view(blockNameBuffer.data(), length);

			if (g_globalBlockNames.contains(blockDef.name))
			{
				auto it = g_globalBlockDefinitions.find(blockDef.name);
				if (it == g_globalBlockDefinitions.end())
				{
					assert(g_globalBlockDefinitions.size() < MaxGlobalBlocks);

					blockDef.bindSlot = g_globalBlockDefinitions.size();
					it = g_globalBlockDefinitions.emplace(blockDef.name, GlobalUniformBlockDefinition{ blockDef }).first;
				}
				else
				{
					blockDef.bindSlot = it->second.bindSlot;
				}

				blockIndexRealIndex.push_back(-2);
				blockIndexToDefinitions.push_back(&it->second.m_uniformDefinitions);
			}
			else
			{
				blockDef.bindSlot = MaxGlobalBlocks + m_blockDefinitions.size();
				m_blockDefinitions.emplace_back(blockDef);

				blockIndexRealIndex.push_back(m_blockDefinitions.size() - 1);
				blockIndexToDefinitions.push_back(&m_uniformDefinitions);
			}

			glUniformBlockBinding(id(), blockIndex, blockDef.bindSlot);
		}

		GLint uniformCount{};
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &uniformCount);

		GLint maxUniformNameLength{};
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
		std::vector<GLchar> uniformNameBuffer(maxUniformNameLength);

		m_textureSlotCount = 0;

		for (GLuint i{}; i < uniformCount; ++i)
		{
			GLsizei length; 
			GLint size; 
			GLenum type;
			glGetActiveUniform(m_id, i, maxUniformNameLength, &length, &size, &type, uniformNameBuffer.data());

			std::string name(uniformNameBuffer.data());
			const auto charIndex = name.find('[');
			if (charIndex != std::string::npos)
			{
				name.resize(charIndex);
			}

			GLint blockIdx = -1;
			glGetActiveUniformsiv(m_id, 1, &i, GL_UNIFORM_BLOCK_INDEX, &blockIdx);

			GLint pos{};

			if (blockIdx < 0)
			{
				pos = glGetUniformLocation(m_id, uniformNameBuffer.data());
			}
			else
			{
				glGetActiveUniformsiv(m_id, 1, &i, GL_UNIFORM_OFFSET, &pos);
			}

			auto* targetUniformDefinitions = blockIdx < 0 ? &m_uniformDefinitions : blockIndexToDefinitions[blockIdx];
			const auto realBlockIndex = blockIdx < 0 ? -1 : blockIndexRealIndex[blockIdx];

			auto& def = (*targetUniformDefinitions)[name] = { realBlockIndex, pos, size, type, {}, name };

			if (blockIdx < 0 && (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE))
			{
				def.textureSlot = m_textureSlotCount++;
			}
		}
	}
}