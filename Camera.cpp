#include "Camera.h"

Camera::Camera()
{	
	viewType = 0;
	flag = Rotate | Translate;
	rotateX = 0.0f;
	rotateY = 0.0f;
	rotateZ = 0.0f;
	distance = 20.0f;
	height = 10.0f;
	move = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	eye = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	
	XMMATRIX cameraRotationMATRIX = XMMatrixIdentity(); 
	XMStoreFloat4x4(&cameraRotation, cameraRotationMATRIX);
	XMMATRIX viewMATRIX = XMMatrixIdentity();
	XMStoreFloat4x4(&view, viewMATRIX);
}
void Camera::init(){
	XMMATRIX cameraRotationMATRIX = XMMatrixIdentity();
	XMStoreFloat4x4(&cameraRotation, cameraRotationMATRIX);
	XMMATRIX viewMATRIX = XMMatrixIdentity();
	XMStoreFloat4x4(&view, viewMATRIX);
}
void Camera::Pitch(float angle)
{
	rotateX += angle;
	flag |= Rotate;
}

void Camera::Yaw(float angle)
{
	rotateY += angle;
	flag |= Rotate;
}

void Camera::SetEye(const XMVECTOR& position)
{
	eye = position;
	flag |= Translate;
}

void Camera::SetUp(const XMVECTOR& position)
{
	up = position;
	flag |= Translate;
}

void Camera::SetAt(const XMVECTOR& position)
{
	at = position;
	flag |= Translate;
}

const XMVECTOR& Camera::GetEye()
{
	return eye;
}


const XMVECTOR& Camera::GetUp()
{
	return up;
}


const XMVECTOR& Camera::GetAt()
{
	return at;
}

void Camera::MoveForwardBy(float value)
{
	move = XMVectorSetZ(move, value);
	flag |= Translate;
}

void Camera::MoveRightBy(float value)
{
	move = XMVectorSetX(move, value);
	flag |= Translate;
}

void Camera::MoveUpBy(float value)
{
	move = XMVectorSetY(move, value);
	flag |= Translate;
}

void Camera::Apply()
{
	XMMATRIX cameraRotationMATRIX;
	if (flag != None)
	{
		if ((flag&Rotate) != 0)
		{
			flag &= ~Rotate;
			cameraRotationMATRIX = XMMatrixRotationX(rotateX)*XMMatrixRotationY(rotateY)*XMMatrixRotationZ(rotateZ);
			XMStoreFloat4x4(&cameraRotation, cameraRotationMATRIX);
		}
		if ((flag&Translate) != 0)
		{
			flag &= ~Translate;
			cameraRotationMATRIX = XMLoadFloat4x4(&cameraRotation);
			eye += XMVector4Transform(move, cameraRotationMATRIX);
			move = XMVectorZero();
		}
		cameraRotationMATRIX = XMLoadFloat4x4(&cameraRotation);
		at = eye + XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), cameraRotationMATRIX);
		up = XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),cameraRotationMATRIX);
		XMMATRIX viewMATRIX = XMMatrixLookAtLH(eye, at, up);
		XMStoreFloat4x4(&view, viewMATRIX);
	}
}

const XMFLOAT4X4& Camera::GetView()
{
	return view;
}

void Camera::SetView(XMFLOAT3 carPos,float angle){
	if (!viewType){
		distance = 0.0f;
		height = 10.0f;
	}
	else if (viewType){
		distance =20.0f;
		height = 10.0f;
	}
	SetEye(XMVectorSet(carPos.x, carPos.y+height, carPos.z, 1.0f));
	MoveForwardBy(distance);
	Yaw(angle);
	MoveForwardBy(-distance);
	//MoveForwardBy(0.0f);
	Apply();
}

void Camera::SwitchView()
{
	this->viewType = !viewType;
}



