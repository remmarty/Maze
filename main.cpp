#include <iostream>
#include <fstream>
#include <string>


struct MapElement {
	static const char VERTICAL = 186; // ║
	static const char HORIZONTAL = 205; // ═
	static const char UPPER_RIGHT_CORNER = 201; // ╔ 
	static const char UPPER_LEFT_CORNER = 187; // ╗
	static const char BOTTOM_RIGHT_CORNER = 200; // ╚
	static const char BOTTOM_LEFT_CORNER = 188; // ╝
	static const char EMPTY = ' '; 
	static const char PLAYER = 'X';
	static const char FOG = 177; // ▒
};

struct Symbols {

	static const int VERTICAL = 1;
	static const int HORIZONTAL = 2;
	static const int UPPER_RIGHT_CORNER = 3;
	static const int UPPER_LEFT_CORNER = 4;
	static const int BOTTOM_RIGHT_CORNER = 5;
	static const int BOTTOM_LEFT_CORNER = 6;
	static const int EMPTY = 0;
	static const int PLAYER = 9;

};

struct Settings {
	static const int MAP_WIDTH = 20;
	static const int MAP_HEIGHT = 13;
	static constexpr double FOG_RADIUS = 3.0;
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

	void makeAction(char key, Level level) {
		if (key == 'w')
			move(-1, 0);
		else if (key == 's')
			move(1, 0);
		else if (key == 'a')
			move(0, -1);
		else if (key == 'd')
			move(0, 1);
		else
			std::cout << "Key not handled TODO" << std::endl;
	}

	void move(int vertical_step, int horizontal_step){
		y += vertical_step;
		x += horizontal_step;
	}
};

struct GameEngine {
	Level level;
	Player player;

	char readInput() {
		char key;
		std::cin >> key;
		return key;
	}

	void triggerActions(char key) {
		if (key == 'w')
			player.move(-1, 0);
		else if (key == 's')
			player.move(1, 0);
		else if (key == 'a')
			player.move(0, -1);
		else if (key == 'd')
			player.move(0, 1);
		else if (key == 'q')
			exit(0);
		else
			std::cout << "Key not handled TODO" << std::endl;
	}

	void runGameLoop() {
		// TODO exit on Q
		while (true) {
			drawLevel();

			// Clear old posistion in map
			level.map[player.y][player.x] = Symbols::EMPTY;

			// Blocking until it gets some value from keyboard
			char input = readInput();
			triggerActions(input);

			// Put player in new position on map
			level.map[player.y][player.x] = Symbols::PLAYER;
			system("cls");
			//clearScreen();
		}
	}

	void drawLevel() {
		for (int y = 0; y < Settings::MAP_HEIGHT; y++) {
			for (int x = 0; x < Settings::MAP_WIDTH; x++) {
				// Simulate fog using Euclidean distance
				double distanceFromPlayer = sqrt(pow(player.y - y, 2) + pow(player.x - x, 2));
				if (distanceFromPlayer > Settings::FOG_RADIUS) {
					// (x, y) is away from the player
					std::cout << MapElement::FOG;
				}
				else {
					// (x, y) is within player sight
					switch (level.map[y][x])
					{
						case Symbols::EMPTY:
							std::cout << MapElement::EMPTY;
							break;
						case Symbols::PLAYER:
							std::cout << MapElement::PLAYER;
							break;

						case Symbols::VERTICAL:
							std::cout << MapElement::VERTICAL;
							break;

						case Symbols::HORIZONTAL:
							std::cout << MapElement::HORIZONTAL;
							break;

						case Symbols::UPPER_RIGHT_CORNER:
							std::cout << MapElement::UPPER_RIGHT_CORNER;
							break;

						case Symbols::UPPER_LEFT_CORNER:
							std::cout << MapElement::UPPER_LEFT_CORNER;
							break;

						case Symbols::BOTTOM_RIGHT_CORNER:
							std::cout << MapElement::BOTTOM_RIGHT_CORNER;
							break;

						case Symbols::BOTTOM_LEFT_CORNER:
							std::cout << MapElement::BOTTOM_LEFT_CORNER;
							break;

						default:
							// Symbol to MapElement not implemented
							std::cout << '!';
					}
				}
			}
			std::cout << std::endl;
		}
	}
};

int main() {
	GameEngine engine = GameEngine();
	engine.level.loadFromFile("D:\\Repos\\Maze\\lvl1.txt");
	engine.runGameLoop();
	return 0;
}