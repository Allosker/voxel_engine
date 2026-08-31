#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Operate as the master of the game, manages all big components and allows communication from one to another
* ==============================================-
*/

#include <memory>

#include "gfx/debugRenderer.hpp"
#include "sys/assetsManager.hpp"
#include "sys/opengl_debug.hpp"

#include "sys/window.hpp"

#include "gfx/camera.hpp"

#include "gfx/mesh.hpp"

#include "gfx/world.hpp"

#include "gfx/player.hpp"

#include "gfx/inventory.hpp"
#include "gui/inventoryGUI.hpp"

#include "gui/itemStackGUI.hpp"


// Only one instance of the game must exist at a time
class Game
{
private:

	Game() noexcept; // Singleton


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




	std::unique_ptr<Window> window;

	gfx::Camera camera{};

	gfx::World world{};

	gfx::Player player{ &camera };

	bool showDebugMenus{};

	m4f32 orthographic_proj{ glm::ortho(0.f, Window::g_gui_view_size.x, Window::g_gui_view_size.y, 0.f ) };

	gfx::Inventory    inv{};
	gui::InventoryGUI gui_inv{};


	class DeltaTime
	{
	public:

		f64 get() const noexcept { return dt; }

		void update(f64 current_time) noexcept
		{
			dt = current_time - last_frame;
			last_frame = glfwGetTime();
		}

		void limit() noexcept { dt = std::min(dt, 1. / 30.); }

	private:

		f64 dt{};
		f64 last_frame{};
	} delta_time;


	struct RuntimeSettings
	{
		bool paused{ false };
	} runtime_settings;

	f32 fps{};
	f32 target_fps{ 160 };


	v2f32 last_mouse_window_pos{};
	f32 yaw{}, pitch{};

	struct RenderSettings
	{
		u32 MSAA{ 8 };

	} render_settings;

	// Debug
	bool compute_noise_map{ false };

};