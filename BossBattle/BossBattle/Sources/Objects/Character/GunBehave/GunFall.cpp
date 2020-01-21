#include"GunFall.h"

GunFall::GunFall(std::shared_ptr<Param> param) : GunBehave()
{
}

void GunFall::Update(std::shared_ptr<Param> param)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (param->ground)
		nextBehave = GUN_BEHAVE::BehaveName::WAIT;

	if (InputController::getInstance().IsPressKey(DIK_LEFT))
	{
		param->speed.x += -0.001f;
		param->direction.z = -DirectX::XM_PIDIV2;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT))
	{
		param->speed.x += 0.001f;
		param->direction.z = DirectX::XM_PIDIV2;
	}

	ChackAttack();

}