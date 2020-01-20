#include"SceneController.h"
#include"Title/Title.h"
#include"Battle/Battle.h"

SceneController::SceneController(ID3D11Device* pDevice)
{
	scene = std::make_unique<Battle>(pDevice);
}

SceneController::~SceneController()
{

}

void SceneController::Update()
{
	scene->Update();
}

void SceneController::Draw(ID3D11DeviceContext* pDeviceContext)
{
	scene->Draw(pDeviceContext);
}

void SceneController::EndUpdate(ID3D11Device* pDevice)
{
	scene->EndUpdate();
	ChackNextScene(pDevice);
}

void SceneController::ChackNextScene(ID3D11Device* pDevice)
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