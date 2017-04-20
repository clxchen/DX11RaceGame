#pragma once

//----------依赖项----------
#include <Windows.h> // 用于播放音效

// DirectX11相关头文件
#include <xnamath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>

// DirectX11相关库
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//----------模版----------
// 释放指向动态分配的堆资源的指针的模版

template<class T> void Delete(T t) {
	if(t){
		delete t;
		t = NULL;
	}
}

// 释放指向D3D接口生成的指针的模版
template<class T> void Release(T t) {
	if(t){
		t->Release();
		t = NULL;
	}
}

//----------宏定义函数----------
// 报告错误的宏定义
#define ReportError(hr, message)\
	if(FAILED(hr)){\
		MessageBox(NULL, message, L"Error", NULL);\
		}

//----------宏定义常量----------
#define SCREEN_WIDTH  800 // 程序主窗口高度（像素）
#define SCREEN_HEIGHT 600 // 程序主窗口宽度（像素）

// 限制摄像机的活动范围
static const float MAX_EYE_OFF_X = 5.0f;  // 相机位置沿X轴正向的最大偏移量
static const float MIN_EYE_OFF_X = -5.0f; // 相机位置沿X轴负向的最大偏移量
static const float MAX_AT_OFF_X = 5.0f;   // 焦点位置沿X轴正向的最大偏移量
static const float MIN_AT_OFF_X = -5.0f;  // 焦点位置沿X轴负向的最大偏移量
static const float MAX_AT_OFF_Y = 10.0f;  // 焦点位置沿Y轴正向的最大偏移量
static const float MIN_AT_OFF_Y = -2.0f;  // 焦点位置沿Y轴负向的最大偏移量

// 默认摄像机焦点沿Y轴方向偏移量
static const float fDefaultAtOffY = 2.0f; 

// 投影矩阵
static const XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 6000.0f);

// 通用顶点输入布局结构
static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
static const UINT nInputLayoutDescElemNum = ARRAYSIZE(layout);

//----------自定义结构体----------
// 通用顶点结构
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

// 各种坐标系结构
struct CoordinateSystem {
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4   eyePosition;
};

// 速度结构
struct Velocity {
	XMFLOAT3 Direction; // 速度方向
	float    Power;     // 速度大小
};

//----------枚举----------
// 定义游戏可能状态
enum gameState {
	state_building,   // 正在生成D3D游戏环境
	state_ready,	  // 游戏中
	state_showResult  // 展示本次投篮结果的状态
};
