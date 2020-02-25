#include"SpaceBossAvatar.h"

SpaceBossAvatar::SpaceBossAvatar(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
	std::vector< std::shared_ptr< BaseCharacter>>& _charaReserves,
	std::shared_ptr<Camera> _camera,
	DirectX::XMFLOAT2 _posBehabe, BehaveName _toBehave, int _startTime) :
	SpaceBoss(objectManager, _light, _effectReserves, _charaReserves, _camera)
{
	animNum = AnimNumber::WAIT;
	animSpeedDiv = 1;
	model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);

	toBehave = _toBehave;
	SetBehave(BehaveName::LAND);

	posBehave = _posBehabe;
	position.x = 1000.0f;
	behaveTime -= _startTime;
}

void SpaceBossAvatar::SetColor(DirectX::XMVECTOR* color, DirectX::XMVECTOR* edgeColor)
{
	*color = DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);

	*edgeColor = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
}


void SpaceBossAvatar::SetNextBehave()
{
	if (behave == BehaveName::LAND)
	{
		SetBehave(BehaveName::WAIT);
		waitTime = 500;
	}
	else if (behave == BehaveName::WAIT)
	{
		SetBehave(toBehave);
	}
	else if (behave == toBehave)
		SetBehave(BehaveName::LEAVE);
	else
		isToDelete = true;

}

