
#include <iostream>
#include <stdlib.h>
#include <string>
#include "windows.h"

/*
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"

#define TFT_DC 9
#define TFT_CS 10
*/

using namespace std;

const int screensizeX = 200;
const int screensizeY = 200;
const int shift = 2;

const int sizeX = 20;
const int sizeY = 20;

const int ratio = screensizeX / sizeX;

int lifepoints = 4;
int enemyCount = 15;
int gamespeed = 100;
int shootingrate = 3;

char spaceship = '^';
char enemy = 'O';
char spaceshipMissile = ':';
char enemyMissile = '-';
char space = ' ';




bool endgame = false;
bool gameover = false;

string life = "Life Points: ";
string lifesymbol = "* ";
string win = "Congratulations!";
string lose = "Game Over!";


char GameSpace[20][20] = {
	"...................",
	".                 .",
	". O O O O O O O O .",
	".  O O O O O O O  .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".                 .",
	".              ^  .",
	"..................."
};


void convertToPixel(int x, int y, int * pixX, int * pixY) {

	*pixX = x * ratio;
	*pixY = y * ratio;
}

void updateGameSpace(char array[sizeX][sizeY]) {

	int * pixelX;
	int * pixelY;

	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {

			convertToPixel(i, j, pixelX, pixelY);

			if(array[i][j] == ' ')
				tft.fillRect(pixelX + shift, pixelY + shift, pixelX + ratio - shift, pixelY + ratio - shift, ILI9341_WHITE);
			else if (array[i][j] == '.')
				tft.fillRect(pixelX + shift, pixelY + shift, pixelX + ratio - shift, pixelY + ratio - shift, ILI9341_GREEN);
			else if (array[i][j] == 'O')
				tft.fillRect(pixelX + shift, pixelY + shift, pixelX + ratio - shift, pixelY + ratio - shift, ILI9341_BLACK);
			else if (array[i][j] == '^')
				tft.fillRect(pixelX + shift, pixelY + shift, pixelX + ratio - shift, pixelY + ratio - shift, ILI9341_BLUE);
			else if (array[i][j] == ':')
				tft.fillRect(pixelX + (shift + 2), pixelY + (shift + 2), pixelX + ratio - (shift + 2), pixelY + ratio - (shift + 2), ILI9341_ORANGE);
			else if(array[i][j] = '-')
				tft.fillRect(pixelX + (shift + 2), pixelY + (shift + 2), pixelX + ratio - (shift + 2), pixelY + ratio - (shift + 2), ILI9341_ORANGE);
		}
	
	}
		
		tft.setCursor(screensizeX + ratio, screensizeY + ratio);
		tft.print("Life Points: ");
		tft.print(life);

}

int main() {


	while(endgame == false) {
		
		// Clears previous game state.
		system("cls");

		// Print current game state
		for (int posY = 0; posY < 20; posY++) {
			for (int posX = 0; posX < 20; posX++)
				cout << GameSpace[posY][posX];
				cout << endl;
		}

		cout << life;
		for (int i = 0; i < lifepoints; i++)
			cout << lifesymbol;
		cout << endl;
		
		// Iterate through each game state position and do updates
		for(int posY = 0; posY < sizeY; posY++) {
			for(int posX = 0; posX < sizeX; posX++){

				char current = GameSpace[posY][posX];

				// Current position is where the spaceship is
				if (current == spaceship) {

					// User pressed Left Key, move spaceship one position left if it isnt against the corner
					if (GetAsyncKeyState(VK_LEFT) != 0) {

						if (posX > 1) {
							GameSpace[posY][posX] = space;
							posX--;
							GameSpace[posY][posX] = spaceship;
						}

					}

					// User pressed Right Key, move spaceship one position right if it isnt against the corner
					else if (GetAsyncKeyState(VK_RIGHT) != 0) {

						if (posX < sizeX - 3) {
							GameSpace[posY][posX] = space;
							posX++;
							GameSpace[posY][posX] = spaceship;
						}
					}

					// User pressed Space Key, create missile one position up from spaceship.
					if (GetAsyncKeyState(VK_SPACE) != 0) {

						posY--;
						GameSpace[posY][posX] = spaceshipMissile;
					}

				}

				// Current position is where an enemy is. Randomize number to check if that enemy is firing a missile.
				else if (current == enemy) {

					if (rand() % 100 < shootingrate) {
						posY++;
						GameSpace[posY][posX] = enemyMissile;
					}
				}

				// Current position is where spaceship missile is.
				else if (current == spaceshipMissile) {

					// If missile isnt against top corner, it either moves up one or it hits an enemy
					if (posY > 1) {

						// Erase current position of missile
						GameSpace[posY][posX] = space;
						posY--;
						
						// If next position up is empty space put the missile there
						if(GameSpace[posY][posX] == space)
							GameSpace[posY][posX] = spaceshipMissile;
						
						// If next position up is enemy, delete enemy, update enemy count.
						else if (GameSpace[posY][posX] == enemy) {
							GameSpace[posY][posX] = space;
							enemyCount--;

							// No more enemies, user wins.
							if (enemyCount == 0) {
								gameover = false;
								endgame = true;
							}
						}
					}

					// If missile reaches corner, eliminate the missile.
					else if (posY == 1)
						GameSpace[posY][posX] = space;
				}
				
				// Current postion is where enemy missile is.
				else if (current == enemyMissile) {

					// User was hit. Reduce life points. Check if game over.
					if (GameSpace[posY + 1][posX] == spaceship) {
						
						lifepoints--;
						
						if (lifepoints == 0) {
							gameover = true;
							endgame = true;
						}
					}

					// If missile is in between, advance missile one more towards bottom corner.
					else if (posY < sizeY - 2 && GameSpace[posY + 1][posX] == space) {
						GameSpace[posY][posX] = space;
						posY++;
						GameSpace[posY][posX] = enemyMissile;
					}

					// Enemy missile reached bottom corner without hitting user. Erace enemy missile
					else if (posY == (sizeY - 2) && GameSpace[posY][posX] != spaceship)
						GameSpace[posY][posX] = space;
				}
			
			}
		}

		Sleep(gamespeed);
		

		// Update game space for each arrat element
		updateGameState(GameSpace);
	}

	// Game is finished. Prints congratulations or game over accordingly.
	system("cls");

	if (gameover)
		cout << lose << endl;
	else
		cout << win << endl;

	system("PAUSE");

	return 0;
};

