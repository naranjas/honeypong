/*
TODOs:
Add: Initial splash screen
Fix: Game state for game over
Fix: Only increment speed when bouncing paddles
Fix: mirrored coordinates for right player
Add: gitHub repo for code, drawings and bins
Fix: reset ball speed after each score
Fix: reset paddles speed after each score
TODO - Fix: configuration method make 'config.cfg' (new line style)
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
