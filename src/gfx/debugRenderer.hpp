#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple debug renderer -- 
* ==============================================-
* Code given by https://github.com/lapinozz
* ==============================================-
*/

#include "sys/graphics.hpp"
#include "vertices.hpp"


namespace gfx
{

	class DebugRenderer
	{
		static constexpr const char* linePointVertShaderSrc = "\n"
			"#version 150\n"
			"\n"
			"in vec3 in_Position;\n"
			"in vec4 in_Color;\n"
			"\n"
			"out vec4 v_Color;\n"
			"uniform mat4 u_MvpMatrix;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    gl_Position  = u_MvpMatrix * vec4(in_Position, 1.0);\n"
			"    v_Color      = in_Color;\n"
			"}\n";

		static constexpr const char* linePointFragShaderSrc = "\n"
			"#version 150\n"
			"\n"
			"in  vec4 v_Color;\n"
			"out vec4 out_FragColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    out_FragColor = v_Color;\n"
			"}\n";

		DebugRenderer()
		{
			linesWorld.primitiveSize = 2;
			linesWorld.primitiveSize = 2;
			trianglesWorld.primitiveSize = 3;
			trianglesForeground.primitiveSize = 3;

			GLuint linePointVS = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(linePointVS, 1, &linePointVertShaderSrc, nullptr);
			glCompileShader(linePointVS);

			GLint linePointFS = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(linePointFS, 1, &linePointFragShaderSrc, nullptr);
			glCompileShader(linePointFS);

			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, linePointVS);
			glAttachShader(shaderProgram, linePointFS);

			glBindAttribLocation(shaderProgram, 0, "in_Position");
			glBindAttribLocation(shaderProgram, 1, "in_Color");
			glLinkProgram(shaderProgram);

			shaderMatrixLocation = glGetUniformLocation(shaderProgram, "u_MvpMatrix");

			const auto& setupArrayList = [&](VertexList& list)
				{
					glGenVertexArrays(1, &list.VAO);
					glGenBuffers(1, &list.VBO);

					glBindVertexArray(list.VAO);
					glBindBuffer(GL_ARRAY_BUFFER, list.VBO);

					// RenderInterface will never be called with a batch larger than
					// DEBUG_DRAW_VERTEX_BUFFER_SIZE vertexes, so we can allocate the same amount here.
					glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

					// Set the ::data::vertex format expected by 3D points and lines:
					std::size_t offset = 0;

					glEnableVertexAttribArray(0); // in_Position (vec3)
					glVertexAttribPointer(
						/* index     = */ 0,
						/* size      = */ 3,
						/* type      = */ GL_FLOAT,
						/* normalize = */ GL_FALSE,
						/* stride    = */ sizeof(gfx::VertexRGBA),
						/* offset    = */ reinterpret_cast<void*>(offset));
					offset += sizeof(float) * 3;

					glEnableVertexAttribArray(1); // in_Color (vec4)
					glVertexAttribPointer(
						/* index     = */ 1,
						/* size      = */ 4,
						/* type      = */ GL_FLOAT,
						/* normalize = */ GL_FALSE,
						/* stride    = */ sizeof(gfx::VertexRGBA),
						/* offset    = */ reinterpret_cast<void*>(offset));

					// VAOs can be a pain in the neck if left enabled...
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				};

			setupArrayList(linesWorld);
			setupArrayList(linesForeground);
			setupArrayList(trianglesWorld);
			setupArrayList(trianglesForeground);
		}


	public:

		

		static DebugRenderer& get()
		{
			static DebugRenderer instance;
			return instance;
		}

		struct VertexList
		{
			int primitiveSize{};

			std::vector<gfx::VertexRGBA> vertices;
			std::vector<float> pendings;

			GLuint VAO{};
			GLuint VBO{};

			bool dirty{};
		};

		void addLine(v3f32 start, v3f32 end, v3f32 color, float duration = 0.f, bool foreground = true)
		{
			auto& list = foreground ? linesForeground : linesWorld;

			list.vertices.push_back({ start, color });
			list.vertices.push_back({ end, color });

			list.pendings.push_back(duration < 0.f ? -1.f : time + duration);

			list.dirty = true;
		}

		void addTriangle(v3f32 p1, v3f32 p2, v3f32 p3, v3f32 color, float duration = 0.f, bool foreground = true)
		{
			auto& list = foreground ? trianglesForeground : trianglesWorld;

			list.vertices.push_back({ p1, color });
			list.vertices.push_back({ p2, color });
			list.vertices.push_back({ p3, color });

			list.pendings.push_back(duration < 0.f ? -1.f : time + duration);

			list.dirty = true;
		}

		void update(float currentTime)
		{
			time = currentTime;

			const auto updateList = [&](VertexList& list)
				{
					for (int x = 0; x < list.pendings.size(); x++)
					{
						float endTime = list.pendings[x];
						if (endTime < 0.f || endTime > currentTime)
							continue;

						std::swap(list.pendings[x], list.pendings.back());
						list.pendings.resize(list.pendings.size() - 1);

						for (int y = 0; y < list.primitiveSize; y++)
							std::swap(list.vertices[x * list.primitiveSize + y], list.vertices[list.vertices.size() - list.primitiveSize + y]);

						list.vertices.resize(list.vertices.size() - list.primitiveSize);

						list.dirty = true;
					}
				};

			updateList(linesWorld);
			updateList(linesForeground);
			updateList(trianglesWorld);
			updateList(trianglesForeground);
		}
		
		void erase_all()
		{
			const auto eraseList = [&](VertexList& list)
				{
					for (int x = 0; x < list.pendings.size(); x++)
					{
						std::vector<gfx::VertexRGBA> vertices;
						std::vector<float> pendings;

						list.vertices.swap(vertices);
						list.pendings.swap(pendings);

						list.dirty = true;
					}
				};

			eraseList(linesWorld);
			eraseList(linesForeground);
			eraseList(trianglesWorld);
			eraseList(trianglesForeground);
		}

		void render(m4f32 mvpMatrix)
		{
			glDisable(GL_CULL_FACE);

			const auto renderList = [&](VertexList& list, bool depthEnabled, GLenum type)
				{
					glBindVertexArray(list.VAO);
					glUseProgram(shaderProgram);

					glUniformMatrix4fv(shaderMatrixLocation, 1, GL_FALSE, mvpMatrix.data.data());

					if (depthEnabled)
					{
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						glDisable(GL_DEPTH_TEST);
					}

					// NOTE: Could also use glBufferData to take advantage of the buffer orphaning trick...
					glBindBuffer(GL_ARRAY_BUFFER, list.VBO);

					if (list.dirty)
					{
						glBufferData(GL_ARRAY_BUFFER, list.vertices.size() * sizeof(gfx::VertexRGBA), list.vertices.data(), GL_DYNAMIC_DRAW);
						list.dirty = false;
					}

					glDrawArrays(type, 0, list.vertices.size());

					glUseProgram(0);
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				};

			renderList(linesWorld, true, GL_LINES);
			renderList(linesForeground, false, GL_LINES);

			renderList(trianglesWorld, true, GL_TRIANGLES);
			renderList(trianglesForeground, false, GL_TRIANGLES);
		}


	private:

		float time{};

		VertexList linesWorld;
		VertexList linesForeground;

		VertexList trianglesWorld;
		VertexList trianglesForeground;

		GLuint shaderProgram;
		GLint  shaderMatrixLocation;
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