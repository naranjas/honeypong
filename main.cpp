/*
TODOs:
Add: Initial splash screen
Fix: Game state for game over
Fix: Only increment speed when bouncing paddles
TODO - Fix: mirrored coordinates for right player
TODO - Fix: reset ball speed after each score
TODO - Add: gitHub repo for code, drawings and bins
TODO - Add: Initial ball speed configurable
TODO - Fix: ball going through the bar
TODO - Add: 'Wall' option
TODO - Fix: CPU intelligence
TODO - Add: Scoring
TODO - Add: a better method for configuring inputs
*/

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include ".\Includes\Game.h"

int main ()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	Game game;
	game.runGame();
	return 0;
}
