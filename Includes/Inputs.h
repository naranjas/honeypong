#ifndef _INPUTS_H_
#define _INPUTS_H_
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Cpu.h"
using namespace std;

typedef enum
{
	INPUT_SOURCE_KEYBOARD = 0,
	INPUT_SOURCE_SERIAL,
	INPUT_SOURCE_CPU,
	INPUT_SOURCE_WALL,
	INPUT_SOURCE_NONE,
	INPUT_SOURCE_COUNT
}InputSource;

typedef enum
{
	IE_KEY_PRESS  ,
	IE_SER_DATA   ,
	IE_CPU_ACTION ,
	IE_NOTHING    ,
	IE_CLOSED     ,
	IE_COUNT
}InputEvent;

class Input
{
public:
	InputSource source;
	Cpu cpu;

	int last_x;
	int last_y;
	//Serial data information
	unsigned int comPort=0; // 0 means COMM1
	unsigned int baudRate=115200;
	void sendPosition(int,int,float);
};

class Inputs
{
private:
	bool loadFromFile(const char *);
public:
	Input left;
	Input right;

	sf::Keyboard::Key last_key;
	InputEvent pollInputs(sf::RenderWindow *);
	bool loadConfig(const char* path);
	bool init(void);
	int getComCount(int *);
	bool initialized=false;
};

#endif // _INPUTS_H_
