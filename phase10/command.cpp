#include "command.h"

Commands::Commands() :map ({
		{"add"     , fn_add},
		{"calc"    , fn_calc},
		{"edit"    , fn_edit},
		{"exit"    , fn_exit},
		{"help"    , fn_help},
		{"rm"      , fn_rm},
		{"rules"    , fn_rules},
		{"start"   , fn_start},
		{"show"    , fn_show},
		}) {}

command_fn Commands::at(const string & cmd) {
	command_map::iterator iter = map.find(cmd);
	if (iter == map.end()) {
		//cout << "no such command.\n";
		return nullptr;
	}
	else
		return iter->second;
}

void fn_add(Game game) {
	cout << "input new player: \n";
	string name;
	getline(cin, name);

	game.add_player(name);
}

void fn_calc(Game game) {
	game.calc();
}

void fn_edit(Game game) {
	game.edit();
}

void fn_exit(Game game) {
	exit(0);
}

void fn_help(Game game) {
	cout << "   add:    Add a new player.\n";
	cout << "   calc:   Calculate the latest score and level.\n";
	cout << "   edit:   Change a player.\n";
	cout << "   exit:   End current game.\n";
	cout << "   rules:  Show the rules.\n";
}

void fn_rm(Game game) {
	cout << "call rm\n";
}

void fn_rules(Game game) {
	game.show_rules();
}

void fn_start(Game game) {
	//game.create_player();
}

void fn_show(Game game) {
	game.show_players();
}
