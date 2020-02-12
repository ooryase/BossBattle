#include"Battle.h"
#include"../../System/InputController.h"
#include <algorithm>

Battle::Battle(ComPtr<ID3D11Device> pDevice) : BaseScene()
{


	// 定数バッファの設定
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	pDevice->CreateBuffer(&cb, NULL, pConstantBuffer.GetAddressOf());

	D3D11_BUFFER_DESC lcb;
	lcb.ByteWidth = sizeof(Light);
	lcb.Usage = D3D11_USAGE_DYNAMIC;
	lcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lcb.MiscFlags = 0;
	lcb.StructureByteStride = 0;
	pDevice->CreateBuffer(&lcb, NULL, lightConstantBuffer.GetAddressOf());

	objectManager = std::make_unique<ObjectManager>();
	objectManager->SstModelMap(pDevice, "spaceBoss");
	objectManager->SstModelMap(pDevice, "gunbreaker");
	objectManager->SstModelMap(pDevice, "grid");
	objectManager->SstModelShader(pDevice, L"shader");
	objectManager->SstModelShader(pDevice, L"alpha");
	objectManager->SstSpriteMap(pDevice, L"Tex");
	objectManager->SstOctagonSpriteMap(pDevice, L"space");
	objectManager->SstSpriteMap(pDevice, L"slash");
	objectManager->SstSpriteMap(pDevice, L"kira");
	objectManager->SstSpriteMap(pDevice, L"black");
	objectManager->SstSpriteShader(pDevice, L"shader");

	light = std::make_shared<Light>();
	light->Directional = DirectX::XMFLOAT4(0.0f, 0.9f, -0.3f, 0.0f);
	light->Player = DirectX::XMFLOAT4(40.1f, -20.0f, -5.0f, 0.0f);
	light->PColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->PAttenuation = DirectX::XMFLOAT4(1.0f, 0.0005f, 0.0005f, 0.0f);
	light->ELCount = DirectX::XMINT4( 0, 0, 0, 0);
	for (int i = 0; i < 8; i++)
	{
		light->Enemy[i] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->EColor[i] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->EAttenuation[i] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	player = std::make_shared<GunBreaker>(objectManager, light,playerEffectReserves);
	bossEnemy = std::make_shared<SpaceBoss>(objectManager,light, enemyEffectReserves);

	backGround = std::make_unique<Space>(objectManager);
}

Battle::~Battle()
{}

void Battle::Update()
{
	UpdateObjects();
	UpdateCollision();
}

void Battle::UpdateObjects()
{
	player->Update();
	bossEnemy->Update();

	for(auto&& var : playerEffects)
	{
		var->Update();
	}

	backGround->Update();
}

void Battle::UpdateCollision()
{
	CheckCollision(player, bossEnemy);

	for (auto&& var : playerEffects)
	{
		if (var->GetTag() == ObjectTag::DAMAGE)
			CheckCollisionDamageObj(var, bossEnemy);
	}
}

void Battle::CheckCollision(std::shared_ptr<BaseObject> obj1, std::shared_ptr<BaseObject> obj2)
{
	DirectX::XMFLOAT3 pos1 = obj1->GetPos();
	DirectX::XMFLOAT3 pos2 = obj2->GetPos();
	DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);
	float length;
	if (IsCollide(delta, obj1->GetRadius() + obj2->GetRadius(), &length))
	{
		length = std::sqrt(length);
		delta = NormalizeFloat3(delta);

		obj1->OnCollisionEnter(obj2->GetTag(), DirectX::XMFLOAT3(delta.x * length, delta.y * length, delta.z * length));
		obj2->OnCollisionEnter(obj1->GetTag(), DirectX::XMFLOAT3(-delta.x * length, -delta.y * length, -delta.z * length));
	}
}

void Battle::CheckCollisionDamageObj(std::shared_ptr<BaseEffect> dObj, std::shared_ptr<BaseCharacter> chara)
{
	DirectX::XMFLOAT3 pos1 = dObj->GetPos();
	DirectX::XMFLOAT3 pos2 = chara->GetPos();
	DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);
	float length;
	if (IsCollide(delta, dObj->GetRadius() + chara->GetRadius(), &length))
	{
		//length = std::sqrt(length);
		//delta = NormalizeFloat3(delta);

		chara->CollisionDamage(&(dObj->dParam));
	}
}

bool Battle::IsCollide(DirectX::XMFLOAT3 delta, float r, float* l)
{
	*l = r * r - (delta.x * delta.x + delta.y * delta.y);

	return (*l > 0.0f);
}

DirectX::XMFLOAT3 Battle::NormalizeFloat3(DirectX::XMFLOAT3 f3)
{
	auto l = std::sqrt(f3.x * f3.x + f3.y * f3.y + f3.z * f3.z);

	return DirectX::XMFLOAT3(f3.x / l, f3.y / l, f3.z / l);
}

void Battle::EndUpdate()
{
	player->EndUpdate();
	bossEnemy->EndUpdate();

	for (auto&& var : playerEffects)
	{
		var->EndUpdate();
	}
	auto itr =
		std::remove_if(playerEffects.begin(), playerEffects.end(), [](
			std::shared_ptr<BaseObject>am) {return  am->IsDead(); });
	playerEffects.erase(itr, playerEffects.end());

	if (!playerEffectReserves.empty())
	{
		for (auto&& var : playerEffectReserves)
			playerEffects.push_back(var);
		playerEffectReserves.clear();
	}
}

void Battle::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{

	SetViewProj(pDeviceContext);
	SetLight(pDeviceContext);

	player->Draw(pDeviceContext);

	bossEnemy->Draw(pDeviceContext);



	///////////////////////////////////////////////////////

	backGround->Draw(pDeviceContext);

	for (auto&& var : playerEffects)
	{
		var->Draw(pDeviceContext);
	}

	player->DrawGauge(pDeviceContext);


}

void Battle::SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	DirectX::XMVECTOR eye_pos = DirectX::XMVectorSet(0.0f, 10.0f, -50.0f, 1.0f);
	DirectX::XMVECTOR eye_lookat = DirectX::XMVectorSet(0.0f, 10.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR eye_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);
	//DirectX::XMMATRIX Proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (FLOAT)1280 / (FLOAT)720, 0.1f, 500.0f);
	DirectX::XMMATRIX Proj = DirectX::XMMatrixOrthographicLH(1280.0f * 0.05f, 720.0f * 0.05f, 0.1f, 500.0f);

	D3D11_MAPPED_SUBRESOURCE data;
	CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.View, DirectX::XMMatrixTranspose(View));
	DirectX::XMStoreFloat4x4(&cb.Projection, DirectX::XMMatrixTranspose(Proj));
	pDeviceContext->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&cb), sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer.Get(), 0);
}

void Battle::SetLight(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->VSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());
	pDeviceContext->GSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());

	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(lightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(light.get()), sizeof(*light));
	pDeviceContext->Unmap(lightConstantBuffer.Get(), 0);
}