#pragma once
#include "GameStandard.h"

struct CarSpeed
{
	float velocity;
	XMFLOAT3 vDirection;
};

struct CarAccele
{
	float accele;
	XMFLOAT3 aDirection;
};

/************基类：D3D对象类************/
class D3DObject{
protected:
	//-----成员变量-----
	Vertex*  pVertice;						 // 顶点序列
	UINT	 nVertexNum;					 // 顶点数目
	UINT	 nVertexStructSize;			     // 顶点结构的大小
	unsigned int* pIndice;                   // 索引序列
	UINT     nIndexNum;                      // 索引数目
	UINT	 nIndexStructSize;               // 索引结构的大小
	XMFLOAT3 offset;						 // 相对于原本顶点序列在世界坐标系中的位置，要位移的量
	ID3D11InputLayout*		  pVertexLayout; // 由D3D设备生成的它能识别的顶点输入布局对象指针，需要绘制时与其绑定（最初设置一次即可，以后直接取出绘制，不然每次绘制前都要重复做设置操作）
	ID3DX11Effect*		      pEffect;	     // 与Effect文件绑定后，由D3D设备创建的Effect资源的指针
	ID3DX11EffectTechnique*   pTechnique;    // 与Effect文件中Technique关联的指针，需要绘制时从中获取pass结构
	ID3D11Buffer*		      pVertexBuffer; // 指向D3D设备生成的顶点缓存的指针，需要绘制时与其绑定（最初设置一次即可，以后直接取出绘制，不然每次绘制前都要重复做设置操作）
	ID3D11Buffer*		      pIndexBuffer;  // 索引缓存
	ID3D11ShaderResourceView* pTexture;      // 由D3D设备生成的它能识别的材质对象指针，需要绘制时与其绑定（最初设置一次即可，以后直接取出绘制，不然每次绘制前都要重复做设置操作）

public:
	//-----成员函数（公有）-----
	D3DObject();
	~D3DObject();

	XMFLOAT3 GetOffset();														  // 获取位移量
	void	 SetOffset(float offsetX, float offsetY, float offsetZ);			  // 设置位移量
	void	 AppendOffset(float offsetX, float offsetY, float offsetZ);			  // 在原有位移量的基础上加上新的位移量
	const XMMATRIX&		      GetOffsetTranslationMatrix();						  // 获取位移变换矩阵
	ID3D11InputLayout*		  GetVertexLayout();								  // 获取输入布局对象指针
	ID3D11Buffer*			  GetVertexBuffer();								  // 获取顶点缓存指针
	ID3DX11EffectTechnique*   GetTechnique();									  // 获取Technique指针
	ID3DX11Effect*            GetEffect();	// 获取Effect指针


	//-----成员函数（虚拟）-----
	virtual void BindFxFile(WCHAR* fxPath, ID3D11Device* device);       // 与fx文件绑定，并利用绑定的D3D设备创建好Effect资源，成员变量指针pEffect指向了这个资源
	virtual void BindTexture(LPCWSTR texPath, ID3D11Device* device);     // 与材质文件绑定，由D3D设备生成的它能识别的纹理对象，成员变量指针pTexture指向了这个资源
	virtual void BindIndice(unsigned int* indice, const UINT & indexNum);// 绑定好需要绘制的索引序列，视实际情况调整参数类型及函数体
	virtual void BindVertice(Vertex* vertice, const UINT & vertexNum);	// 绑定好需要绘制的顶点序列，视实际情况调整参数类型及函数体
	virtual void CreateInputLayout(ID3D11Device* device);			    // 创建对象的输入布局，由D3D设备生成的它能识别的顶点输入布局对象，成员变量指针pVertexLayout指向这个对象
	virtual void CreateVertexBuffer(ID3D11Device* device);			    // 创建对象的顶点缓存，由D3D设备生成的它能识别的顶点缓存对象，成员变量指针pVertexBuffer指向这个对象
	virtual void CreateIndexBuffer(ID3D11Device* device);			    // 创建对象的索引缓存，由D3D设备生成的它能识别的索引缓存对象，成员变量指针pIndexBuffer指向这个对象
	virtual void Draw(ID3D11DeviceContext* immediateContext,
		const CoordinateSystem & coord);						        // 在D3D设备上绘制对象

private:
	//-----成员函数（内在实现）-----
	void ReleasePointers(); // 释放指针
	void Init();			// 初始化成员变量

};
//-----函数实现（内联）-----
inline		D3DObject::D3DObject(){
	Init();
}
inline		D3DObject::~D3DObject(){
	ReleasePointers();
}
inline void D3DObject::Init(){
	//-----默认值-----
	pVertice = NULL;
	nVertexNum = 0;
	nVertexStructSize = 0;
	offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pVertexLayout = NULL;
	pEffect = NULL;
	pTechnique = NULL;
	pVertexBuffer = NULL;
	pTexture = NULL;
}


inline void D3DObject::ReleasePointers(){
	Release(pVertexLayout);
	Release(pEffect);
	Release(pVertexBuffer);
	Release(pTexture);
}
inline XMFLOAT3	 D3DObject::GetOffset() {
	return offset;
}
inline void						 D3DObject::SetOffset(float offsetX, float offsetY, float offsetZ) {
	offset = XMFLOAT3(offsetX, offsetY, offsetZ);
}
inline void						 D3DObject::AppendOffset(float offsetX, float offsetY, float offsetZ) {
	offset.x += offsetX;
	offset.y += offsetY;
	offset.z += offsetZ;
}
inline const XMMATRIX&		     D3DObject::GetOffsetTranslationMatrix() {
	return XMMatrixTranslation(offset.x, offset.y, offset.z);
}
inline ID3D11InputLayout*		 D3DObject::GetVertexLayout() {
	return pVertexLayout;
}
inline ID3D11Buffer*			 D3DObject::GetVertexBuffer() {
	return pVertexBuffer;
}
inline ID3DX11EffectTechnique*   D3DObject::GetTechnique(){
	return pTechnique;
}
inline ID3DX11Effect*			 D3DObject::GetEffect(){
	return pEffect;
}

/************子类：车类************/
class Car : public D3DObject{
private:
	//-----成员变量-----
	XMFLOAT3 carVertex[2]; //车顶点
	CarSpeed carSpeed;		//车速度
	CarAccele carAccele;		//车加速度
	XMFLOAT3 carPosition;   // 车位置
	D3DObject* NOS;			//氮气条
	float  NOSf;		//氮气值
	float carAngle;		//车的角度


public:
	//-----成员函数（公有）-----
	Car();
	~Car();
	void	 SetBoundingBoxVertex(XMFLOAT3 carVertex[]); // 设置车顶点
	XMFLOAT3* GetBoundingBoxVertex();						//获取车顶点	

	void     SetSpeed(float v,XMFLOAT3 d);	//设置车速度
	CarSpeed   GetSpeed();					//获取车速度

	void  SetAccele(float a,XMFLOAT3 d);			//设置车加速度
	CarAccele GetAccele();						//获取车加速度

	XMFLOAT3 GetPosition();					//获取车位置
	void SetPosition(XMFLOAT3 p);					//设置车的位置

	
	void CarYaw(float angle);				//车的旋转角度
	

	float CalSpeed();                       //计算车速度
	void SetAcceleDirection(XMFLOAT3);

	void SetAngle(float angle);
	float GetAngle();

	XMFLOAT3 InitPosition();
	

	//void Draw(ID3D11DeviceContext* immediateContext, const CoordinateSystem & );
};
//-----函数实现（内联）-----
inline			Car::Car() : D3DObject(){
	carVertex[0] = { 0.0f, 0.0f, 0.0f };
	carVertex[1] = { 0.0f, 0.0f, 0.0f };
	carSpeed.velocity = 0;
	carSpeed.vDirection = { 0, 0, 0 };
	carAccele.accele = 0;
	carAccele.aDirection = { 0, 0, 0 };
	carPosition = { 0, 0, 0 };
}
inline void Car::SetBoundingBoxVertex(XMFLOAT3 carvertex[]){
	int i = 0;
	for (i = 0; i < 2; i++)
		carVertex[i] = carvertex[i];
}

inline XMFLOAT3* Car::GetBoundingBoxVertex()
{
	return carVertex;
}

inline			Car::~Car(){
}
inline void		Car::SetSpeed(float v,XMFLOAT3 d) {
	carSpeed.velocity = v;
	carSpeed.vDirection = d;
}
inline CarSpeed Car::GetSpeed(){
	return carSpeed;
}
inline XMFLOAT3 Car::GetPosition(){
	return carPosition;

}
inline XMFLOAT3 Car::InitPosition(){
	XMFLOAT3 position;
	position.x = (carVertex[0].x + carVertex[1].x) / 2;
	position.y = (carVertex[0].y + carVertex[1].y) / 2;
	position.z = (carVertex[0].z + carVertex[1].z) / 2;

	
	return position;
}

inline void Car::SetPosition(XMFLOAT3 p)
{
	carPosition = p;
}

inline void	Car::SetAccele(float a,XMFLOAT3 d) {
	carAccele.accele = a;
	carAccele.aDirection = d;
}

inline CarAccele Car::GetAccele(){
	return carAccele;
}
inline void Car::SetAngle(float angle){
	carAngle = angle;
}
inline float Car::GetAngle(){
	return carAngle;
}

