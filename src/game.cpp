#include "game.hpp"

static std::unique_ptr<Window> init_glfw()
{
	if (!glfwInit())
		return nullptr;

	glfwWindowHint(GLFW_CONTEXT_DEBUG, true);


	std::unique_ptr<Window> window = std::make_unique<Window>(v2i32{ 640, 480 }, "test");
	window->toggle_cursor();


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

DebugMessage Game::run()
{
	window = init_glfw();
	if (!window)
		return DebugMessage{ .msg{"Error::Cannot initialise window"}, .severity{DebugMessage::Critical} };

	init_imgui(*window).print_to_console();


	camera.setFramebufferSize(window->getSize());


	AssetsManager::get(); // load all assets

	// Temporary

	gfx::Mesh mesh
	{
		std::vector<gfx::Vertex>
		{
			{ {-0.5, -0.5, -1}, {0, 0} },
			{ { 0.5, -0.5, -1}, {1, 0} },
			{ { 0.5,  0.5, -1}, {1, 1} },
			{ {-0.5,  0.5, -1}, {0, 1} }
		},
		{
			0, 1, 2,
			0, 2, 3
		}
	};

	world.update_grid({0, 0, 0}, true);

	// Main Loop
	while (window->isOpen())
	{
		// Start of Frame
		delta_time.update();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		window->clear_states(); 


		inputs();

		logic();

		debug();

		render_on_screen();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return DebugMessage{ .msg{"Info::Game ran successfully"}, .severity{DebugMessage::Info} };
}



void Game::inputs()
{
	while (auto event = window->poll_event())
	{
		if (auto key = event->get_if<Event::KeyPressed>())
		{
			if (key->scancode == Keys::Escape)
				window->close();

			if (key->scancode == Keys::F1)
				window->toggle_cursor();

			if (key->scancode == Keys::F2)
				world.debug.show_chunk_borders = !world.debug.show_chunk_borders;

			if (key->scancode == Keys::F3)
				compute_noise_map = true;

			if (key->scancode == Keys::F4)
				world.debug.update_world = !world.debug.update_world;

			if (key->scancode == Keys::F)
				world.set_voxel(static_cast<types::voxel_point>(camera.getPosition()), gfx::Voxel{ .type_id{} });
		}


		if (auto f = event->get_if<Event::Resized>())
		{
			camera.setFramebufferSize(f->size);
		}

		if (auto p = event->get_if<Event::MouseMoved>())
		{
			v2f32 offset{ p->pos - last_mouse_window_pos };
			last_mouse_window_pos = p->pos;

			// Do after last_mouse_window_pos was updated to avoid jumps
			if (!window->isCursorHidden())
			{

				offset *= 0.1f;

				yaw += offset.x;
				pitch += offset.y;


				if (pitch > 89.f)
					pitch = 89.f;
				if (pitch < -89.f)
					pitch = -89.f;

				v3f32 direction{};

				float radPitch{ mpml::to_radians(pitch) };
				float cosPitch{ std::cos(radPitch) };
				float radYaw{ mpml::to_radians(yaw) };

				direction.x = std::cos(radYaw) * cosPitch;
				direction.y = -std::sin(radPitch);
				direction.z = std::sin(radYaw) * cosPitch;

				camera.setDirections(direction.normal());
			}
		}

	}


	if (window->isKeyPressed(Keys::W))
		camera.move_front(delta_time.get(), camera.speed);

	if (window->isKeyPressed(Keys::S))
		camera.move_back(delta_time.get(), camera.speed);

	if (window->isKeyPressed(Keys::D))
		camera.move_right(delta_time.get(), camera.speed);

	if (window->isKeyPressed(Keys::A))
		camera.move_left(delta_time.get(), camera.speed);

	if (window->isKeyPressed(Keys::Space))
		camera.move_up(delta_time.get(), camera.speed);

	if (window->isKeyPressed(Keys::Left_shift))
		camera.move_down(delta_time.get(), camera.speed);
}



void Game::logic()
{
	types::chunk_loc player_loc = gfx::World::to_chunkLoc(camera.getPosition());

	world.update_grid(player_loc);
}



void Game::debug()
{
	gfx::DebugRenderer::get().update(glfwGetTime());

	debug_imgui();
}

	void Game::debug_imgui()
	{
		static bool show_general{ true };
		static bool show_tg{};

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

				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
		

		if (show_general)
		{
			if (ImGui::Begin("General"/*nullptr, ImGuiWindowFlags_MenuBar*/))
			{
				const auto player_loc = gfx::World::to_chunkLoc(camera.getPosition());

				ImGui::Text("Camera Pos: %d %d %d", (i32)camera.getPosition().x, (i32)camera.getPosition().y, (i32)camera.getPosition().z);
				ImGui::Text("Camera Pos: %d %d %d", player_loc.x, player_loc.y, player_loc.z);
				ImGui::DragFloat("Speed", &camera.speed);

				ImGui::BeginGroup();
				{
					const u32 max{ 8 };
					const u32 min{ 1 };

					ImGui::SliderScalar("Render Distance	: ", ImGuiDataType_U32, &world.get_chunkGrid().parameters.r_dist, &min, &max);
					ImGui::SliderScalar("Render Height	: ", ImGuiDataType_U32, &world.get_chunkGrid().parameters.r_height, &min, &max);

				}
				ImGui::EndGroup();

			}
			ImGui::End();
		}

		if(show_tg)
		{
			if (ImGui::Begin("Terrain Generation"))
			{
				static gfx::Image noise_image{ v2i32{}, GL_RED };
				static gfx::Texture noise_texture{ noise_image };

				static i32 renderdistance{};
				const i32 l1{ 0 }, l2{ 100 };
				ImGui::SliderScalar("Render Distance", ImGuiDataType_S32, &renderdistance, &l1, &l2);

				auto pos = static_cast<v3i32>(camera.getPosition());
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
				ImGui::Image(noise_texture.ID(), ImVec2(noise_texture.getSize().x * scale, noise_texture.getSize().y * scale));
			}
			ImGui::End();
		}
	}



void Game::render_on_screen()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	AssetsManager::get().shader.bind();

	AssetsManager::get().shader.setValue("vp", camera.getViewProj());
	AssetsManager::get().shader.setValue("model", m4f32::Identity);

	AssetsManager::get().tex.bind();

	world.draw();

	AssetsManager::get().tex.unbind();
	AssetsManager::get().shader.unbind();




	/*= Debug Draws =*/

	gfx::DebugRenderer::get().render(camera.getViewProj());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	window->display(); // Swap Window Buffer With the Graphics Card's One
}
