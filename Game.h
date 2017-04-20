
#pragma once

#include "D3DDevice.h"
#include "D3DObject.h"
#include "Loader.h"
#include "Camera.h"
#include "GameState.h"

class Game {
public:
	//-----成员变量-----
	D3DDevice*		  pD3DDevice;  // D3D设备框架对象指针
	Camera*           pCamera;	   // D3D游戏里的摄像机对象指针，相当于玩家的可视视角
	GameState*		  pGameState;  // 游戏状态对象指针
	CoordinateSystem  coord;       // 各种坐标系
	Car*              pCar;        // 车
	D3DObject*        pTrack;      // 车道
	D3DObject*        pGround;     // 地面
	D3DObject*        pSky;        // 天空
	int				  nRound;      // 记录一次投篮的结果

public:
	//-----成员函数（公有）-----
	Game();
	~Game();
	void Build(HINSTANCE hInstance);			// 搭建游戏环境
	void ResetWorld();							// 重置世界坐标系 
	void UpdateView();							// 从当前Camera对象状态更新观察坐标系
	void WorldAttachCamera();				    // 将当前Camera的视角偏移应用到世界坐标系变换，完成UI在Camera移动前提上的跟踪
	void CheckRound();						    // 检测圈数
	void UpdateWindow(const float & timeDelta);	// 更新当前窗口应该绘制的内容

private:
	//-----成员函数（内在实现）
	void Init();
	void ReleasePointers();
	void SetObjects(); 
};
//----- 函数实现-----
inline Game::Game() {
	Init();
}
inline Game::~Game() {
	ReleasePointers();
}
inline void Game::Init(){
	pD3DDevice = new D3DDevice();
	pCamera = new Camera();
	pGameState = new GameState();
	pCar = new Car();
	pTrack = new D3DObject();
	pGround = new D3DObject();
	pSky = new D3DObject();
	nRound = 0;
	pCamera->init();
	
	ResetWorld();
	UpdateView();
	XMStoreFloat4x4(&(coord.projection), projection);
	XMFLOAT4 eyeValue = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	XMStoreFloat4(&eyeValue, pCamera->GetEye());
	coord.eyePosition.x = eyeValue.x;
	coord.eyePosition.y = eyeValue.y;
	coord.eyePosition.z = eyeValue.z;
	coord.eyePosition.w = 1.0f;	
}
inline void Game::ReleasePointers(){
	Delete(pD3DDevice);
	Delete(pCamera);
	Delete(pGameState);
	Delete(pCar);
	Delete(pTrack);
	Delete(pGround);
	Delete(pSky);
}
inline void Game::ResetWorld(){
	XMMATRIX world = XMMatrixIdentity(); // 用于世界变换的矩阵
	XMStoreFloat4x4(&(coord.world), world);
}
inline void Game::UpdateView(){
	
	pCamera->SetView( pCar->GetPosition(), 0);
	coord.view = pCamera->GetView();
}
inline void Game::WorldAttachCamera(){
	/*
	XMMATRIX newWorld = XMLoadFloat4x4(&(coord.world)) * pCamera->GetCameraAttachMatrix();
	XMStoreFloat4x4(&(coord.world), newWorld);
	*/
	coord.world = pCamera->GetView();
}

void Game::SetObjects(){
	// 与fx文件绑定，并利用绑定的D3D设备创建好Effect资源，成员变量指针pEffect指向了这个资源
	pCar->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	pTrack->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	pGround->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	pSky->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	
	// 从文件导入物体
	Loader* carLoader = new Loader();
	Loader* trackLoader = new Loader();
	Loader* groundLoader = new Loader();
	Loader* skyLoader = new Loader();
	
	// 设置车
	
	carLoader->LoadCarFormFile("media/car.obj");
	pCar->BindTexture(L"media/iron.jpg", pD3DDevice->GetDevice());
	pCar->BindVertice(carLoader->GetVertex(), carLoader->GetVertexCount());
	pCar->CreateInputLayout(pD3DDevice->GetDevice());
	pCar->CreateVertexBuffer(pD3DDevice->GetDevice());
	pCar->SetBoundingBoxVertex(carLoader->GetBoundBoxVertex());
	pCar->BindIndice(carLoader->GetIndex(), carLoader->GetIndexCount());
	pCar->CreateIndexBuffer(pD3DDevice->GetDevice());
	// 设置赛道
	trackLoader->LoadCarFormFile("media/wall.obj");
	pTrack->BindTexture(L"media/iron.jpg", pD3DDevice->GetDevice());
	pTrack->BindVertice(trackLoader->GetVertex(), trackLoader->GetVertexCount());
	pTrack->CreateInputLayout(pD3DDevice->GetDevice());
	pTrack->CreateVertexBuffer(pD3DDevice->GetDevice());
	pTrack->BindIndice(trackLoader->GetIndex(), trackLoader->GetIndexCount());
	pTrack->CreateIndexBuffer(pD3DDevice->GetDevice());

	// 设置地面
	groundLoader->LoadCarFormFile("media/ground.obj");
	pGround->BindTexture(L"media/iron.jpg", pD3DDevice->GetDevice());
	pGround->BindVertice(groundLoader->GetVertex(), groundLoader->GetVertexCount());
	pGround->CreateInputLayout(pD3DDevice->GetDevice());
	pGround->CreateVertexBuffer(pD3DDevice->GetDevice());
	pGround->BindIndice(groundLoader->GetIndex(), groundLoader->GetIndexCount());
	pGround->CreateIndexBuffer(pD3DDevice->GetDevice());

	// 设置天空
	skyLoader->LoadCarFormFile("media/sky.obj");
	pSky->BindTexture(L"media/sky.jpg", pD3DDevice->GetDevice());
	pSky->BindVertice(skyLoader->GetVertex(), skyLoader->GetVertexCount());
	pSky->CreateInputLayout(pD3DDevice->GetDevice());
	pSky->CreateVertexBuffer(pD3DDevice->GetDevice());
	pSky->BindIndice(skyLoader->GetIndex(), skyLoader->GetIndexCount());
	pSky->CreateIndexBuffer(pD3DDevice->GetDevice());
	//pUI->Set(pD3DDevice->GetDevice());


}
void Game::Build(HINSTANCE hInstance){
	// 初始化D3D框架设备
	pD3DDevice->InitD3D(hInstance); 
	
	// 建立好所有对象（载入顶点序列等）
	SetObjects();

	// 改变状态为Ready
	pGameState->StateChange(state_ready);
}
void Game::CheckRound(){
	
}
void Game::UpdateWindow(const float & timeDelta){
	switch (pGameState->GetState()){
	case state_building:break;
	case state_ready:{ // 游戏阶段
		// 绘制车
		UpdateView();
		
		pCamera->SetView( pCar->GetPosition(), 0);
		
		ResetWorld();
		//WorldAttachCamera();

		
		XMMATRIX rotate = XMMatrixRotationX(-XM_PI*0.5)*XMMatrixRotationY(XM_PI*0.5f);
		XMMATRIX scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
		XMMATRIX t = XMMatrixTranslation(pCar->GetPosition().x, pCar->GetPosition().y,pCar->GetPosition().z);
		XMMATRIX world;
	
	

		
		world=XMLoadFloat4x4(&coord.world);
		pCar->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate*XMMatrixRotationY(pCar->GetAngle())*t));
		XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate*XMMatrixRotationY(pCar->GetAngle())*t));
		pCar->Draw(pD3DDevice->GetImmediateContext(), coord);
		pCamera->SetView( pCar->GetPosition(), 0);


		// 绘制赛道
		ResetWorld();
		scale = XMMatrixScaling(30.0f, 30.0f, 30.0f);
		world = XMLoadFloat4x4(&coord.world);
		pTrack->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate));
		XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate));
		pTrack->Draw(pD3DDevice->GetImmediateContext(), coord);
		
		// 绘制地面
		ResetWorld();
		scale = XMMatrixScaling(30.0f, 30.0f, 30.0f);
		world = XMLoadFloat4x4(&coord.world);
		pGround->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate));
		 XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate));
		pGround->Draw(pD3DDevice->GetImmediateContext(), coord);
		
		// 绘制天空
		 ResetWorld();
		 scale = XMMatrixScaling(30.0f, 30.0f, 30.0f);
		 world = XMLoadFloat4x4(&coord.world);
		 pSky->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate));
		 XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate));
		 pSky->Draw(pD3DDevice->GetImmediateContext(), coord);

		pD3DDevice->GetSwapChain()->Present(0, 0);
	}break;
	case state_showResult:{//游戏结束
		UpdateView();
		ResetWorld();
		pD3DDevice->GetSwapChain()->Present(0, 0);
	}
	}
}