#include"Camera.h"
#include"../System/Timer.h"

Camera::Camera(DirectX::XMFLOAT3 _eyeLookAt, DirectX::XMFLOAT3 _eyePos)
{
	eyeLookAt = _eyeLookAt;
	eyePos = _eyePos;
	state = State::STOP;

	isQuake = false;
}

void Camera::Update()
{
	if (isQuake)
		UpdateQuake();

	if (state != State::STOP)
		UpdateMove();
}

void Camera::UpdateQuake()
{
	quakeTime += Timer::GetInstance().GetDeltaTime();

	const int QUAKE_TIME = 500;
	const int END_TIME_Y = 50;
	const int START_TIME_XZ = 50;
	if (quakeTime < QUAKE_TIME - END_TIME_Y)
	{
		float rate = quakeTime / static_cast<float>(QUAKE_TIME - END_TIME_Y);
		quakeQuantity.y = sinf( DirectX::XM_PI * 4.0f * rate) * (1.0f - rate * rate);
	}

	if (quakeTime > START_TIME_XZ)
	{
		float eyeLenghtX = abs(eyeLookAt.x - eyePos.x);
		float eyeLenghtZ = abs(eyeLookAt.z - eyePos.z);
		float quantityZ = eyeLenghtX / (eyeLenghtX + eyeLenghtZ);
		float quantityX = eyeLenghtZ / (eyeLenghtX + eyeLenghtZ);
		float rate = (quakeTime - START_TIME_XZ) / static_cast<float>(QUAKE_TIME - START_TIME_XZ);
		//quakeQuantity.x = cosf(DirectX::XM_PI * 4.0f * rate) * quantityX * (1.0f - rate * rate);
		//quakeQuantity.z = cosf(DirectX::XM_PI * 4.0f * rate) * quantityZ * (1.0f - rate * rate);
	}

	if (quakeTime > QUAKE_TIME)
		isQuake = false;
}

void Camera::UpdateMove()
{
	auto delta = currentTime / static_cast<float>(moveTime);
	switch (state)
	{
	case Camera::State::LINER:
		eyePos = AddFloat3(MultiFloat3(endEyePos, delta), MultiFloat3(startEyePos, 1.0f - delta));
		eyeLookAt = AddFloat3(MultiFloat3(endLookAt, delta), MultiFloat3(startLookAt, 1.0f - delta));
		break;
	case Camera::State::SQUARE:
		eyePos = AddFloat3(MultiFloat3(endEyePos, 1.0f - (1.0f - delta) * (1.0f - delta)), MultiFloat3(startEyePos, (1.0f - delta) * (1.0f - delta)));
		eyeLookAt = AddFloat3(MultiFloat3(endLookAt, 1.0f - (1.0f - delta) * (1.0f - delta)), MultiFloat3(startLookAt, (1.0f - delta) * (1.0f - delta)));
		break;
	case Camera::State::CURVE:
		eyeLookAt = AddFloat3(MultiFloat3(endLookAt, delta), MultiFloat3(startLookAt, 1.0f - delta));
		eyePos = AddFloat3(
			endLookAt, MultiFloat3(
				NormalizeFloat3(
					AddFloat3(startEyePos,
						DirectX::XMFLOAT3(
							cosf(endEyePos.y * delta) + sinf(endEyePos.z * delta),
							cosf(endEyePos.z * delta) + sinf(endEyePos.x * delta),
							cosf(endEyePos.x * delta) + sinf(endEyePos.y * delta)
						))),
				endLenght));
		break;
	default:
		break;
	}

	currentTime += Timer::GetInstance().GetDeltaTime();
	if (currentTime > moveTime)
	{
		if (state == State::CURVE)
		{
			eyePos = AddFloat3(
				endLookAt, MultiFloat3(
					NormalizeFloat3(
						AddFloat3(startEyePos,
							DirectX::XMFLOAT3(
								cosf(endEyePos.y) + sinf(endEyePos.z),
								cosf(endEyePos.z) + sinf(endEyePos.x),
								cosf(endEyePos.x) + sinf(endEyePos.y)
							))),
					endLenght));
		}
		else
			eyePos = endEyePos;
		eyeLookAt = endLookAt;
		state = State::STOP;
	}
}

void Camera::SetCameraPos(State _state, DirectX::XMFLOAT3 _endEyePos, DirectX::XMFLOAT3 _endLookAt, int _moveTime, float _lenght)
{
	if (_moveTime == 0)
	{
		state = State::STOP;
		eyePos = _endEyePos;
		eyeLookAt = _endLookAt;
		return;
	}

	startEyePos = eyePos;
	startLookAt = eyeLookAt;

	state = _state;
	endEyePos = _endEyePos;
	endLookAt = _endLookAt;
	moveTime = _moveTime;
	currentTime = 0;

	if (state == State::CURVE)
	{
		startEyePos = NormalizeFloat3(DirectX::XMFLOAT3(eyeLookAt.x - eyePos.x, eyeLookAt.y - eyePos.y, eyeLookAt.z - eyePos.z));
		startLenght = sqrtf(
			(eyeLookAt.x - eyePos.x) * (eyeLookAt.x - eyePos.x)
			+ (eyeLookAt.y - eyePos.y) * (eyeLookAt.y - eyePos.y)
			+ (eyeLookAt.z - eyePos.z) * (eyeLookAt.z - eyePos.z));
		endLenght = _lenght;
	}
}

void Camera::Quake()
{
	isQuake = true;
	quakeTime = 0;
	quakeQuantity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

DirectX::XMFLOAT3 Camera::GetEyeDirection() const
{
	return NormalizeFloat3(DirectX::XMFLOAT3(eyeLookAt.x - eyePos.x, eyeLookAt.y - eyePos.y, eyeLookAt.z - eyePos.z));
}


DirectX::XMMATRIX Camera::GetView() const
{
	DirectX::XMFLOAT3 setEyePos = isQuake ? AddFloat3(eyePos, quakeQuantity) : eyePos;
	DirectX::XMVECTOR eye_pos = DirectX::XMVectorSet(setEyePos.x,setEyePos.y,setEyePos.z, 1.0f);
	DirectX::XMVECTOR eye_lookat = DirectX::XMVectorSet(eyeLookAt.x, eyeLookAt.y, eyeLookAt.z, 1.0f);
	DirectX::XMVECTOR eye_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);

	return View;
}

DirectX::XMFLOAT3 Camera::NormalizeFloat3(DirectX::XMFLOAT3 f3) const
{
	auto l = std::sqrt(f3.x * f3.x + f3.y * f3.y + f3.z * f3.z);

	return DirectX::XMFLOAT3(f3.x / l, f3.y / l, f3.z / l);
}

DirectX::XMFLOAT3 Camera::AddFloat3(DirectX::XMFLOAT3 f1, DirectX::XMFLOAT3 f2) const
{
	return DirectX::XMFLOAT3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z);
}

DirectX::XMFLOAT3 Camera::MultiFloat3(DirectX::XMFLOAT3 f3, float f) const
{
	return DirectX::XMFLOAT3(f3.x * f, f3.y * f, f3.z * f);
}