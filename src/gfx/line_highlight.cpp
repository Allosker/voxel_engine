#include "line_highlight.hpp"
#include <vector>

namespace gfx
{


	void LineHighlight::construct(const phy::HitboxAABB& hitbox) noexcept
	{
		std::vector<Line::LineVertex> mesh(24);

		const auto min = static_cast<v3f32>(hitbox.get_pos() - hitbox.get_extent());
		const auto max = static_cast<v3f32>(hitbox.get_pos() + hitbox.get_extent());

		const v3f32 corners[] = {
			{min.x, min.y, min.z}, // base corner
			{min.x, max.y, min.z}, // upper base
			{max.x, max.y, min.z}, // upper left
			{max.x, min.y, min.z}, // lower left

			{min.x, min.y, max.z}, // front
			{min.x, max.y, max.z}, // upper front
			{max.x, max.y, max.z}, // opposite
			{max.x, min.y, max.z}, // left front
		};

		mesh.emplace_back(Line::LineVertex{ corners[0], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[1], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[1], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[2], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[2], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[3], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[3], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[0], m_color });

		mesh.emplace_back(Line::LineVertex{ corners[4], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[5], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[5], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[6], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[6], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[7], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[7], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[4], m_color });

		mesh.emplace_back(Line::LineVertex{ corners[0], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[4], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[1], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[5], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[2], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[6], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[3], m_color });
		mesh.emplace_back(Line::LineVertex{ corners[7], m_color });

		m_mesh.update_buffer(mesh);
	}



}