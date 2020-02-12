#include"ObjectManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::SstModelMap(ComPtr<ID3D11Device> pDevice,std::string keyName)
{
	std::string filename = "Assets/Model/" + keyName + ".fbx";
	modelMap.insert(make_pair(keyName, std::make_shared<FbxModel>(pDevice, filename.c_str())));
}

void ObjectManager::SstSpriteMap(ComPtr<ID3D11Device> pDevice, std::wstring keyName)
{
	std::wstring filename = L"Assets/Textures/" + keyName + L".png";
	spriteMap.insert(make_pair(keyName, std::make_shared<DefaultSprite>(pDevice, filename.c_str())));

}

void ObjectManager::SstOctagonSpriteMap(ComPtr<ID3D11Device> pDevice, std::wstring keyName)
{
	std::wstring filename = L"Assets/Textures/" + keyName + L".png";
	spriteMap.insert(make_pair(keyName, std::make_shared<OctagonSprite>(pDevice, filename.c_str())));

}

void ObjectManager::SstModelShader(ComPtr<ID3D11Device> pDevice, std::wstring keyName)
{
	std::wstring filename = L"Shader/Model/" + keyName + L".hlsl";
	std::wstring edgeFilename = L"Shader/Model/" + keyName + L"Edge.hlsl";
	modelShader.insert(make_pair(keyName, std::make_shared<ModelShader>(pDevice, filename.c_str(), edgeFilename.c_str())));
}

void ObjectManager::SstSpriteShader(ComPtr<ID3D11Device> pDevice, std::wstring keyName)
{
	std::wstring filename = L"Shader/Sprite/" + keyName + L".hlsl";
	spriteShader.insert(make_pair(keyName, std::make_shared<SpriteShader>(pDevice, filename.c_str())));
}

std::shared_ptr < BaseModel> ObjectManager::GetModel(std::string key)
{
	return modelMap.at(key);
}

std::shared_ptr<Sprite> ObjectManager::GetSprite(std::wstring key)
{
	return spriteMap.at(key);
}

std::shared_ptr<ModelShader> ObjectManager::GetModelShader(std::wstring key)
{
	return modelShader.at(key);
}

std::shared_ptr<SpriteShader> ObjectManager::GetSpriteShader(std::wstring key)
{
	return spriteShader.at(key);
}
