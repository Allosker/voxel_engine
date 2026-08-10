#include "gfx_chunk.hpp"


namespace gfx
{
    Chunk::Chunk(types::chunk_loc location) noexcept
        : m_loc{ location }
    {
        m_voxels.resize(g_absolute_size);
    }

    void Chunk::set_voxel_at(types::voxel_loc loc, Voxel new_voxel) noexcept
    {
        if (m_empty)
            m_voxels.resize(g_absolute_size);

        at(loc) = new_voxel;
    }

    void Chunk::set_empty() noexcept
    {
        m_empty = true;

        m_voxels.resize(1); // minimal size
        m_voxels.shrink_to_fit();

        m_voxels[0] = Voxel{ .type_id{0} };
    }

}