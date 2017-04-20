
#pragma once

#include "D3DDevice.h"
#include "D3DObject.h"
#include "Loader.h"
#include "Camera.h"
#include "GameState.h"

class Game {
public:
	//-----��Ա����-----
	D3DDevice*		  pD3DDevice;  // D3D�豸��ܶ���ָ��
	Camera*           pCamera;	   // D3D��Ϸ������������ָ�룬�൱����ҵĿ����ӽ�
	GameState*		  pGameState;  // ��Ϸ״̬����ָ��
	CoordinateSystem  coord;       // ��������ϵ
	Car*              pCar;        // ��
	D3DObject*        pTrack;      // ����
	D3DObject*        pGround;     // ����
	D3DObject*        pSky;        // ���
	int				  nRound;      // ��¼һ��Ͷ���Ľ��

public:
	//-----��Ա���������У�-----
	Game();
	~Game();
	void Build(HINSTANCE hInstance);			// ���Ϸ����
	void ResetWorld();							// ������������ϵ 
	void UpdateView();							// �ӵ�ǰCamera����״̬���¹۲�����ϵ
	void WorldAttachCamera();				    // ����ǰCamera���ӽ�ƫ��Ӧ�õ���������ϵ�任�����UI��Camera�ƶ�ǰ���ϵĸ���
	void CheckRound();						    // ���Ȧ��
	void UpdateWindow(const float & timeDelta);	// ���µ�ǰ����Ӧ�û��Ƶ�����

private:
	//-----��Ա����������ʵ�֣�
	void Init();
	void ReleasePointers();
	void SetObjects(); 
};
//----- ����ʵ��-----
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
	XMMATRIX world = XMMatrixIdentity(); // ��������任�ľ���
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
	// ��fx�ļ��󶨣������ð󶨵�D3D�豸������Effect��Դ����Ա����ָ��pEffectָ���������Դ
	pCar->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	pTrack->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	pGround->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	pSky->BindFxFile(L"Shader.fx", pD3DDevice->GetDevice());
	
	// ���ļ���������
	Loader* carLoader = new Loader();
	Loader* trackLoader = new Loader();
	Loader* groundLoader = new Loader();
	Loader* skyLoader = new Loader();
	
	// ���ó�
	
	carLoader->LoadCarFormFile("media/car.obj");
	pCar->BindTexture(L"media/iron.jpg", pD3DDevice->GetDevice());
	pCar->BindVertice(carLoader->GetVertex(), carLoader->GetVertexCount());
	pCar->CreateInputLayout(pD3DDevice->GetDevice());
	pCar->CreateVertexBuffer(pD3DDevice->GetDevice());
	pCar->SetBoundingBoxVertex(carLoader->GetBoundBoxVertex());
	pCar->BindIndice(carLoader->GetIndex(), carLoader->GetIndexCount());
	pCar->CreateIndexBuffer(pD3DDevice->GetDevice());
	// ��������
	trackLoader->LoadCarFormFile("media/wall.obj");
	pTrack->BindTexture(L"media/iron.jpg", pD3DDevice->GetDevice());
	pTrack->BindVertice(trackLoader->GetVertex(), trackLoader->GetVertexCount());
	pTrack->CreateInputLayout(pD3DDevice->GetDevice());
	pTrack->CreateVertexBuffer(pD3DDevice->GetDevice());
	pTrack->BindIndice(trackLoader->GetIndex(), trackLoader->GetIndexCount());
	pTrack->CreateIndexBuffer(pD3DDevice->GetDevice());

	// ���õ���
	groundLoader->LoadCarFormFile("media/ground.obj");
	pGround->BindTexture(L"media/iron.jpg", pD3DDevice->GetDevice());
	pGround->BindVertice(groundLoader->GetVertex(), groundLoader->GetVertexCount());
	pGround->CreateInputLayout(pD3DDevice->GetDevice());
	pGround->CreateVertexBuffer(pD3DDevice->GetDevice());
	pGround->BindIndice(groundLoader->GetIndex(), groundLoader->GetIndexCount());
	pGround->CreateIndexBuffer(pD3DDevice->GetDevice());

	// �������
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
	// ��ʼ��D3D����豸
	pD3DDevice->InitD3D(hInstance); 
	
	// ���������ж������붥�����еȣ�
	SetObjects();

	// �ı�״̬ΪReady
	pGameState->StateChange(state_ready);
}
void Game::CheckRound(){
	
}
void Game::UpdateWindow(const float & timeDelta){
	switch (pGameState->GetState()){
	case state_building:break;
	case state_ready:{ // ��Ϸ�׶�
		// ���Ƴ�
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


		// ��������
		ResetWorld();
		scale = XMMatrixScaling(30.0f, 30.0f, 30.0f);
		world = XMLoadFloat4x4(&coord.world);
		pTrack->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate));
		XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate));
		pTrack->Draw(pD3DDevice->GetImmediateContext(), coord);
		
		// ���Ƶ���
		ResetWorld();
		scale = XMMatrixScaling(30.0f, 30.0f, 30.0f);
		world = XMLoadFloat4x4(&coord.world);
		pGround->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate));
		 XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate));
		pGround->Draw(pD3DDevice->GetImmediateContext(), coord);
		
		// �������
		 ResetWorld();
		 scale = XMMatrixScaling(30.0f, 30.0f, 30.0f);
		 world = XMLoadFloat4x4(&coord.world);
		 pSky->GetEffect()->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(world*scale*rotate));
		 XMStoreFloat4x4(&coord.world, (float*)&(world*scale*rotate));
		 pSky->Draw(pD3DDevice->GetImmediateContext(), coord);

		pD3DDevice->GetSwapChain()->Present(0, 0);
	}break;
	case state_showResult:{//��Ϸ����
		UpdateView();
		ResetWorld();
		pD3DDevice->GetSwapChain()->Present(0, 0);
	}
	}
}