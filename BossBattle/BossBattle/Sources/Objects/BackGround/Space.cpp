#include"Space.h"
#include"../../System/Timer.h"

Space::Space(std::shared_ptr<ObjectManager> objectManager)
	: starPosSize(200)
{
	grid = objectManager->GetModel("grid");
	gridShader = objectManager->GetModelShader(L"alpha");

	space = objectManager->GetSprite(L"space");
	spaceShader = objectManager->GetSpriteShader(L"shader");
	
	star = objectManager->GetSprite(L"kira");
	starShader = objectManager->GetSpriteShader(L"shader");

	black = objectManager->GetSprite(L"black");
	blackShader = objectManager->GetSpriteShader(L"shader");

	time = 0;

	for (int i = 0; i < starPosSize; i++)
	{
		starPos[i].radian = i % 3 / 3.0f * DirectX::XM_2PI + i / 0.17f;
		starPos[i].time = i * 50;
		starPos[i].speed = i % 3 + 1;
	}
}

void Space::Update()
{
	time += Timer::GetInstance().GetDeltaTime();

	float scrollParam = static_cast<float>(time % 2000) / 2000.0f;
	space->Scroll(scrollParam);

	for (int i = 0; i < starPosSize; i++)
	{
		starPos[i].time += Timer::GetInstance().GetDeltaTime() / starPos[i].speed;
		starPos[i].time = (starPos[i].time > 10000) ? 0 : starPos[i].time;
	}
}

void Space::EndUpdate()
{

}

void Space::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);
	// 描画実行
	pDeviceContext->DrawIndexed(space->GetIndexCount(), 0, 0);

	star->DrawSet(pDeviceContext);
	starShader->DrawSet(pDeviceContext);
	for (int i = 0; i < starPosSize; i++)
	{
		DrawSetStar(pDeviceContext, starPos[i]);
		pDeviceContext->DrawIndexed(star->GetIndexCount(), 0, 0);
	}

	DrawSetBlack(pDeviceContext);
	pDeviceContext->DrawIndexed(black->GetIndexCount(), 0, 0);

	DrawSetGrid(pDeviceContext);
	// 描画実行
	pDeviceContext->DrawIndexed(grid->GetIndexCount(), 0, 0);

}

void Space::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 10.0f, 10.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);

	world *= scale * rotate * offset;

	// パラメータの受け渡し
	SPRITE::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	spaceShader->SetConstantBuffer(pDeviceContext, cb);


	space->DrawSet(pDeviceContext);
	spaceShader->DrawSet(pDeviceContext);
}


void Space::DrawSetGrid(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 35.0f, 8.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f);

	world *= scale * rotate * offset;

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	gridShader->SetConstantBuffer(pDeviceContext, cb);


	grid->DrawSet(pDeviceContext);
	gridShader->DrawSet(pDeviceContext);
}

void Space::DrawSetStar(ComPtr<ID3D11DeviceContext> pDeviceContext, StarPos pos)
{
	// パラメータの計算
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 10.0f, 10.0f);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(cosf(pos.radian) * (pos.time / 100.0f), sinf(pos.radian) * (pos.time / 100.0f),
		8.0f * sinf(-pos.time / 10000.0f * DirectX::XM_PIDIV2));
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f);

	world *= scale * rotate * offset * trans;

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	gridShader->SetConstantBuffer(pDeviceContext, cb);

}

void Space::DrawSetBlack(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 10.0f, 9.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2 + time / 1000.0f);
	float scaleParam = 11.0f + 3.0f * sinf(time / 1000.0f);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(scaleParam, scaleParam, scaleParam);

	world *= scale * rotate * offset;

	// パラメータの受け渡し
	SPRITE::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	blackShader->SetConstantBuffer(pDeviceContext, cb);


	black->DrawSet(pDeviceContext);
	blackShader->DrawSet(pDeviceContext);
}

void Space::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}
