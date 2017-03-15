#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Inputs.h"
using namespace std;

#define INITIAL_PADDLE_SPEED (400.f)
#define INITIAL_BALL_SPEED (400.f)

class Paddle: public sf::RectangleShape
{
public:
	// Shape & pos
	const int width=25;
	const int height=100;
	// Physics
	float speed = INITIAL_PADDLE_SPEED;
	// Methods
	//Paddle(); // Constructor is not needed
	//int move();
	bool setDestiny(int y);
	bool setStart(float x, float y); // call this before using
};

class Paddles
{
private:
	void processInput(bool, Input *, float);
public:
	Paddle right;
	Paddle left;

	void actuate(Inputs *, float);
};

class Ball :public sf::CircleShape
{
private:
	float angle;
public:
	// Shape & pos
	const float Radius=10.f;
	// Physics
	float speed = INITIAL_BALL_SPEED;
	enum ObjectHit{
		HIT_NOTHING = 0,
		HIT_LEFT_PADDLE,
		HIT_RIGHT_PADDLE,
		HIT_LEFT_BORDER,
		HIT_RIGHT_BORDER,
		HIT_TOP_BORDER,
		HIT_BOTTOM_BORDER,
	};
	ObjectHit hit = HIT_NOTHING;

	// Methods
	//int playSound();
	float getAngle();
	float getAngleMirror(); // 0-360 degrees
	bool checkCollision(Paddles * ps);
	void setStart(float x, float y); //call this before using
};

#endif
