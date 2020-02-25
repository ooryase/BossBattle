#include "BossBeam2.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

BossBeam2::BossBeam2(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player, float _directionY)
	: BaseEffect()
{
	tag = ObjectTag::STEALTH;

	model = objectManager->GetModel("Effects/spine");
	modelShader = objectManager->GetModelShader(L"shader");

	direction = _player->GetDirection();
	basePos = _player->GetPos();
	basePos.x += (direction.z == DirectX::XM_PIDIV2) ? -10.0f : 10.0f;
	basePos.y -= 5.0f;
	position.x = basePos.x;
	time = 0;
	chageTime = 2000;
	endTime = 3000;

	radius = 2.5f;

	dParam.damage = 10.0f;
	if (direction.z == DirectX::XM_PIDIV2)
		dParam.direction = DirectX::XMFLOAT3(-0.2f, 0.2f, 0.0f);
	else
		dParam.direction = DirectX::XMFLOAT3(0.2f, 0.2f, 0.0f);

	directionY = _directionY;
}

void BossBeam2::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	if (chageTime < time)
	{
		if (time < endTime)
		{
			position.x += ((direction.z == DirectX::XM_PIDIV2) ? -1.0f : 1.0f);
			position.y += 0.3f * directionY;
			tag = ObjectTag::DAMAGE;
		}
		else
			tag = ObjectTag::STEALTH;
	}
	else
	{
		chageLenght = time * 10.0f / chageTime;
		position.y = basePos.y + sinf(time / 100.0f + directionY * DirectX::XM_PIDIV2) * chageLenght;
		position.z = basePos.z + cosf(time / 100.0f + directionY * DirectX::XM_PIDIV2) * chageLenght;
	}

	model->Update();
}

void  BossBeam2::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void BossBeam2::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	model->DrawSet(pDeviceContext);
	modelShader->DrawSet(pDeviceContext);

	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

	model->DrawLineAdjSet(pDeviceContext);
	modelShader->DrawLineAdjSet(pDeviceContext);

	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);
}

void BossBeam2::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{

	float scale;
	if (chageTime > time)
		scale = time / static_cast<float>(chageTime) * 2.5f;
	else
		scale = 2.5f;

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

void BossBeam2::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}