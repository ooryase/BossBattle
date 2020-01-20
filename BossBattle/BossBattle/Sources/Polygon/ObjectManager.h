#pragma once
#include"../System/Resource.h"
#include<map>
#include"Model/BaseModel.h"
#include"Model/FbxModel.h"
#include"Sprite/Sprite.h"
#include"../Shader/ModelShader.h"
#include"../Shader/SpriteShader.h"


class ObjectManager
{
	std::map<std::string, std::shared_ptr<BaseModel>> modelMap;
	std::map<std::wstring, std::shared_ptr<Sprite>> spriteMap;
	std::map<std::wstring, std::shared_ptr<ModelShader>> modelShader;
	std::map<std::wstring, std::shared_ptr<SpriteShader>> spriteShader;

public:
	ObjectManager();
	~ObjectManager();

	void SstModelMap(ID3D11Device* pDevice, std::string);
	void SstSpriteMap(ID3D11Device* pDevice, std::wstring);
	void SstModelShader(ID3D11Device* pDevice, std::wstring);
	void SstSpriteShader(ID3D11Device* pDevice, std::wstring);

	std::shared_ptr<BaseModel> GetModel(std::string key);
	std::shared_ptr<Sprite> GetSprite(std::wstring key);
	std::shared_ptr<ModelShader> GetModelShader(std::wstring key);
	std::shared_ptr<SpriteShader> GetSpriteShader(std::wstring key);
};