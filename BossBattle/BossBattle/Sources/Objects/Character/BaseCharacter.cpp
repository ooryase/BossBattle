#include"BaseCharacter.h"

BaseCharacter::BaseCharacter(std::shared_ptr<Light> _light, 
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
	std::shared_ptr<ObjectManager> _objectManager) 
	: BaseObject(),
	effectReserves(&_effectReserves),
	objectManager(_objectManager)
{
	light = _light;
	hp = 100.0f;
	e = 1.0f;
	isDead = false;  // isDead
	isToDelete = false;  // isToDelete

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

void BaseCharacter::CollisionNormal(DirectX::XMFLOAT3 delta)
{
	position.x += delta.x * 0.4f * e;
	position.y += delta.y * 0.4f * e;

}
void BaseCharacter::CollisionDamage(DParam* dParam)
{
	hp -= dParam->damage;

	param->speed.x = dParam->direction.x * e;
	param->speed.y = dParam->direction.y * e;
	param->speed.z = dParam->direction.z * e;
}


//override‚³‚ê‚È‚¢ê‡ ‰½‚à‚µ‚È‚¢
void BaseCharacter::AttackHit(int type, int quantity)
{}

//override‚³‚ê‚È‚¢ê‡ ‰½‚à‚µ‚È‚¢
void BaseCharacter::SetObjectReserved(std::shared_ptr<BaseCharacter> _chara)
{
}

//override‚³‚ê‚È‚¢ê‡ ‰½‚à‚µ‚È‚¢
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
