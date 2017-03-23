#include "./Includes/Inputs.h"
#include "./Includes/rs232.h"
#include "./Includes/Game.h"
#include "./Includes/Cpu.h"
#include <cmath>

static int rad2deg (float rad)
{
	float deg;
	deg = fmod(rad,2*M_PI);
	deg *= 360/(2*M_PI);
	if(deg < 0.f) deg+=360;
	return (int)deg;
}

int getline(char *lineptr, size_t n, FILE *stream)
{
	static char line[256];
	char *ptr;
	unsigned int len;

	if (lineptr == NULL || n == 0 || ferror (stream) || feof(stream))
	{
		return -1;
	}

	fgets(line,256,stream);

	ptr = strchr(line,'\n');
	if (ptr)
		*ptr = '\0';

	len = strlen(line);

	if (ptr == NULL)
		return(-1);

	strncpy(lineptr,line,(n)-1);
	return(len);
}

bool Inputs::loadFromFile(const char * filename)
{
	FILE * fp;
	char line[256];
	ssize_t readstr;
	int baud;
	bool success = true;

	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror("Error: config file not found");
		success=false;
	}

	while ((readstr = getline(line, sizeof(line), fp)) != -1 && success==true) {

		if(line[0]==';') continue;  //ignore comments

		if(strstr(line,"PLAYER1"))
		{
			if(strstr(line,"COM"))
			{
				left.source = INPUT_SOURCE_SERIAL;
				if(!sscanf(line,"PLAYER1=COM%i",&left.comPort))
				{
					perror("Unable to get comm port number for player 1\n");
					success = false;
				}
			}
			else if (strstr(line,"KEY"))  left.source = INPUT_SOURCE_KEYBOARD;
			else if (strstr(line,"CPU"))  left.source = INPUT_SOURCE_CPU;
			else if (strstr(line,"WALL")) left.source = INPUT_SOURCE_WALL;
			else { left.source = INPUT_SOURCE_NONE; success=false; }
		}
		else if(strstr(line,"PLAYER2"))
		{
			if(strstr(line,"COM"))
			{
				right.source = INPUT_SOURCE_SERIAL;
				if(!sscanf(line,"PLAYER2=COM%i",&right.comPort))
				{
					perror("Unable to get comm port number for player 2\n");
					return false;
				}
			}
			else if (strstr(line,"KEY"))  right.source = INPUT_SOURCE_KEYBOARD;
			else if (strstr(line,"CPU"))  right.source = INPUT_SOURCE_CPU;
			else if (strstr(line,"WALL")) right.source = INPUT_SOURCE_WALL;
			else { right.source = INPUT_SOURCE_NONE; success=false; }
		}
		else if(strstr(line,"BAUD"))
		{
			if (sscanf(line,"BAUD=%i",&baud))
			{
				right.baudRate = baud;
				left.baudRate = baud;
			}
			else
			{
				perror("Unable to get baud rate value\n");
				success=false;
			}
		}

	}

	fclose(fp);

	return success;
}

bool Inputs::init (void)
{
	if(right.source == INPUT_SOURCE_SERIAL)
	{
		char mode[]={'8','N','1',0};
		right.last_y = GAME_HEIGHT/2;
		if (RS232_OpenComport(right.comPort-1,right.baudRate,mode))
		{
			perror("Unable to open com port for right player");
			return false;
		}
		// wait for an instant before trying to use com ports
		sf::sleep(sf::milliseconds(400));
	}
	if(left.source == INPUT_SOURCE_SERIAL)
	{
		char mode[]={'8','N','1',0};
		left.last_y = GAME_HEIGHT/2;
		if (RS232_OpenComport(left.comPort-1,left.baudRate,mode))
		{
			perror("Unable to open com port for right player");
			return false;
		}
		// wait for an instant before trying to use com ports
		sf::sleep(sf::milliseconds(400));
	}

	initialized = true;
	return initialized;
}

bool Inputs::loadConfig(const char* path)
{
	if(!loadFromFile(path))
		return false; // operation failed

	return true;
}

InputEvent Inputs::pollInputs(sf::RenderWindow * w)
{
	sf::Event event;
	sf::Keyboard keyboard;

	// Check exit condition
	while(w->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			return IE_CLOSED;

		if(keyboard.isKeyPressed(sf::Keyboard::Up)
				|| keyboard.isKeyPressed(sf::Keyboard::Down)
				|| keyboard.isKeyPressed(sf::Keyboard::Left)
				|| keyboard.isKeyPressed(sf::Keyboard::Right)
				|| keyboard.isKeyPressed(sf::Keyboard::Escape)
				|| keyboard.isKeyPressed(sf::Keyboard::Space)
				|| keyboard.isKeyPressed(sf::Keyboard::Return)
				|| keyboard.isKeyPressed(sf::Keyboard::C)
		)
		{
			last_key = event.key.code;
			return IE_KEY_PRESS;
		}
	}

	if(right.source == INPUT_SOURCE_SERIAL)
	{
		unsigned char buf[61];
		int n= RS232_PollComport(right.comPort-1, buf, 60);

		if(n > 0)
		{
			sscanf((const char*)buf,"POS:%i",&right.last_y);
			printf(">>rx POS:%i\n", right.last_y);

			return IE_SER_DATA;
		}
	}
	if(left.source == INPUT_SOURCE_SERIAL)
	{
		unsigned char buf[61];
		int n= RS232_PollComport(left.comPort-1, buf, 60);

		if(n > 0)
		{
			sscanf((const char*)buf,"POS:%i",&left.last_y);
			printf(">>rx POS:%i\n", left.last_y);

			return IE_SER_DATA;
		}
	}


	return IE_NOTHING;
}

void Input::sendPosition(int x, int y, float rad)
{
	if(source == INPUT_SOURCE_SERIAL)
	{
		char outstr[100];
		sprintf(outstr,"BALL[X,Y]: %i,%i Angle: %i\r\n", x, y, (int)rad2deg(rad));
		printf(outstr);
		RS232_cputs(comPort-1, outstr);
	}
	else if(source == INPUT_SOURCE_CPU)
	{
		char outstr[100];
		sprintf(outstr,"CPU [X,Y]: %i,%i Angle: %i\r\n", x, y, (int)rad2deg(rad));
		printf(outstr);
		last_y = cpu.move(x, y, (int)rad2deg(rad));
	}
}

int Inputs::getComCount(int * i) {	return RS232_GetPorts(i);  }
