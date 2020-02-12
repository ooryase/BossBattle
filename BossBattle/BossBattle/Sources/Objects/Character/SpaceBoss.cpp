#include"SpaceBoss.h"
#include"../../System/InputController.h"

SpaceBoss::SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
	std::vector< std::shared_ptr< BaseEffect>>& _effectReserves)
	: BaseCharacter(_light, _effectReserves, objectManager)
{
	model = objectManager->GetModel("spaceBoss");
	shader = objectManager->GetModelShader(L"shader");

	gauge = objectManager->GetSprite(L"Tex");
	gaugeShader = objectManager->GetSpriteShader(L"shader");

	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotateDef = DirectX::XMFLOAT3(-DirectX::XM_PIDIV2, 0.0f, DirectX::XM_PI);

	param = std::make_shared<Param>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0025f);
	//behave = std::make_shared<GunWait>(param);

	position.x = 10.0f;
	e = 1.0f;
	radius = 1.0f;

	tag = ObjectTag::NORMAL;

	model->SetAnimSackNumber(1);
}

SpaceBoss::~SpaceBoss()
{

}

void SpaceBoss::Update()
{
	//behave->Update(position, light);
	if (InputController::getInstance().IsPressKey(DIK_E))
	{
		position.x = 10.0f;
	}


	position.x += param->speed.x;
	position.y += param->speed.y;
	position.z += param->speed.z;

	param->speed.y -= param->gravity;
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
		param->ground = true;
	}
	else
		param->ground = false;

	model->Update();
}

void SpaceBoss::EndUpdate()
{
	//if (behave->IsNextBehave())
	//{
	//	GUN_BEHAVE::BehaveName temp = behave->GetNextBehave();
	//
	//	model->SetAnimSackNumber(static_cast<int>(temp));
	//
	//	behave.reset();
	//}

}

void SpaceBoss::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX m_World = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX m_RotateDef = DirectX::XMMatrixRotationRollPitchYaw(rotateDef.x, rotateDef.y, rotateDef.z);
	DirectX::XMMATRIX m_Rotate3 = DirectX::XMMatrixRotationRollPitchYaw(param->direction.x, param->direction.y, param->direction.z);
	DirectX::XMMATRIX m_Rotate2 = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	DirectX::XMMATRIX m_Rotate = XMMatrixTranspose(m_RotateDef * m_Rotate3 * m_Rotate2);
	DirectX::XMMATRIX m_Scale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	m_World *= m_Scale * m_Rotate * m_Offset;

	//DirectX::XMVECTOR m_Light = DirectX::XMVectorSet(std::cos(4.0f) * 40.1f, -20.0f, std::sin(4.0f) * 40.1f, 0.0f);
	//DirectX::XMVECTOR m_Attenuation = DirectX::XMVectorSet(1.0f, 0.0005f, 0.0005f, 0.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER ccb;

	DirectX::XMStoreFloat4x4(&ccb.World, DirectX::XMMatrixTranspose(m_World));

	shader->SetConstantBuffer(pDeviceContext, ccb);

}

void SpaceBoss::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSet(pDeviceContext);

	model->DrawSet(pDeviceContext);
	shader->DrawSet(pDeviceContext);

	// 描画実行
	pDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);

	model->DrawLineAdjSet(pDeviceContext);
	shader->DrawLineAdjSet(pDeviceContext);

	// 描画実行
	pDeviceContext->DrawIndexed(model->GetLineAdjIndexCount(), 0, 0);

}

void SpaceBoss::DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DirectX::XMMATRIX m_World = DirectX::XMMatrixTranslation(0.0f, 0.0f, -2.0f);
	m_World *= DirectX::XMMatrixScaling(1.6f, 1.6f, 1.6f);

	SPRITE::CONSTANT_BUFFER ccb2;
	DirectX::XMStoreFloat4x4(&ccb2.World, DirectX::XMMatrixTranspose(m_World));
	gaugeShader->SetConstantBuffer(pDeviceContext, ccb2);

	//DirectX::XMMATRIX World = DirectX::XMMatrixRotationY(x += 0.001f);
//DirectX::XMMATRIX Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f);
//World *= Offset;
}

void SpaceBoss::DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	DrawSetGauge(pDeviceContext);

	gauge->DrawSet(pDeviceContext);
	gaugeShader->DrawSet(pDeviceContext);

	//pDeviceContext->Draw(sprite->GetIndexCount(), 0);

	pDeviceContext->Draw(gauge->GetIndexCount(), 0);
}