#include"SpaceBoss.h"
#include"../../System/InputController.h"
#include"../../System/Timer.h"
#include"../Effect/SpaceBoss/BossBeam.h"

using DirectX::XMFLOAT3;

SpaceBoss::SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
	std::shared_ptr<Camera> _camera)
	: BaseCharacter(_light, _effectReserves, objectManager)
{
	model = objectManager->GetModel("spaceBoss");
	shader = objectManager->GetModelShader(L"shader");

	gauge = objectManager->GetSprite(L"UI/Battle/Gauge/bossHpGauge");
	frame = objectManager->GetSprite(L"UI/Battle/Gauge/bossHpFrame");
	gaugeShader = objectManager->GetSpriteShader(L"shader");

	position = DirectX::XMFLOAT3(20.0f, 10.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XM_PI);
	rotateDef = DirectX::XMFLOAT3(2.36f, 1.58f, 1.96f);

	param = std::make_shared<Param>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0025f);
	//behave = std::make_shared<GunWait>(param);

	camera = _camera;

	e = 0.0f;
	radius = 8.0f;
	param->gravity = 0.0f;

	tag = ObjectTag::NORMAL;

	behave = BehabeName::AWAKE;
	animNum = AnimNumber::AWAKE;
	model->SetAnimSackNumber(animNum);
	behaveTime = 0;
	behaveStep = 0;
	animSpeedDiv = 1;

	maxHp = 10000.0f;
	hp = maxHp;
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
		behave = BehabeName::BEAM;
		behaveStep = 0;
		behaveTime = 0;
		animNum = AnimNumber::BEAM;
		model->SetAnimSackNumber(animNum);
		effectReserves->push_back(std::make_shared<BossBeam>(objectManager, shared_from_this()));

	}
	if (InputController::getInstance().IsPressKey(DIK_J))
	{
		rotateDef.x -= 0.01f;
		//		param->direction.x += -0.01f; // DirectX::XM_PIDIV2;
	}
	if (InputController::getInstance().IsPressKey(DIK_Y))
	{
		rotateDef.y += 0.01f;
		//position.x += 0.03f;
		//param->direction.z = DirectX::XM_PIDIV2;
	}
	if (InputController::getInstance().IsPressKey(DIK_U))
	{
		rotateDef.y -= 0.01f;
		//position.x -= 0.03f;
		//param->direction.z = -DirectX::XM_PIDIV2;
	}
	if (InputController::getInstance().IsPressKey(DIK_N))
	{
		rotateDef.z += 0.01f;

		//		param->direction.z += 0.01f;
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
	case BehabeName::AWAKE:
		UpdateAwake();
		break;
	case BehabeName::BEAM:
		UpdateBeam();
		break;
	default:
		break;
	}

	gauge->Scroll(hp / maxHp);

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
		behave = BehabeName::WAIT;
		animNum = AnimNumber::WAIT;
		animSpeedDiv = 1;
		model->SetAnimSackNumber(animNum);
	}


}

void SpaceBoss::UpdateBeam()
{
	if (behaveTime > 2000)
	{
		behave = BehabeName::WAIT;
		animNum = AnimNumber::WAIT;
		animSpeedDiv = 1;
		model->SetAnimSackNumber(animNum);
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

	DirectX::XMVECTOR Color = DirectX::XMVectorSet(0.2f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR EdgeColor;
	if (tag == ObjectTag::NORMAL)
		EdgeColor = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	else
		EdgeColor = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);

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

void SpaceBoss::DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(-20.0f, 25.0f, -2.0f);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(30.0f, 5.0f, 1.6f);
	DirectX::XMMATRIX World = Scale * Offset;

	SPRITE::CONSTANT_BUFFER ccb2;
	DirectX::XMStoreFloat4x4(&ccb2.World, DirectX::XMMatrixTranspose(World));
	gaugeShader->SetConstantBuffer(pDeviceContext, ccb2);

}

void SpaceBoss::DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSetGauge(pDeviceContext);

	gaugeShader->DrawSet(pDeviceContext);


	frame->DrawSet(pDeviceContext);
	pDeviceContext->Draw(frame->GetIndexCount(), 0);

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(-34.6f + 14.6f * hp / maxHp, 26.0f, -2.1f);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(29.2f * hp / maxHp, 2.5f, 1.6f);
	DirectX::XMMATRIX World = Scale * Offset;

	SPRITE::CONSTANT_BUFFER ccb2;
	DirectX::XMStoreFloat4x4(&ccb2.World, DirectX::XMMatrixTranspose(World));
	gaugeShader->SetConstantBuffer(pDeviceContext, ccb2);

	gauge->DrawSet(pDeviceContext);

	//pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	pDeviceContext->Draw(gauge->GetIndexCount(), 0);
}