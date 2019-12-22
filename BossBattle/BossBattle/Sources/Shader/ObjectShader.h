#pragma once
#include"../System/Resource.h"

class ObjectShader
{
	ID3D11InputLayout* pVertexLayout;

	ID3D11VertexShader* pVertexShader;
	ID3D11GeometryShader* pGeometryShader;
	ID3D11PixelShader* pPixelShader;

public:
	ObjectShader(ID3D11Device* pDevice, const wchar_t fileName[]);
	~ObjectShader();
};