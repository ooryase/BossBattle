#include"BaseCharacter.h"

BaseCharacter::BaseCharacter(std::shared_ptr<Light> _light) : BaseObject(_light)
{
	hp = 100.0f;
	e = 1.0f;
	die = false;
	dead = false;

}

void BaseCharacter::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{
	switch (_tag)
	{
	case ObjectTag::NORMAL:
		position.x += delta.x * 0.4f * e;
		position.y += delta.y * 0.4f * e;

		break;
	case ObjectTag::STEALTH:
		break;
	case ObjectTag::DAMAGE:
		break;
	default:
		break;
	}

}
