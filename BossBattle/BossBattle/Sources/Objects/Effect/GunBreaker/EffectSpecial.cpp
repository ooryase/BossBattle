#include "EffectSpecial.h"
#include"../../../System/Timer.h"
#include"../../Character/GunBreaker.h"

EffectSpecial::EffectSpecial(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player)
	: BaseEffect(),
	useChara(_player)

{
	tag = ObjectTag::DAMAGE;

	model = objectManager->GetModel("Effects/bullet");
	modelShader = objectManager->GetModelShader(L"shader");

	direction = useChara->GetDirection();
	position = useChara->GetPos();
	position.x += (direction.z == DirectX::XM_PIDIV2) ? 5.0f : -5.0f;
	time = 0;
	endTime = 3000;

	radius = 10.0f;

	dParam.damage = 100.0f;
	if (direction.z == DirectX::XM_PIDIV2)
		dParam.direction = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
	else
		dParam.direction = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);

}

void EffectSpecial::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	position.x += ((direction.z == DirectX::XM_PIDIV2) ? 1.0f : -1.0f);
}

void  EffectSpecial::EndUpdate()
{
	if (time > endTime)
		isToDelete = true;
}

void EffectSpecial::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	model->DrawSet(pDeviceContext);
	modelShader->DrawSet(pDeviceContext);

	//pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);


	model->DrawLineAdjSet(pDeviceContext);
	modelShader->DrawLineAdjSet(pDeviceContext);

	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);
}

void EffectSpecial::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	float scale = 1.0f;

	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	Offset *= DirectX::XMMatrixTranslation(0.0f, 0.0f, -2.0f);
	DirectX::XMMATRIX RotateDef = DirectX::XMMatrixRotationY(direction.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale, scale, scale);
	DirectX::XMMATRIX World = Scale * RotateDef * Offset;

	DirectX::XMVECTOR Color = DirectX::XMVectorSet(0.0f, 0.0f, 0.2f, 0.0f);
	DirectX::XMVECTOR EdgeColor = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	// ƒpƒ‰ƒ[ƒ^‚ÌŽó‚¯“n‚µ
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(World));
	DirectX::XMStoreFloat4(&cb.Color, Color);
	DirectX::XMStoreFloat4(&cb.EdgeColor, EdgeColor);
	modelShader->SetConstantBuffer(pDeviceContext, cb);

}

void EffectSpecial::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}

bool EffectSpecial::IsNewHit(BaseObject* obj)
{
	auto itr = std::find(collisionObj.begin(), collisionObj.end(), obj);
	if (itr != collisionObj.end()) {
		return false;
	}
	else {
		collisionObj.push_back(obj);
		const int quantitiy = 10;
		useChara->AttackHit(0, quantitiy);
		return true;
	}
}