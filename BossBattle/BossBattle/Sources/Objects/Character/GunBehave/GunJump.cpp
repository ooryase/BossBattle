#include"GunJump.h"
#include"../../../System/InputController.h"

GunJump::GunJump(std::shared_ptr<Param> param) : GunBehave()
{
	param->speed.y = 0.2f;
	param->ground = false;
}

void GunJump::Update(std::shared_ptr<Param> param)
{
	//param->speed.y -= 0.0002f;

	if(param->ground)
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

}