#include"GunBreaker.h"
#include"../../System/InputController.h"

#include"GunBehave/GunWait.h"
#include"GunBehave/GunDash.h"
#include"GunBehave/GunRun.h"
#include"GunBehave/GunJump.h"
#include"GunBehave/GunFall.h"
#include"GunBehave/GunBread1.h"
#include"GunBehave/GunBread2.h"
#include"GunBehave/GunBread3.h"
#include"GunBehave/GunGun1.h"
#include"GunBehave/GunGun2.h"
#include"GunBehave/GunGun3.h"
#include"GunBehave/ShiftBread1.h"
#include"GunBehave/ShiftBread2.h"
#include"GunBehave/ShiftGun1.h"
#include"GunBehave/ShiftGun2.h"

GunBreaker::GunBreaker(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _playerEffectReserves) 
	: BaseCharacter(_light, _playerEffectReserves, objectManager)
{
	typeGauge[BREAD] = 0;
	typeGauge[GUN] = 0;

	model = objectManager->GetModel("gunbreaker");
	shader = objectManager->GetModelShader(L"shader");

	gauge = objectManager->GetSprite(L"Tex");
	gaugeShader = objectManager->GetSpriteShader(L"shader");

	position = DirectX::XMFLOAT3(-20.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotateDef = DirectX::XMFLOAT3(-DirectX::XM_PIDIV2, 0.0f, DirectX::XM_PI);
	
	param = std::make_shared<Param>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0025f);

	e = 1.0f;
	radius = 2.0f;

	tag = ObjectTag::NORMAL;
}

GunBreaker::~GunBreaker()
{

}

void GunBreaker::Update()
{
	if(behave == nullptr)
		behave = std::make_shared<GunWait>(param, shared_from_this());

	behave->Update(position, light);

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

	model->Update();
}

void GunBreaker::EndUpdate()
{
	if (behave->IsNextBehave())
	{
		GUN_BEHAVE::BehaveName temp = behave->GetNextBehave();

		model->SetAnimSackNumber(static_cast<int>(temp));

		behave.reset();
		switch (temp)
		{
		case GUN_BEHAVE::BehaveName::WAIT:
			behave = std::make_shared<GunWait>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::DASH:
			behave = std::make_shared<GunDash>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::RUN:
			behave = std::make_shared<GunRun>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::JUMP:
			behave = std::make_shared<GunJump>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::FALL:
			behave = std::make_shared<GunFall>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::BREAD1:
			behave = std::make_shared<GunBread1>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::BREAD2:
			behave = std::make_shared<GunBread2>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::BREAD3:
			behave = std::make_shared<GunBread3>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::GUN1:
			behave = std::make_shared<GunGun1>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::GUN2:
			behave = std::make_shared<GunGun2>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::GUN3:
			behave = std::make_shared<GunGun3>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::SHIFT_BREAD1:
			behave = std::make_shared<ShiftBread1>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::SHIFT_BREAD2:
			behave = std::make_shared<ShiftBread2>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::SHIFT_GUN1:
			behave = std::make_shared<ShiftGun1>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::SHIFT_GUN2:
			behave = std::make_shared<ShiftGun2>(param, shared_from_this());
			break;
		default:
			break;
		}
	}

}

void GunBreaker::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX m_World = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(position.x,position.y,position.z);
	DirectX::XMMATRIX m_RotateDef = DirectX::XMMatrixRotationRollPitchYaw(rotateDef.x, rotateDef.y, rotateDef.z);
	DirectX::XMMATRIX m_Rotate3 = DirectX::XMMatrixRotationRollPitchYaw(param->direction.x, param->direction.y, param->direction.z);
	DirectX::XMMATRIX m_Rotate2 = DirectX::XMMatrixRotationRollPitchYaw(rotate.x,rotate.y,rotate.z);
	DirectX::XMMATRIX m_Rotate = XMMatrixTranspose(m_RotateDef * m_Rotate3 * m_Rotate2);
	DirectX::XMMATRIX m_Scale = DirectX::XMMatrixScaling(scale.x,scale.y, scale.z);

	m_World *= m_Scale * m_Rotate * m_Offset;

	//DirectX::XMVECTOR m_Light = DirectX::XMVectorSet(std::cos(4.0f) * 40.1f, -20.0f, std::sin(4.0f) * 40.1f, 0.0f);
	//DirectX::XMVECTOR m_Attenuation = DirectX::XMVectorSet(1.0f, 0.0005f, 0.0005f, 0.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER ccb;

	DirectX::XMStoreFloat4x4(&ccb.World, DirectX::XMMatrixTranspose(m_World));

	shader->SetConstantBuffer(pDeviceContext, ccb);

}

void GunBreaker::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
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

void GunBreaker::DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DirectX::XMMATRIX m_World = DirectX::XMMatrixTranslation(-10.0f, 10.0f, -2.0f);
	m_World *= DirectX::XMMatrixScaling(1.6f, 1.6f, 1.6f);

	SPRITE::CONSTANT_BUFFER ccb2;
	DirectX::XMStoreFloat4x4(&ccb2.World, DirectX::XMMatrixTranspose(m_World));
	gaugeShader->SetConstantBuffer(pDeviceContext, ccb2);

	//DirectX::XMMATRIX World = DirectX::XMMatrixRotationY(x += 0.001f);
//DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f);
//World *= Offset;
}

void GunBreaker::DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSetGauge(pDeviceContext);

	gauge->DrawSet(pDeviceContext);
	gaugeShader->DrawSet(pDeviceContext);

	pDeviceContext->Draw(gauge->GetIndexCount(), 0);
}

void GunBreaker::CollisionWallUpdate(float wall)
{
	if (position.x < -wall + radius)
		position.x = -wall + radius;
	else if(position.x > wall - radius)
		position.x = wall - radius;
}

void GunBreaker::AttackHit(int type, int quantity)
{
	typeGauge[type] += quantity;
}

void GunBreaker::SetEffectReserved(std::shared_ptr<BaseEffect> _obj)
{
	effectReserves->push_back(_obj);
}