#pragma once
#include"../System/Resource.h"

class Camera
{
	DirectX::XMFLOAT3 eyeLookAt;
	DirectX::XMFLOAT3 eyePos;

public:
	enum class State
	{
		STOP,
		LINER,
		SQUARE,
		CURVE,
	};
private:
	State state;

	DirectX::XMFLOAT3 startEyePos;
	DirectX::XMFLOAT3 endEyePos;
	DirectX::XMFLOAT3 startLookAt;
	DirectX::XMFLOAT3 endLookAt;
	int currentTime;
	int moveTime;
	float startLenght;
	float endLenght;

	bool isQuake;
	int quakeTime;

public:
	Camera(
		DirectX::XMFLOAT3 _eyeLookAt,
		DirectX::XMFLOAT3 _eyePos
	);
	

	void Update();

	void SetCameraPos(State _state, DirectX::XMFLOAT3 _endEyePos, DirectX::XMFLOAT3 _endLookAt, int _moveTime, float _length = 0.0f);
	void Quake();

	DirectX::XMFLOAT3 GetEyeDirection() const;

	DirectX::XMMATRIX GetView() const;

	DirectX::XMFLOAT3 NormalizeFloat3(DirectX::XMFLOAT3 f3) const;
	DirectX::XMFLOAT3 AddFloat3(DirectX::XMFLOAT3 f1, DirectX::XMFLOAT3 f2) const;
	DirectX::XMFLOAT3 MultiFloat3(DirectX::XMFLOAT3 f3, float f) const;
};