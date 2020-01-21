#include"Battle.h"
#include"../../System/InputController.h"

Battle::Battle(ID3D11Device* pDevice) : BaseScene()
{


	// 定数バッファの設定
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

	objectManager = std::make_unique<ObjectManager>();
	//objectManager->SstModelMap(pDevice, "Monkey");
	objectManager->SstModelMap(pDevice, "gunbreaker2");
	objectManager->SstModelShader(pDevice, L"shader");
	objectManager->SstSpriteMap(pDevice, L"Tex");
	objectManager->SstSpriteShader(pDevice, L"shader");

	player = std::make_shared<GunBreaker>(objectManager);

}

Battle::~Battle()
{
	SAFE_RELEASE(pConstantBuffer);
}

void Battle::Update()
{
	player->Update();
}

void Battle::EndUpdate()
{
	player->EndUpdate();
}

void Battle::Draw(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

	DirectX::XMVECTOR eye_pos = DirectX::XMVectorSet(0.0f, 1.0f, -20.0f, 1.0f);
	DirectX::XMVECTOR eye_lookat = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR eye_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);
	DirectX::XMMATRIX Proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (FLOAT)1280 / (FLOAT)720, 0.1f, 500.0f);

	D3D11_MAPPED_SUBRESOURCE data;
	CONSTANT_BUFFER cb;
	DirectX::XMStoreFloat4x4(&cb.View, DirectX::XMMatrixTranspose(View));
	DirectX::XMStoreFloat4x4(&cb.Projection, DirectX::XMMatrixTranspose(Proj));
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&cb), sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer, 0);

	player->Draw(pDeviceContext);

}