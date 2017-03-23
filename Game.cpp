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
			case sf::Keyboard::C:      state = CONFIG; printf("config!!\r\n"); break;
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

void Game::config()
{
	sf::Text lbl[2];
	sf::Text set[2];
	int option[2]={0,0};

	sf::RectangleShape box;
	sf::Vector2f boxsize(100,40);
	int boxpos = 0; //left by default

	int optioncount = 0; //get options count
	int serials[8] = {0,0,0,0,0,0,0,0};
	char strbuf[16];

	optioncount = 1 + inputs.getComCount(serials);

	printf("--------\r\noptioncount = %i\r\n-----------",optioncount);

	box.setSize(boxsize);
	box.setOutlineColor(sf::Color::Yellow);
	box.setFillColor(sf::Color::Black);
	box.setOutlineThickness(5);
	box.setOrigin( boxsize / 2.f );
	box.setPosition((GAME_WIDTH/3)+50,GAME_HEIGHT/2);

	lbl[0].setFont(font);
	lbl[0].setString("Player1");
	lbl[0].setPosition(GAME_WIDTH/3,GAME_HEIGHT/2);
	lbl[0].setFillColor(sf::Color::Yellow);

	lbl[1].setFont(font);
	lbl[1].setString("Player2");
	lbl[1].setPosition(GAME_WIDTH*2/3,GAME_HEIGHT/2);
	lbl[1].setFillColor(sf::Color::Yellow);

	set[0].setFont(font);
	set[0].setString("KEY");
	set[0].setPosition(GAME_WIDTH/3,50+(GAME_HEIGHT/2));
	set[0].setFillColor(sf::Color::White);

	set[1].setFont(font);
	set[1].setString("KEY");
	set[1].setPosition(GAME_WIDTH*2/3,50+(GAME_HEIGHT/2));
	set[1].setFillColor(sf::Color::White);

	while(state == CONFIG)
	{
		InputEvent ie = inputs.pollInputs(&window);
		if(ie == IE_KEY_PRESS)
		{
			switch(inputs.last_key){
			case sf::Keyboard::Escape: state = END;  break;
			case sf::Keyboard::Space:
			case sf::Keyboard::Return:
				if     (option[0]==0) inputs.left.source = INPUT_SOURCE_KEYBOARD;
				else if(option[0]==1) inputs.left.source = INPUT_SOURCE_CPU;
				else
				{
					inputs.left.source = INPUT_SOURCE_SERIAL;
					inputs.left.comPort = serials[option[0]-2]+1; // Port numbers start from 0 (COM1)
					printf("serials[option[0]-1] = %i;",serials[option[0]-2] );
				}
				if     (option[1]==0) inputs.right.source = INPUT_SOURCE_KEYBOARD;
				else if(option[1]==1) inputs.right.source = INPUT_SOURCE_CPU;
				else
				{
					inputs.right.source = INPUT_SOURCE_SERIAL;
					inputs.right.comPort = serials[option[1]-2]+1; // Port numbers start from 0 (COM1)
					printf("serials[option[0]-1] = %i;",serials[option[1]-2] );
				}
				state = GAME;
				break;
			case sf::Keyboard::Left:
				boxpos=0;
				break;
			case sf::Keyboard::Right:
				boxpos=1;
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

		if(boxpos==0)
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
		window.draw(lbl[0]);
		window.draw(lbl[1]);
		window.draw(set[0]);
		window.draw(set[1]);
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
