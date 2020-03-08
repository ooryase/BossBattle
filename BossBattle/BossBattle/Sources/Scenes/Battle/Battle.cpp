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
objectManager->SstSpriteMap(pDevice, L"UI/pause", D3D11_FILTER_MIN_MAG_MIP_POINT);
objectManager->SstSpriteMap(pDevice, L"UI/clear", D3D11_FILTER_MIN_MAG_MIP_POINT);
objectManager->SstSpriteMap(pDevice, L"UI/gameover", D3D11_FILTER_MIN_MAG_MIP_POINT);
objectManager->SstSpriteMap(pDevice, L"UI/retry", D3D11_FILTER_MIN_MAG_MIP_POINT);
objectManager->SstSpriteMap(pDevice, L"UI/title", D3D11_FILTER_MIN_MAG_MIP_POINT);

objectManager->SstSpriteShader(pDevice, L"shader");
objectManager->SstSpriteShader(pDevice, L"weight");
objectManager->SstSpriteShader(pDevice, L"UI");

light = std::make_shared<Light>();
light->Directional = DirectX::XMFLOAT3(0.0f, 0.9f, -0.3f);
light->PointCount = 0;

const DirectX::XMFLOAT3 eyeLookAt = DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f);
const DirectX::XMFLOAT3 eyePos = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
camera = std::make_shared<Camera>(eyeLookAt, DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f));

player = std::make_shared<GunBreaker>(objectManager, light, playerEffectReserves, enemyReserves);
bossEnemy = std::make_shared<SpaceBoss>(objectManager, light, enemyEffectReserves, enemyReserves, camera);

backGround = std::make_unique<Space>(objectManager);

phase = Phase::START;
phaseTime = 0;

proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (FLOAT)1280 / (FLOAT)720, 0.1f, 500.0f);

uiPause = objectManager->GetSprite(L"UI/pause");
uiClear = objectManager->GetSprite(L"UI/clear");
uiGameOver = objectManager->GetSprite(L"UI/gameover");
uiRetry = objectManager->GetSprite(L"UI/retry");
uiTitle = objectManager->GetSprite(L"UI/title");
uiShader = objectManager->GetSpriteShader(L"UI");
isSelectRetry = true;
isPause = false;

fade = objectManager->GetModel("plane");
fadeShader = objectManager->GetModelShader(L"noLight");

}

Battle::~Battle()
{}

void Battle::Update()
{
	if (isPause)
		UpdateSelect();
	else
	{
		UpdateObjects();
		UpdateCollision();
	}
	UpdatePhase();

	ChangePause();
}

void Battle::ChangePause()
{
	if (phase == Phase::BATTLE && (
		InputController::getInstance().IsPushKey(DIK_ESCAPE) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_START)))
	{
		isPause = !isPause;
	}
}

void Battle::UpdateObjects()
{
	light->PointCount = 0;
	if(phase != Phase::START)
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
	if (phase == Phase::BATTLE)
	{
		CheckCollision(player, bossEnemy);
		for (auto&& var : enemies)
			CheckCollision(player, var);

		CheckCollisionDamageObjcts(playerEffects, bossEnemy);
		CheckCollisionDamageObjcts(enemyEffects, player);
	}

	const float wallPosX = 30.0f;
	player->CollisionWallUpdate(wallPosX);

}

void Battle::UpdatePhase()
{
	phaseTime += Timer::GetInstance().GetDeltaTime();

	if (phase == Phase::START && phaseTime > 13000)
	{
		phase = Phase::BATTLE;
		phaseTime = 0;
	}
	else if (phase == Phase::FINISH && phaseTime > 5000)
	{
		UpdateSelect();
	}
}

void Battle::UpdateSelect()
{
	if (InputController::getInstance().IsPushKey(DIK_UP) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_DPAD_UP) ||
		InputController::getInstance().IsPushThumbL(THUMB_LEFT::UP) ||
		InputController::getInstance().IsPushKey(DIK_DOWN) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_DPAD_DOWN) ||
		InputController::getInstance().IsPushThumbL(THUMB_LEFT::DOWN))
	{
		isSelectRetry = !isSelectRetry;
	}
	else if (InputController::getInstance().IsPushKey(DIK_Z) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_A))
	{
		nextScene = isSelectRetry ? SceneName::BATTLE : SceneName::TITLE;
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

	if (phase != Phase::FINISH && 
		(bossEnemy->IsDead() || player->IsDead()))
	{
		phase = Phase::FINISH;
		phaseTime = 0;
		isSelectRetry = true;
	}
}

void Battle::EndUpdateEffects(vector< shared_ptr< BaseEffect>>* effects, vector< shared_ptr< BaseEffect>>* effectReserves)
{
	for (auto&& var : *effects)
	{
		var->EndUpdate();
	}
	auto itr =
		std::remove_if(effects->begin(), effects->end(), [](
			std::shared_ptr<BaseObject>am) {return  am->IsToDelete(); });
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
			std::shared_ptr<BaseObject>am) {return  am->IsToDelete(); });
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

	DrawFade(pDeviceContext);
}

void Battle::DrawFade(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ADD);

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	MODEL::CONSTANT_BUFFER cb;
	cb.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	if (phase == Phase::START && phaseTime > 12000)
	{
		cb.Color.x = (phaseTime - 12000) / 200.0f;
		cb.Color.y = 0.0f;
	}
	else if (phase == Phase::BATTLE && phaseTime < 500)
	{
		cb.Color.x = (500 - phaseTime) / 100.0f;
		cb.Color.y = 0.0f;
	}
	else if (phase == Phase::FINISH)
		cb.Color.x = phaseTime / 5000.0f;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));
	fadeShader->SetConstantBuffer(pDeviceContext, cb);


	fade->DrawSet(pDeviceContext);
	fadeShader->DrawSet(pDeviceContext);

	pDeviceContext->DrawIndexed(fade->GetIndexCount(), 0, 0);

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ALIGMENT);

}

void Battle::DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext)
{

	if ((phase == Phase::FINISH && phaseTime > 5000) || isPause)
	{
		bool win = bossEnemy->IsDead();
		uiShader->DrawSet(pDeviceContext);

		std::shared_ptr<Sprite> tag;
		DirectX::XMVECTOR selectWeight;
		DirectX::XMVECTOR anotherWeight;
		float col = sinf(phaseTime / 100.0f) * 0.6f + 0.7f;
		if (phase == Phase::BATTLE)
		{
			tag = uiPause;
			selectWeight = DirectX::XMVectorSet(col, col, col, 1.0f);
			anotherWeight = DirectX::XMVectorSet(0.6f, 0.6f,0.6f, 1.0f);
		}
		else if (bossEnemy->IsDead())
		{
			tag = uiClear;
			selectWeight = DirectX::XMVectorSet(0.3f * col, 0.5f * col, col, 1.0f);
			anotherWeight = DirectX::XMVectorSet(0.3f, 0.5f, 1.0f, 1.0f);
		}
		else
		{
			tag = uiGameOver;
			selectWeight = DirectX::XMVectorSet(col, 0.5f * col, 0.3f * col, 1.0f);
			anotherWeight = DirectX::XMVectorSet(1.0f, 0.5f, 0.3f, 1.0f);
		}

		DirectX::XMMATRIX tagOffset = DirectX::XMMatrixTranslation(0.0f, 0.5f, 0.4f);
		DirectX::XMMATRIX tagScale = DirectX::XMMatrixScaling(1.2f, 0.34f, 1.0f);
		DirectX::XMVECTOR tagWeight = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		DrawUI(pDeviceContext, tagScale * tagOffset, tagWeight, tag);

		DirectX::XMMATRIX selectScale = DirectX::XMMatrixScaling(0.37f, 0.17f, 1.0f);

		DirectX::XMMATRIX retryOffset = DirectX::XMMatrixTranslation(0.0f, -0.2f, 0.4f);
		DrawUI(pDeviceContext, selectScale * retryOffset, isSelectRetry ? selectWeight : anotherWeight, uiRetry);

		DirectX::XMMATRIX titleOffset = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.4f);
		DrawUI(pDeviceContext, selectScale * titleOffset, isSelectRetry ? anotherWeight : selectWeight, uiTitle);
	}
}

void Battle::DrawUI(
	ComPtr<ID3D11DeviceContext> pDeviceContext,
	DirectX::XMMATRIX _world,
	DirectX::XMVECTOR _weight,
	std::shared_ptr<Sprite> _sprite)
{
	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(_world));
	DirectX::XMStoreFloat4(&cb.Weight, _weight);
	uiShader->SetConstantBuffer(pDeviceContext, cb);

	_sprite->DrawSet(pDeviceContext);
	pDeviceContext->Draw(_sprite->GetIndexCount(), 0);
}


void Battle::SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

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
