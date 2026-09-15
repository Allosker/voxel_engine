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

	Time now() const
	{
		return std::chrono::duration<Time, std::micro>(std::chrono::high_resolution_clock::now() - startTime).count() / 1000.0;
	}

	// Records a checkpoint and measures the time since the last one
	void add(const std::string& sectionName)
	{
		const auto prevTime = lastTime;
		lastTime = now();

		sections.push_back({sectionName, lastTime - prevTime});
	}

	void printAll() const 
	{
		const auto total = now();
		for(const auto& section : sections)
		{
			std::print("[{}] {}ms\n", section.name, section.duration);
		}

		const Time endTime = now();
		std::print("[Total Time] {}ms\n", total);
	}

private:

	struct Section
	{
		std::string name;
		Time duration{};
	};

	std::vector<Section> sections;

	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	Time lastTime{};
};