#include"Battle.h"
#include"../../System/InputController.h"
#include"../../System/DeviceManager.h"
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
	objectManager->SstModelMap(pDevice, "Effects/spine");
	objectManager->SstModelMap(pDevice, "Effects/bullet");
	objectManager->SstModelMap(pDevice, "Effects/sphere");
	objectManager->SstModelMap(pDevice, "grid2");
	objectManager->SstModelMap(pDevice, "plane");
	objectManager->SstModelShader(pDevice, L"shader");
	objectManager->SstModelShader(pDevice, L"alpha");
	objectManager->SstModelShader(pDevice, L"enemy");
	objectManager->SstModelShader(pDevice, L"noLight");
	objectManager->SstSpriteMap(pDevice, L"Tex");
	objectManager->SstOctagonSpriteMap(pDevice, L"space");
	objectManager->SstSpriteMap(pDevice, L"slash");
	objectManager->SstSpriteMap(pDevice, L"kira");
	objectManager->SstSpriteMap(pDevice, L"black");
	objectManager->SstSpriteMap(pDevice, L"particle1");
	objectManager->SstSpriteMap(pDevice, L"san");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/bossHpFrame");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/bossHpGauge");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/hpFrame");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/hpGauge");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/GunBreFrame");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/BreadGauge");
	objectManager->SstSpriteMap(pDevice, L"UI/Battle/Gauge/GunGauge");
	objectManager->SstSpriteShader(pDevice, L"shader");
	objectManager->SstSpriteShader(pDevice, L"weight");

	light = std::make_shared<Light>();
	light->Directional = DirectX::XMFLOAT3(0.0f, 0.9f, -0.3f);
	//light->Player = DirectX::XMFLOAT4(100.1f, 100.0f, 0.0f, 0.0f);
	//light->PColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//light->PAttenuation = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	light->PointCount = 0;
	//for (int i = 0; i < 8; i++)
	//{
	//	light->Enemy[i] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->EColor[i] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->EAttenuation[i] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//}
	camera = std::make_shared<Camera>(eyeLookAt, DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f));

	player = std::make_shared<GunBreaker>(objectManager, light,playerEffectReserves, enemyReserves);
	bossEnemy = std::make_shared<SpaceBoss>(objectManager,light, enemyEffectReserves, enemyReserves, camera);

	backGround = std::make_unique<Space>(objectManager);

	phase = Phase::START;
	phaseTime = 0;

	eyeDirection = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	eyeLookAt = DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f);
	eyeLenght = 50.0f;

	proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (FLOAT)1280 / (FLOAT)720, 0.1f, 500.0f);

	fade = objectManager->GetModel("plane");
	fadeShader = objectManager->GetModelShader(L"noLight");


}

Battle::~Battle()
{}

void Battle::Update()
{
	UpdateObjects();
	UpdateCollision();
	UpdatePhase();
}

void Battle::UpdateObjects()
{
	light->PointCount = 0;
	player->Update();
	bossEnemy->Update();

	for (auto&& var : enemies)
		var->Update();
	for(auto&& var : playerEffects)
		var->Update();
	for (auto&& var : enemyEffects)
		var->Update();

	backGround->Update();
	camera->Update();
}

void Battle::UpdateCollision()
{
	CheckCollision(player, bossEnemy);
	for (auto&& var : enemies)
		CheckCollision(player, var);
	const float wallPosX = 30.0f;
	player->CollisionWallUpdate(wallPosX);

	CheckCollisionDamageObjcts(playerEffects, bossEnemy);
	CheckCollisionDamageObjcts(enemyEffects, player);
}

void Battle::UpdatePhase()
{
	phaseTime += Timer::GetInstance().GetDeltaTime();

	if (phase == Phase::START && phaseTime > 19000)
	{
		phase = Phase::BATTLE;
		phaseTime = 0;
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

void Battle::CheckCollisionDamageObjcts(vector< shared_ptr< BaseEffect>> dObjs, std::shared_ptr<BaseCharacter> chara)
{
	if (chara->GetTag() == ObjectTag::STEALTH)
		return;
	for (auto&& var : dObjs)
	{
		if (var->GetTag() == ObjectTag::DAMAGE)
			CheckCollisionDamageObj(var, chara);
	}
}

void Battle::CheckCollisionDamageObj(std::shared_ptr<BaseEffect> dObj, std::shared_ptr<BaseCharacter> chara)
{
	DirectX::XMFLOAT3 pos1 = dObj->GetPos();
	DirectX::XMFLOAT3 pos2 = chara->GetPos();
	DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);
	float length;
	if (IsCollide(delta, dObj->GetRadius() + chara->GetRadius(), &length) &&
		dObj->IsNewHit(chara.get()))
	{
		chara->CollisionDamage(&(dObj->dParam));
	}
}

bool Battle::IsCollide(DirectX::XMFLOAT3 delta, float r, float* l)
{
	*l = r * r - (delta.x * delta.x + delta.y * delta.y);

	return (*l > 0.0f);
}

void Battle::EndUpdate()
{
	player->EndUpdate();
	bossEnemy->EndUpdate();
	EndUpdateVector(&enemies, &enemyReserves);
	EndUpdateVector(&playerEffects, &playerEffectReserves);
	EndUpdateVector(&enemyEffects, &enemyEffectReserves);

}

void Battle::EndUpdateEffects(vector< shared_ptr< BaseEffect>>* effects, vector< shared_ptr< BaseEffect>>* effectReserves)
{
	for (auto&& var : *effects)
	{
		var->EndUpdate();
	}
	auto itr =
		std::remove_if(effects->begin(), effects->end(), [](
			std::shared_ptr<BaseObject>am) {return  am->IsDead(); });
	effects->erase(itr, effects->end());

	if (!effectReserves->empty())
	{
		for (auto&& var : *effectReserves)
			effects->push_back(var);
		effectReserves->clear();
	}
}

template<typename U, typename V>
void Battle::EndUpdateVector(vector< shared_ptr< U>>* vectorMain, vector< shared_ptr< V>>* vectorReserves)
{
	for (auto&& var : *vectorMain)
	{
		var->EndUpdate();
	}
	auto itr =
		std::remove_if(vectorMain->begin(), vectorMain->end(), [](
			std::shared_ptr<BaseObject>am) {return  am->IsDead(); });
	vectorMain->erase(itr, vectorMain->end());

	if (!vectorReserves->empty())
	{
		for (auto&& var : *vectorReserves)
			vectorMain->push_back(var);
		vectorReserves->clear();
	}
}


void Battle::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{

	SetViewProj(pDeviceContext);
	SetLight(pDeviceContext);

	player->Draw(pDeviceContext);

	bossEnemy->Draw(pDeviceContext);

	for (auto&& var : enemies)
	{
		var->Draw(pDeviceContext);
	}


	///////////////////////////////////////////////////////




	backGround->Draw(pDeviceContext);
	backGround->DrawBillBoard(pDeviceContext, camera->GetEyeDirection());

	for (auto&& var : playerEffects)
	{
		var->Draw(pDeviceContext);
	}

	for (auto&& var : enemyEffects)
	{
		var->Draw(pDeviceContext);
	}

	if (phase == Phase::BATTLE)
	{
		player->DrawGauge(pDeviceContext);
		bossEnemy->DrawGauge(pDeviceContext);
	}
	

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ADD);

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	MODEL::CONSTANT_BUFFER cb;

	if (phase == Phase::START && phaseTime > 18000)
	{
		cb.Color.x = (phaseTime - 18000) / 200.0f;
		cb.Color.y = 0.0f;
	}
	else if (phase == Phase::BATTLE && phaseTime < 500)
	{
		cb.Color.x = (500 - phaseTime) / 100.0f;
		cb.Color.y = 0.0f;
	}
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));
	fadeShader->SetConstantBuffer(pDeviceContext, cb);


	fade->DrawSet(pDeviceContext);
	fadeShader->DrawSet(pDeviceContext);

	pDeviceContext->DrawIndexed(fade->GetIndexCount(), 0, 0);

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ALIGMENT);

}

void Battle::SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	DirectX::XMVECTOR eye_pos = DirectX::XMVectorSet(
		eyeDirection.x * eyeLenght + eyeLookAt.x,
		eyeDirection.y * eyeLenght + eyeLookAt.y,
		eyeDirection.z * eyeLenght + eyeLookAt.z, 1.0f);
	DirectX::XMVECTOR eye_lookat = DirectX::XMVectorSet(eyeLookAt.x, eyeLookAt.y, eyeLookAt.z, 1.0f);
	DirectX::XMVECTOR eye_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);
	//DirectX::XMMATRIX Proj = DirectX::XMMatrixOrthographicLH(1280.0f * 0.05f, 720.0f * 0.05f, 0.1f, 500.0f);

	D3D11_MAPPED_SUBRESOURCE data;
	CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.View, DirectX::XMMatrixTranspose(camera->GetView()));
	DirectX::XMStoreFloat4x4(&cb.Projection, DirectX::XMMatrixTranspose(proj));
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

DirectX::XMFLOAT3 Battle::NormalizeFloat3(DirectX::XMFLOAT3 f3)
{
	auto l = std::sqrt(f3.x * f3.x + f3.y * f3.y + f3.z * f3.z);

	return DirectX::XMFLOAT3(f3.x / l, f3.y / l, f3.z / l);
}
