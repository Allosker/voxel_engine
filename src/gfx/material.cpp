#include "Material.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "texture.hpp"

namespace gfx
{
	void Material::updateBlocks() noexcept
	{
		for (auto& block : m_blocks)
		{
			block.update();
		}
	}

	void Material::bindTextures() const noexcept
	{
		for (int index{}; index < m_textures.size(); index++)
		{	
			if(const auto* texture = m_textures[index])
			{ 
				texture->bind(index);
			}
		}
	}

	void Material::bindBlocks() const noexcept
	{
		for (int index{}; index < m_blocks.size(); index++)
		{
			const auto& block = m_blocks[index]; 
			
			glUniformBlockBinding(m_shader->id(), index, index);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, block.id);
		}
	}

	Material::Material(Shader* shader) noexcept : m_shader(shader)
	{
		m_textures.resize(shader->get_texture_count());

		const auto& blockDefs = shader->get_block_definitions();
		m_blocks.reserve(blockDefs.size());
		for (const auto& blockDef : blockDefs)
		{
			m_blocks.emplace_back(blockDef);
		}
	}

	BlockInstance::BlockInstance(const Shader::BlockDefinition& def)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferData(GL_UNIFORM_BUFFER, def.totalSize, nullptr, GL_DYNAMIC_DRAW);

		data.resize(def.totalSize);

		isDirty = true;
	}

	BlockInstance& BlockInstance::operator=(BlockInstance&& other) noexcept
	{
		id = other.id;
		data = std::move(other.data);
		isDirty = other.isDirty;

		other.id = -1;
		other.data.clear();
		isDirty = false;

		return *this;
	}

	BlockInstance::BlockInstance(BlockInstance&& other) noexcept
	{
		*this = std::move(other);
	}

	BlockInstance::~BlockInstance()
	{
		glDeleteBuffers(1, &id);
	}

	void BlockInstance::update()
	{
		if (isDirty)
		{
			isDirty = false;

			glBindBuffer(GL_UNIFORM_BUFFER, id);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, data.size(), data.data());
		}
	}

} // namespace gfx