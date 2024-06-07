#include "Utilities/RNG.h"
#include <stdint.h>

RNG::RNG()
	: m_RandomEngine{}
	, m_Distribution{}
{
	Init();
}

void RNG::Init()
{
	m_RandomEngine.seed( std::random_device()() );
}

float RNG::Float()
{
	return (float)m_Distribution( m_RandomEngine ) / (float)std::numeric_limits<uint32_t>::max();
}
