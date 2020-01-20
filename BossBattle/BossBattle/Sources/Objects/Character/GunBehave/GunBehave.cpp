#include"GunBehave.h"

GunBehave::GunBehave()
{
	nextBehave = GUN_BEHAVE::BehaveName::NONE;
}


bool GunBehave::IsNextBehave()
{
	return (nextBehave != GUN_BEHAVE::BehaveName::NONE);
}

GUN_BEHAVE::BehaveName GunBehave::GetNextBehave()
{
	return nextBehave;
}