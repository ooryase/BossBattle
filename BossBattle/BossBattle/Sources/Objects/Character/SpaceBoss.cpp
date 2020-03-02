#include"SpaceBoss.h"
#include"../../System/InputController.h"
#include"../../System/DeviceManager.h"
#include"../../System/Timer.h"
#include"../Effect/SpaceBoss/BossBeam.h"
#include"../Effect/SpaceBoss/BossBeam2.h"
#include"../Effect/SpaceBoss/BossSlash.h"
#include"../Effect/SpaceBoss/BossSan.h"
#include"SpaceBossAvatar.h"

using DirectX::XMFLOAT3;

SpaceBoss::SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
	std::vector< std::shared_ptr< BaseCharacter>>& _enemyReserves,
	std::shared_ptr<Camera> _camera)
	: BaseCharacter(_light, _effectReserves, _enemyReserves, objectManager),
	scaleDefault(1.5f)
{
	model = objectManager->GetModel("spaceBoss");
	shader = objectManager->GetModelShader(L"enemy");

	gauge = objectManager->GetSprite(L"UI/Battle/Gauge/bossHpGauge");
	frame = objectManager->GetSprite(L"UI/Battle/Gauge/bossHpFrame");
	gaugeShader = objectManager->GetSpriteShader(L"weight");

	position = DirectX::XMFLOAT3(30.0f, 7.0f, 0.0f);
	scale = DirectX::XMFLOAT3(scaleDefault, scaleDefault, scaleDefault);
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
	model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
	behaveTime = 0;
	behaveStep = 0;
	animSpeedDiv = 1;

	maxHp = 10000.0f;
	hp = maxHp;
	damagedHp = maxHp;

	shaderGSLength = 0.0f;

	waitTime = 3000;
	phaseNum = 3;
	decisionBehave = DecisionBehavePhase::WAIT_NEXT;
}

SpaceBoss::~SpaceBoss()
{

}

void SpaceBoss::Update()
{
	position.x += param->speed.x;
	position.y += param->speed.y;
	position.z += param->speed.z;

	//model->Update(animSpeedDiv);
	behaveTime += Timer::GetInstance().GetDeltaTime();
	switch (behave)
	{
	case BehaveName::AWAKE:
		UpdateAwake();
		break;
	case BehaveName::BEAM:
	case BehaveName::BEAM2:
		UpdateBeam();
		break;
	case BehaveName::SLASH:
		UpdateSlash();
		break;
	case BehaveName::LEAVE:
		UpdateLeave();
		break;
	case BehaveName::LAND:
		UpdateLand();
		break;
	case BehaveName::PHASE2_ATTACK:
		UpdatePhase2Attack();
		break;
	case BehaveName::PHASE3_ATTACK:
		UpdatePhase3Attack();
		break;
	case BehaveName::PHASE3_ATTACK_STEP2:
		UpdatePhase3AttackStep2();
		break;
	case BehaveName::WAIT:
		UpdateWait();
		break;
	default:
		break;
	}


	UpdateDamaged();
}


void SpaceBoss::SetNextBehave()
{

	if (decisionBehave == DecisionBehavePhase::WAIT_NEXT)
	{
		moveCount = std::rand() % (2 + phaseNum);
		decisionBehave = DecisionBehavePhase::MOVE;
		behaveTime = 0;
		waitTime = 500;
	}
	else if (decisionBehave == DecisionBehavePhase::MOVE)
	{
		if (behave == BehaveName::WAIT)
		{
			if (moveCount == 0)
				DecisionAttackBehave();
			else
			{
				moveCount--;
				SetBehave(BehaveName::LEAVE);
			}
		}
		else if (behave == BehaveName::LEAVE)
		{
			SetBehave(BehaveName::LAND);
			posBehave = DirectX::XMFLOAT2(static_cast<float>(rand() % 2 * 2 - 1), static_cast<float>(rand() % 2 * 2 - 1));
		}
		else if(behave == BehaveName::LAND)
			SetBehave(BehaveName::WAIT);
	}
	else if (decisionBehave == DecisionBehavePhase::ATTACK)
	{
		if (behave == BehaveName::LEAVE)
		{
			posBehave = phase3SetPos;
			SetBehave(BehaveName::LAND);
		}
		else if (behave == BehaveName::LAND)
		{
			//if(phaseNum == 3)
			SetBehave(BehaveName::PHASE3_ATTACK_STEP2);
		}
		else
		{
			waitTime = 3000;
			SetBehave(BehaveName::WAIT);
			decisionBehave = DecisionBehavePhase::WAIT_NEXT;
		}
	}
}

void SpaceBoss::DecisionAttackBehave()
{
	decisionBehave = DecisionBehavePhase::ATTACK;

	if (hp / maxHp > 0.5f)
		phaseNum = 1;
	else if (hp / maxHp > 0.2f)
		phaseNum = 2;
	else
		phaseNum = 3;

	if (phaseNum == 1)
	{
		auto random = rand() % 3;
		if (random == 0)
			SetBehave(BehaveName::SLASH);
		else if(random == 1)
			SetBehave(BehaveName::BEAM);
		else
			SetBehave(BehaveName::BEAM2);

	}
	else if (phaseNum == 2)
	{
		auto random = rand() % 3;
		SetBehave(BehaveName::PHASE2_ATTACK);
		if(random == 0)
			charaReserves->push_back(std::make_shared<SpaceBossAvatar>(objectManager, light, *effectReserves, *charaReserves, camera,
				GetAvatarPos(), BehaveName::BEAM, 1000));
		else if(random == 1)
			charaReserves->push_back(std::make_shared<SpaceBossAvatar>(objectManager, light, *effectReserves, *charaReserves, camera,
				GetAvatarPos(), BehaveName::BEAM2, 1000));
		else
			effectReserves->push_back(std::make_shared<BossSan>(objectManager, shared_from_this(), 1000, (rand() % 3 - 1) * 20.0f));

	}
	else if (phaseNum == 3)
	{
		SetBehave(BehaveName::PHASE3_ATTACK);
		auto sanNum = rand() % 4;

		auto sanPos = rand() % 6;
		for (int i = 0; i < sanNum; i++)
		{
			auto sanPosX = (sanPos + (sanPos % 2 * 2 + 2) * i) % 3 - 1;
			effectReserves->push_back(std::make_shared<BossSan>(objectManager, shared_from_this(), 2500 + 500 * i, sanPosX * 20.0f));
		}

		std::vector<DirectX::XMFLOAT2> posVec;
		posVec.push_back(DirectX::XMFLOAT2(1.0f, 1.0f));
		posVec.push_back(DirectX::XMFLOAT2(-1.0f, 1.0f));
		posVec.push_back(DirectX::XMFLOAT2(1.0f, -1.0f));
		posVec.push_back(DirectX::XMFLOAT2(-1.0f, -1.0f));

		std::vector<BehaveName> behaveVec;
		behaveVec.push_back(BehaveName::BEAM);
		behaveVec.push_back(BehaveName::BEAM2);
		behaveVec.push_back(BehaveName::SLASH);

		auto randPos = rand() % 4;
		phase3SetPos = posVec.at(randPos);
		posVec.erase(posVec.begin() + randPos);

		auto randBehave = rand() % 3;
		phase3Attack = behaveVec.at(randBehave);
		behaveVec.erase(behaveVec.begin() + randBehave);

		for (int i = 0; i < 3 - sanNum; i++)
		{
			if (i == 2)
			{
				charaReserves->push_back(std::make_shared<SpaceBossAvatar>(objectManager, light, *effectReserves, *charaReserves, camera,
					posVec.at(0), BehaveName::BEAM2, 2500));
				break;
			}
		
			randPos = rand() % (3 - i);
			randBehave = rand() % (2 - i);
			charaReserves->push_back(std::make_shared<SpaceBossAvatar>(objectManager, light, *effectReserves, *charaReserves, camera,
				posVec.at(randPos), behaveVec.at(randBehave), 2500));
			posVec.erase(posVec.begin() + randPos);
			behaveVec.erase(behaveVec.begin() + randBehave);
		
		}
	}
}

DirectX::XMFLOAT2 SpaceBoss::GetAvatarPos()
{
	auto pos = DirectX::XMFLOAT2(static_cast<float>(rand() % 2 * 2 - 1), static_cast<float>(rand() % 2 * 2 - 1));
	while (true)
	{
		if (posBehave.x != pos.x || posBehave.y != pos.y)
			return pos;
		else
			pos.x *= -1.0f;

		if (posBehave.x != pos.x || posBehave.y != pos.y)
			return pos;
		else
			pos.y *= -1.0f;
	}
}


void SpaceBoss::SetBehave(BehaveName setBehave)
{
	behave = setBehave;
	behaveStep = 0;
	behaveTime = 0;
	animSpeedDiv = 1;

	switch (behave)
	{
	case SpaceBoss::BehaveName::AWAKE:
		animNum = AnimNumber::AWAKE;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
		break;
	case SpaceBoss::BehaveName::BEAM:
	case SpaceBoss::BehaveName::BEAM2:
		animNum = AnimNumber::BEAM;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
		break;
	case SpaceBoss::BehaveName::SLASH:
		animNum = AnimNumber::SLASH;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
		break;
	case SpaceBoss::BehaveName::PHASE2_ATTACK:
	case SpaceBoss::BehaveName::PHASE3_ATTACK:
		animNum = AnimNumber::ROAR2;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
		break;
	case SpaceBoss::BehaveName::WAIT:
	case SpaceBoss::BehaveName::LAND:
	case SpaceBoss::BehaveName::LEAVE:
	case SpaceBoss::BehaveName::PHASE3_ATTACK_STEP2:
		animNum = AnimNumber::WAIT;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
		break;
	default:
		break;
	}
}


void SpaceBoss::UpdateAwake()
{
	if (behaveTime < 2000)
	{
		if (behaveStep == 0)
		{
			camera->SetCameraPos(Camera::State::LINER, XMFLOAT3(-10.0f, 6.0f, 0.0f), XMFLOAT3(20.0f, 40.0f, 0.0f), 0);
			camera->SetCameraPos(Camera::State::LINER, XMFLOAT3(-10.0f, 6.0f, 0.0f), XMFLOAT3(20.0f, 6.0f, 0.0f), 2000);
			model->UpdateAnotherTimeCount(animSpeedDiv, modelAnimTimeCount);
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
	else if (behaveTime < 15000)
	{
		if (behaveStep == 1)
		{
			animNum = AnimNumber::ROAR2;
			animSpeedDiv = 3;
			model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
			behaveStep++;
		}
	}
	else if (behaveTime < 19000)
	{
		if (behaveStep == 2)
		{
			behaveStep++;
			camera->Quake();
			
			camera->SetCameraPos(Camera::State::SQUARE, XMFLOAT3(-45.0f, 14.0f, 0.0f), XMFLOAT3(20.0f, 6.0f, 0.0f), 4000);
			//camera->SetCameraPos(Camera::State::SQUARE, XMFLOAT3(0.0f, 10.0f, -50.0f), XMFLOAT3(0.0f, 10.0f, 0.0f), 4000);
			camera->Quake();
			DeviceManager::GetInstance().SetRadialBlur(DirectX::XMFLOAT2(0.5f, 0.5f), 4000, 1.0f);
		}
	}
	else
	{
		camera->SetCameraPos(Camera::State::SQUARE, XMFLOAT3(0.0f, 10.0f, -50.0f), XMFLOAT3(0.0f, 10.0f, 0.0f), 0);
		behave = BehaveName::WAIT;
		animNum = AnimNumber::WAIT;
		behaveTime = 0;
		animSpeedDiv = 1;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
	}


}

void SpaceBoss::UpdateBeam()
{
	if (behaveStep == 0)
	{
		if(behave == BehaveName::BEAM)
			effectReserves->push_back(std::make_shared<BossBeam>(objectManager, shared_from_this()));
		else
		{
			effectReserves->push_back(std::make_shared<BossBeam2>(objectManager, shared_from_this(), 1.0f));
			effectReserves->push_back(std::make_shared<BossBeam2>(objectManager, shared_from_this(), -1.0f));
		}
		behaveStep++;
	}
	
	if (behaveTime < 2000)
		animSpeedDiv = 2;
	else if(behaveTime < 2160)
		animSpeedDiv = 1;
	else
		animSpeedDiv = 4;

	if (behaveTime < 3000)
	{
		float quantity = 2.0f - ((behaveTime < 2000) ? behaveTime / 1000.0f : (3000 - behaveTime) / 500.0f);
		light->SetPointLight(
			DirectX::XMFLOAT4(position.x - 10.0f * sin(param->direction.z), position.y - 5.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT4(1.0f, 0.4f, 0.4f, 0.0f),
			DirectX::XMFLOAT4(quantity, quantity / 150.0f, quantity / 200.0f, 0.0f)
		);
	}

	if (behaveTime > 4500)
		SetNextBehave();
}

void SpaceBoss::UpdateSlash()
{
	if(behaveTime < 2100)
		animSpeedDiv = 3;
	else if (behaveStep == 0)
	{
		animSpeedDiv = 1;
		effectReserves->push_back(std::make_shared<BossSlash>(objectManager, shared_from_this()));
		behaveStep++;
	}
	else if (behaveTime > 3500)
		animSpeedDiv = 4;
	else if(behaveTime > 2500)
		animSpeedDiv = 3;


	if (behaveTime > 4500)
		SetNextBehave();
}

void SpaceBoss::UpdateLeave()
{
	if (behaveTime < 500)
	{
		const float SCALE = scaleDefault * (1.0f - behaveTime / 500.0f);
		scale = DirectX::XMFLOAT3(SCALE, SCALE, SCALE);
		shaderGSLength = behaveTime / 12.5f;
		position.x = (30.0f + 10.0f * behaveTime / 500.0f) * posBehave.x;
	}
	else
		SetNextBehave();
}

void SpaceBoss::UpdateLand()
{
	if (behaveTime > 0 && behaveStep == 0)
	{
		position.y = 14.5f + 7.5f * posBehave.y;
		param->direction.z = DirectX::XM_PIDIV2 * posBehave.x;
		rotateDef.z = 1.645f + 0.315f * posBehave.x;
		behaveStep++;
	}

	if (behaveStep == 1)
	{
		if ( behaveTime < 1000)
		{
			const float SCALE = scaleDefault * behaveTime / 1000.0f;
			scale = DirectX::XMFLOAT3(SCALE, SCALE, SCALE);
			shaderGSLength = 40.0f - behaveTime / 25.0f;
			position.x = (40.0f - 10.0f * behaveTime / 1000.0f) * posBehave.x;
		}
		else
		{
			position.x = 30.0f * posBehave.x;
			scale = DirectX::XMFLOAT3(scaleDefault, scaleDefault, scaleDefault);
			shaderGSLength = 0.0f;
			SetNextBehave();
		}
	}
}

void SpaceBoss::UpdatePhase2Attack()
{
	if (behaveTime > 2000 && behaveStep == 0)
	{
		animNum = AnimNumber::WAIT;
		model->SetAnimSackNumberAnotherTimeCount(animNum, modelAnimTimeCount);
		behaveStep++;
	}
	else if (behaveTime > 2500)
	{
		auto random = rand() % 3;
		if(random == 0)
			SetBehave(BehaveName::SLASH);
		else if (random == 1)
			SetBehave(BehaveName::BEAM);
		else
			SetBehave(BehaveName::BEAM2);
	}
}

void SpaceBoss::UpdatePhase3Attack()
{
	if (behaveTime > 2000)
	{
		SetBehave(BehaveName::LEAVE);
	}
}

void SpaceBoss::UpdatePhase3AttackStep2()
{
	if (behaveTime > 500)
	{
		SetBehave(phase3Attack);
	}
}



void SpaceBoss::UpdateWait()
{
	if (behaveTime > waitTime)
		SetNextBehave();
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
	DirectX::XMVECTOR EdgeColor;
	SetColor(&Color, &EdgeColor);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);
	cb.Lenght.x = shaderGSLength;

	shader->SetConstantBuffer(pDeviceContext, cb);

}

void SpaceBoss::SetColor(DirectX::XMVECTOR* color, DirectX::XMVECTOR* edgeColor)
{
	if (isDamaged)
		*color = DirectX::XMVectorSet(0.2f, -0.2f, -0.2f, 1.0f);
	else
		*color = DirectX::XMVectorSet(0.2f, 0.0f, 0.0f, 1.0f);

	if (tag == ObjectTag::NORMAL)
		*edgeColor = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	else
		*edgeColor = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
}

void SpaceBoss::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	model->SetAnimSackNumber(animNum);
	model->UpdateAnotherTimeCount(animSpeedDiv, modelAnimTimeCount);

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