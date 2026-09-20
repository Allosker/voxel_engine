#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <string>

class DebugTimer
{
public:

	using Time = double;

	struct Section
	{
		std::string name;
		Time duration{};
	};

	struct Frame
	{
		std::vector<Section> sections;
	};

	Time now() const
	{
		return std::chrono::duration<Time, std::micro>(std::chrono::high_resolution_clock::now() - startTime).count() / 1000.0;
	}

	void start()
	{
		if (started)
		{
			end();
		}

		startTime = std::chrono::high_resolution_clock::now();
		lastTime = 0;
		currentFrame = {};
		started = true;
	}

	void end()
	{
		add("Total Time");
		startTime = {};
		started = false;

		lastFrame = std::move(currentFrame);
		currentFrame = {};
	}

	// Records a checkpoint and measures the time since the last one
	void add(const std::string& sectionName)
	{
		const auto prevTime = lastTime;
		lastTime = now();

		currentFrame.sections.push_back({sectionName, lastTime - prevTime});
	}

	void printAll() 
	{
		for(const auto& section : lastFrame.sections)
		{
			std::print("[{}] {}ms\n", section.name, section.duration);
		}
	}

	void showInImgui()
	{
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("PerformanceTable", 2, flags, ImVec2(0.0f, 200.0f)))
		{
			// Setup columns
			ImGui::TableSetupColumn("Function / System", ImGuiTableColumnFlags_WidthStretch);
			//ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Time (ms)", ImGuiTableColumnFlags_WidthFixed);
			//ImGui::TableSetupColumn("Average (ms)", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();

			for (const auto& section : lastFrame.sections)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", section.name.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%f", section.duration);
			}

			ImGui::EndTable();
		}
	}

private:


	bool started{};
	Time lastTime{};

	Frame currentFrame;
	Frame lastFrame;

	std::chrono::high_resolution_clock::time_point startTime;
};