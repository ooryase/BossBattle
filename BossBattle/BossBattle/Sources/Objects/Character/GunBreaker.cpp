#include"GunBreaker.h"
#include"../../System/InputController.h"

#include"GunBehave/GunWait.h"
#include"GunBehave/GunDash.h"
#include"GunBehave/GunRun.h"
#include"GunBehave/GunJump.h"
#include"GunBehave/GunFall.h"
#include"GunBehave/GunDameged.h"
#include"GunBehave/GunRecover.h"
#include"GunBehave/GunStep.h"
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
#include"GunBehave/GunSpecial.h"

GunBreaker::GunBreaker(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _playerEffectReserves,
	std::vector< std::shared_ptr< BaseCharacter>>& _charaReserves)
	: BaseCharacter(_light, _playerEffectReserves, _charaReserves, objectManager)
{
	model = objectManager->GetModel("gunbreaker");
	shader = objectManager->GetModelShader(L"shader");

	hpGauge = objectManager->GetSprite(L"UI/Battle/Gauge/hpGauge");
	hpFrame = objectManager->GetSprite(L"UI/Battle/Gauge/hpFrame");
	GBFrame = objectManager->GetSprite(L"UI/Battle/Gauge/GunBreFrame");
	gunGauge = objectManager->GetSprite(L"UI/Battle/Gauge/GunGauge");
	breadGauge = objectManager->GetSprite(L"UI/Battle/Gauge/BreadGauge");
	gaugeShader = objectManager->GetSpriteShader(L"weight");

	position = DirectX::XMFLOAT3(-20.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(0.95f, 0.95f, 0.95f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotateDef = DirectX::XMFLOAT3(-DirectX::XM_PIDIV2, 0.0f, DirectX::XM_PI);
	
	param = std::make_shared<Param>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0025f);

	e = 1.0f;
	radius = 3.0f;

	tag = ObjectTag::NORMAL;

	maxHp = 100.0f;
	hp = maxHp;
	damagedHp = maxHp;


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
	if (position.y < -2.0f)
	{
		position.y = -2.0f;
		param->ground = true;
	}
	else
		param->ground = false;

	model->Update();

	UpdateDamaged();
}

void GunBreaker::EndUpdate()
{
	if (behave->NextBehave != GUN_BEHAVE::BehaveName::NONE)
	{
		GUN_BEHAVE::BehaveName temp = behave->NextBehave;

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
		case GUN_BEHAVE::BehaveName::DAMEGED:
			behave = std::make_shared<GunDameged>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::RECOVER:
			behave = std::make_shared<GunRecover>(param, shared_from_this());
			break;
		case GUN_BEHAVE::BehaveName::STEP:
			behave = std::make_shared<GunStep>(param, shared_from_this());
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
		case GUN_BEHAVE::BehaveName::SPECIAL:
			behave = std::make_shared<GunSpecial>(param, shared_from_this());
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

	DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(position.x,position.y + 4.0f,position.z);
	DirectX::XMMATRIX m_RotateDef = DirectX::XMMatrixRotationRollPitchYaw(rotateDef.x, rotateDef.y, rotateDef.z);
	DirectX::XMMATRIX m_Rotate3 = DirectX::XMMatrixRotationRollPitchYaw(param->direction.x, param->direction.y, param->direction.z);
	DirectX::XMMATRIX m_Rotate2 = DirectX::XMMatrixRotationRollPitchYaw(rotate.x,rotate.y,rotate.z);
	DirectX::XMMATRIX m_Rotate = XMMatrixTranspose(m_RotateDef * m_Rotate3 * m_Rotate2);
	DirectX::XMMATRIX m_Scale = DirectX::XMMatrixScaling(scale.x,scale.y, scale.z);

	m_World *= m_Scale * m_Rotate * m_Offset;

	DirectX::XMVECTOR Color;
	if (isDamaged)
		Color = DirectX::XMVectorSet(0.0f, -0.2f, 0.0f, 1.0f);
	else
		Color = DirectX::XMVectorSet(0.0f, 0.0f, 0.2f, 1.0f);
	DirectX::XMVECTOR EdgeColor;
	if(tag == ObjectTag::NORMAL)
		EdgeColor = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	else
		EdgeColor = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(m_World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);

	shader->SetConstantBuffer(pDeviceContext, cb);

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

void GunBreaker::DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext, DirectX::XMFLOAT3 _offset, DirectX::XMFLOAT3 _scale,
	DirectX::XMVECTOR _weight,
	std::shared_ptr<Sprite> _sprite)
{
	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(_offset.x, _offset.y, _offset.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(Scale * Offset));
	DirectX::XMStoreFloat4(&cb.Weight, _weight);
	gaugeShader->SetConstantBuffer(pDeviceContext, cb);

	_sprite->DrawSet(pDeviceContext);
	pDeviceContext->Draw(_sprite->GetIndexCount(), 0);
}

void GunBreaker::DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	gaugeShader->DrawSet(pDeviceContext);


	DrawSetGauge(pDeviceContext, 
		DirectX::XMFLOAT3(20.0f, -5.0f, -10.0f),
		DirectX::XMFLOAT3(14.0f, 2.6f, 1.6f),
		DirectX::XMVectorSet(1.0f,1.0f,1.0f,1.0f),
		hpFrame);

	hpGauge->Scroll(damagedHp / maxHp);
	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(26.8f - 6.8f * damagedHp / maxHp, -4.6f, -10.1f),
		DirectX::XMFLOAT3(13.6f * damagedHp / maxHp, 1.4f, 1.6f),
		DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f),
		hpGauge);

	hpGauge->Scroll(hp / maxHp);
	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(26.8f - 6.8f * hp / maxHp, -4.6f, -10.2f),
		DirectX::XMFLOAT3(13.6f * hp / maxHp, 1.4f, 1.6f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		hpGauge);

	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(20.0f, -2.2f, -10.0f),
		DirectX::XMFLOAT3(15.0f, 3.2f, 1.6f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		GBFrame);

	const int GUN = 0;
	const int BREAD = 1;

	breadGauge->Scroll(param->typeGauge[BREAD] / 100.0f);
	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(27.2f - 7.2f * param->typeGauge[BREAD] / 100.0f, -1.7f, -10.1f),
		DirectX::XMFLOAT3(14.4f * param->typeGauge[BREAD] / 100.0f, 0.8f, 1.6f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		breadGauge);

	gunGauge->Scroll(param->typeGauge[GUN] / 100.0f);
	DrawSetGauge(pDeviceContext,
		DirectX::XMFLOAT3(28.0f - 5.0f * param->typeGauge[GUN] / 100.0f, -2.6f, -10.1f),
		DirectX::XMFLOAT3(10.0f * param->typeGauge[GUN] / 100.0f, 0.8f, 1.6f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		gunGauge);
}

void GunBreaker::CollisionWallUpdate(float wall)
{
	if (position.x < -wall + radius)
		position.x = -wall + radius;
	else if(position.x > wall - radius)
		position.x = wall - radius;
}

void GunBreaker::CollisionDamage(DParam* dParam)
{
	behave->NextBehave = GUN_BEHAVE::BehaveName::DAMEGED;

	damagedTime = 0;
	isDamaged = true;
	hp -= dParam->damage;
	if (hp < 0.0f)
		hp = 0.0f;

	param->speed.x = dParam->direction.x * e;
	param->speed.y = dParam->direction.y * e;
	param->speed.z = dParam->direction.z * e;
}

void GunBreaker::AttackHit(int type, int quantity)
{
	param->typeGauge[type] += quantity;
	param->typeGauge[type] = (param->typeGauge[type] > 100) ? 100 : param->typeGauge[type];
}

void GunBreaker::SetEffectReserved(std::shared_ptr<BaseEffect> _obj)
{
	effectReserves->push_back(_obj);
}