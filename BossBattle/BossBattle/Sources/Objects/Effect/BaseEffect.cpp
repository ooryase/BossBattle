#include "BaseEffect.h"

BaseEffect::BaseEffect()
{
	dParam.damage = 0.0f;
	dParam.direction = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
}



bool BaseEffect::IsNewHit(BaseObject* obj)
{
	auto itr = std::find(collisionObj.begin(), collisionObj.end(), obj);
	if (itr != collisionObj.end()) {
		return false;
	}
	else {
		collisionObj.push_back(obj);
		return true;
	}
}