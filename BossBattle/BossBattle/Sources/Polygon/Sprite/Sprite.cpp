#include"Sprite.h"

using SPRITE::VERTEX;
using namespace DirectX;

Sprite::Sprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[])
{
	TextureLoad(pDevice, filename);
}

Sprite::~Sprite()
{}

void Sprite::TextureLoad(ComPtr<ID3D11Device> pDevice, const wchar_t filename[])
{
	ComPtr<IWICImagingFactory> pFactory;
	ComPtr<IWICBitmapDecoder> pDecoder;
	ComPtr<IWICBitmapFrameDecode> pFrame;
	ComPtr<IWICFormatConverter> pFormatConverter;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(pFactory.GetAddressOf()));
	pFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, pDecoder.GetAddressOf());
	pDecoder->GetFrame(0, pFrame.GetAddressOf());
	pFactory->CreateFormatConverter(pFormatConverter.GetAddressOf());
	pFormatConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeErrorDiffusion, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);

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
	pDevice->CreateSamplerState(&smpdesc, pSampler.GetAddressOf());

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
	pDevice->CreateTexture2D(&texdec, &initData, pTexture.GetAddressOf());

	// シェーダリソースビュー(テクスチャ用)の設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(pTexture.Get(), &srv, pSRV.GetAddressOf());
}

void Sprite::Scroll(float param)
{
}

void Sprite::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVerBuffer.GetAddressOf(), &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	pDeviceContext->PSSetShaderResources(0,1,pSRV.GetAddressOf());
	pDeviceContext->RSSetState(pRasterizerState.Get());
}