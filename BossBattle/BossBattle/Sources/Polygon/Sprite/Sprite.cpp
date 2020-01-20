#include"Sprite.h"

using namespace SPRITE;
using namespace DirectX;

Sprite::Sprite(ID3D11Device* pDevice, const wchar_t filename[])
{
	TextureLoad(pDevice, filename);
	CreateVertexBuffer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateRasterizerState(pDevice);
}

Sprite::~Sprite()
{
	SAFE_RELEASE(pVerBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pRasterizerState);
}

void Sprite::TextureLoad(ID3D11Device* pDevice, const wchar_t filename[])
{
	IWICImagingFactory* pFactory = NULL;
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pFrame = NULL;
	IWICFormatConverter* pFormatConverter = NULL;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&pFactory));
	pFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	pDecoder->GetFrame(0, &pFrame);
	pFactory->CreateFormatConverter(&pFormatConverter);
	pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeErrorDiffusion, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);

	UINT imgWidth;
	UINT imgHeight;
	pFormatConverter->GetSize(&imgWidth, &imgHeight);

	//サンプラーの設定
	D3D11_SAMPLER_DESC smpdesc;
	ZeroMemory(&smpdesc, sizeof(D3D11_SAMPLER_DESC));
	smpdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pDevice->CreateSamplerState(&smpdesc, &pSampler);

	std::vector<unsigned char> buffer(imgWidth * imgHeight * 4);
	pFormatConverter->CopyPixels(NULL, imgWidth * 4, imgWidth * imgHeight * 4, &buffer[0]);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &buffer[0];
	initData.SysMemPitch = imgWidth * 4;
	initData.SysMemSlicePitch = imgWidth * imgHeight * 4;

	// テクスチャの設定
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth;
	texdec.Height = imgHeight;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	pDevice->CreateTexture2D(&texdec, &initData, &pTexture);

	// シェーダリソースビュー(テクスチャ用)の設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV);

	SAFE_RELEASE(pFormatConverter);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFactory);
}

void Sprite::CreateVertexBuffer(ID3D11Device* pDevice)
{
	vertices.push_back(VERTEX(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT2(0, 1)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.5f, 0.5f, 0.0f), XMFLOAT2(0, 0)));
	vertices.push_back(VERTEX(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT2(1, 1)));
	vertices.push_back(VERTEX(XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT2(1, 0)));

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX) * vertices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();
	pDevice->CreateBuffer(&bd, &initData, &pVerBuffer);
}

void Sprite::CreateIndexBuffer(ID3D11Device* pDevice)
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices.data();
	pDevice->CreateBuffer(&bd, &initData, &pIndexBuffer);
}

void Sprite::CreateRasterizerState(ID3D11Device* pDevice)
{
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0;
	rdc.SlopeScaledDepthBias = 0;
	rdc.DepthClipEnable = TRUE;
	rdc.ScissorEnable = FALSE;
	rdc.MultisampleEnable = FALSE;
	rdc.AntialiasedLineEnable = FALSE;
	pDevice->CreateRasterizerState(&rdc, &pRasterizerState);
}

void Sprite::DrawSet(ID3D11DeviceContext* pDeviceContext)
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVerBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->PSSetSamplers(0, 1, &pSampler);
	pDeviceContext->PSSetShaderResources(0,1,&pSRV);
	pDeviceContext->RSSetState(pRasterizerState);
}