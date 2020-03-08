#include"Title.h"
#include"../../System/InputController.h"
#include"../../System/DeviceManager.h"

Title::Title(ComPtr<ID3D11Device> pDevice) : BaseScene()
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
	objectManager->SstModelMap(pDevice, "grid2");
	objectManager->SstModelMap(pDevice, "plane");
	objectManager->SstModelShader(pDevice, L"shader");
	objectManager->SstModelShader(pDevice, L"alpha");
	objectManager->SstModelShader(pDevice, L"noLight");
	objectManager->SstOctagonSpriteMap(pDevice, L"space");
	objectManager->SstSpriteMap(pDevice, L"slash");
	objectManager->SstSpriteMap(pDevice, L"kira");
	objectManager->SstSpriteMap(pDevice, L"black");
	objectManager->SstSpriteMap(pDevice, L"particle1");
	objectManager->SstSpriteShader(pDevice, L"shader");
	objectManager->SstSpriteShader(pDevice, L"weight");
	objectManager->SstSpriteShader(pDevice, L"UI");
	objectManager->SstSpriteMap(pDevice, L"UI/theogony", D3D11_FILTER_MIN_MAG_MIP_POINT);
	objectManager->SstSpriteMap(pDevice, L"UI/start", D3D11_FILTER_MIN_MAG_MIP_POINT);
	objectManager->SstSpriteMap(pDevice, L"UI/quit", D3D11_FILTER_MIN_MAG_MIP_POINT);

	light = std::make_shared<Light>();
	light->Directional = DirectX::XMFLOAT3(0.0f, 0.9f, -0.3f);
	light->PointCount = 0;

	const DirectX::XMFLOAT3 eyeLookAt = DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f);
	const DirectX::XMFLOAT3 eyePos = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
	camera = std::make_shared<Camera>(eyeLookAt, eyePos);

	backGround = std::make_unique<Space>(objectManager);

	phase = Phase::START;
	phaseTime = 0;

	proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (FLOAT)1280 / (FLOAT)720, 0.1f, 500.0f);

	fade = objectManager->GetModel("plane");
	fadeShader = objectManager->GetModelShader(L"noLight");


	texTitle = objectManager->GetSprite(L"UI/theogony");
	texStart = objectManager->GetSprite(L"UI/start");
	texQuit = objectManager->GetSprite(L"UI/quit");
	texShader = objectManager->GetSpriteShader(L"UI");

	isSelectStart = true;

}

Title::~Title()
{}

void Title::Update()
{
	backGround->Update();
	camera->Update();

	phaseTime += Timer::GetInstance().GetDeltaTime();



	UpdatePhase();
}

void Title::UpdatePhase()
{
	switch (phase)
	{
	case Title::Phase::START:
		if (phaseTime > 1000)
		{
			phase = Phase::SELECT;
			phaseTime = 0;
		}
		else if (InputController::getInstance().IsPushKey(DIK_Z) ||
			InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_A))
		{
			phase = Phase::FINISH;
			phaseTime = 0;
		}
		break;
	case Title::Phase::SELECT:
		if (InputController::getInstance().IsPushKey(DIK_UP) ||
			InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_DPAD_UP) ||
			InputController::getInstance().IsPushThumbL(THUMB_LEFT::UP) ||
			InputController::getInstance().IsPushKey(DIK_DOWN) ||
			InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_DPAD_DOWN) ||
			InputController::getInstance().IsPushThumbL(THUMB_LEFT::DOWN))
		{
			isSelectStart = !isSelectStart;
		}
		else if (InputController::getInstance().IsPushKey(DIK_Z) ||
			InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_A))
		{
			DeviceManager::GetInstance().SetRadialBlur(DirectX::XMFLOAT2(0.5f, 0.5f), 1000, 1.0f);
			phase = Phase::FINISH;
			phaseTime = 0;
		}
		break;
	case Title::Phase::FINISH:

		break;
	default:
		break;
	}

}

void Title::EndUpdate()
{
	if (phase == Phase::FINISH)
	{
		if (!isSelectStart)
			nextScene = SceneName::QUIT;
		else if (phaseTime > 1000)
			nextScene = SceneName::BATTLE;
	}
}

void Title::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	SetViewProj(pDeviceContext);
	SetLight(pDeviceContext);


	backGround->Draw(pDeviceContext);
	backGround->DrawBillBoard(pDeviceContext, camera->GetEyeDirection());


	texShader->DrawSet(pDeviceContext);		

	DirectX::XMMATRIX titleOffset = DirectX::XMMatrixTranslation(0.0f, 0.5f, 0.4f);
	DirectX::XMMATRIX titleScale = DirectX::XMMatrixScaling(0.6f, 0.36f, 1.0f);
	DirectX::XMVECTOR titleWeight = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DrawUI(pDeviceContext, titleScale * titleOffset, titleWeight, texTitle);

	DirectX::XMMATRIX startOffset = DirectX::XMMatrixTranslation(0.0f, -0.2f, 0.4f);
	DirectX::XMMATRIX startScale = DirectX::XMMatrixScaling(0.37f, 0.17f, 1.0f);
	float startCol = isSelectStart ? sinf(phaseTime / 100.0f) * 0.6f + 0.7f : 0.6f;
	DirectX::XMVECTOR startWeight = DirectX::XMVectorSet(
		startCol, startCol, startCol, 1.0f);
	DrawUI(pDeviceContext, startScale * startOffset, startWeight, texStart);

	DirectX::XMMATRIX quitOffset = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.4f);
	DirectX::XMMATRIX quitScale = DirectX::XMMatrixScaling(0.37f, 0.17f, 1.0f);
	float quitCol = isSelectStart ? 0.6f : sinf(phaseTime / 100.0f) * 0.7f + 0.6f;
	DirectX::XMVECTOR quitWeight = DirectX::XMVectorSet(
		quitCol, quitCol, quitCol, 1.0f);
	DrawUI(pDeviceContext, quitScale * quitOffset, quitWeight, texQuit);



	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ADD);

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	MODEL::CONSTANT_BUFFER cb;
	cb.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	if (phase == Phase::START)
		cb.Color.x = 1.0f -  phaseTime / 1000.0f;
	else if (phase == Phase::FINISH)
		cb.Color.x = phaseTime / 500.0f;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));
	fadeShader->SetConstantBuffer(pDeviceContext, cb);


	fade->DrawSet(pDeviceContext);
	fadeShader->DrawSet(pDeviceContext);

	pDeviceContext->DrawIndexed(fade->GetIndexCount(), 0, 0);

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ALIGMENT);

}

void Title::DrawUI(
	ComPtr<ID3D11DeviceContext> pDeviceContext,
	DirectX::XMMATRIX _world,
	DirectX::XMVECTOR _weight,
	std::shared_ptr<Sprite> _sprite)
{
	SPRITE::CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(_world));
	DirectX::XMStoreFloat4(&cb.Weight, _weight);
	texShader->SetConstantBuffer(pDeviceContext, cb);

	_sprite->DrawSet(pDeviceContext);
	pDeviceContext->Draw(_sprite->GetIndexCount(), 0);
}

void Title::DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext)
{}

void Title::SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext)
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


void Title::SetLight(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->VSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());
	pDeviceContext->GSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());

	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(lightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(light.get()), sizeof(*light));
	pDeviceContext->Unmap(lightConstantBuffer.Get(), 0);
}
