#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Operate as the master of the game, manages all big components and allows communication from one to another
* ==============================================-
*/ 

#include "sys/opengl_debug.hpp"
#include "sys/assetsManager.hpp"
#include "gfx/debugRenderer.hpp"

#include "sys/window.hpp"

#include "gfx/camera.hpp"

#include "gfx/mesh.hpp"
#include "gfx/shader.hpp"
#include "gfx/texture.hpp"

#include "gfx/world.hpp"




// Only one instance of the game must exist at a time
class Game
{
private:

	Game() noexcept = default; // Singleton


public:

	DebugMessage run();

	static Game& get() noexcept
	{
		static Game instance;

		return instance;
	}


private: // Internal Communication/Logic
/*
* Separate each component privately to easily make bridge between them
*/

	// = Main Components

	void inputs();

	void logic();

	void debug();
		
		// - Debug Specific
		
		void debug_imgui();


	void render_on_screen();


public:


	std::unique_ptr<Window> window;

	gfx::Camera camera{};


	gfx::World world{};


	class DeltaTime
	{
	public:

		f32 get() const noexcept { return dt; }

		void update(f32 current_time) noexcept
		{
			dt = current_time - last_frame;
			last_frame = glfwGetTime();
		}

		void limit() noexcept { dt = std::min(dt, 1.f / 30.f); }

	private:

		f32 dt{};
		f32 last_frame{};
	} delta_time;

	f32 fps{};
	f32 target_fps{ 160 };


	v2f32 last_mouse_window_pos{};
	f32 yaw{}, pitch{};

	bool was_f2_pressed{};

	struct RenderSettings
	{
		u32 MSAA{ 8 };

	} render_settings;

	// Debug
	bool compute_noise_map{ false };

};