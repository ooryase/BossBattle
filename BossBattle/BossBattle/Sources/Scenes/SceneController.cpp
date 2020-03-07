#include"SceneController.h"
#include"Title/Title.h"
#include"Battle/Battle.h"
#include <time.h>

SceneController::SceneController(ComPtr<ID3D11Device> pDevice)
{
	scene = std::make_unique<Title>(pDevice);
	Timer::GetInstance().Init();
	srand((unsigned int)time(NULL));

	isDelete = false;
}

SceneController::~SceneController()
{}

void SceneController::Update()
{
	Timer::GetInstance().Update();
	scene->Update();
}

void SceneController::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	scene->Draw(pDeviceContext);
}

void SceneController::DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	scene->DrawAfterRadialBlur(pDeviceContext);
}

void SceneController::EndUpdate(ComPtr<ID3D11Device> pDevice)
{
	scene->EndUpdate();
	ChackNextScene(pDevice);

	Timer::GetInstance().Sleep();
}

void SceneController::ChackNextScene(ComPtr<ID3D11Device> pDevice)
{
	if (scene->GetNextScene() == SceneName::NONE)
		return;

	auto nextScene = scene->GetNextScene();
	scene.reset();

	switch (nextScene)
	{
	case SceneName::TITLE:
		scene = std::make_unique<Title>(pDevice);
		break;
	case SceneName::SELECT:
		//scene = std::make_unique<>();
		break;
	case SceneName::BATTLE:
		scene = std::make_unique<Battle>(pDevice);
		break;
	case SceneName::CLEAR:
		//scene = std::make_unique<>();
		break;
	case SceneName::QUIT:
		isDelete = true;
		break;
	default:
		return;
	}
}

bool SceneController::IsDelete() const
{
	return isDelete;
}