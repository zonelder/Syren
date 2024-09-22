#pragma once

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,(hr),_infoManager.getMessage())
#define GFX_THROW_INFO(hrcall) _infoManager.set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemoveException(__LINE__,__FILE__,(hr),_infoManager.getMessage())
#define GFX_THROW_INFO_ONLY(call) _infoManager.set();(call);{auto v = _infoManager.getMessage(); if(!v.empty()){throw Graphics::InfoException(__LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemoveException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif // !NDEBUG
#define INFOMAN(gfx) HRESULT hr; DxgiInfoManager& _infoManager = gfx.getInfoManager()
