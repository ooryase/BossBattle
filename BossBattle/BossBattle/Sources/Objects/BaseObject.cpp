#include"BaseObject.h"
#include"../System/InputController.h"

BaseObject::BaseObject()
{
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotateDef = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);

}

BaseObject::~BaseObject()
{

}

void BaseObject::EndUpdate()
{

}

void BaseObject::DrawSet(ID3D11DeviceContext* pDeviceContext)
{
	// パラメータの計算
	DirectX::XMMATRIX m_World = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX m_RotateDef = DirectX::XMMatrixRotationRollPitchYaw(rotateDef.x, rotateDef.y, rotateDef.z);
	DirectX::XMMATRIX m_Rotate2 = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	DirectX::XMMATRIX m_Rotate = XMMatrixTranspose(m_RotateDef * m_Rotate2);
	DirectX::XMMATRIX m_Scale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	m_World *= m_Scale * m_Rotate * m_Offset;

	DirectX::XMVECTOR m_Light = DirectX::XMVectorSet(std::cos(4.0f) * 40.1f, -20.0f, std::sin(4.0f) * 40.1f, 0.0f);
	DirectX::XMVECTOR m_Attenuation = DirectX::XMVectorSet(1.0f, 0.0005f, 0.0005f, 0.0f);

	// パラメータの受け渡し
	MODEL::CONSTANT_BUFFER ccb;

	DirectX::XMStoreFloat4x4(&ccb.World, DirectX::XMMatrixTranspose(m_World));
	DirectX::XMStoreFloat4(&ccb.Light, m_Light);
	DirectX::XMStoreFloat4(&ccb.Attenuation, m_Attenuation);

	shader->SetConstantBuffer(pDeviceContext, ccb);
}

void BaseObject::Draw(ID3D11DeviceContext* pDeviceContext)
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