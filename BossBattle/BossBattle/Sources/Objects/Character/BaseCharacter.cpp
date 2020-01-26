#include"BaseCharacter.h"

BaseCharacter::BaseCharacter(std::shared_ptr<Light> _light) : BaseObject(_light)
{
	hp = 100.0f;
	e = 1.0f;
	die = false;
	dead = false;

	param = std::make_shared<Param>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
}