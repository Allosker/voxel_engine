#include <print>

#include "game.hpp"


int main()
try
{
	DebugMessage end_msg = Game::get().run();
	
	end_msg.print_to_console();

	if (end_msg.severity != DebugMessage::Info)
		return -1;

	return 0;
}
catch (std::runtime_error& e)
{
	std::println("{}", e.what());
}
