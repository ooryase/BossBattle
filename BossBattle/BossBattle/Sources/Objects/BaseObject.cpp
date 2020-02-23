#include"BaseObject.h"
#include"../System/InputController.h"

BaseObject::BaseObject()
{
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotateDef = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);

	isToDelete = false;
}

BaseObject::~BaseObject()
{

}

DirectX::XMFLOAT3 BaseObject::GetPos() const
{
	return position;
}

float BaseObject::GetRadius() const
{
	return radius;
}

ObjectTag BaseObject::GetTag() const
{
	return tag;
}

void BaseObject::SetTag(ObjectTag _tag)
{
	tag = _tag;
}

bool BaseObject::IsDead() const
{
	return isToDelete;
}

bool Light::SetPointLight(DirectX::XMFLOAT4 _pos,
	DirectX::XMFLOAT4 _col,
	DirectX::XMFLOAT4 _attenuation)
{
	if (PointCount > 7)
		return false;

	Pos[PointCount] = _pos;
	Color[PointCount] = _col;
	Attenuation[PointCount] = _attenuation;
	PointCount++;
	return true;
}
