#pragma once
#include"Resource.h"

enum class BLEND_STATE
{
	ALIGMENT,
	ADD,
};

class DeviceManager
{
	DeviceManager(const DeviceManager&) = delete;
	DeviceManager operator = (const DeviceManager&) = delete;
	DeviceManager() {};

	UINT CLIENT_WIDTH;
	UINT CLIENT_HEIGHT;

	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pDeviceContext;
	ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	ComPtr<ID3D11Texture2D> pDepthStencil;
	ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	ComPtr<ID3D11BlendState> pBlendState;
	ComPtr<ID3D11BlendState> pAddBlendState;

	D3D11_VIEWPORT viewPort[1];

	////////////////////////////////////////////////////////
	ComPtr<ID3D11Texture2D> pRTTexture;		// �`��\�e�N�X�`��
	ComPtr<ID3D11RenderTargetView> pRTTextureRTV;	// �`��^�[�Q�b�g �r���[
	ComPtr<ID3D11ShaderResourceView> pRTTextureSRV;	// �V�F�[�_ ���\�[�X �r���[
	ComPtr<ID3D11Texture2D> pRTDepthStencil;		// �[�x/�X�e���V��
	ComPtr<ID3D11DepthStencilView> pRTDepthStencilView;	// �[�x/�X�e���V���E�r���[

	D3D11_VIEWPORT vpRTTexture[1];			// �`��\�e�N�X�`���̃r���[�|�[�g�ݒ�

	ComPtr<ID3D11InputLayout> pRTVertexLayout;
	ComPtr<ID3D11VertexShader> pRTVertexShader;   // ���_�V�F�[�_
	ComPtr<ID3D11GeometryShader> pRTGeometryShader; // �W�I���g���E�V�F�[�_
	ComPtr<ID3D11PixelShader> pRTPixelShader;    // �s�N�Z���E�V�F�[�_
	ComPtr<ID3D11Buffer> pRadialBlurBuffer;		// �萔�o�b�t�@
	ComPtr<ID3D11SamplerState> pRTTextureSampler; // �T���v���[

	ComPtr<ID3D11RasterizerState> pRTRasterizerState;

	ComPtr<ID3D11Buffer> pVerBuffer;
	ComPtr<ID3D11Buffer> pIndexBuffer;

	struct VERTEX
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 TexUV;

		VERTEX(DirectX::XMFLOAT3 _Pos, DirectX::XMFLOAT2 _uv) :
			Pos(_Pos), TexUV(_uv) {}
	};

	std::vector<VERTEX> vertices;
	std::vector<UINT> indices;

	struct RADIAL_BLUR
	{
		DirectX::XMFLOAT2 Center;
		DirectX::XMFLOAT2 Value;
	} radialBlur;
	bool isRadialBlur;
	int radialBlurTime;
	int radialBlurMaxTime;
	float radialBlurStrenght;

public:
	HRESULT Init(HWND WHandle);
	void Release();

	HRESULT CreateDeviceAndSwapChain(HWND WHandle);
	HRESULT CreateBackBuffer();
	HRESULT CreateBlendState();
	HRESULT CreateAddBlendState();
	HRESULT CreateRenderTarget();

	HRESULT CreateRadialBlur();


	void RenderBegin();

	void SetBerendState(BLEND_STATE blendState);
	void SetRadialBlur(DirectX::XMFLOAT2 pos, int time, float strenght);

	void RenderEnd();
private:
	void RenderToBackBuffer();
	void UpdateRadialBlur();

public:
	ComPtr<ID3D11Device> GetDevice() { return pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return pDeviceContext; }

	static DeviceManager& GetInstance();
};