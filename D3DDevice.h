#pragma once
#include "GameStandard.h"

// 声明程序入口Game.cpp中的回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class D3DDevice
{
private:
	//-----成员变量-----
	ID3D11RenderTargetView*		pRenderTargetView;			// 渲染目标视图
	ID3D11DeviceContext*		pImmediateContext;			// 立即执行上下文
	IDXGISwapChain*				pSwapChain;					// 交换链
	ID3D11Device*				pDevice;					// D3D11设备接口
	ID3D11Texture2D*			pDepthStencilBuffer;		// 深度缓存
	ID3D11DepthStencilView*		pDepthStencilView;			// 深度模版

public:
	D3DDevice() {}							   // 默认构造函数
	~D3DDevice();							   // 析构函数
	bool InitD3D(HINSTANCE hInstance);		   // 初始化D3D
	void ClearView(const float ClearColor[4]); // 清理深度模版视图，设置程序默认背景色

	//--------------Get（内联）-----------------------
	ID3D11RenderTargetView*  GetRenderTargetView()		{ return pRenderTargetView; }
	ID3D11DeviceContext*     GetImmediateContext()		{ return pImmediateContext; }
	IDXGISwapChain*		     GetSwapChain()				{ return pSwapChain; }
	ID3D11Device*		     GetDevice()				{ return pDevice; }
	ID3D11Texture2D*		 GetDepthStencilBuffer()	{ return pDepthStencilBuffer; }
	ID3D11DepthStencilView*  GetDepthStencilView()	    { return pDepthStencilView; }

	//---------------Set（内联）---------------------
	void SetRenderTargetView(ID3D11RenderTargetView* tmp){ pRenderTargetView = tmp; }
	void SetImmediateContext(ID3D11DeviceContext* tmp)	 { pImmediateContext = tmp; }
	void SetSwapChain(IDXGISwapChain* tmp)				 { pSwapChain = tmp; }
	void SetDevice(ID3D11Device* tmp)					 { pDevice = tmp; }
	void SetDepthStencilBuffer(ID3D11Texture2D* tmp)	 { pDepthStencilBuffer = tmp; }
	void SetDepthStencilView(ID3D11DepthStencilView* tmp){ pDepthStencilView = tmp; }
};
inline D3DDevice::~D3DDevice() {
	Release(pRenderTargetView); 
	Release(pImmediateContext);  
	Release(pSwapChain);
	Release(pDevice);           
	Release(pDepthStencilBuffer); 
	Release(pDepthStencilView);
}