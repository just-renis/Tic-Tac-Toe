#include "SpearTrap.h"

SpearTrap::SpearTrap(Texture* tex, Vector2f pos, float frameSecs, float cooldownSecs)
	: Trap_nStop{ "SpearTrap", tex, pos, frameSecs, cooldownSecs, 160, IntRect{280, 0, 40, 60}, 4, false}
{

}