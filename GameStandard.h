#pragma once

//----------������----------
#include <Windows.h> // ���ڲ�����Ч

// DirectX11���ͷ�ļ�
#include <xnamath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>

// DirectX11��ؿ�
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//----------ģ��----------
// �ͷ�ָ��̬����Ķ���Դ��ָ���ģ��

template<class T> void Delete(T t) {
	if(t){
		delete t;
		t = NULL;
	}
}

// �ͷ�ָ��D3D�ӿ����ɵ�ָ���ģ��
template<class T> void Release(T t) {
	if(t){
		t->Release();
		t = NULL;
	}
}

//----------�궨�庯��----------
// �������ĺ궨��
#define ReportError(hr, message)\
	if(FAILED(hr)){\
		MessageBox(NULL, message, L"Error", NULL);\
		}

//----------�궨�峣��----------
#define SCREEN_WIDTH  800 // ���������ڸ߶ȣ����أ�
#define SCREEN_HEIGHT 600 // ���������ڿ�ȣ����أ�

// ����������Ļ��Χ
static const float MAX_EYE_OFF_X = 5.0f;  // ���λ����X����������ƫ����
static const float MIN_EYE_OFF_X = -5.0f; // ���λ����X�Ḻ������ƫ����
static const float MAX_AT_OFF_X = 5.0f;   // ����λ����X����������ƫ����
static const float MIN_AT_OFF_X = -5.0f;  // ����λ����X�Ḻ������ƫ����
static const float MAX_AT_OFF_Y = 10.0f;  // ����λ����Y����������ƫ����
static const float MIN_AT_OFF_Y = -2.0f;  // ����λ����Y�Ḻ������ƫ����

// Ĭ�������������Y�᷽��ƫ����
static const float fDefaultAtOffY = 2.0f; 

// ͶӰ����
static const XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 6000.0f);

// ͨ�ö������벼�ֽṹ
static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
static const UINT nInputLayoutDescElemNum = ARRAYSIZE(layout);

//----------�Զ���ṹ��----------
// ͨ�ö���ṹ
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

// ��������ϵ�ṹ
struct CoordinateSystem {
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4   eyePosition;
};

// �ٶȽṹ
struct Velocity {
	XMFLOAT3 Direction; // �ٶȷ���
	float    Power;     // �ٶȴ�С
};

//----------ö��----------
// ������Ϸ����״̬
enum gameState {
	state_building,   // ��������D3D��Ϸ����
	state_ready,	  // ��Ϸ��
	state_showResult  // չʾ����Ͷ�������״̬
};
