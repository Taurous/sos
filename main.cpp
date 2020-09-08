#include "game.hpp"

int main(int argc, char ** argv)
{
	
#ifndef _DEBUG
	std::ofstream log("errorlog.txt");
	if (log.is_open())
	{
		std::cerr.rdbuf(log.rdbuf());
	}
#endif

	game g;

	int msg = g.init();
	if (msg) return msg;

	g.run();

	return msg;
}