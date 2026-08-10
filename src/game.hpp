#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Operate as the master of the game, manages all big components and allows communication from one to another
* ==============================================-
*/ 

#include "sys_opengl_debug.hpp"
#include "sys_assetsManager.hpp"
#include "gfx_debugRenderer.hpp"

#include "sys_window.hpp"

#include "gfx_camera.hpp"

#include "gfx_mesh.hpp"
#include "gfx_shader.hpp"
#include "gfx_texture.hpp"

#include "gfx_world.hpp"




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

		void update() noexcept
		{
			dt = glfwGetTime() - last_frame;
			last_frame = glfwGetTime();
		}

	private:

		f32 dt{};
		f32 last_frame{};
	} delta_time;

	v2f32 last_mouse_window_pos{};
	f32 yaw{}, pitch{};

	bool was_f2_pressed{};

	// Debug
	bool compute_noise_map{ false };

};