#include "Grafika.h"
#define GFX_THROW_FAILED(hrcall) if ( FAILED(result=(hrcall))) throw Grafika::HrException(__LINE__,__FILE__,result)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Grafika::DeviceRemovedexc(__LINE__,__FILE__,(hr))
Grafika::Grafika(HWND hwnd, RECT recti)
{
	rect = recti;			
	GFX_THROW_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pFactory.GetAddressOf()));
	GetClientRect(hwnd, &rect);
	GFX_THROW_FAILED(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd,D2D1::SizeU(rect.right,rect.bottom)), pRTarget.GetAddressOf()));
}

Grafika::~Grafika()
// uwolnienie zasobów;
{
}

void Grafika::ClearBuffer(float r, float g, float b)
{
	pRTarget->Clear(D2D1_COLOR_F(r, g, b));
}


void Grafika::BeginFrame()
{
	pRTarget->BeginDraw();
}

void Grafika::ENDFRAME()
{
	pRTarget->EndDraw();
}

void Grafika::Draw1()
{
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
	GFX_THROW_FAILED(pRTarget->CreateSolidColorBrush(D2D1::ColorF(255, 0, 0), pBrush.GetAddressOf()));
	pRTarget->DrawLine(D2D1::Point2F(0, 0), D2D1::Point2F(100, 100),pBrush.Get(),3.0f);
}

const char* Grafika::DeviceRemovedexc::ErrorType()
{
	return "B³¹d Urz¹dzenia";
}

Grafika::HrException::HrException(int line, const char* file, HRESULT hr)
	:
	gException(line, file)

{
	this->hr = hr;
}

const char* Grafika::HrException::Result()
{
	 std::ostringstream oss;
	oss << ErrorType() << std::endl <<
		"[Kod B³êdu] " <<std::hex<< GetErrorCode() <<std::dec<<"("<<(unsigned long)GetErrorCode()<< std::endl <<
		"[Opis] " << GetErrorDescription() << std::endl <<
		"[ErrorString]"<< GetErrorString() << OGString();
	bufor = oss.str();
	return bufor.c_str();;
}

const char* Grafika::HrException::ErrorType()
{
	return "B³¹d Graficzny";
}


HRESULT Grafika::HrException::GetErrorCode()
{
	return hr;
}

std::string Grafika::HrException::GetErrorString()
{
	return DXGetErrorStringA(hr);
}

std::string Grafika::HrException::GetErrorDescription()
{
	char buff[512];
	  DXGetErrorDescriptionA(hr,buff,sizeof(buff) );
	  return buff;
}



