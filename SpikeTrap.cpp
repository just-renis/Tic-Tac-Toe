#include "SpikeTrap.h"

SpikeTrap::SpikeTrap(Texture* tex, Vector2f pos, float frameSecs)
	: Trap{ "SpikeTrap", tex, pos, frameSecs, IntRect{40, 0, 40, 40}, 1, false}
{
	
}