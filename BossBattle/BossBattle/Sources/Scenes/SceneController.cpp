#include"SceneController.h"
#include"Title/Title.h"
#include"Battle/Battle.h"

SceneController::SceneController(ComPtr<ID3D11Device> pDevice)
{
	scene = std::make_unique<Battle>(pDevice);
	Timer::GetInstance().Init();
}

SceneController::~SceneController()
{

}

void SceneController::Update()
{
	Timer::GetInstance().Update();
	scene->Update();
}

void SceneController::Draw(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	scene->Draw(pDeviceContext);
}

void SceneController::EndUpdate(ComPtr<ID3D11Device> pDevice)
{
	scene->EndUpdate();
	ChackNextScene(pDevice);

	Timer::GetInstance().Sleep();
}

void SceneController::ChackNextScene(ComPtr<ID3D11Device> pDevice)
{
	switch (scene->GetNextScene())
	{
	case SceneName::TITLE:
		scene = std::make_unique<Title>(pDevice);
		break;
	case SceneName::SELECT:
		//scene = std::make_unique<>();
		break;
	case SceneName::BATTLE:
		//scene = std::make_unique<>();
		break;
	case SceneName::CLEAR:
		//scene = std::make_unique<>();
		break;
	default:
		return;
	}
}