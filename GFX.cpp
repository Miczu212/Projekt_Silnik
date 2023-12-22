#include "GFX.h"
#define GFX_THROW_FAILED(hrcall) if ( FAILED(result=(hrcall))) throw GFX::HrException(__LINE__,__FILE__,result)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) GFX::DeviceRemovedexc(__LINE__,__FILE__,(hr))
GFX::GFX(HWND hwnd, RECT recti)
{
	rect = recti;			
	GFX_THROW_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pFactory.GetAddressOf()));
	GetClientRect(hwnd, &rect);
	GFX_THROW_FAILED(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd,D2D1::SizeU(rect.right,rect.bottom)), pRTarget.GetAddressOf()));
}

GFX::~GFX()
// uwolnienie zasobów;
{

}

void GFX::EndFrame()
{
	pRTarget->EndDraw();
}

void GFX::ClearBuffer(float r, float g, float b)
{
	pRTarget->Clear(D2D1_COLOR_F(r, g, b));
}


void GFX::BeginFrame()
{
	pRTarget->BeginDraw();
}


void GFX::Draw()
{
	
	GFX_THROW_FAILED(pRTarget->CreateSolidColorBrush(D2D1::ColorF(255, 0, 0), pBrush.GetAddressOf()));
	pRTarget->DrawLine(D2D1::Point2F(0, 0), D2D1::Point2F(100, 100),pBrush.Get(),3.0f);
}

const char* GFX::DeviceRemovedexc::ErrorType()
{
	return "B³¹d Urz¹dzenia";
}

GFX::HrException::HrException(int line, const char* file, HRESULT hr)
	:
	gException(line, file)

{
	this->hr = hr;
}

const char* GFX::HrException::Result()
{
	 std::ostringstream oss;
	oss << ErrorType() << std::endl <<
		"[Kod B³êdu] " <<std::hex<< GetErrorCode() <<std::dec<<"("<<(unsigned long)GetErrorCode()<< std::endl <<
		"[Opis] " << GetErrorDescription() << std::endl <<
		"[ErrorString]"<< GetErrorString() << OGString();
	bufor = oss.str();
	return bufor.c_str();;
}

const char* GFX::HrException::ErrorType()
{
	return "B³¹d Graficzny";
}


HRESULT GFX::HrException::GetErrorCode()
{
	return hr;
}

std::string GFX::HrException::GetErrorString()
{
	return DXGetErrorStringA(hr);
}

std::string GFX::HrException::GetErrorDescription()
{
	char buff[512];
	  DXGetErrorDescriptionA(hr,buff,sizeof(buff) );
	  return buff;
}



