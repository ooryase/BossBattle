#include "BossSan.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"
#include"../../../System/DeviceManager.h"

BossSan::BossSan(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect()
{
	tag = ObjectTag::STEALTH;

	model = objectManager->GetModel("Effects/sphere");
	modelShader = objectManager->GetModelShader(L"shader");

	sprite = objectManager->GetSprite(L"san");
	spriteShader = objectManager->GetSpriteShader(L"weight");

	direction = _player->GetDirection();
	position = _player->GetPos();
	position.x = 0.0f;
	position.y = 35.0f;
	time = 0;
	fallTime = 2500;
	endTime = 3000;

	radius = 10.0f;

	dParam.damage = 10.0f;
	dParam.direction = DirectX::XMFLOAT3(0.0f,0.3f,0.0f);
}

void BossSan::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	if (fallTime < time)
	{
		if (time < endTime)
		{
			tag = ObjectTag::DAMAGE;
			radius = 5.0f + 15.0f *(time - fallTime) / static_cast<float>(endTime - fallTime);
		}
		else
			tag = ObjectTag::STEALTH;
	}
	else
		position.y -= 0.016f * Timer::GetInstance().GetDeltaTime();

	model->Update();

}

void  BossSan::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void BossSan::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);


}

void BossSan::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	float rate = (time - fallTime) / static_cast<float>(endTime - fallTime);


	float scale;
	if (time < fallTime)
		scale = 5.0f;
	else
		scale = rate * 15.0f + 5.0f;

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationZ(time * 0.004f);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	World *= Scale * Rotate * Offset;

	float alpha;
	if (time < fallTime)
		alpha = 1.0f;
	else
		alpha = 1.0f - rate * rate;

	DirectX::XMVECTOR Color;
	DirectX::XMVECTOR EdgeColor;
	DirectX::XMVECTOR Weight;
	if (tag == ObjectTag::DAMAGE)
	{
		Color = DirectX::XMVectorSet(1.0f, 0.1f, 0.1f, alpha);
		EdgeColor = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, alpha);
		Weight = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, alpha);
	}
	else
	{
		Color = DirectX::XMVectorSet(0.5f, 0.7f, 0.7f, alpha);
		EdgeColor = DirectX::XMVectorSet(0.5f, 0.0f, 0.0f, alpha);
		Weight = DirectX::XMVectorSet(0.6f, 1.0f, 1.0f, alpha);
	}

	Scale = DirectX::XMMatrixScaling(scale * 3.0f, scale * 3.0f, scale * 3.0f);
	World = Rotate * Scale * Offset;

	SPRITE::CONSTANT_BUFFER scb;
	DirectX::XMStoreFloat4x4(&scb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&scb.Weight, Weight);
	spriteShader->SetConstantBuffer(pDeviceContext, scb);

	sprite->DrawSet(pDeviceContext);
	spriteShader->DrawSet(pDeviceContext);
	pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z - 0.05f);
	Rotate = DirectX::XMMatrixRotationZ(-time * 0.004f);
	World = Rotate * Scale * Offset;
	DirectX::XMStoreFloat4x4(&scb.World, DirectX::XMMatrixTranspose(World));
	spriteShader->SetConstantBuffer(pDeviceContext, scb);
	pDeviceContext->Draw(sprite->GetIndexCount(), 0);



	Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	Rotate = DirectX::XMMatrixRotationZ(time * 0.004f);
	World = Rotate * Scale * Offset;

	// ƒpƒ‰ƒ[ƒ^‚ÌŽó‚¯“n‚µ
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);

	modelShader->SetConstantBuffer(pDeviceContext, cb);

	model->DrawSet(pDeviceContext);
	modelShader->DrawSet(pDeviceContext);

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ADD);
	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

	model->DrawLineAdjSet(pDeviceContext);
	modelShader->DrawLineAdjSet(pDeviceContext);

	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);


	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ALIGMENT);


}

void BossSan::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}