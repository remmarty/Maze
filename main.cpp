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
	static const char EXIT_AREA = 'W';
};

struct Symbols {

	static const int EMPTY = 0;
	static const int VERTICAL = 1;
	static const int HORIZONTAL = 2;
	static const int UPPER_RIGHT_CORNER = 3;
	static const int UPPER_LEFT_CORNER = 4;
	static const int BOTTOM_RIGHT_CORNER = 5;
	static const int BOTTOM_LEFT_CORNER = 6;
	static const int EXIT_AREA = 7;
	static const int PLAYER = 9;

};

struct Settings {
	static const int MAP_WIDTH = 20;
	static const int MAP_HEIGHT = 13;
	static constexpr double FOG_RADIUS = 5.0;
	static const char KEY_EXIT = 'q';
	static const char KEY_PLAYER_LEFT = 'a';
	static const char KEY_PLAYER_RIGHT = 'd';
	static const char KEY_PLAYER_UP = 'w';
	static const char KEY_PLAYER_DOWN = 's';
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
	bool exitAreaEntered = false;

	void move(int vertical_step, int horizontal_step, int map[Settings::MAP_HEIGHT][Settings::MAP_WIDTH]){
		// Pretend this will be our final position
		int tmp_y = y + vertical_step;
		int tmp_x = x + horizontal_step;

		if (map[tmp_y][tmp_x] == Symbols::EXIT_AREA) {
			exitAreaEntered = true;
		} else if (map[tmp_y][tmp_x] == Symbols::EMPTY) {
			y += vertical_step;
			x += horizontal_step;
		}
	}

};

struct GameEngine {
	Level level;
	Player player;

	char readInput() {
		/*  Block until user will press a key */
		char key;
		std::cin >> key;
		return key;
	}

	void triggerActions(char key) {
		/* Call behaviour depending on keyboard input */
		if (key == Settings::KEY_PLAYER_UP)
			player.move(-1, 0, level.map);
		else if (key == Settings::KEY_PLAYER_DOWN)
			player.move(1, 0, level.map);
		else if (key == Settings::KEY_PLAYER_LEFT)
			player.move(0, -1, level.map);
		else if (key == Settings::KEY_PLAYER_RIGHT)
			player.move(0, 1, level.map);
		else
			// TODO
			std::cout << "Key not handled TODO" << std::endl;
	}

	void runGameLoop() {
		char input = NULL;

		// Initialize player position on the map
		level.map[player.y][player.x] = Symbols::PLAYER;

		while (true) {
			drawLevel();

			// Check if game engine should terminate loop
			if (player.exitAreaEntered) {
				std::cout << "You win!" << std::endl;
				break;
			}
			if (input == Settings::KEY_EXIT) {
				std::cout << "Exiting..." << std::endl;
				break;
			}

			// Clear old posistion in map
			level.map[player.y][player.x] = Symbols::EMPTY;

			// Blocking until it gets some value from keyboard
			input = readInput();
			triggerActions(input);

			// Put player in new position on map
			level.map[player.y][player.x] = Symbols::PLAYER;

			// Clear console
			system("cls");
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

						case Symbols::EXIT_AREA:
							std::cout << MapElement::EXIT_AREA;
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