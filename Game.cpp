#include "./Includes/Game.h"

#include <cmath>
#include <ctime>
#include <stdio.h>
#include <cstdlib>

Game::Game()
{
	state = END;
	if (!ballSoundBuffer.loadFromFile("resources/plop.ogg"))
		return ;
	if (inputs.loadConfig(".\\resources\\config.cfg")!= true)
		return ;
	if (splashTexture.loadFromFile(".\\resources\\splash.jpg") != true)
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

void Game::play()
{
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
