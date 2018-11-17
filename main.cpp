#include <iostream>
#include <fstream>
#include <string>

struct MapElement {
	static const char vertical = 186; // '║'
	static const char horizontal = '═';
	static const char empty = '.';
	static const char player = 'X';
};

struct Symbols {
	// TODO tmp
	static const int tmp_border = 1;
	static const int player = 9;
	static const int empty = 0;

};

struct Settings {
	static const int MAP_WIDTH = 20;
	static const int MAP_HEIGHT = 10;
};

struct Level {
	int map[Settings::MAP_HEIGHT][Settings::MAP_WIDTH];

	void loadFromFile(std::string filename) {
		std::ifstream file;
		file.open(filename);
		if (!file.is_open())
			std::cout << "Could not read file: " << filename << std::endl;
		else {
			for (int y = 0; y < Settings::MAP_HEIGHT; y++) {
				for (int x = 0; x < Settings::MAP_WIDTH; x++) {
					file >> map[y][x];
				}
			}
		}
		file.close();
	}
};

struct Player {
	int x = 5;
	int y = 5;
};

struct GameEngine {
	Level level;

	void drawLevel() {
		for (int y = 0; y < Settings::MAP_HEIGHT; y++) {
			for (int x = 0; x < Settings::MAP_WIDTH; x++) {
				switch (level.map[y][x])
				{
				case Symbols::empty:
					std::cout << MapElement::empty;
					break;
				case Symbols::player:
					std::cout << MapElement::player;
					break;
					//TODO
				case Symbols::tmp_border:
					std::cout << MapElement::vertical;
					break;
				default:
					// Symbol to MapElement not implemented
					std::cout << '!';
				}
			}
			std::cout << std::endl;
		}
	}
};

int main() {
	GameEngine engine = GameEngine();
	engine.level.loadFromFile("D:\\Repos\\Maze\\lvl1.txt");
	engine.drawLevel();
	return 0;
}