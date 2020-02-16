#include "BossBeam.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

BossBeam::BossBeam(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect()
{
	tag = ObjectTag::STEALTH;

	model = objectManager->GetModel("Cube");
	modelShader = objectManager->GetModelShader(L"shader");

	direction = _player->GetDirection();
	position = _player->GetPos();
	position.x += (direction.z == DirectX::XM_PIDIV2) ? -5.0f : 5.0f;
	time = 0;
	chageTime = 1000;
	endTime = 3000;

	radius = 10.0f;

	dParam.damage = 10.0f;
	if (direction.z == DirectX::XM_PIDIV2)
		dParam.direction = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	else
		dParam.direction = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);

}

void BossBeam::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	if (chageTime < time)
	{
		if (time < endTime)
		{
			position.x += ((direction.z == DirectX::XM_PIDIV2) ? -1.0f : 1.0f);
			tag = ObjectTag::DAMAGE;
		}
		else
			tag = ObjectTag::STEALTH;
	}

	model->Update();
}

void  BossBeam::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void BossBeam::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	model->DrawSet(pDeviceContext);
	modelShader->DrawSet(pDeviceContext);


	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

}

void BossBeam::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	
	float scale;
	if (chageTime > time)
		scale = time / static_cast<float>(chageTime) * 5.0f;
	else
		scale = 5.0f;

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationZ(-time / 130.0f + DirectX::XM_PIDIV2);
	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationY(direction.z - DirectX::XM_PIDIV2);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	World *= Scale * Offset;

	MODEL::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	modelShader->SetConstantBuffer(pDeviceContext, cb);

}

void BossBeam::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}