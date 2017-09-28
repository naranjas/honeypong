#include "./Includes/Game.h"

#include <cmath>
#include <ctime>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>

Game::Game()
{
	state = END;
	if (!ballSoundBuffer.loadFromFile("resources/plop.ogg"))
		return ;
	if (inputs.loadConfig(".\\resources\\config.cfg")!= true)
		return ;
	if (splashTexture.loadFromFile(".\\resources\\splash.png") != true)
		return ;
	if(font.loadFromFile(".\\resources\\sansation.ttf")!=true)
		return;

	text.setFont(font);
	ballSound.setBuffer(ballSoundBuffer);
	window.create(sf::VideoMode(800, 600), "Honey Pong!",
			sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	sounds = true;
	state = MENU;
}


void Game::runGame()
{
	while(state!=END)
	{
		switch(state)
		{
		case MENU:
			menu();
			break;
		case GAME:
			play();
			break;
		case SCORE:
			score();
			break;
		case CONFIG:
			config();
			break;
		case END:
			break;
		default:
			break;
		}
	}
	return;
}

void Game::menu()
{
    menu_sprite.setTexture(splashTexture);
    menu_sprite.setOrigin(sf::Vector2f(splashTexture.getSize()) / 2.f);
    menu_sprite.setPosition(menu_sprite.getOrigin());

	while(state == MENU)
	{
		InputEvent ie = inputs.pollInputs(&window);
		if(ie == IE_KEY_PRESS)
		{
			switch(inputs.last_key){
			case sf::Keyboard::Escape: state = END;  break;
			case sf::Keyboard::Space:  state = GAME; break;
			case sf::Keyboard::C:      state = CONFIG; break;
			default: break;
			}
		}

		// Clear the window
		window.clear(sf::Color::Black);

		// Redraw objects (should happen about 60 times per second)
		window.draw(menu_sprite);
		window.display();
	}

	return;
}

void Game::score()
{
	text.setString("SCORE!!");
	text.setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
	text.setFillColor(sf::Color::White);
	// Redraw objects (should happen about 60 times per second)
	window.draw(text);
	window.display();

	while(state == SCORE)
	{
		InputEvent ie = inputs.pollInputs(&window);
		if(ie == IE_KEY_PRESS)
		{
			switch(inputs.last_key){
			case sf::Keyboard::Escape: state = END;  break;
			case sf::Keyboard::Space:  state = GAME; break;
			default: break;
			}
		}
	}

	return;
}

enum players
{
	LEFT_PLAYER  = 0,
	RIGHT_PLAYER = 1,
	PLAYER_COUNT = 2
};

void Game::config()
{
	sf::Text lbl[PLAYER_COUNT];
	sf::Text set[PLAYER_COUNT];
	int option[PLAYER_COUNT]={0,0};

	sf::RectangleShape box;
	sf::Vector2f boxsize(100,40);
	int boxpos = 0; //left by default

	int optioncount = 0; //get options count
	int serials[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
	char strbuf[16];

	optioncount = 1 + inputs.getComCount(serials);

	box.setSize(boxsize);
	box.setOutlineColor(sf::Color::Yellow);
	box.setFillColor(sf::Color::Black);
	box.setOutlineThickness(5);
	box.setOrigin( boxsize / 2.f );
	box.setPosition((GAME_WIDTH/3)+50,GAME_HEIGHT/2);

	lbl[LEFT_PLAYER].setFont(font);
	lbl[LEFT_PLAYER].setString("Player1");
	lbl[LEFT_PLAYER].setPosition(GAME_WIDTH/3,GAME_HEIGHT/2);
	lbl[LEFT_PLAYER].setFillColor(sf::Color::Yellow);

	lbl[RIGHT_PLAYER].setFont(font);
	lbl[RIGHT_PLAYER].setString("Player2");
	lbl[RIGHT_PLAYER].setPosition(GAME_WIDTH*2/3,GAME_HEIGHT/2);
	lbl[RIGHT_PLAYER].setFillColor(sf::Color::Yellow);

	set[LEFT_PLAYER].setFont(font);
	set[LEFT_PLAYER].setString("KEY");
	set[LEFT_PLAYER].setPosition(GAME_WIDTH/3,50+(GAME_HEIGHT/2));
	set[LEFT_PLAYER].setFillColor(sf::Color::White);

	set[RIGHT_PLAYER].setFont(font);
	set[RIGHT_PLAYER].setString("KEY");
	set[RIGHT_PLAYER].setPosition(GAME_WIDTH*2/3,50+(GAME_HEIGHT/2));
	set[RIGHT_PLAYER].setFillColor(sf::Color::White);

	while(state == CONFIG)
	{
		InputEvent ie = inputs.pollInputs(&window);
		if(ie == IE_KEY_PRESS)
		{
			switch(inputs.last_key){
			case sf::Keyboard::Escape: state = END;  break;
			case sf::Keyboard::Space:
			case sf::Keyboard::Return:
				if     (option[LEFT_PLAYER]==0) inputs.left.source = INPUT_SOURCE_KEYBOARD;
				else if(option[LEFT_PLAYER]==1) inputs.left.source = INPUT_SOURCE_CPU;
				else
				{
					inputs.left.source = INPUT_SOURCE_SERIAL;
					inputs.left.comPort = serials[option[LEFT_PLAYER]-2]+1; // Port numbers start from 0 (COM1)
				}
				if     (option[RIGHT_PLAYER]==0) inputs.right.source = INPUT_SOURCE_KEYBOARD;
				else if(option[RIGHT_PLAYER]==1) inputs.right.source = INPUT_SOURCE_CPU;
				else
				{
					inputs.right.source = INPUT_SOURCE_SERIAL;
					inputs.right.comPort = serials[option[RIGHT_PLAYER]-2]+1; // Port numbers start from 0 (COM1)
				}
				state = GAME;
				break;
			case sf::Keyboard::Left:
				boxpos=LEFT_PLAYER;
				break;
			case sf::Keyboard::Right:
				boxpos=RIGHT_PLAYER;
				break;
			case sf::Keyboard::Up:
				if(option[boxpos]<optioncount)
					option[boxpos]++;
				else
					option[boxpos]=0;
				break;
			case sf::Keyboard::Down:
				if(option[boxpos]>0)
					option[boxpos]--;
				else
					option[boxpos]=optioncount;
				break;
			default: break;
			}
		}

		if(boxpos==LEFT_PLAYER)
			box.setPosition((GAME_WIDTH/3)+50,70+(GAME_HEIGHT/2));
		else
			box.setPosition(50+(GAME_WIDTH*2/3),70+(GAME_HEIGHT/2));

		if (option[boxpos]==0)
			set[boxpos].setString("KEY");
		else if (option[boxpos]==1)
			set[boxpos].setString("CPU");
		else
		{
			sprintf(strbuf,"COM%i",serials[option[boxpos]-2]+1);
			set[boxpos].setString(strbuf);
		}

		window.clear(sf::Color::Black);

	// Redraw objects (should happen about 60 times per second)
		window.draw(box);
		window.draw(lbl[LEFT_PLAYER]);
		window.draw(lbl[RIGHT_PLAYER]);
		window.draw(set[LEFT_PLAYER]);
		window.draw(set[RIGHT_PLAYER]);
		window.display();
	}

	return;
}

void Game::play()
{
	if(!inputs.initialized)
		inputs.init();

	// Initialize positions
	ball.setStart(Width/2,Height/2);
	paddles.left.setStart(paddles.left.width,Height/2);
	paddles.right.setStart(Width - (paddles.right.width),Height/2);

	inputs.left.sendPosition(ball.getPosition().x,ball.getPosition().y, ball.getAngle());
	inputs.right.sendPosition(GAME_WIDTH-ball.getPosition().x,ball.getPosition().y, ball.getAngleMirror());

	sf::Clock clock;
	while (state==GAME)
	{
		float deltaTime = clock.restart().asSeconds();

		InputEvent ie = inputs.pollInputs(&window);

		// Check exit conditions
		if(( (ie == IE_KEY_PRESS) && (inputs.last_key == sf::Keyboard::Escape)) ||
				(ie == IE_CLOSED) )
			state = END;

		// Take care of the ball
		float factor = ball.speed * deltaTime;
		ball.move(std::cos(ball.getAngle()) * factor, std::sin(ball.getAngle()) * factor);
		if(ball.checkCollision(&paddles))
		{
			switch(ball.hit)
			{
			case ball.HIT_RIGHT_BORDER:
			case ball.HIT_LEFT_BORDER:
				state = SCORE;
				break;
			case ball.HIT_LEFT_PADDLE:
			case ball.HIT_RIGHT_PADDLE:
				ball.speed+=10;
				paddles.left.speed+=5;
				paddles.right.speed+=5;
				ballSound.play();
				inputs.left.sendPosition(ball.getPosition().x,ball.getPosition().y, ball.getAngle());
				inputs.right.sendPosition(GAME_WIDTH-ball.getPosition().x,ball.getPosition().y, ball.getAngleMirror());
				break;
			default:
				inputs.left.sendPosition(ball.getPosition().x,ball.getPosition().y, ball.getAngle());
				inputs.right.sendPosition(GAME_WIDTH-ball.getPosition().x,ball.getPosition().y, ball.getAngleMirror());
				ballSound.play();
				break;
			}
		}

		// Move the paddles
		paddles.actuate(&inputs,deltaTime);

		// Clear the window
		window.clear(sf::Color::Black);

		// Redraw objects (should happen about 60 times per second)
		window.draw(ball);
		window.draw(paddles.left);
		window.draw(paddles.right);
		window.display();
	}

	return;
}
