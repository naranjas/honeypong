#ifndef _GAME_H_
#define _GAME_H_
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Objects.h"
#include "Inputs.h"
using namespace std;

#define GAME_WIDTH   800
#define GAME_HEIGHT  600

class Game: public sf::Transformable
{
public:
	const int Width =GAME_WIDTH;
	const int Height=GAME_HEIGHT;
    enum GameState {MENU, GAME, CONFIG, SCORE, GAME_OVER, END};
    GameState state;

    sf::RenderWindow window;
    Ball ball;
    Paddles paddles;
    Inputs  inputs;
    sf::Font font;
	sf::Text text;
	sf::Text error;
	sf::Texture splashTexture;
	sf::Sprite menu_sprite;
	bool sounds;
    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    sf::Sound ballSound;

	Game();

	void runGame();

	void menu();

	void play();

	void score();

	void config();

	void options();
};

#endif // _GAME_H_
