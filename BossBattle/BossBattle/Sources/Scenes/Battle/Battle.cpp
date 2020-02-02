#include"Battle.h"
#include"../../System/InputController.h"

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

	objectManager = std::make_unique<ObjectManager>();
	objectManager->SstModelMap(pDevice, "Cube");
	objectManager->SstModelMap(pDevice, "gunbreaker");
	objectManager->SstModelMap(pDevice, "grid");
	objectManager->SstModelShader(pDevice, L"shader");
	objectManager->SstModelShader(pDevice, L"alpha");
	objectManager->SstSpriteMap(pDevice, L"Tex");
	objectManager->SstSpriteShader(pDevice, L"shader");

	light = std::make_shared<Light>();

	player = std::make_shared<GunBreaker>(objectManager, light);
	bossEnemy = std::make_shared<SpaceBoss>(objectManager,light);

	grid = objectManager->GetModel("grid");
	gridShader = objectManager->GetModelShader(L"alpha");
}

Battle::~Battle()
{
	//SAFE_RELEASE(pConstantBuffer);
}

void Battle::Update()
{
	ObjectUpdate();
	CollisionUpdate();
}

void Battle::ObjectUpdate()
{
	player->Update();
	bossEnemy->Update();
}

void Battle::CollisionUpdate()
{
	DirectX::XMFLOAT3 obj1 = player->GetPos();
	DirectX::XMFLOAT3 obj2 = bossEnemy->GetPos();
	DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(obj1.x - obj2.x, obj1.y - obj2.y, obj1.z - obj2.z);
	float length;
	if (IsCollision(delta, player->GetRadius() + bossEnemy->GetRadius(), &length))
	{
		length = std::sqrt(length);
		delta = NormalizeFloat3(delta);
		player->OnCollisionEnter(bossEnemy->GetTag(), DirectX::XMFLOAT3(delta.x * length, delta.y * length, delta.z * length));
		bossEnemy->OnCollisionEnter(player->GetTag(), DirectX::XMFLOAT3(-delta.x * length, -delta.y * length, -delta.z * length));
	}

}

bool Battle::IsCollision(DirectX::XMFLOAT3 delta, float r, float* l)
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
}

void Battle::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{

	SetViewProj(pDeviceContext);

	player->Draw(pDeviceContext);

	bossEnemy->Draw(pDeviceContext);

	///////////////////////////////////////////////////////
		// パラメータの計算
	DirectX::XMMATRIX m_World = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(0.0f,35.0f,10.0f);
	DirectX::XMMATRIX m_Rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2);
	DirectX::XMMATRIX m_Scale = DirectX::XMMatrixScaling(50.0f, 50.0f, 50.0f);

	m_World *= m_Scale * m_Rotate * m_Offset;

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER ccb;

	DirectX::XMStoreFloat4x4(&ccb.World, DirectX::XMMatrixTranspose(m_World));
	DirectX::XMStoreFloat4(&ccb.Light,
		DirectX::XMVectorSet(
			light->playerLight.x,
			light->playerLight.y,
			light->playerLight.z, 0.0f));
	DirectX::XMStoreFloat4(&ccb.Attenuation,
		DirectX::XMVectorSet(
			light->playerAttenuation.x,
			light->playerAttenuation.y,
			light->playerAttenuation.z,
			light->playerAttenuation.w));

	gridShader->SetConstantBuffer(pDeviceContext, ccb);


	grid->DrawSet(pDeviceContext);
	gridShader->DrawSet(pDeviceContext);

	// 描画実行
	pDeviceContext->DrawIndexed(grid->GetIndexCount(), 0, 0);

	//model->DrawLineAdjSet(pDeviceContext);
	//shader->DrawLineAdjSet(pDeviceContext);
	//
	//// 描画実行
	//pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);

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