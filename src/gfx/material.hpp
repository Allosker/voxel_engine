#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
*/

#include "gfx/shader.hpp"


namespace gfx
{
	class Texture;

	enum class RenderSlot
	{
		Scene,

		Dynamic_Start,
	};

	struct BlockInstance
	{
		BlockInstance(const Shader::BlockDefinition& def);
		~BlockInstance();

		BlockInstance(const BlockInstance&) = delete;
		BlockInstance& operator=(const BlockInstance&) = delete;

		BlockInstance(BlockInstance&& other) noexcept;
		BlockInstance& operator=(BlockInstance&& other) noexcept;

		void update();

	private:
		uint32_t id = -1;
		std::vector<char> data;		
		bool isDirty = true;

		friend class Material;
	};
	
	class Material
	{
	public:
		Material(Shader* shader) noexcept;

		Material(Material&& other) noexcept = default;
		Material& operator=(Material&& other) noexcept = default;

		template<typename T>
		void set(StringHash name, const T& value) noexcept
		{
			auto it = m_shader->get_uniform_definitions().find(name);
			if (it != m_shader->get_uniform_definitions().end())
			{
				const Shader::UniformDefinition& def = it->second;
				if (def.block >= 0)
				{
					auto& block = m_blocks[def.block];

					const auto i = sizeof(T);
					assert(def.pos + sizeof(T) <= block.data.size());

					std::memcpy(block.data.data() + def.pos, &value, sizeof(T));

					block.isDirty = true;
				}
				else
				{
					if constexpr (std::is_same_v<T, Texture*>)
					{
						assert(def.type == GL_SAMPLER_2D || def.type == GL_SAMPLER_3D);
						m_textures[def.textureSlot] = value;
					}
					else
					{
						assert(false && "wrong type passed");
					}
				}
			}
		}

		Shader& get_shader() const
		{
			return *m_shader;
		}

		void updateBlocks() noexcept;

		void bindTextures() const noexcept;
		void bindBlocks() const noexcept;

	private:

		std::vector<BlockInstance> m_blocks;
		std::vector<Texture*> m_textures;

		Shader* m_shader;
	};
}