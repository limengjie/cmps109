#ifndef __COMMAND__
#define __COMMAND__

#include <map>
using namespace std;

#include "phase10.h"

using command_fn = void(*)(Game game);
using command_map = map<string, command_fn>;

class Commands {
	private:
		command_map map;
	public:
		Commands();
		command_fn at (const string & cmd);
};

void fn_add  (Game game);
void fn_calc (Game game);
void fn_edit (Game game);
void fn_exit (Game game);
void fn_help (Game game);
void fn_rm   (Game game);
void fn_rules(Game game);
void fn_start(Game game);
void fn_show (Game game);

#endif
