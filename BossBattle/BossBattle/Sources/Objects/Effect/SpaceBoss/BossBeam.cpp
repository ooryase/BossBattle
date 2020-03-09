#include "BossBeam.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

BossBeam::BossBeam(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect()
{
	tag = ObjectTag::STEALTH;

	model = objectManager->GetModel("Effects/spine");
	modelShader = objectManager->GetModelShader(L"shader");

	direction = _player->GetDirection();
	position = _player->GetPos();
	position.x += (direction.z == DirectX::XM_PIDIV2) ? -10.0f : 10.0f;
	position.y -= 5.0f;
	time = 0;
	chageTime = 2000;
	endTime = 3000;

	radius = 5.0f;

	dParam.damage = 10.0f;
	if (direction.z == DirectX::XM_PIDIV2)
		dParam.direction = DirectX::XMFLOAT3(-0.2f, 0.2f, 0.0f);
	else
		dParam.direction = DirectX::XMFLOAT3(0.2f, 0.2f, 0.0f);

}

void BossBeam::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	if (chageTime < time)
	{
		if (time < endTime)
		{
			position.x += ((direction.z == DirectX::XM_PIDIV2) ? -1.0f : 1.0f) * Timer::GetInstance().GetDeltaTime() * 0.2f;
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

	model->DrawLineAdjSet(pDeviceContext);
	modelShader->DrawLineAdjSet(pDeviceContext);

	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);
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
	DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationRollPitchYaw(time / 130.0f, time / 200.0f, time / 300.0f);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	World *= Scale * Rotate * Offset;

	DirectX::XMVECTOR Color = DirectX::XMVectorSet(0.4f, 0.0f, 0.2f, 1.0f);
	DirectX::XMVECTOR EdgeColor;
	if (tag == ObjectTag::DAMAGE)
		EdgeColor = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	else
		EdgeColor = DirectX::XMVectorSet(0.5f, 0.0f, 0.0f, 1.0f);

	// ƒpƒ‰ƒ[ƒ^‚ÌŽó‚¯“n‚µ
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);

	modelShader->SetConstantBuffer(pDeviceContext, cb);

}

void BossBeam::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}