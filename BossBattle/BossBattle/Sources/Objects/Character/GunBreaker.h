#pragma once
#include "BaseCharacter.h"
#include "GunBehave/GunBehave.h"
//#include"../../System/Resource.h"
//#include "../../Polygon/Model/FbxModel.h"
//#include "../../Shader/ModelShader.h"
//#include "../../Polygon/ObjectManager.h"

class GunBreaker : public BaseCharacter, public std::enable_shared_from_this<GunBreaker>
{

public:

	GunBreaker( std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light);
	~GunBreaker();

	std::shared_ptr<GunBehave> behave;

	void Update();
	void EndUpdate();
	void Draw(ID3D11DeviceContext* pDeviceContext) override;

private:
	void DrawSet(ID3D11DeviceContext* pDeviceContext) override;
};