#include "EffectChage.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"
#include"../../../System/DeviceManager.h"
#include<algorithm>

EffectChage::EffectChage(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect(),
	useChara(_player)

{
	tag = ObjectTag::STEALTH;

	sprite = objectManager->GetSprite(L"particle1");
	spriteShader = objectManager->GetSpriteShader(L"shader");

	position = useChara->GetPos();
	direction = (useChara->GetDirection().z == DirectX::XM_PIDIV2) ? -1.0f : 1.0f;

	position.x += 20.0f * direction;
	time = 0;
	endTime = 1000;

	radius = 0.0f;

	dParam.damage = 100.0f;
	dParam.direction = DirectX::XMFLOAT3(0.2f * direction, 0.2f, 0.0f);
}

void EffectChage::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	position = useChara->GetPos();
	//position.y += sin(-time / 130.0f + DirectX::XM_PIDIV2) * 5.0f;
	//position.x += cos(-time / 130.0f + DirectX::XM_PIDIV2) *
		//((direction.z == DirectX::XM_PIDIV2) ? 7.0f : -7.0f);


	for (auto&& var : pPos)
	{
		var.time += Timer::GetInstance().GetDeltaTime();

		if (var.time > 500)
		{
			if (time > endTime - 500)
				var.active = false;
			else
				var.time = 0;
		}
	}

	std::sort(pPos.begin(), pPos.end(), [](const ParticlePos &a, const ParticlePos &b) {
        return a.time > b.time;
    });

	if (static_cast<int>(pPos.size()) < time / 16 && pPos.size() < 40)
	{
		pPos.push_back(time / 7.0f);
	}

}

void  EffectChage::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void EffectChage::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	sprite->DrawSet(pDeviceContext);
	spriteShader->DrawSet(pDeviceContext);

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ADD);

	for (unsigned int i = 0; i < pPos.size(); i++)
	{
		if (pPos[i].active)
		{
			DrawSetParticle(pDeviceContext, pPos[i].radian, pPos[i].time);
			pDeviceContext->Draw(sprite->GetIndexCount(), 0);
		}
	}

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ALIGMENT);

}

void EffectChage::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{}

void EffectChage::DrawSetParticle(ComPtr<ID3D11DeviceContext> pDeviceContext, float _radian, int _time)
{
	float rate = _time / static_cast<float>(500);
	float scale = abs((rate * (1.0f - rate)) * 20.0f);
	float lenght = (1.0f - rate * rate) * 10.0f;

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX Trans = DirectX::XMMatrixTranslation(lenght * cosf(_radian), 4.0f + lenght * sinf(_radian), -2.0f + _time / 500.0f);

	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationY((direction - 1.0f) * DirectX::XM_PIDIV2);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	DirectX::XMMATRIX World = Scale * Trans * Offset;

	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	spriteShader->SetConstantBuffer(pDeviceContext, cb);

}

void EffectChage::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}