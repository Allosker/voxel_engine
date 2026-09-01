#include "game.hpp"

#include <thread>
#include <chrono>

#include "sys/inputManager.hpp"

#include "gfx/rayTraversal.hpp"
#include "gfx/meshInstance.hpp"


static std::unique_ptr<Window> init_glfw(bool AA, u32 MSAA)
{
	if (!glfwInit())
		return nullptr;

	if (AA)
		glfwWindowHint(GLFW_SAMPLES, MSAA);

	glfwWindowHint(GLFW_CONTEXT_DEBUG, true);

	/* Three Base Resolutions possible: 
	* 640, 360
	* 1920, 1080
	* 2560, 1440
	*/
	std::unique_ptr<Window> window = std::make_unique<Window>(v2i32{ 640, 360 }, "test");
	window->toggle_cursor();

	if (AA)
		glEnable(GL_MULTISAMPLE);

	// Must come once the Context Flags have been intialised
	GLint flags{};
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	return window;
}

static DebugMessage init_imgui(Window& window)
{
	IMGUI_CHECKVERSION(); // Setup Dear ImGui context
	if (!ImGui::CreateContext())
		return DebugMessage{ .msg{"Error::ImGui_Init::Cannot create ImGui context"}, .severity{DebugMessage::Critical} };
	//ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		  // Enable Docking

	// Setup Platform/Renderer backends
	if (!ImGui_ImplGlfw_InitForOpenGL(window.get(), true)) // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		return DebugMessage{ .msg{"Error::ImGui_Init::Cannot initialise ImGui GLFW for OpenGL context"}, .severity{DebugMessage::Critical} };
	if (!ImGui_ImplOpenGL3_Init())
		return DebugMessage{ .msg{"Error::ImGui_Init::Cannot initialise ImGui OpenGL context"}, .severity{DebugMessage::Critical} };

	return DebugMessage{ .msg{"Info::ImGui_Init::Successful"}, .severity{DebugMessage::Info} };
}

Game::Game() noexcept
	: window{ init_glfw(true, render_settings.MSAA) }
{
}

DebugMessage Game::run()
{
	if (!window)
		return DebugMessage{ .msg{"Error::Cannot initialise window"}, .severity{DebugMessage::Critical} };


	glEnable(GL_MULTISAMPLE);
	init_imgui(*window).print_to_console();


	camera.set_FBS(window->getSize());


	AssetsManager::get(); // load all assets

	AssetsManager::get().shaders.at("shaders/twoD").bind();
	AssetsManager::get().shaders.at("shaders/twoD").set_value("ortho", orthographic_proj);
	AssetsManager::get().shaders.at("shaders/twoD").unbind();

	AssetsManager::get().shaders.at("shaders/twoD_to_3D").bind();
	AssetsManager::get().shaders.at("shaders/twoD_to_3D").set_value("ortho", orthographic_proj);
	AssetsManager::get().shaders.at("shaders/twoD_to_3D").unbind();


	world.update_grid({ 0, 0, 0 }, true);

	while (world.get_voxel(gfx::World::to_voxelPos(player.get_pos())))
	{
		player.set_pos(player.get_pos() + types::pos{ 0.0, 1.0, 0.0 });
	}

	player.set_pos(player.get_pos() + types::pos{ 0.0, 2.0, 0.0 });

	auto& am = AssetsManager::get();

	auto& model = am.models.begin()->second;
	world.m_meshInstances.push_back(gfx::MeshInstance{model.mesh, &am.shaders.at("shaders/static_mesh"), model.textures[0]});
	world.m_meshInstances.back().set_pos({1.0, 8.0, 2.0});


	// Main Loop
	while (window->isOpen())
	{
		const f32 time_at_frame_start = glfwGetTime();

		delta_time.update(time_at_frame_start);
		fps = 1.f / delta_time.get();
		delta_time.limit();


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		gfx::DebugRenderer::get().update(glfwGetTime());

		window->clear_states();


		inputs();

		if (!runtime_settings.paused)
			logic();

		debug();


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);


			AssetsManager::get().shaders.at("shaders/world_chunks").bind();

			AssetsManager::get().shaders.at("shaders/world_chunks").set_value("vp", camera.get_VP());
			AssetsManager::get().shaders.at("shaders/world_chunks").set_value("model", m4f32{ 1. });

			world.draw(camera);

			AssetsManager::get().shaders.at("shaders/world_chunks").unbind();


			/*= Debug Draw =*/ gfx::DebugRenderer::get().render3D(camera.get_VP());

			glDisable(GL_DEPTH_TEST);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			AssetsManager::get().shaders.at("shaders/twoD").bind();

			gui_inv.draw(
				{ .sha{ &AssetsManager::get().shaders.at("shaders/twoD") } }, 
				{ 
					.sha{ &AssetsManager::get().shaders.at("shaders/twoD_to_3D") },
					.tex{ &AssetsManager::get().textures.at("textures/voxels/atlas") }
				}
			);

			AssetsManager::get().shaders.at("shaders/twoD").unbind();


			glDisable(GL_BLEND);


			/*= Debug Draws =*/

			gfx::DebugRenderer::get().render2D(orthographic_proj);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			window->display(); // Swap Window Buffer With the Graphics Card's One


		auto frameExecutionTime = glfwGetTime() - time_at_frame_start;
		if (frameExecutionTime < 1.f / target_fps)
		{
			std::this_thread::sleep_for(std::chrono::duration<f32>(1.f / target_fps - frameExecutionTime));
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return DebugMessage{ .msg{"Info::Game ran successfully"}, .severity{DebugMessage::Info} };
}



void Game::inputs()
{
	static gfx::RayCastResult ray{};

	while (auto event = window->poll_event())
	{
		if (auto focus_changed = event->get_if<Event::FocusChanged>())
			if (focus_changed->focus)
				runtime_settings.paused = false;
			else
				runtime_settings.paused = true;

		if (auto f = event->get_if<Event::Resized>())
		{
			camera.set_FBS(f->size);
		}

		if (auto key = event->get_if<Event::KeyEvent>())
		{

			if (!runtime_settings.paused)
				sys::InputManager::get().add_key_event(*key);


			if (sys::InputManager::pressed(*key, Keys::Escape))
				window->close();

			if (sys::InputManager::pressed(*key, Keys::F1))
				window->toggle_cursor();

			if (sys::InputManager::pressed(*key, Keys::F2))
				world.debug.show_chunk_borders = !world.debug.show_chunk_borders;

			if (sys::InputManager::pressed(*key, Keys::F3))
				compute_noise_map = true;

			if (sys::InputManager::pressed(*key, Keys::F4))
				world.debug.update_world = !world.debug.update_world;

			if (sys::InputManager::pressed(*key, Keys::F5))
				showDebugMenus = !showDebugMenus;

			if (sys::InputManager::pressed(*key, Keys::F))
				player.m_mov.flying = !player.m_mov.flying;

			if (sys::InputManager::pressed(*key, Keys::G))
				player.m_mov.ghost = !player.m_mov.ghost;
				
		}

		if (runtime_settings.paused)
			return;

		if (auto mouse = event->get_if<Event::MouseButtonEvent>())
		{
			sys::InputManager::get().add_mouseButton_event(*mouse);


			if (sys::InputManager::pressed(*mouse, MouseButtons::Left))
			{
				if (auto r = world.raycast(camera.get_pos(), camera.get_front(), 200))
				{
					ray = *r;
					auto pos = r->voxel_pos;

					world.set_voxel(pos, gfx::Voxel{ .type_id{} });
				}
			}
		}

		if (auto p = event->get_if<Event::MouseMoved>())
		{
			v2f32 offset{ p->pos - last_mouse_window_pos };
			last_mouse_window_pos = p->pos;

			// Do after last_mouse_window_pos was updated to avoid jumps
			if (!window->isCursorHidden())
			{

				offset *= 0.1;

				yaw += offset.x;
				pitch += offset.y;


				if (pitch > 89.)
					pitch = 89.;
				if (pitch < -89.)
					pitch = -89.;

				types::pos direction{};

				f64 radPitch{ glm::radians(pitch) };
				f64 cosPitch{ std::cos(radPitch) };
				f64 radYaw{ glm::radians(yaw) };

				direction.x = std::cos(radYaw) * cosPitch;
				direction.y = -std::sin(radPitch);
				direction.z = std::sin(radYaw) * cosPitch;

				camera.set_dirs(glm::normalize(direction));
			}
		}

	}


	gfx::line((v3f32)gfx::World::to_voxelPos(ray.origin), (v3f32)gfx::World::to_voxelPos(ray.hit_pos), { 0, 0, 0, 1 }, 0, false);
	gfx::line((v3f32)ray.origin, (v3f32)ray.hit_pos, { 1, 1, 1, 1 }, 0, false);


	if (window->isKeyPressed(Keys::W))
		player.move(Keys::W, delta_time.get());

	if (window->isKeyPressed(Keys::S))
		player.move(Keys::S, delta_time.get());

	if (window->isKeyPressed(Keys::D))
		player.move(Keys::D, delta_time.get());;

	if (window->isKeyPressed(Keys::A))
		player.move(Keys::A, delta_time.get());


	if (window->isKeyPressed(Keys::Space))
		player.move(Keys::Space, delta_time.get());

	if (window->isKeyPressed(Keys::Left_shift))
		player.move(Keys::Left_shift, delta_time.get());


	sys::InputManager::get().update();
}



void Game::logic()
{
	types::chunk_loc player_loc = gfx::World::to_chunkLoc(camera.get_pos());

	world.update_grid(player_loc);

	player.update(world, delta_time.get());


	gui_inv.update(inv, Window::to_gui_coordinates(*window, window->get_cursor_pos()));

}



void Game::debug()
{
	if (showDebugMenus)
	{
		debug_imgui();
	}
}

void Game::debug_imgui()
{
	static bool show_general{ true };
	static bool show_tg{ true };
	static bool show_player{ true };

	// Clear Inputs when window->cursor can't be seen, avoids weird behaviours
	if (window->isCursorHidden())
	{
		auto& io = ImGui::GetIO();

		io.ClearInputKeys();
		io.ClearInputKeys();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("General", nullptr, &show_general);
			ImGui::MenuItem("Terrain Generation", nullptr, &show_tg);
			ImGui::MenuItem("Player Settings", nullptr, &show_player);

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (show_general)
	{
		if (ImGui::Begin("General", &show_general))
		{
			const auto player_loc = gfx::World::to_chunkLoc(player.get_pos());

			ImGui::Text("FPS: %f", fps);

			const auto camVoxelPos = gfx::World::to_voxelPos(player.get_pos());
			ImGui::Text("Pos Absolute: %d %d %d", camVoxelPos.x, camVoxelPos.y, camVoxelPos.z);

			if (auto* c = world.get_chunkGrid().at_chunk(player_loc))
			{
				const auto camInChunk = gfx::Chunk::to_voxelLoc(*c, camVoxelPos);
				ImGui::Text("Pos Location: %d %d %d", player_loc.x, player_loc.y, player_loc.z);
				ImGui::Text("Pos In Chunk: %d %d %d", camInChunk.x, camInChunk.y, camInChunk.z);
			}
			else
				ImGui::Text("No chunk at location");

			ImGui::Text("Pos Discrete: %f %f %f", player.get_pos().x, player.get_pos().y, player.get_pos().z);


			ImGui::BeginGroup();
			{
				const u32 max{ 8 };
				const u32 min{ 1 };

				ImGui::SliderScalar("Render Distance", ImGuiDataType_U32, &world.get_chunkGrid().parameters.r_dist, &min, &max);
				ImGui::SliderScalar("Render Height", ImGuiDataType_U32, &world.get_chunkGrid().parameters.r_height, &min, &max);

			}
			ImGui::EndGroup();


		}
		ImGui::End();
	}

	if (show_tg)
	{
		if (ImGui::Begin("Terrain Generation", &show_tg))
		{
			static gfx::Image noise_image{ v2i32{}, GL_RED };
			static gfx::Texture noise_texture{ noise_image };

			static i32 renderdistance{};
			const i32 l1{ 0 }, l2{ 100 };
			ImGui::SliderScalar("Render Distance", ImGuiDataType_S32, &renderdistance, &l1, &l2);

			auto pos = static_cast<v3i32>(camera.get_pos());
			v3i32 min{ pos - renderdistance * gfx::Chunk::g_size<i32> };
			v3i32 max{ pos + renderdistance * gfx::Chunk::g_size<i32> };

			if (compute_noise_map)
			{
				compute_noise_map = false;

				v3i32 gap{ max - min };

				noise_image = gfx::Image{ (v2u32)v2i32 { gap.x, gap.z }, GL_RGB };


				for (i32 x{ min.x }; x < max.x; x++)
					for (i32 z{ min.z }; z < max.z; z++)
					{
						auto t = gfx::continentalness(world.terrain_context, z, x);

						i32 grayscale = t * 255;

						noise_image.getData().at(((x - min.x) + (z - min.z) * gap.x) * noise_image.getChannel()) = grayscale;
						noise_image.getData().at(((x - min.x) + (z - min.z) * gap.x) * noise_image.getChannel() + 1) = grayscale;
						noise_image.getData().at(((x - min.x) + (z - min.z) * gap.x) * noise_image.getChannel() + 2) = grayscale;
					}

				noise_texture.update(noise_image);
			}

			static float scale{ 10.f };
			ImGui::SliderFloat("Noise scale: ", &scale, 0.0001f, 10.0f);
			ImGui::Image(noise_texture.id(), ImVec2(noise_texture.get_size().x * scale, noise_texture.get_size().y * scale));
		}
		ImGui::End();
	}

	if (show_player)
	{
		if (ImGui::Begin("Player & World", &show_player))
		{
			ImGui::DragScalar("Max Speed", ImGuiDataType_Double, &player.m_mov.max_speed);
			ImGui::DragScalar("Acceleration", ImGuiDataType_Double, &player.m_mov.acceleration);
			ImGui::Text("Player Velocity %f %f %f", player.m_mov.velocity.x, player.m_mov.velocity.y, player.m_mov.velocity.z);

			ImGui::Checkbox("Flying", &player.m_mov.flying);
			ImGui::Checkbox("Ghost", &player.m_mov.ghost);
			ImGui::Checkbox("Show Hitbox", &player.debug.show_hitbox);

			ImGui::Text("Other Settings");

			ImGui::DragScalar("Jump Velocity", ImGuiDataType_Double, &player.m_mov.jump_velocity);
			ImGui::DragScalar("Gravity", ImGuiDataType_Double, &world.gravity);
			ImGui::DragScalar("Friction", ImGuiDataType_Double, &player.m_mov.friction);


		}
		ImGui::End();
	}


}



void Game::render_on_screen()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	AssetsManager::get().shaders.at("shaders/world_chunks").bind();

		AssetsManager::get().shaders.at("shaders/world_chunks").set_value("vp", camera.get_VP());
		AssetsManager::get().shaders.at("shaders/world_chunks").set_value("model", m4f32{ 1 });

		AssetsManager::get().textures.at("textures/voxels/stone").bind();

			world.draw(camera);

		AssetsManager::get().textures.at("textures/voxels/stone").unbind();

	AssetsManager::get().shaders.at("shaders/world_chunks").unbind();





	/*= Debug Draw =*/ gfx::DebugRenderer::get().render3D(camera.get_VP());

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	AssetsManager::get().shaders.at("shaders/twoD").bind();

		//gui_inv.draw(AssetsManager::get().shaders.at("shaders/twoD"));

	AssetsManager::get().shaders.at("shaders/twoD").unbind();

	glDisable(GL_BLEND);


	/*= Debug Draws =*/

	gfx::DebugRenderer::get().render2D(orthographic_proj);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	window->display(); // Swap Window Buffer With the Graphics Card's One
}
