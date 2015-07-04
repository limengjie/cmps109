#ifndef __PHASE10__
#define __PHASE10__

#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <vector>

using namespace std;

class Player;
class Game;
using player_ptr = shared_ptr<Player>;
using map_ptr = shared_ptr<map<string, player_ptr> >;

class Player {
	friend class Game;
	private:
		string name;
		int score {0};
		int level {1};
	public:
		Player(const string & p_name);
		void show();
		void set_name(const string & new_name);
		string get_name() {return name;};
		void set_score(int new_score);
		int get_score() {return score;};
		void set_level(int new_level);
		int get_level() {return level;};
		int add_pts(int points);
};

class Game {
	private:
		map<string, player_ptr> players;
	public:
		Game();
		vector<string> split(const string & line, 
				const string & delimiter);
		//void create_player();
		void add_player(const string & p_name);
		void calc();
		void edit();
		player_ptr get_player(const string & p_name);
		void show_players();
		void show_rules();
		bool search_map(const string & name);
		void congrats();
		int get_size() {return players.size();};
		map<string, player_ptr> get_players() {return players;};
};

#endif
