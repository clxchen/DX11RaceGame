#pragma once
#include "GameStandard.h"

// �����������Game.cpp�еĻص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class D3DDevice
{
private:
	//-----��Ա����-----
	ID3D11RenderTargetView*		pRenderTargetView;			// ��ȾĿ����ͼ
	ID3D11DeviceContext*		pImmediateContext;			// ����ִ��������
	IDXGISwapChain*				pSwapChain;					// ������
	ID3D11Device*				pDevice;					// D3D11�豸�ӿ�
	ID3D11Texture2D*			pDepthStencilBuffer;		// ��Ȼ���
	ID3D11DepthStencilView*		pDepthStencilView;			// ���ģ��

public:
	D3DDevice() {}							   // Ĭ�Ϲ��캯��
	~D3DDevice();							   // ��������
	bool InitD3D(HINSTANCE hInstance);		   // ��ʼ��D3D
	void ClearView(const float ClearColor[4]); // �������ģ����ͼ�����ó���Ĭ�ϱ���ɫ

	//--------------Get��������-----------------------
	ID3D11RenderTargetView*  GetRenderTargetView()		{ return pRenderTargetView; }
	ID3D11DeviceContext*     GetImmediateContext()		{ return pImmediateContext; }
	IDXGISwapChain*		     GetSwapChain()				{ return pSwapChain; }
	ID3D11Device*		     GetDevice()				{ return pDevice; }
	ID3D11Texture2D*		 GetDepthStencilBuffer()	{ return pDepthStencilBuffer; }
	ID3D11DepthStencilView*  GetDepthStencilView()	    { return pDepthStencilView; }

	//---------------Set��������---------------------
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