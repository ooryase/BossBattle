#include "EffectBread2.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

EffectBread2::EffectBread2(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player, int type)
	: BaseEffect(),
	useChara(_player)

{
	tag = ObjectTag::STEALTH;
	breadType = type;

	sprite = objectManager->GetSprite(L"slash");
	spriteShader = objectManager->GetSpriteShader(L"shader");

	position = useChara->GetPos();
	direction = (useChara->GetDirection().z == DirectX::XM_PIDIV2) ? 1.0f : -1.0f;

	position.x += 5.0f * direction;
	time = 0;
	endTime = 500;

	radius = 10.0f;

	dParam.damage = 110.0f;
	dParam.direction = DirectX::XMFLOAT3(0.2f * direction, 0.2f, 0.0f);

}

void EffectBread2::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	position = useChara->GetPos();
	//position.y += sin(-time / 130.0f + DirectX::XM_PIDIV2) * 5.0f;
	//position.x += cos(-time / 130.0f + DirectX::XM_PIDIV2) *
		//((direction.z == DirectX::XM_PIDIV2) ? 7.0f : -7.0f);

	if (100 < time)
	{
		if (time < endTime - 50)
			tag = ObjectTag::DAMAGE;
		else
			tag = ObjectTag::STEALTH;
	}

}

void  EffectBread2::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void EffectBread2::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	sprite->DrawSet(pDeviceContext);
	spriteShader->DrawSet(pDeviceContext);

	//pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	pDeviceContext->Draw(sprite->GetIndexCount(), 0);

}

void EffectBread2::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	float rate = time / static_cast<float>(endTime);
	float scale = abs((rate * (1 - rate)) * 40.0f);

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX Trans = DirectX::XMMatrixTranslation(direction * 5.0f, 0.0f, -2.0f);
	DirectX::XMMATRIX Rotate;
	if (breadType == 2)
		Rotate = DirectX::XMMatrixRotationZ(direction * (rate - 1.2f) * DirectX::XM_PIDIV2);
	else if (breadType == 3)
		Rotate = DirectX::XMMatrixRotationZ(direction * (0.8f - rate) * 3.0f * DirectX::XM_PIDIV2);
	else if (breadType == 4)
		Rotate = DirectX::XMMatrixRotationZ(direction * (rate - 1.0f) * 1.5f * DirectX::XM_PIDIV2);
	else if (breadType == 5)
		Rotate = DirectX::XMMatrixRotationZ(direction * (0.8f - rate) * 3.0f * DirectX::XM_PIDIV2);
	else
		Rotate = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationY((direction - 1.0f) * DirectX::XM_PIDIV2);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	DirectX::XMMATRIX World = Scale * RotateDef * Trans * Rotate * Offset;

	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	spriteShader->SetConstantBuffer(pDeviceContext, cb);

}

void EffectBread2::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}

bool EffectBread2::IsNewHit(BaseObject* obj)
{
	auto itr = std::find(collisionObj.begin(), collisionObj.end(), obj);
	if (itr != collisionObj.end()) {
		return false;
	}
	else {
		collisionObj.push_back(obj);
		const int quantitiy = 10;
		useChara->AttackHit(1, quantitiy);
		return true;
	}
}