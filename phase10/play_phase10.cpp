#include <iostream>
#include "phase10.h"
#include "command.h"

int main () {


	//player_ptr p1 = (player_ptr) new Player("lmj");
	////p1->set_name("limengjie");
	//p1->show();

	//Game game1;
	//game1.create_player();
	//game1.show_rules();
	//game1.show_players();
	
	Commands cmdmap;
	Game game;

	for (;;) {
		cout << "% ";
		string line;
		getline (cin, line);

		command_fn fn = cmdmap.at(line);
		if (fn)
			fn (game);
	}

	game.show_players();
}
