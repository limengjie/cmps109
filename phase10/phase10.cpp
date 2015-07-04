#include <iostream>
#include "phase10.h"


Player::Player(const string & p_name) {
	name = p_name;
}

void Player::show() {
	cout << name << "\t  ";
	cout << level << "\t  ";
	cout << score << "\n";
}

void Player::set_name(const string & new_name) {
	name = new_name;
}

void Player::set_score(int new_score) {
	score = new_score;
}

void Player::set_level(int new_level) {
	level = new_level;
}

int Player::add_pts(int points) {
	score += points;

	if (level < 8 and score >= 500) {
		if (level > 1)
			level--;
		score = 0;
	}
	else if (level >= 8 and score >= 200) {
		level--;
		score = 0;
	}

	if (points < 50) {
		level++;
		if (level == 8) score = 0;
	}

	if (level > 10) {
		return 1;
	else 
		return 0;
}

Game::Game() {
	cout << "            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   \n";
	cout << "            ~  Welcome to phase 10's world! ~   \n";
	cout << "            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   \n";

	cout << "input players:\n";
	string line;
	getline(cin, line);
	vector<string> names = split(line, " ");
	for (size_t i = 0; i < names.size(); ++i) {
		if (search_map(names.at(i))) {
			//check if the player exists or not
			cout << "duplicated names\n";
			continue;
		}
		player_ptr p = (player_ptr) new Player(names.at(i));
		players[names.at(i)] = p;
	}
}


void Game::add_player(const string & name) {
	if (search_map(name)) {
		//check if the player exists or not
		cout << "name exists\n";
		return;
	}

	//get map 
	map<string, player_ptr> p_map = get_players();
	//map_ptr p_map = get_players();
	//create a new player
	player_ptr p = (player_ptr) new Player(name);
	//modify map
	p_map[name] = p;
	//players->insert(pair<string, player_ptr>(name, p));
	// update map
	players = p_map;

	////print res
	//map<string, player_ptr>::iterator iter = players.begin();
	//for (; iter != players.end(); ++iter) {
		//player_ptr p = iter->second;
		//p->show();
	//}
}	


//void Game::create_player() {
	//cout << "input players:\n";
	//map<string, player_ptr> p_map = get_players();
	//string name;
	//cin >> name;
	//while (name != "end") {
		//player_ptr p = (player_ptr) new Player(name);
		//p_map[name] = p;
		//cin >> name;
	//}
	//players = p_map;

//}

void Game::calc() {
	int res = 0, isOver;
	map<string, player_ptr>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter) {
		player_ptr p_ptr = iter->second;
		cout << iter->first << ": ";
		int i_pts;
		string pts;
		cin >> pts;
		i_pts = stoi(pts);
		isOver = p_ptr->add_pts(i_pts);
		res += isOver;
	}

	
	if (res != 0) {
		congrats();
		exit(0);
	}

	show_players();
}

void Game::congrats() {

	player_ptr winner, plr;
	map<string, player_ptr>::iterator iter = players.begin();
	winner = iter->second;
	iter++;
	for (; iter != players.end(); ++iter) {
		plr = iter->second;
		if (plr->get_level() == winner->get_level() and
				winner->get_level() == 11) {
			// multiple winners
			if (plr->get_score() < winner->get_score())
				winner = plr;
		}
		else if (plr->get_level() > winner->get_level() and
				plr->get_level() == 11)
			winner = plr;
	}

	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "Congratulations! The winner is " << winner->get_name();
	cout << ".\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void Game::edit() {
	cout << "which player you want to change?\n";

	string line;
	getline(cin, line);
	vector<string> words = split(line, " ");
	player_ptr p_ptr = get_player(words.at(0));
	string n, s, l;
	int i_s, i_l;
	if (words.size() == 1) {
		cout << "new name:\n";
		cin >> n;
		p_ptr->set_name(n);
		cout << "new score:\n";
		cin >> s;
		i_s = stoi(s);
		p_ptr->set_score(i_s);
		cout << "new level:\n";
		cin >> l;
		i_l = stoi(l);
		p_ptr->set_level(i_l);
	}
	else if (words.size() == 2) {
		if (words.at(1) == "name") {
			cout << "new name:\n";
			cin >> n;
			p_ptr->set_name(n);
		}
		else if (words.at(1) == "score") {
			cout << "new score:\n";
			cin >> s;
			i_s = stoi(s);
			p_ptr->set_score(i_s);
		}
		else if (words.at(1) == "level") {
			cout << "new level:\n";
			cin >> l;
			i_l = stoi(l);
			p_ptr->set_level(i_l);
		}
	}
}

player_ptr Game::get_player(const string & p_name) {
	map<string, player_ptr>::iterator iter = players.find(p_name);
	if (iter == players.end()) 
		return nullptr;
	else
		return iter->second;
}

bool Game::search_map(const string & name) {
	map<string, player_ptr>::iterator iter = players.find(name);
	if (iter == players.end()) 
		return false;
	else
		return true;
}

void Game::show_players() {
	if (get_size() == 0) {
		cout << "empty map\n";
		return;
	}

	cout << "=========================\n";
	cout << "Name\tLevel\tScore\n";
	map<string, player_ptr>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter) {
		player_ptr p = iter->second;
		p->show();
	}
	cout << "=========================\n";
}

void Game::show_rules() {
	cout << " ================================\n";
	cout << "   1.  2 sets of 3\n";
	cout << "   2.  1 set of 3 + 1 run of 4\n";
	cout << "   3.  1 set of 4 + 1 run of 4\n";
	cout << "   4.  1 run of 7\n";
	cout << "   5.  1 run of 8\n";
	cout << "   6.  1 run of 9\n";
	cout << "   7.  2 sets of 4\n";
	cout << "   8.  7 cards of 1 color\n";
	cout << "   9.  1 set of 5 + 1 set of 2\n";
	cout << "   10. 1 set of 5 + 1 set of 3\n";
	cout << " ================================\n";
}
	
vector<string> Game::split(const string & line, 
		const string & delimiter) {
	vector<string> words;
	size_t end = 0;
	for (;;) {
		size_t start = line.find_first_not_of (delimiter, end);
		if (start == string::npos) break;
		end = line.find_first_of (delimiter, start);
		words.push_back(line.substr (start, end - start));
	}

	return words;
}
