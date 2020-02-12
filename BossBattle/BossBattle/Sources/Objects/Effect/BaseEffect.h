#include"../BaseObject.h"
#pragma once

class BaseEffect : public BaseObject
{
protected:

	std::vector<BaseObject*> collisionObj;

public:
	DParam dParam;

	BaseEffect();

	//void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

	virtual bool IsNewHit(BaseObject* obj);

};