/*
 * Cpu.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: H183853
 */

#include <cmath>
#include "./Includes/Game.h"

#define PADDLE_SURFACE (50)

static inline double deg2rad (int deg)
{
	return (((double)deg/360)*(2*M_PI));
}

int Cpu::move(int x, int y, int angle)
{
	int response = 50;
	enum {QI,QII,QIII,QIV} quadrant = QI;
	float xvsy = 1.0;

	printf(">>cpu %i,%i,%i  \r\n",x,y,angle);

	// determine the quadrant for the given angle
	if(angle>0 && angle<=90)
		quadrant = QI;
	else if(angle>90 && angle<=180)
		quadrant = QII;
	else if(angle>180 && angle<=270)
		quadrant = QIII;
	else //(angle>180 && angle<=270)
		quadrant = QIV;

	printf(" Q[%i] ",quadrant);

	// 1) get the relation between x and y (literally the division x/y)
	switch(quadrant)
	{
	case QI: // going right
		xvsy = tan(deg2rad(90-angle));
		break;
	case QII: // going left
		xvsy = tan(deg2rad(angle-90));
		break;
	case QIII: //going left
		xvsy = tan(deg2rad(270-angle));
		break;
	case QIV: // going right
		xvsy = tan(deg2rad(angle-270));
		break;
	}
	printf(" xvsy[%f] ",xvsy);

	if(quadrant == QII || quadrant == QIII) // ball is coming
	{
		printf(" INCOMING!! ");

		// check if given current angle and y coord is coming to our paddle
		if(( (1/xvsy)*(x) ) < 0 || ( (1/xvsy)*(x) ) > GAME_HEIGHT)
			printf(" gonna bounce ");

		if((x - (xvsy*(GAME_HEIGHT-y))) < PADDLE_SURFACE)
			printf( "TO OUR PADDLE" );
	}
	else //ball is heading the other direction
	{}

	// this is done by estimating how much it's moving in x at given y

	// estimate the point where it'll impact the paddle

	// prepare response

	if(y>GAME_HEIGHT/2) //TODO: remove this
		response = 550;
	else
		response = 50;

	printf("<<cpu %i \r\n",response);

	return response;
}


