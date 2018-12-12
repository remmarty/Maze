// POP_2018_12_13_projekt_1_Martyniak_Remigiusz_EIT_7_176036 Visual Studio Community 2017 MSVC (VC++, Toolset 141)

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct MapElement {
	static const char VERTICAL = 186; // ║
	static const char HORIZONTAL = 205; // ═
	static const char UPPER_RIGHT_CORNER = 201; // ╔ 
	static const char UPPER_LEFT_CORNER = 187; // ╗
	static const char BOTTOM_RIGHT_CORNER = 200; // ╚
	static const char BOTTOM_LEFT_CORNER = 188; // ╝
	static const char EMPTY = ' ';
	static const char PLAYER = 'X';
	static const char FOG = 177; // 177 ▒
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
	int FOG_RADIUS = 4.0;
	int SQUARE_FOG_RANGE = 4;
	static const char KEY_EXIT = 'q';
	static const char KEY_PLAYER_LEFT = 'a';
	static const char KEY_PLAYER_RIGHT = 'd';
	static const char KEY_PLAYER_UP = 'w';
	static const char KEY_PLAYER_DOWN = 's';
	static const char KEY_HELP = '?';
} settings;

struct Level {
	int** map; // array of indicators
	int mapHeight;
	int mapWidth;

	bool loadFromFile(string filename) {
		ifstream file;

		file.open(filename);
		if (!file.is_open()) {
			cout << "Could not read file: " << filename << endl;
			file.close();
			return false;
		}

		// File successfully opened
		// Read map dimensions
		file >> mapWidth;
		file >> mapHeight;

		// Allocate required amount of space

		// Each element is a pointer to some array (int*)
		map = new int*[mapHeight];
		for (int i = 0; i < mapHeight; i++) {
			// Find empty memory (where element is int) and assign that address to that pointer
			map[i] = new int[mapWidth];
		}

		// Load map data
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				file >> map[y][x];
			}
		}
		file.close();
		return true;
	}
};

struct Player {
	int x = 2;
	int y = 10;
	bool exitAreaEntered = false;

	void move(int vertical_step, int horizontal_step, int** map) {
		// Pretend this will be our final position
		int tmp_y = y + vertical_step;
		int tmp_x = x + horizontal_step;

		if (map[tmp_y][tmp_x] == Symbols::EXIT_AREA) {
			exitAreaEntered = true;
		}
		else if (map[tmp_y][tmp_x] == Symbols::EMPTY) {
			y += vertical_step;
			x += horizontal_step;
		}
	}

};

struct GameEngine {
	Level level;
	Player player;

	char readInput() {
		//  Block until user will press a key 
		char key;
		cin >> key;
		return key;
	}

	bool triggerActions(char key) {
		// Call behaviour depending on keyboard input 
		if (key == Settings::KEY_PLAYER_UP)
			player.move(-1, 0, level.map);
		else if (key == Settings::KEY_PLAYER_DOWN)
			player.move(1, 0, level.map);
		else if (key == Settings::KEY_PLAYER_LEFT)
			player.move(0, -1, level.map);
		else if (key == Settings::KEY_PLAYER_RIGHT)
			player.move(0, 1, level.map);
		else if (key != Settings::KEY_PLAYER_UP && key != Settings::KEY_PLAYER_UP && key != Settings::KEY_PLAYER_DOWN && key != Settings::KEY_PLAYER_LEFT && key != Settings::KEY_PLAYER_RIGHT && key != Settings::KEY_EXIT && key != Settings::KEY_HELP) {
			cout << "Unsupported key, try again:" << endl;
			return false;
		}
		return true;

	}

	void runGameLoop() {
		char input = NULL;

		// Initialize player position on the map
		level.map[player.y][player.x] = Symbols::PLAYER;

		while (true) {
			drawLevel();
			if (input == Settings::KEY_EXIT) {
				cout << "Exiting..." << endl;
				break;
			}
			else if (input == Settings::KEY_HELP) {

				cout << "Your task is to escape from the maze, by walking into 'W' letter " << endl;
				cout << "Press 'w' to move up" << endl;
				cout << "Press 's' to move down" << endl;
				cout << "Press 'a' to move left" << endl;
				cout << "Press 'd' to move right" << endl;
				cout << "Your next move: ";

			}

			// Check if game engine should terminate loop
			if (player.exitAreaEntered) {
				cout << "You win!" << endl;
				break;
			}

			// Clear old posistion in map
			level.map[player.y][player.x] = Symbols::EMPTY;

			// Blocking until it gets valid value from keyboard

			bool keyAccepted = false;
			do {
				input = readInput();
				keyAccepted = triggerActions(input);
			} while (!keyAccepted);

			// Put player in new position on map
			level.map[player.y][player.x] = Symbols::PLAYER;

			// Clear console
			system("cls");
		}
	}

	void drawLevel() {
		for (int y = 0; y < level.mapHeight; y++) {
			for (int x = 0; x < level.mapWidth; x++) {
				// CIRCLE FOG
				// Simulate fog using Euclidean distance
				double distanceFromPlayer = sqrt(pow(player.y - y, 2) + pow(player.x - x, 2));
				if (distanceFromPlayer > settings.FOG_RADIUS) {
					// (x, y) is away from the player
					cout << MapElement::FOG;
				}
					// SQUARE FOG
				//if (abs(player.x - x) > settings.SQUARE_FOG_RANGE || abs(player.y - y) > settings.SQUARE_FOG_RANGE) {
					//cout << MapElement::FOG;
				//}



				else {
					// (x, y) is within player sight
					switch (level.map[y][x])
					{
					case Symbols::EMPTY:
						cout << MapElement::EMPTY;
						break;
					case Symbols::PLAYER:
						cout << MapElement::PLAYER;
						break;

					case Symbols::VERTICAL:
						cout << MapElement::VERTICAL;
						break;

					case Symbols::HORIZONTAL:
						cout << MapElement::HORIZONTAL;
						break;

					case Symbols::UPPER_RIGHT_CORNER:
						cout << MapElement::UPPER_RIGHT_CORNER;
						break;

					case Symbols::UPPER_LEFT_CORNER:
						cout << MapElement::UPPER_LEFT_CORNER;
						break;

					case Symbols::BOTTOM_RIGHT_CORNER:
						cout << MapElement::BOTTOM_RIGHT_CORNER;
						break;

					case Symbols::BOTTOM_LEFT_CORNER:
						cout << MapElement::BOTTOM_LEFT_CORNER;
						break;

					case Symbols::EXIT_AREA:
						cout << MapElement::EXIT_AREA;
						break;

					default:


						// Symbol to MapElement not implemented
						cout << '!';
					}
				}
			}
			cout << endl;
		}
	}
};

struct Menu {

	int availableLevels = 3;

	string askWhichLevel() {

		string levelPaths[] = { "D:\\Repos\\Maze\\lvl1.txt", "D:\\Repos\\Maze\\lvl2.txt", "D:\\Repos\\Maze\\lvl3.txt" };

		// Display available levels
		for (int lvlID = 1; lvlID <= availableLevels; lvlID++) {
			cout << "Level " << lvlID << endl;
		}
		cout << "Which level you want to play?" << endl;

		// Choose level
		int selectedLevelID;
		cin >> selectedLevelID;


		while (selectedLevelID > availableLevels || selectedLevelID < 1) {

			cout << "Incorrect lvl number, try again..." << endl;
			cin >> selectedLevelID;
			
		}

		cout << "Set fog range(1-20): ";

		// Choose fog range
		int fogRange;
		cin >> fogRange;


		//settings.SQUARE_FOG_RANGE = fogRange;
		settings.FOG_RADIUS = fogRange;

		// Clear console
		system("cls");

		return levelPaths[selectedLevelID - 1];
	}

};

int main() {
	Menu menu;
	GameEngine engine;

	string lvlPath = menu.askWhichLevel();
	bool lvlLoadSuccess = engine.level.loadFromFile(lvlPath);

	if (lvlLoadSuccess)
		engine.runGameLoop();
	return 0;
}