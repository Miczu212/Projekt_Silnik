#include "Font.h"
#include"d2d1.h"
#include"ConstantDefines.h"
#include"ComManager.h"
#include "WND.h"
#include "Player.h"
#include "Timer.h"
#include <filesystem>
#include <iostream>
#include <wincodec.h>
#include <ShObjIdl.h>
#include <gdiplus.h>
#include<vector>
#include"LevelInstance.h"
#include"TextureInstance.h"
#include"FunctionDefines.h"
#include <codecvt>
#include "SoundHandler.h"
Font::Font()
{

}
void Font::InitializeFont(Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, std::vector< Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const

{
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 32; i++)
		{
			LoadLetter(pRenderTarget, D2D1::RectF(i * 13, j * 24, i * 13 + 13, j * 24 + 24), ppBitmap);
		}
	}
}
void Font::LoadLetter(Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, const D2D1_RECT_F& sourceRegion, std::vector< Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const
{
	const std::wstring filePath = L"Q:\\Studia\\inne\\SILNIK\\Projekt_Silnik\\Fonts\\Consolas13x24.bmp";
	Microsoft::WRL::ComPtr<IWICImagingFactory> pWICFactory;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> pDecoder;
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pFrame;
	Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	if (SUCCEEDED(hr))
	{
		hr = pWICFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
			if (SUCCEEDED(hr))
			{
				hr = pWICFactory->CreateFormatConverter(&pConverter);
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
					if (SUCCEEDED(hr))
					{

						UINT width, height;
						hr = pConverter->GetSize(&width, &height);
						if (SUCCEEDED(hr))
						{
							assert(sourceRegion.right <= width);
							assert(sourceRegion.bottom <= height);
							// subregion musi siê mieœciæ w bitmapie

							UINT subWidth = static_cast<UINT>(sourceRegion.right - sourceRegion.left);
							UINT subHeight = static_cast<UINT>(sourceRegion.bottom - sourceRegion.top);

							std::vector<BYTE> pixelData(subWidth * subHeight * 4);
							D2D1_BITMAP_PROPERTIES bitmapProperties;
							bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
							bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
							bitmapProperties.dpiX = 96.0f;
							bitmapProperties.dpiY = 96.0f;

							WICRect rect = { static_cast<INT>(sourceRegion.left), static_cast<INT>(sourceRegion.top), static_cast<INT>(sourceRegion.right)-static_cast<INT>(sourceRegion.left), static_cast<INT>(sourceRegion.bottom)- static_cast<INT>(sourceRegion.top) };
							hr = pConverter->CopyPixels(&rect, subWidth * 4, subWidth * subHeight * 4, pixelData.data());
							if (SUCCEEDED(hr))
							{
								for (UINT i = 0; i < subWidth * subHeight; ++i)
								{
									if (*reinterpret_cast<UINT32*>(&pixelData[i * 4]) == 0xFFFF00FF)
									{
										*reinterpret_cast<UINT32*>(&pixelData[i * 4]) = 0; //alpha to 0
									}
								}
								//faktyczne tworzenie bitmapy
								Microsoft::WRL::ComPtr<ID2D1Bitmap> pBitmap;
								hr = pRenderTarget->CreateBitmap(
									D2D1::SizeU(subWidth, subHeight),
									pixelData.data(),
									subWidth * 4,
									&bitmapProperties,
									pBitmap.GetAddressOf()
								);
								ppBitmap.push_back(pBitmap);

							}
						}
					}
				}
			}
		}
	}
}
