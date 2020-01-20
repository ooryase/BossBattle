#include"Title.h"
#include"../../System/InputController.h"

Title::Title(ID3D11Device* pDevice) : BaseScene()
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

	x = 0.0f;


	objectManager = std::make_shared<ObjectManager>();
	objectManager->SstModelMap(pDevice, "Monkey");
	objectManager->SstModelMap(pDevice, "gunbreaker");
	objectManager->SstModelShader(pDevice, L"shader");
	objectManager->SstSpriteMap(pDevice, L"Tex");
	objectManager->SstSpriteShader(pDevice, L"shader");

	player = std::make_shared<GunBreaker>(objectManager);

	sprite = objectManager->GetSprite(L"Tex");// std::make_unique<Sprite>(pDevice, L"Assets/Textures/Tex.jpg");
	spriteShader = objectManager->GetSpriteShader(L"shader"); //std::make_unique<SpriteShader>(pDevice, L"Shader/shader.hlsl");

	model = objectManager->GetModel("Monkey"); //std::make_unique<FbxModel>(pDevice, "Assets/Model/Monkey.fbx");
	mShader = objectManager->GetModelShader(L"shader"); //std::make_unique<ModelShader>(pDevice, L"Shader/shaderPoint.hlsl", L"Shader/shaderEdge.hlsl");

}

Title::~Title()
{
	SAFE_RELEASE(pConstantBuffer);
}

void Title::Update()
{
	if (InputController::getInstance().IsPressKey(DIK_A))
	{
		std::cout << "iketaaaa" << std::endl;
	}
	player->Update();
}

void Title::Draw(ID3D11DeviceContext* pDeviceContext)
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

	////////////////////////////////////////////////////////////////////////////////
	//sprite
	sprite->DrawSet(pDeviceContext);
	spriteShader->DrawSet(pDeviceContext);

	// パラメータの計算
	DirectX::XMMATRIX World = DirectX::XMMatrixRotationY(x += 0.001f);
	DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f);
	World *= Offset;

	// パラメータの受け渡し
	SPRITE::CONSTANT_BUFFER spriteCB;
	DirectX::XMStoreFloat4x4(&spriteCB.World, DirectX::XMMatrixTranspose(World));

	spriteShader->SetConstantBuffer(pDeviceContext, spriteCB);


	// 描画実行
	pDeviceContext->Draw(sprite->GetIndexCount(), 0);
	
	/////////////////////////////////////////////////////////////////////////////////////
	//model
	model->DrawSet(pDeviceContext);
	mShader->DrawSet(pDeviceContext);

	// パラメータの計算
	DirectX::XMMATRIX m_World = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX m_RotateDef = DirectX::XMMatrixRotationX(DirectX::XM_PIDIV2);
	DirectX::XMMATRIX m_Rotate2 = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 3.0f);
	DirectX::XMMATRIX m_Rotate = XMMatrixTranspose(m_RotateDef * m_Rotate2);
	DirectX::XMMATRIX m_Scale = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);

	m_World *= m_Scale * m_Rotate * m_Offset;

	DirectX::XMVECTOR m_Light = DirectX::XMVectorSet(std::cos(x * 4.0f) * 40.1f, -20.0f, std::sin(x * 4.0f) * 40.1f, 0.0f);
	DirectX::XMVECTOR m_Attenuation = DirectX::XMVectorSet(1.0f, 0.0005f, 0.0005f, 0.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER ccb;

	DirectX::XMStoreFloat4x4(&ccb.World, DirectX::XMMatrixTranspose(m_World));
	DirectX::XMStoreFloat4(&ccb.Light, m_Light);
	DirectX::XMStoreFloat4(&ccb.Attenuation, m_Attenuation);

	mShader->SetConstantBuffer(pDeviceContext, ccb);

	//pDeviceContext->VSSetConstantBuffers(1, 1, &pModelBuffer);
	//pDeviceContext->GSSetConstantBuffers(1, 1, &pModelBuffer);
	//pDeviceContext->PSSetConstantBuffers(1, 1, &pModelBuffer);


	//pDeviceContext->UpdateSubresource(pMConstantBuffer, 0, NULL, &ccb, 0, 0);


	// 描画実行
	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

	model->DrawLineAdjSet(pDeviceContext);
	mShader->DrawLineAdjSet(pDeviceContext);

	// 描画実行
	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);

	player->Draw(pDeviceContext);

}