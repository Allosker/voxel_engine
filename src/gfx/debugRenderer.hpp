#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple debug renderer --
* ==============================================-
* Code given by https://github.com/lapinozz
* ==============================================-
*/

#include <vector>

#include "gfx/mesh.hpp"
#include "gfx/shader.hpp"
#include "sys/types.hpp"
#include "vertices.hpp"
#include "sys/graphics.hpp"


namespace gfx
{
	/// <summary>
	/// Simple debug renderer, everything is considered by default to be 3D unless said otherwise
	/// </summary>
	class DebugRenderer
	{
		static constexpr const char* linePoint3DVertShaderSrc = "\n"
			"#version 460 core\n"
			"\n"
			"layout (location = 0) in vec3 in_Position;\n"
			"layout (location = 1) in vec4 in_Color;\n"
			"\n"
			"out vec4 v_Color;\n"
			"uniform mat4 vp;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    gl_Position  = vp * vec4(in_Position, 1.0);\n"
			"    v_Color      = in_Color;\n"
			"}\n";

		static constexpr const char* linePoint2DVertShaderSrc = "\n"
			"#version 460 core\n"
			"\n"
			"layout (location = 0) in vec2 in_Position;\n"
			"layout (location = 1) in vec4 in_Color;\n"
			"\n"
			"out vec4 v_Color;\n"
			"uniform mat4 ortho;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    gl_Position  = ortho * vec4(in_Position, 0., 1.);\n"
			"    v_Color      = in_Color;\n"
			"}\n";

		static constexpr const char* linePointFragShaderSrc = "\n"
			"#version 460 core\n"
			"\n"
			"in  vec4 v_Color;\n"
			"out vec4 out_FragColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    out_FragColor = v_Color;\n"
			"}\n";



		DebugRenderer()
			: shader_3D{ linePoint3DVertShaderSrc, linePointFragShaderSrc }, shader_2D{ linePoint2DVertShaderSrc, linePointFragShaderSrc }
		{
			m_lines_2D.mesh.create_buffer<Vertex2DRGBA>(false);

			m_lines_world.mesh.create_buffer<VertexRGBA>(false);
			m_lines_foreground.mesh.create_buffer<VertexRGBA>(false);

			m_triangles_world.mesh.create_buffer<VertexRGBA>(false);
			m_triangles_foreground.mesh.create_buffer<VertexRGBA>(false);


			shader_3D_vp_loc = shader_3D.get_uni_loc("vp");
			shader_2D_ortho_loc = shader_2D.get_uni_loc("ortho");
		}

		template<typename T>
		struct VertexList
		{
			/// <summary>
			/// First is the time the resource was allocated
			/// <para> Second is the resource (vertex) pushed at the time of allocation </para>
			/// </summary>
			std::vector<T> data;
			std::vector<f32> pendings;

			Mesh mesh;

			bool dirty{};
		};


	public:

		static DebugRenderer& get()
		{
			static DebugRenderer instance;
			return instance;
		}

		
		void add_2D_line(v2f32 start, v2f32 end, v3f32 color, f32 duration = 0.f)
		{
			m_lines_2D.data.push_back({ start, color });
			m_lines_2D.data.push_back({ end, color });

			const auto dur = m_time + duration;
			m_lines_2D.pendings.push_back(dur);
			m_lines_2D.pendings.push_back(dur);

			m_lines_2D.dirty = true;
		}

		void addLine(v3f32 start, v3f32 end, v3f32 color, f32 duration = 0.f, bool foreground = true)
		{
			auto& list = foreground ? m_lines_foreground : m_lines_world;

			list.data.push_back({ start, color });
			list.data.push_back({ end, color });

			const auto dur = m_time + duration;
			list.pendings.push_back(dur);
			list.pendings.push_back(dur);

			list.dirty = true;
		}

		void addTriangle(v3f32 p1, v3f32 p2, v3f32 p3, v3f32 color, float duration = 0.f, bool foreground = true)
		{
			auto& list = foreground ? m_triangles_foreground : m_triangles_world;

			list.data.push_back({ p1, color });
			list.data.push_back({ p2, color });
			list.data.push_back({ p3, color });

			const auto dur = duration < 0.f ? -1.f : m_time + duration;
			list.pendings.push_back(dur);
			list.pendings.push_back(dur);
			list.pendings.push_back(dur);

			list.dirty = true;
		}

		void update(f32 current_time)
		{
			m_time = current_time;

			const auto update_list = [&](auto& list) -> void
			{
				size_t j{};
				for (auto i{ list.pendings.begin() }; i != list.pendings.end();)
				{
					if (*i <= 0.f || *i < current_time)
					{
						i = list.pendings.erase(i);
						list.data.erase(list.data.begin() + j);

						list.dirty = true;
					}
					else
					{
						i++;
						j++;
					}
				}
			};

			update_list(m_lines_world);
			update_list(m_lines_foreground);
			update_list(m_triangles_world);
			update_list(m_triangles_foreground);

			update_list(m_lines_2D);
		}

		void render3D(const m4f32& vp_3D)
		{
			const auto render_list = [&](auto& list)
			{
				if (list.dirty)
				{
					list.mesh.update_buffer(list.data, GL_DYNAMIC_DRAW);
					list.dirty = false;
				}
				
				list.mesh.draw();
			};


			glDisable(GL_CULL_FACE);

			shader_3D.bind();

			shader_3D.set_value_loc(shader_3D_vp_loc, vp_3D);

			glDisable(GL_DEPTH_TEST);
			render_list(m_lines_foreground);
			render_list(m_triangles_foreground);

			glEnable(GL_DEPTH_TEST);
			render_list(m_lines_world);
			render_list(m_triangles_world);
		}

		void render2D(const m4f32& ortho_2D)
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);

			shader_2D.bind();

			shader_2D.set_value_loc(shader_2D_ortho_loc, ortho_2D);


			if (m_lines_2D.dirty)
			{
				m_lines_2D.mesh.update_buffer(m_lines_2D.data, GL_DYNAMIC_DRAW);
				m_lines_2D.dirty = false;
			}

			m_lines_2D.mesh.draw();

			shader_2D.unbind();
		}


	private:

		f32 m_time{};

		VertexList<Vertex2DRGBA> m_lines_2D;

		VertexList<VertexRGBA> m_lines_world;
		VertexList<VertexRGBA> m_lines_foreground;

		VertexList<VertexRGBA> m_triangles_world;
		VertexList<VertexRGBA> m_triangles_foreground;


		Shader shader_3D;
		Shader shader_2D;

		GLint  shader_3D_vp_loc;
		GLint  shader_2D_ortho_loc;
	};


	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="color"></param>
	/// <param name="duration: -1 for infinite duration"></param>
	/// <param name="foreground"></param>
	inline void line(v3f32 start, v3f32 end, v3f32 color, float duration = 0.f, bool foreground = true)
	{
		gfx::DebugRenderer::get().addLine(start, end, color, duration, foreground);
	}

	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="color"></param>
	/// <param name="duration: -1 for infinite duration"></param>
	/// <param name="foreground"></param>
	inline void aabb(v3f32 center, v3f32 extent, v3f32 color, float duration = 0.f, bool foreground = true)
	{
		const auto min = center - extent;
		const auto max = center + extent;

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

		auto& renderer = gfx::DebugRenderer::get();

		renderer.addLine(corners[0], corners[1], color, duration, foreground);
		renderer.addLine(corners[1], corners[2], color, duration, foreground);
		renderer.addLine(corners[2], corners[3], color, duration, foreground);
		renderer.addLine(corners[3], corners[0], color, duration, foreground);

		renderer.addLine(corners[4], corners[5], color, duration, foreground);
		renderer.addLine(corners[5], corners[6], color, duration, foreground);
		renderer.addLine(corners[6], corners[7], color, duration, foreground);
		renderer.addLine(corners[7], corners[4], color, duration, foreground);

		renderer.addLine(corners[0], corners[4], color, duration, foreground);
		renderer.addLine(corners[1], corners[5], color, duration, foreground);
		renderer.addLine(corners[2], corners[6], color, duration, foreground);
		renderer.addLine(corners[3], corners[7], color, duration, foreground);
	}

	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <param name="color"></param>
	/// <param name="duration"></param>
	/// <param name="foreground"></param>
	inline void aabb_min_max(v3f32 min, v3f32 max, v3f32 color, float duration = 0.f, bool foreground = true)
	{
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

		auto& renderer = gfx::DebugRenderer::get();

		renderer.addLine(corners[0], corners[1], color, duration, foreground);
		renderer.addLine(corners[1], corners[2], color, duration, foreground);
		renderer.addLine(corners[2], corners[3], color, duration, foreground);
		renderer.addLine(corners[3], corners[0], color, duration, foreground);

		renderer.addLine(corners[4], corners[5], color, duration, foreground);
		renderer.addLine(corners[5], corners[6], color, duration, foreground);
		renderer.addLine(corners[6], corners[7], color, duration, foreground);
		renderer.addLine(corners[7], corners[4], color, duration, foreground);

		renderer.addLine(corners[0], corners[4], color, duration, foreground);
		renderer.addLine(corners[1], corners[5], color, duration, foreground);
		renderer.addLine(corners[2], corners[6], color, duration, foreground);
		renderer.addLine(corners[3], corners[7], color, duration, foreground);
	}

	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <param name="color"></param>
	/// <param name="duration"></param>
	/// <param name="foreground"></param>
	inline void aabb2D_min_max(v2f32 min, v2f32 max, v3f32 color, float duration = 0.f, bool foreground = true)
	{
		const v2f32 corners[] = {
			{min.x, min.y }, // base corner
			{min.x, max.y }, // upper base
			{max.x, max.y }, // upper left
			{max.x, min.y }, // lower left
		};

		auto& renderer = gfx::DebugRenderer::get();

		renderer.add_2D_line(corners[0], corners[1], color, duration);
		renderer.add_2D_line(corners[1], corners[2], color, duration);
		renderer.add_2D_line(corners[2], corners[3], color, duration);
		renderer.add_2D_line(corners[3], corners[0], color, duration);
	}


	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="color"></param>
	/// <param name="single_transformation: local transforms that mesh"></param>
	/// <param name="duration: -1 for infinite duration"></param>
	/// <param name="foreground"></param>
	inline void obb(v3f32 center, v3f32 extent, v3f32 color, m4f32 single_transform, float duration = 0.f, bool foreground = true)
	{
		const auto min = center - extent;
		const auto max = center + extent;

		v3f32 corners[] = {
			{min.x, min.y, min.z}, // base corner
			{min.x, max.y, min.z}, // upper base
			{max.x, max.y, min.z}, // upper left
			{max.x, min.y, min.z}, // lower left

			{min.x, min.y, max.z}, // front
			{min.x, max.y, max.z}, // upper front
			{max.x, max.y, max.z}, // opposite
			{max.x, min.y, max.z}, // left front
		};

		auto& renderer = gfx::DebugRenderer::get();

		for (size_t i{}; i < 8; i++)
		{
			v4f32 vec{ corners[i], 1.f };

			vec = single_transform * vec;

			vec /= vec.w;

			corners[i] = v3f32{ vec.x, vec.y, vec.z };
		}

		renderer.addLine(corners[0], corners[1], color, duration, foreground);
		renderer.addLine(corners[1], corners[2], color, duration, foreground);
		renderer.addLine(corners[2], corners[3], color, duration, foreground);
		renderer.addLine(corners[3], corners[0], color, duration, foreground);

		renderer.addLine(corners[4], corners[5], color, duration, foreground);
		renderer.addLine(corners[5], corners[6], color, duration, foreground);
		renderer.addLine(corners[6], corners[7], color, duration, foreground);
		renderer.addLine(corners[7], corners[4], color, duration, foreground);

		renderer.addLine(corners[0], corners[4], color, duration, foreground);
		renderer.addLine(corners[1], corners[5], color, duration, foreground);
		renderer.addLine(corners[2], corners[6], color, duration, foreground);
		renderer.addLine(corners[3], corners[7], color, duration, foreground);
	}

	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="color"></param>
	/// <param name="duration: -1 for infinite duration"></param>
	/// <param name="foreground"></param>
	inline void triangle_fill(v3f32 p1, v3f32 p2, v3f32 p3, v3f32 color, float duration = 0.f, bool foreground = true)
	{
		gfx::DebugRenderer::get().addTriangle(p1, p2, p3, color, duration, foreground);
	}


}