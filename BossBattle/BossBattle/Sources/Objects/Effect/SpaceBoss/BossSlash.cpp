#include "BossSlash.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

BossSlash::BossSlash(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect(),
	useChara(_player)

{
	tag = ObjectTag::STEALTH;

	sprite = objectManager->GetSprite(L"slash");
	spriteShader = objectManager->GetSpriteShader(L"shader");

	position = useChara->GetPos();
	direction = (useChara->GetDirection().z == DirectX::XM_PIDIV2) ? -1.0f : 1.0f;

	position.x += 20.0f * direction;
	time = 0;
	endTime = 500;

	radius = 20.0f;

	dParam.damage = 100.0f;
	dParam.direction = DirectX::XMFLOAT3(0.2f * direction, 0.2f, 0.0f);

}

void BossSlash::Update()
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

void  BossSlash::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void BossSlash::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	sprite->DrawSet(pDeviceContext);
	spriteShader->DrawSet(pDeviceContext);

	//pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	pDeviceContext->Draw(sprite->GetIndexCount(), 0);

}

void BossSlash::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	float rate = time / static_cast<float>(endTime);
	float scale = abs((rate * (1 - rate)) * 60.0f);

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX Trans = DirectX::XMMatrixTranslation(direction * 15.0f, 0.0f, -2.0f);
	DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationZ(direction * (rate * 1.5f - 1.2f) * DirectX::XM_PIDIV2);

	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationY((direction - 1.0f) * DirectX::XM_PIDIV2);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	DirectX::XMMATRIX World = Scale * RotateDef * Trans * Rotate * Offset;

	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	spriteShader->SetConstantBuffer(pDeviceContext, cb);

}

void BossSlash::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}

bool BossSlash::IsNewHit(BaseObject* obj)
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