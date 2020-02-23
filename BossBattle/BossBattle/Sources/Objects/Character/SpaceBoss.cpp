#include"SpaceBoss.h"
#include"../../System/InputController.h"
#include"../../System/Timer.h"
#include"../Effect/SpaceBoss/BossBeam.h"
#include"../Effect/SpaceBoss/BossSlash.h"
#include"../Effect/SpaceBoss/BossSan.h"

using DirectX::XMFLOAT3;

SpaceBoss::SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
	std::shared_ptr<Camera> _camera)
	: BaseCharacter(_light, _effectReserves, objectManager)
{
	model = objectManager->GetModel("spaceBoss");
	shader = objectManager->GetModelShader(L"enemy");

	gauge = objectManager->GetSprite(L"UI/Battle/Gauge/bossHpGauge");
	frame = objectManager->GetSprite(L"UI/Battle/Gauge/bossHpFrame");
	gaugeShader = objectManager->GetSpriteShader(L"weight");

	position = DirectX::XMFLOAT3(20.0f, 10.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XM_PI);
	rotateDef = DirectX::XMFLOAT3(2.36f, 1.58f, 1.96f);
	posBehave = DirectX::XMFLOAT2(1.0f, -1.0f);

	param = std::make_shared<Param>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0025f);
	//behave = std::make_shared<GunWait>(param);

	camera = _camera;

	e = 0.0f;
	radius = 8.0f;
	param->gravity = 0.0f;

	tag = ObjectTag::NORMAL;

	behave = BehaveName::AWAKE;
	animNum = AnimNumber::AWAKE;
	model->SetAnimSackNumber(animNum);
	behaveTime = 0;
	behaveStep = 0;
	animSpeedDiv = 1;

	maxHp = 10000.0f;
	hp = maxHp;
	damagedHp = maxHp;

	shaderGSLength = 0.0f;

}

SpaceBoss::~SpaceBoss()
{

}

void SpaceBoss::Update()
{
	position.x += param->speed.x;
	position.y += param->speed.y;
	position.z += param->speed.z;

	param->speed.y -= param->gravity;
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
		param->ground = true;
	}
	else
		param->ground = false;

	model->Update(animSpeedDiv);

	if (InputController::getInstance().IsPushKey(DIK_H))
	{
		behave = BehaveName::BEAM;
		behaveStep = 0;
		behaveTime = 0;
		animNum = AnimNumber::BEAM;
		model->SetAnimSackNumber(animNum);
		effectReserves->push_back(std::make_shared<BossBeam>(objectManager, shared_from_this()));

	}
	if (InputController::getInstance().IsPressKey(DIK_J))
	{
		behave = BehaveName::SLASH;
		behaveStep = 0;
		behaveTime = 0;
		animNum = AnimNumber::SLASH;
		model->SetAnimSackNumber(animNum);
		//effectReserves->push_back(std::make_shared<BossBeam>(objectManager, shared_from_this()));
	}
	if (InputController::getInstance().IsPushKey(DIK_Y))
	{
		//position.x = 25.0f;
		//param->direction.z = DirectX::XM_PIDIV2;
		//rotateDef.z = 1.96f;
		behave = BehaveName::SHIFT_RIGHT_UP;
		behaveStep = 0;
		behaveTime = 0;
	}
	if (InputController::getInstance().IsPushKey(DIK_U))
	{
		//position.x = -25.0f;
		//param->direction.z = -DirectX::XM_PIDIV2;
		//rotateDef.z = 1.33f;

		behave = BehaveName::SHIFT_LEFT_DOWN;
		behaveStep = 0;
		behaveTime = 0;

	}
	if (InputController::getInstance().IsPushKey(DIK_N))
	{
		behave = BehaveName::SAN;
		behaveStep = 0;
		behaveTime = 0;
		animNum = AnimNumber::ROAR2;
		model->SetAnimSackNumber(animNum);
	}
	if (InputController::getInstance().IsPressKey(DIK_M))
	{
		rotateDef.z -= 0.01f;

		//		param->direction.z += -0.01f;
	}
	if (InputController::getInstance().IsPressKey(DIK_K))
	{
		position.z += 0.02f;
	}

	behaveTime += Timer::GetInstance().GetDeltaTime();
	switch (behave)
	{
	case BehaveName::AWAKE:
		UpdateAwake();
		break;
	case BehaveName::BEAM:
		UpdateBeam();
		break;
	case BehaveName::SLASH:
		UpdateSlash();
		break;
	case BehaveName::SAN:
		UpdateSan();
		break;
	case BehaveName::SHIFT_RIGHT_UP:
	case BehaveName::SHIFT_RIGHT_DOWN:
	case BehaveName::SHIFT_LEFT_UP:
	case BehaveName::SHIFT_LEFT_DOWN:
		UpdateShift();
		break;
	default:
		break;
	}


	UpdateDamaged();
}

void SpaceBoss::UpdateAwake()
{
	if (behaveTime < 2000)
	{
		if (behaveStep == 0)
		{
			camera->SetCameraPos(Camera::State::LINER, XMFLOAT3(-10.0f, 6.0f, 0.0f), XMFLOAT3(20.0f, 40.0f, 0.0f), 0);
			camera->SetCameraPos(Camera::State::LINER, XMFLOAT3(-10.0f, 6.0f, 0.0f), XMFLOAT3(20.0f, 6.0f, 0.0f), 2000);
			animSpeedDiv = 0;
			behaveStep++;
		}
		position.y =46.0f - behaveTime / 50.0f;
	}
	else if (behaveTime < 3000)
	{
		position.y = 6.0f;
	}
	else if (behaveTime < 13000)
		animSpeedDiv = 3;
	else if (behaveTime < 20000)
	{
		if (behaveStep == 1)
		{
			animNum = AnimNumber::ROAR2;
			animSpeedDiv = 3;
			model->SetAnimSackNumber(animNum);
			behaveStep++;
			camera->SetCameraPos(Camera::State::LINER, XMFLOAT3(0.0f, 10.0f, -50.0f), XMFLOAT3(0.0f, 10.0f, 0.0f), 7000);
			//camera->SetCameraPos(Camera::State::CURVE, XMFLOAT3(0.0f, DirectX::XM_PIDIV2, 0.0f), XMFLOAT3(0.0f, 10.0f, 0.0f), 7000, 50.0f);
			behaveStep++;
		}
	}
	else
	{
		behave = BehaveName::WAIT;
		animNum = AnimNumber::WAIT;
		animSpeedDiv = 1;
		model->SetAnimSackNumber(animNum);
	}


}

void SpaceBoss::UpdateBeam()
{
	if (behaveTime > 2000)
	{
		behave = BehaveName::WAIT;
		animNum = AnimNumber::WAIT;
		animSpeedDiv = 1;
		model->SetAnimSackNumber(animNum);
	}
}

void SpaceBoss::UpdateSlash()
{
	if(behaveTime < 1400)
		animSpeedDiv = 2;
	else if (behaveStep == 0)
	{
		animSpeedDiv = 1;
		effectReserves->push_back(std::make_shared<BossSlash>(objectManager, shared_from_this()));
		behaveStep++;
	}

	if (behaveTime > 2000)
	{
		behave = BehaveName::WAIT;
		animNum = AnimNumber::WAIT;
		animSpeedDiv = 1;
		model->SetAnimSackNumber(animNum);
	}

}

void SpaceBoss::UpdateSan()
{
	if (behaveTime < 1400)
	{
	}
	else if (behaveStep == 0)
	{
		animSpeedDiv = 1;
		effectReserves->push_back(std::make_shared<BossSan>(objectManager, shared_from_this()));
		behaveStep++;
	}

	if (behaveTime > 2000)
	{
		behave = BehaveName::WAIT;
		animNum = AnimNumber::WAIT;
		animSpeedDiv = 1;
		model->SetAnimSackNumber(animNum);
	}

}

void SpaceBoss::UpdateShift()
{
	if (behaveStep == 0)
	{
		if (behaveTime < 1000)
		{
			shaderGSLength = behaveTime / 50.0f;
			position.x = (30.0f + 20.0f * behaveTime / 1000.0f) * posBehave.x;
		}
		else
		{
			switch (behave)
			{
			case SpaceBoss::BehaveName::SHIFT_RIGHT_UP:
				posBehave = DirectX::XMFLOAT2(1.0f, 1.0f);
				break;
			case SpaceBoss::BehaveName::SHIFT_RIGHT_DOWN:
				posBehave = DirectX::XMFLOAT2(1.0f, -1.0f);
				break;
			case SpaceBoss::BehaveName::SHIFT_LEFT_UP:
				posBehave = DirectX::XMFLOAT2(-1.0f, 1.0f);
				break;
			case SpaceBoss::BehaveName::SHIFT_LEFT_DOWN:
				posBehave = DirectX::XMFLOAT2(-1.0f, -1.0f);
				break;
			default:
				break;
			}
			position.y = 10.0f + 5.0f * posBehave.y;
			param->direction.z = DirectX::XM_PIDIV2 * posBehave.x;
			rotateDef.z = 1.645f + 0.315f * posBehave.x;
			behaveStep++;
			behaveTime = 0;
		}
	}

	if (behaveStep == 1)
	{
		if(behaveTime < 1000)
		{
			shaderGSLength = 20.0f - behaveTime / 50.0f;
			position.x = (50.0f - 20.0f * behaveTime / 1000.0f) * posBehave.x;
		}
		else
		{
			shaderGSLength = 0.0f;
			behave = BehaveName::WAIT;
			animNum = AnimNumber::WAIT;
			animSpeedDiv = 1;
			model->SetAnimSackNumber(animNum);
		}
	}
}

void SpaceBoss::EndUpdate()
{
	//if (behave->IsNextBehave())
	//{
	//	GUN_BEHAVE::BehaveName temp = behave->GetNextBehave();
	//
	//	model->SetAnimSackNumber(static_cast<int>(temp));
	//
	//	behave.reset();
	//}
}

void SpaceBoss::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y - 4.0f, position.z);
	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationRollPitchYaw(rotateDef.x, rotateDef.y, rotateDef.z);
	DirectX::XMMATRIX Rotate3 = DirectX::XMMatrixRotationRollPitchYaw(param->direction.x, param->direction.y, param->direction.z);
	DirectX::XMMATRIX Rotate2 = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	DirectX::XMMATRIX Rotate = XMMatrixTranspose(RotateDef * Rotate3 * Rotate2);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	World *= Scale * Rotate * Offset;

	DirectX::XMVECTOR Color;
	if(isDamaged)
		Color= DirectX::XMVectorSet(0.0f, -0.2f, -0.2f, 1.0f);
	else
		Color = DirectX::XMVectorSet(0.2f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR EdgeColor;
	if (tag == ObjectTag::NORMAL)
		EdgeColor = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	else
		EdgeColor = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);
	cb.Lenght.x = shaderGSLength;

	shader->SetConstantBuffer(pDeviceContext, cb);

}

void SpaceBoss::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	model->DrawSet(pDeviceContext);
	shader->DrawSet(pDeviceContext);

	// 描画実行
	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

	model->DrawLineAdjSet(pDeviceContext);
	shader->DrawLineAdjSet(pDeviceContext);

	// 描画実行
	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);

}

void SpaceBoss::DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext, DirectX::XMFLOAT3 _offset, DirectX::XMFLOAT3 _scale,
	DirectX::XMVECTOR _weight,
	std::shared_ptr<Sprite> _sprite)
{

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(_offset.x, _offset.y, _offset.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	DirectX::XMMATRIX World = Scale * Offset;

	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Weight, _weight);
	gaugeShader->SetConstantBuffer(pDeviceContext, cb);

	_sprite->DrawSet(pDeviceContext);
	pDeviceContext->Draw(_sprite->GetIndexCount(), 0);

}

void SpaceBoss::DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	gaugeShader->DrawSet(pDeviceContext);

	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(-20.0f, 25.0f, -2.0f),
		DirectX::XMFLOAT3(30.0f, 5.0f, 1.6f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		frame);

	gauge->Scroll(damagedHp / maxHp);
	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(-34.6f + 14.6f * damagedHp / maxHp, 26.0f, -2.1f),
		DirectX::XMFLOAT3(29.2f * damagedHp / maxHp, 2.5f, 1.6f),
		DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f),
		gauge);

	gauge->Scroll(hp / maxHp);
	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(-34.6f + 14.6f * hp / maxHp, 26.0f, -2.12f),
		DirectX::XMFLOAT3(29.2f * hp / maxHp, 2.5f, 1.6f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		gauge);
}