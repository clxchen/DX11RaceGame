#include "D3DDevice.h"

bool D3DDevice::InitD3D(HINSTANCE hInstance)
{
	WNDCLASS wc;														 // ���������һ���ṹ�壩
	wc.style = CS_HREDRAW | CS_VREDRAW;									 // ������ʽ����Ϊ�����ڳߴ磨�ᡢ�ݣ��仯ʱ�ػ�
	wc.lpfnWndProc = (WNDPROC)(WndProc);								 // ָ���ص�����ָ��
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;										     // ��ǰӦ�ó���ʵ���ľ��
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);							 // ָ��ͼ��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);						     // ָ�����
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // ָ����ɫ����ɫ
	wc.lpszMenuName = NULL;												 // ��ʹ�ò˵�Ŀ¼
	wc.lpszClassName = L"Direct3D11App";								 // ָ�򴰿�����ָ��

	// 2��ע�ᴰ��
	if (!RegisterClass(&wc)){
		MessageBox(NULL, L"Register Class Failed!\r\n Init d3d failed!", L"Error", MB_OK);
		return false;
	}

	// 3����������
	// ��һ�����������wc.lpszClassName��ͬ
	// WS_OVERLAPPEDWINDOWָ�����ڷ��Ϊ�ص�ʽ����
	HWND hMainWnd = NULL;
	hMainWnd = CreateWindow(L"Direct3D11App", L"D3D11", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	if (!hMainWnd){
		MessageBox(NULL, L"Create Window Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// 4��������ʾ�͸���
	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);


	//----------�ڶ�������ʼ��D3D----------
	// ���裺
	// 1����佻�����ṹ 
	// 2������D3D�豸��������������ִ���豸������ 
	// 3������Ŀ����Ⱦ��ͼ������󶨵���Ⱦ���� 
	// 4�������ӿ�

	// 1����佻�����ṹ 
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;									// �������к�̨����������ͨ��Ϊ1������˫����
	sd.BufferDesc.Width = SCREEN_WIDTH;				    // �������еĴ��ڿ���ʵ����ʾ�Ĵ��ڶ�Ӧ
	sd.BufferDesc.Height = SCREEN_HEIGHT;				// �������еĴ��ڸߣ���ʵ����ʾ�Ĵ��ڶ�Ӧ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // ���ظ�ʽ
	sd.BufferDesc.RefreshRate.Numerator = 60;			// ˢ��Ƶ�ʷ��� 
	sd.BufferDesc.RefreshRate.Denominator = 1;			// ˢ��Ƶ�ʷ�ĸ����1��ˢ��60��
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // ������̨������÷����˴�������ȾĿ������
	sd.OutputWindow = hMainWnd;							// ָ����ȾĿ�괰�ڵľ��
	sd.SampleDesc.Count = 1;							// ÿ�����ص�Ĳ���������Ϊ1��ʾ��ʹ�ö��ز���
	sd.SampleDesc.Quality = 0;							// ���ز���������Ϊ0��ʾ�����������ʹ�ö��ز�����
	sd.Windowed = TRUE;									// ��Ⱦ�Ĵ���Ϊ����ģʽ
	sd.Flags = NULL;									// ȷ����������һЩ������Ϊ�����ﲻʹ��
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			// ����ͼ����ʾ����δ���ǰ���棬�˴�Ϊ��ǰ��̨�����е����ݱ���ʾ�����������

	// 2������D3D�豸��������������ִ���豸������ 
	// ����һ������ȷ�����Դ���FeatureLevels��˳��
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0, // D3D11֧�ֵ�����
		D3D_FEATURE_LEVEL_10_1, // D3D10֧�ֵ�����
		D3D_FEATURE_LEVEL_10_0
	};

	// ��ȡ�������Ԫ�ظ���
	UINT nFeatureLevelsCount = ARRAYSIZE(featureLevels);

	// ����D3D11CreateDeviceAndSwapChain���������豸��������������ִ���豸������
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,					  // ȷ����ʾ��������NULLΪĬ��
		D3D_DRIVER_TYPE_HARDWARE, // ѡ���������ͣ�����ʹ����λӲ������
		NULL,					  // ����һ������Ϊʹ���������ʱ�����õ��������ָ����դ������
		0,					      // ��ѡ��ʶ����ʹ��D3D_CREATE_DEVICE_DEBUG���õ���ģʽ
		featureLevels,			  // ָ���Ӧ����
		nFeatureLevelsCount,      // ����Ԫ�ظ���
		D3D11_SDK_VERSION,		  // SDK�汾��
		&sd,					  // ���õĽ������ṹ
		&pSwapChain,			  // ���ش����õĽ�����ָ��
		&pDevice,				  // ���ش����õ��豸ָ��
		NULL,					  // ���ص�ǰ�豸֧�ֵ�featureLevels�����еĵ�һ�����󣬲��þ�����ΪNULL
		&pImmediateContext		  // ���ش����õ�����ִ���豸������ָ��
		);
	if (FAILED(hr)){
		MessageBox(NULL, L"Create Device Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// 3������Ŀ����Ⱦ��ͼ������󶨵���Ⱦ����
	ID3D11Texture2D* pBackBuffer = NULL; // ָ���̨��������ָ��
	if (FAILED((pSwapChain)->GetBuffer(
		0,						   // ����������һ������Ϊ0
		__uuidof(ID3D11Texture2D), // ����GUID���ӵ����ʽ����������
		(void**)&pBackBuffer       // ָ���̨��������ָ���ָ��
		))){
		MessageBox(NULL, L"Get Buffer Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// ������ȾĿ����ͼ������pRenderTargetView��
	hr = (pDevice)->CreateRenderTargetView(
		pBackBuffer,       // ָ�򽻻�����̨�����ָ��
		NULL,			   // ����NULL�õ�Ĭ�ϵ���ȾĿ����ͼ
		&pRenderTargetView // ���ش����õ���ȾĿ����ͼ
		);

	// �ͷź�̨����
	Release(pBackBuffer);

	if (FAILED(hr)){
		MessageBox(NULL, L"Get Render Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// ������ģ��ṹ
	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ��Ȼ������ݸ�ʽ
	dsDesc.Width = SCREEN_WIDTH;				   // ���������
	dsDesc.Height = SCREEN_HEIGHT;				   // ���Ӵ�ͬ��С
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;   // �󶨱�ʶ����Ϊ���ģ�滺����
	dsDesc.MipLevels = 1;						   // �����е�mipmap�ȼ�
	dsDesc.ArraySize = 1;						   // ���������е�������� 
	dsDesc.CPUAccessFlags = 0;					   // CPU���ʷ�ʽ
	dsDesc.SampleDesc.Count = 1;				   // ��ʹ�ö��ز���
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.MiscFlags = 0;						   // ��ѡ�һ��Ϊ0
	dsDesc.Usage = D3D11_USAGE_DEFAULT;

	// �������ģ��
	hr = (pDevice)->CreateTexture2D(&dsDesc, 0, &pDepthStencilBuffer);
	if (FAILED(hr)){
		MessageBox(NULL, L"Create depth stencil buffer Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}
	hr = (pDevice)->CreateDepthStencilView(pDepthStencilBuffer, 0, &pDepthStencilView);
	if (FAILED(hr)){
		MessageBox(NULL, L"Create depth stencil view Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}
	// ����ȾĿ����ͼ�󶨵���Ⱦ����
	(pImmediateContext)->OMSetRenderTargets(
		1,                  // ��Ŀ����ͼ�ĸ���
		&pRenderTargetView, // Ҫ�󶨵���ȾĿ����ͼ
		pDepthStencilView   // �����ģ����ͼ
		);

	// 4�������ӿ� 
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(SCREEN_WIDTH);	// �ӿڿ�
	vp.Height = static_cast<float>(SCREEN_HEIGHT);  // �ӿڸ�
	vp.MinDepth = 0.0f;						        // �ӿ���С���ֵ��D3D�����ֵ��Χ[0,1]
	vp.MaxDepth = 1.0f;								// �ӿ�������ֵ
	vp.TopLeftX = 0.0f;								// �ӿ����ϽǺ�����
	vp.TopLeftY = 0.0f;								// �ӿ����Ͻ�������
	(pImmediateContext)->RSSetViewports(1, &vp);    // �����ӿڣ��ӿڸ���Ϊ1

	return true;
}
void D3DDevice::ClearView(const float ClearColor[4])
{
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}