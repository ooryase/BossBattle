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

GunBreaker::GunBreaker(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light) : BaseCharacter(_light)
{
	model = objectManager->GetModel("gunbreaker");
	shader = objectManager->GetModelShader(L"shader");

	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotateDef = DirectX::XMFLOAT3(-DirectX::XM_PIDIV2, 0.0f, DirectX::XM_PI);
	
	behave = std::make_shared<GunWait>(param);

	param->gravity = 0.0025f;
}

GunBreaker::~GunBreaker()
{

}

void GunBreaker::Update()
{
	behave->Update(position, param, light);

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
		model->SetAnimSackNumber(static_cast<int>(behave->GetNextBehave()));
		switch (behave->GetNextBehave())
		{
		case GUN_BEHAVE::BehaveName::WAIT:
			behave = std::make_shared<GunWait>(param);
			break;
		case GUN_BEHAVE::BehaveName::DASH:
			behave = std::make_shared<GunDash>(param);
			break;
		case GUN_BEHAVE::BehaveName::RUN:
			behave = std::make_shared<GunRun>(param);
			break;
		case GUN_BEHAVE::BehaveName::JUMP:
			behave = std::make_shared<GunJump>(param);
			break;
		case GUN_BEHAVE::BehaveName::FALL:
			behave = std::make_shared<GunFall>(param);
			break;
		case GUN_BEHAVE::BehaveName::BREAD1:
			behave = std::make_shared<GunBread1>(param);
			break;
		case GUN_BEHAVE::BehaveName::BREAD2:
			behave = std::make_shared<GunBread2>(param);
			break;
		case GUN_BEHAVE::BehaveName::BREAD3:
			behave = std::make_shared<GunBread3>(param);
			break;
		case GUN_BEHAVE::BehaveName::GUN1:
			behave = std::make_shared<GunGun1>(param);
			break;
		case GUN_BEHAVE::BehaveName::GUN2:
			behave = std::make_shared<GunGun2>(param);
			break;
		case GUN_BEHAVE::BehaveName::GUN3:
			behave = std::make_shared<GunGun3>(param);
			break;
		default:
			break;
		}
	}

}

void GunBreaker::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// �p�����[�^�̌v�Z
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

	// �p�����[�^�̎󂯓n��
	MODEL::CONSTANT_BUFFER ccb;

	DirectX::XMStoreFloat4x4(&ccb.World, DirectX::XMMatrixTranspose(m_World));
	DirectX::XMStoreFloat4(&ccb.Light, 
		DirectX::XMVectorSet(
			light->playerLight.x,
			light->playerLight.y,
			light->playerLight.z,0.0f));
	DirectX::XMStoreFloat4(&ccb.Attenuation, 
		DirectX::XMVectorSet(
			light->playerAttenuation.x,
			light->playerAttenuation.y,
			light->playerAttenuation.z,
			light->playerAttenuation.w));

	shader->SetConstantBuffer(pDeviceContext, ccb);
}

void GunBreaker::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	model->DrawSet(pDeviceContext);
	shader->DrawSet(pDeviceContext);

	// �`����s
	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

	model->DrawLineAdjSet(pDeviceContext);
	shader->DrawLineAdjSet(pDeviceContext);

	// �`����s
	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);


}