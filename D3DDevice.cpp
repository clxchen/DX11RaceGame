#include "D3DDevice.h"

bool D3DDevice::InitD3D(HINSTANCE hInstance)
{
	WNDCLASS wc;														 // 窗口类对象（一个结构体）
	wc.style = CS_HREDRAW | CS_VREDRAW;									 // 窗口样式定义为当窗口尺寸（横、纵）变化时重绘
	wc.lpfnWndProc = (WNDPROC)(WndProc);								 // 指定回调函数指针
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;										     // 当前应用程序实例的句柄
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);							 // 指定图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);						     // 指定光标
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // 指定白色背景色
	wc.lpszMenuName = NULL;												 // 不使用菜单目录
	wc.lpszClassName = L"Direct3D11App";								 // 指向窗口名的指针

	// 2：注册窗口
	if (!RegisterClass(&wc)){
		MessageBox(NULL, L"Register Class Failed!\r\n Init d3d failed!", L"Error", MB_OK);
		return false;
	}

	// 3：创建窗口
	// 第一个参数必须和wc.lpszClassName相同
	// WS_OVERLAPPEDWINDOW指定窗口风格为重叠式窗口
	HWND hMainWnd = NULL;
	hMainWnd = CreateWindow(L"Direct3D11App", L"D3D11", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	if (!hMainWnd){
		MessageBox(NULL, L"Create Window Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// 4：窗口显示和更新
	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);


	//----------第二步：初始化D3D----------
	// 步骤：
	// 1：填充交换链结构 
	// 2：创建D3D设备、交换链、立即执行设备上下文 
	// 3：创建目标渲染视图，将其绑定到渲染管线 
	// 4：设置视口

	// 1：填充交换链结构 
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;									// 交换链中后台缓存数量，通常为1，代表双缓存
	sd.BufferDesc.Width = SCREEN_WIDTH;				    // 缓存区中的窗口宽，与实际显示的窗口对应
	sd.BufferDesc.Height = SCREEN_HEIGHT;				// 缓存区中的窗口高，与实际显示的窗口对应
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 像素格式
	sd.BufferDesc.RefreshRate.Numerator = 60;			// 刷新频率分子 
	sd.BufferDesc.RefreshRate.Denominator = 1;			// 刷新频率分母，即1秒刷新60次
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 描述后台缓存的用法，此处用作渲染目标的输出
	sd.OutputWindow = hMainWnd;							// 指向渲染目标窗口的句柄
	sd.SampleDesc.Count = 1;							// 每个像素点的采样个数，为1表示不使用多重采样
	sd.SampleDesc.Quality = 0;							// 多重采样质量，为0表示最低质量（不使用多重采样）
	sd.Windowed = TRUE;									// 渲染的窗口为窗口模式
	sd.Flags = NULL;									// 确定交换链的一些特殊行为，这里不使用
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			// 设置图像显示后如何处理当前缓存，此处为当前后台缓存中的内容被显示后，立刻清除它

	// 2：创建D3D设备、交换链、立即执行设备上下文 
	// 创建一个数组确定尝试创建FeatureLevels的顺序
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0, // D3D11支持的特征
		D3D_FEATURE_LEVEL_10_1, // D3D10支持的特征
		D3D_FEATURE_LEVEL_10_0
	};

	// 获取该数组的元素个数
	UINT nFeatureLevelsCount = ARRAYSIZE(featureLevels);

	// 调用D3D11CreateDeviceAndSwapChain函数创建设备、交换链、立即执行设备上下文
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,					  // 确定显示适配器，NULL为默认
		D3D_DRIVER_TYPE_HARDWARE, // 选择驱动类型，这里使用三位硬件加速
		NULL,					  // 当上一个参数为使用软件加速时，会用到这个参数指定光栅化程序
		0,					      // 可选标识符，使用D3D_CREATE_DEVICE_DEBUG启用调试模式
		featureLevels,			  // 指向对应数组
		nFeatureLevelsCount,      // 数组元素个数
		D3D11_SDK_VERSION,		  // SDK版本号
		&sd,					  // 填充好的交换链结构
		&pSwapChain,			  // 返回创建好的交换链指针
		&pDevice,				  // 返回创建好的设备指针
		NULL,					  // 返回当前设备支持的featureLevels数组中的第一个对象，不用就设置为NULL
		&pImmediateContext		  // 返回创建好的立即执行设备上下文指针
		);
	if (FAILED(hr)){
		MessageBox(NULL, L"Create Device Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// 3：创建目标渲染视图，将其绑定到渲染管线
	ID3D11Texture2D* pBackBuffer = NULL; // 指向后台缓存对象的指针
	if (FAILED((pSwapChain)->GetBuffer(
		0,						   // 缓存索引，一般设置为0
		__uuidof(ID3D11Texture2D), // 检索GUID附加到表达式，缓存类型
		(void**)&pBackBuffer       // 指向后台缓存对象的指针的指针
		))){
		MessageBox(NULL, L"Get Buffer Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// 创建渲染目标视图，存入pRenderTargetView中
	hr = (pDevice)->CreateRenderTargetView(
		pBackBuffer,       // 指向交换链后台缓存的指针
		NULL,			   // 设置NULL得到默认的渲染目标视图
		&pRenderTargetView // 返回创建好的渲染目标视图
		);

	// 释放后台缓存
	Release(pBackBuffer);

	if (FAILED(hr)){
		MessageBox(NULL, L"Get Render Failed!\r\n Init d3d failed!", L"Error", NULL);
		return false;
	}

	// 填充深度模版结构
	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 深度缓存数据格式
	dsDesc.Width = SCREEN_WIDTH;				   // 缓冲区宽高
	dsDesc.Height = SCREEN_HEIGHT;				   // 与视窗同大小
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;   // 绑定标识设置为深度模版缓冲区
	dsDesc.MipLevels = 1;						   // 纹理中的mipmap等级
	dsDesc.ArraySize = 1;						   // 纹理数组中的纹理个数 
	dsDesc.CPUAccessFlags = 0;					   // CPU访问方式
	dsDesc.SampleDesc.Count = 1;				   // 不使用多重采样
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.MiscFlags = 0;						   // 可选项，一般为0
	dsDesc.Usage = D3D11_USAGE_DEFAULT;

	// 创建深度模版
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
	// 将渲染目标视图绑定到渲染管线
	(pImmediateContext)->OMSetRenderTargets(
		1,                  // 绑定目标视图的个数
		&pRenderTargetView, // 要绑定的渲染目标视图
		pDepthStencilView   // 绑定深度模版视图
		);

	// 4：设置视口 
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(SCREEN_WIDTH);	// 视口宽
	vp.Height = static_cast<float>(SCREEN_HEIGHT);  // 视口高
	vp.MinDepth = 0.0f;						        // 视口最小深度值，D3D中深度值范围[0,1]
	vp.MaxDepth = 1.0f;								// 视口最大深度值
	vp.TopLeftX = 0.0f;								// 视口左上角横坐标
	vp.TopLeftY = 0.0f;								// 视口左上角纵坐标
	(pImmediateContext)->RSSetViewports(1, &vp);    // 设置视口，视口个数为1

	return true;
}
void D3DDevice::ClearView(const float ClearColor[4])
{
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}