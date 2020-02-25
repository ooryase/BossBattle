#include"BaseCharacter.h"

BaseCharacter::BaseCharacter(std::shared_ptr<Light> _light, 
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
	std::vector< std::shared_ptr< BaseCharacter>>& _charaReserves,
	std::shared_ptr<ObjectManager> _objectManager) 
	: BaseObject(),
	effectReserves(&_effectReserves),
	charaReserves(&_charaReserves),
	objectManager(_objectManager)
{
	light = _light;
	maxHp = 100.0f;
	hp = maxHp;
	damagedHp = maxHp;
	isDamaged = false;
	e = 1.0f;
	isDead = false;  // isDead
	isToDelete = false;  // isToDelete

}

void BaseCharacter::UpdateDamaged()
{
	damagedTime += Timer::GetInstance().GetDeltaTime();

	if (!isDamaged)
	{
		if (hp < damagedHp)
		{
			damagedHp -= Timer::GetInstance().GetDeltaTime() / 3200.0f * maxHp;
		}
	}
	else if (damagedTime > 500)
		isDamaged = false;
}

void BaseCharacter::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{
	switch (_tag)
	{
	case ObjectTag::NORMAL:
		CollisionNormal(delta);
		break;
	case ObjectTag::STEALTH:
		break;
	case ObjectTag::DAMAGE:
		break;
	default:
		break;
	}

}

void BaseCharacter::CollisionNormal(DirectX::XMFLOAT3 delta)
{
	position.x += delta.x * 0.4f * e;
	position.y += delta.y * 0.4f * e;

}
void BaseCharacter::CollisionDamage(DParam* dParam)
{
	if (tag == ObjectTag::STEALTH)
		return;

	damagedTime = 0;
	isDamaged = true;
	hp -= dParam->damage;
	if (hp < 0.0f)
		hp = 0.0f;

	param->speed.x = dParam->direction.x * e;
	param->speed.y = dParam->direction.y * e;
	param->speed.z = dParam->direction.z * e;
}


//overrideÇ≥ÇÍÇ»Ç¢èÍçá âΩÇ‡ÇµÇ»Ç¢
void BaseCharacter::AttackHit(int type, int quantity)
{}

//overrideÇ≥ÇÍÇ»Ç¢èÍçá âΩÇ‡ÇµÇ»Ç¢
void BaseCharacter::SetObjectReserved(std::shared_ptr<BaseCharacter> _chara)
{
}

//overrideÇ≥ÇÍÇ»Ç¢èÍçá âΩÇ‡ÇµÇ»Ç¢
void BaseCharacter::SetEffectReserved(std::shared_ptr<BaseEffect> _obj)
{
}

std::shared_ptr<ObjectManager> BaseCharacter::GetObjectManager() const
{
	return objectManager;
}

std::shared_ptr<Light> BaseCharacter::GetLight() const
{
	return light;
}

DirectX::XMFLOAT3 BaseCharacter::GetDirection() const
{
	return param->direction;
}
