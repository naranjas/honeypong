#include "./Includes/Objects.h"
#include "./Includes/Game.h"
#include "./Includes/Inputs.h"
#include <math.h>

#define POSITION_TOLERANCE (10)

float Ball::getAngle() { return angle; }

float Ball::getAngleMirror()
{
	float mirror;

	mirror = angle; //start
	while (mirror<0) mirror+=2*M_PI; //convert negatives
	mirror = fmod(mirror,M_PI*2); // limit to 2 pi

	if (mirror < M_PI)
		mirror = M_PI-mirror;
	else // if (mirror > M_PI)
		mirror = (3*M_PI) - mirror;

	return mirror;
}

void Ball::setStart(float x, float y)
{
	setRadius(Radius);
	setOutlineColor(sf::Color::Black);
	setFillColor(sf::Color::White);
	setOrigin(Radius / 2, Radius / 2);
	setPosition(x,y);
	speed = INITIAL_BALL_SPEED;
	// Reset the ball angle (ensure initial angle is not too much vertical)
	do { angle = (std::rand() % 360) * 2 * M_PI / 360; }  while (std::abs(std::cos(angle)) < 0.7f);
}

// TODO: improve collision detection
bool Ball::checkCollision(Paddles * ps)
{
	bool collision = true; //until proven false

	// Check collisions between the ball and the screen
	if (getPosition().x - Radius < 0.f)
	{
		hit = HIT_LEFT_BORDER;
	}
	else if (getPosition().x + Radius > GAME_WIDTH)
	{
		hit = HIT_RIGHT_BORDER;
	}
	else if (getPosition().y - Radius < 0.f)
	{
		hit = HIT_TOP_BORDER;
		angle = -angle;
		setPosition(getPosition().x, Radius + 0.1f);
	}
	else if (getPosition().y + Radius > GAME_HEIGHT)
	{
		hit = HIT_BOTTOM_BORDER;
		angle = -angle;
		setPosition(getPosition().x, GAME_HEIGHT - Radius - 0.1f);
	}
	// Check the collisions between the ball and the paddles
	// Left Paddle
	else if (
			getPosition().x - Radius <  ps->left.getPosition().x + ps->left.getSize().x / 2 &&
			getPosition().x - Radius >  ps->left.getPosition().x - ps->left.getSize().x &&
			getPosition().y + Radius >= ps->left.getPosition().y - ps->left.getSize().y / 2 &&
			getPosition().y - Radius <= ps->left.getPosition().y + ps->left.getSize().y / 2)
	{
		if (getPosition().y > ps->left.getPosition().y)
			angle = M_PI - angle + (std::rand() % 20) * M_PI / 180;
		else
			angle = M_PI - angle - (std::rand() % 20) * M_PI / 180;

		hit = HIT_LEFT_PADDLE;
		setPosition(ps->left.getPosition().x + Radius + ps->left.getSize().x / 2 + 0.1f, getPosition().y);
	}
	// Right Paddle
	else if (
			getPosition().x + Radius >  ps->right.getPosition().x - ps->right.getSize().x / 2 &&
			getPosition().x + Radius <  ps->right.getPosition().x + ps->right.getSize().x &&
			getPosition().y + Radius >= ps->right.getPosition().y - ps->right.getSize().y / 2 &&
			getPosition().y - Radius <= ps->right.getPosition().y + ps->right.getSize().y / 2)
	{
		if (getPosition().y > ps->right.getPosition().y)
			angle = M_PI - angle + (std::rand() % 20) * M_PI / 180;
		else
			angle = M_PI - angle - (std::rand() % 20) * M_PI / 180;

		hit = HIT_RIGHT_PADDLE;
		setPosition(ps->right.getPosition().x - Radius - ps->right.getSize().x / 2 - 0.1f, getPosition().y);
	}
	else
	{
		// No collision
		hit = HIT_NOTHING;
		collision=false;
	}

	return collision;
}

bool Paddle::setStart(float x, float y)
{
	sf::Vector2f paddleSize(width,height);

	setSize(paddleSize);
	setFillColor(sf::Color::Yellow);
	setOrigin(paddleSize / 2.f);
	setPosition(x,y);
	speed = INITIAL_PADDLE_SPEED;
	return true;
}

void Paddles::processInput (bool isleft, Input * in, float dT)
{
	Paddle * pad;

	if (isleft)
		pad = &left;
	else
		pad = &right;

	if (in->source == INPUT_SOURCE_KEYBOARD)
	{
		// Move the player's paddle
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (pad->getPosition().y - pad->height / 2 > 5.f)
				pad->move(0.f, -pad->speed * dT);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (pad->getPosition().y + pad->height / 2 < GAME_HEIGHT - 5.f)
				pad->move(0.f, pad->speed * dT);
		}
	}
	else if (in->source == INPUT_SOURCE_SERIAL || in->source == INPUT_SOURCE_CPU)
	{
		if(pad->getPosition().y > in->last_y)
		{
			// Move only when inside the borders and close enough to desired position
			if (pad->getPosition().y - pad->height / 2 > 5.f)
				pad->move(0.f, -pad->speed * dT);
		}
		else if (pad->getPosition().y < in->last_y)
		{
			// Move only when inside the borders and close enough to desired position
			if (pad->getPosition().y + pad->height / 2 < GAME_HEIGHT - 5.f)
				pad->move(0.f, pad->speed * dT);
		}

		if(abs(pad->getPosition().y - in->last_y) < POSITION_TOLERANCE)
			pad->setPosition(pad->getPosition().x, in->last_y);
	}
}

void Paddles::actuate(Inputs * ins, float dT)
{
	processInput(true ,&ins->left, dT);
	processInput(false,&ins->right,dT);
}


