#include "EffectBread1.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

EffectBread1::EffectBread1(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect(),
	useChara(_player)

{
	tag = ObjectTag::STEALTH;

	sprite = objectManager->GetSprite(L"slash");
	spriteShader = objectManager->GetSpriteShader(L"shader");

	direction = useChara->GetDirection();
	position = useChara->GetPos();
	position.x += (direction.z == DirectX::XM_PIDIV2) ? 5.0f : -5.0f;
	time = 0;
	endTime = 300;

	radius = 20.0f;

	dParam.damage = 100.0f;
	if(direction.z == DirectX::XM_PIDIV2)
		dParam.direction = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	else
		dParam.direction = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
	
}

void EffectBread1::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	position = useChara->GetPos();
	position.y += sin(-time / 130.0f + DirectX::XM_PIDIV2) * 5.0f;
	position.x += cos(-time / 130.0f + DirectX::XM_PIDIV2) * 
		((direction.z == DirectX::XM_PIDIV2) ? 7.0f : -7.0f);

	if (100 < time)
	{
		if (time < 3000)
			tag = ObjectTag::DAMAGE;
		else
			tag = ObjectTag::STEALTH;
	}

}

void  EffectBread1::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void EffectBread1::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	sprite->DrawSet(pDeviceContext);
	spriteShader->DrawSet(pDeviceContext);

	//pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	pDeviceContext->Draw(sprite->GetIndexCount(), 0);

}

void EffectBread1::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	float scale = abs((time *  (time - endTime))/ 4500.0f);

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX World = DirectX::XMMatrixTranslation(0.0f, 0.0f, -2.0f);
	DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationZ(-time / 130.0f + DirectX::XM_PIDIV2);
	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationY(direction.z - DirectX::XM_PIDIV2);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	World *= Scale * Rotate * RotateDef * Offset;

	SPRITE::CONSTANT_BUFFER ccb2;
	DirectX::XMStoreFloat4x4(&ccb2.World, DirectX::XMMatrixTranspose(World));
	spriteShader->SetConstantBuffer(pDeviceContext, ccb2);

}

void EffectBread1::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}

bool EffectBread1::IsNewHit(BaseObject* obj)
{
	auto itr = std::find(collisionObj.begin(), collisionObj.end(), obj);
	if (itr != collisionObj.end()) {
		return false;
	}
	else {
		collisionObj.push_back(obj);
		const int quantitiy = 10;
		useChara->AttackHit(GUN, quantitiy);
		return true;
	}
}