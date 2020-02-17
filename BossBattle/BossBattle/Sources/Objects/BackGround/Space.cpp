#include"Space.h"
#include"../../System/Timer.h"
#include"../../System/DeviceManager.h"

Space::Space(std::shared_ptr<ObjectManager> objectManager)
	: starPosSize(200)
{
	grid = objectManager->GetModel("grid2");
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
		starPos[i].time += Timer::GetInstance().GetDeltaTime() * starPos[i].speed;
		starPos[i].time = (starPos[i].time > 10000) ? 0 : starPos[i].time;
	}
}

void Space::EndUpdate()
{

}

void Space::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);
	// �`����s
	pDeviceContext->DrawIndexed(space->GetIndexCount(), 0, 0);

}

void Space::DrawBillBoard(ComPtr<ID3D11DeviceContext> pDeviceContext, DirectX::XMFLOAT3 eyeDirection)
{
	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ADD);
	star->DrawSet(pDeviceContext);
	starShader->DrawSet(pDeviceContext);
	for (int i = 0; i < starPosSize; i++)
	{
		DrawSetStar(pDeviceContext, starPos[i], eyeDirection);
		pDeviceContext->DrawIndexed(star->GetIndexCount(), 0, 0);
	}

	DeviceManager::GetInstance().SetBerendState(BLEND_STATE::ALIGMENT);
	DrawSetBlack(pDeviceContext);
	pDeviceContext->DrawIndexed(black->GetIndexCount(), 0, 0);

	DrawSetGrid(pDeviceContext);
	// �`����s
	pDeviceContext->DrawIndexed(grid->GetIndexCount(), 0, 0);
}


void Space::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// �p�����[�^�̌v�Z
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 10.0f, 50.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, 0.0f);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(9.0f, 9.0f, 12.0f);

	world *= scale * rotate * offset;

	// �p�����[�^�̎󂯓n��
	SPRITE::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	spaceShader->SetConstantBuffer(pDeviceContext, cb);


	space->DrawSet(pDeviceContext);
	spaceShader->DrawSet(pDeviceContext);
}


void Space::DrawSetGrid(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// �p�����[�^�̌v�Z
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 35.0f, 10.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f);

	world *= scale * rotate * offset;

	// �p�����[�^�̎󂯓n��
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	gridShader->SetConstantBuffer(pDeviceContext, cb);


	grid->DrawSet(pDeviceContext);
	gridShader->DrawSet(pDeviceContext);
}

void Space::DrawSetStar(ComPtr<ID3D11DeviceContext> pDeviceContext, StarPos pos, DirectX::XMFLOAT3 eyeDirection)
{
	// �p�����[�^�̌v�Z
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 10.0f, -5.0f);
	float transLenght = sinf( pos.time / 10000.0f * DirectX::XM_PIDIV4 * 3.0f);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(cosf(pos.radian) * 60.0f * transLenght, sinf(pos.radian) * 40.0f * transLenght, 50.0f * cosf(pos.time / 10000.0f * DirectX::XM_PIDIV4 * 3.0f));
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(
		-atan2f(eyeDirection.y, eyeDirection.x),
		DirectX::XM_PIDIV2 - atan2f(eyeDirection.z, eyeDirection.x),
		-atan2f(eyeDirection.y, eyeDirection.z));
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);

	world *= scale * rotate * trans * offset;

	// �p�����[�^�̎󂯓n��
	MODEL::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	gridShader->SetConstantBuffer(pDeviceContext, cb);

}

void Space::DrawSetBlack(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// �p�����[�^�̌v�Z
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.0f, 10.0f, 44.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XM_PI, 0.0f, -DirectX::XM_PIDIV2 + time / 1000.0f);
	float scaleParam = 16.0f + 4.0f * sinf(time / 500.0f);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(scaleParam + 16.0f, scaleParam, scaleParam);

	world *= rotate * scale * offset;

	// �p�����[�^�̎󂯓n��
	SPRITE::CONSTANT_BUFFER cb;

	DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(world));

	blackShader->SetConstantBuffer(pDeviceContext, cb);


	black->DrawSet(pDeviceContext);
	blackShader->DrawSet(pDeviceContext);
}

void Space::OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta)
{

}
