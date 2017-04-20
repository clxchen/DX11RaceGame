#include "D3DObject.h"
//#include "Algorithm.h"

const float MAX_SPEED = 80.0f;		//车的最大速度

void D3DObject::BindFxFile(WCHAR* fxPath,ID3D11Device* device) {
	HRESULT hr = S_OK;
	ID3DBlob* pTechBlob = NULL;
	// 从fx文件读取着色器相关信息
	hr = D3DX11CompileFromFile(
		fxPath,				// 对应的高级着色语言脚本
		NULL,			    // 可选高级参数，这里不使用
		NULL,			    // 可选高级参数，用于处理着色器中的Include文件，这里不使用
		NULL,               // 对应的着色器函数名，这里是fx文件，所以不用指明具体的函数
		"fx_5_0",		    // 对应的fx着色器版本
		D3DCOMPILE_ENABLE_STRICTNESS, // 使用约束，禁止使用过期的语法
		NULL,						  // Effect文件编译选项，这里不使用
		NULL,						  // 多线程编程时用于指向县城泵，这里不涉及
		&pTechBlob,					  // 返回编译好的所有着色器所在内存的指针
		NULL,
		NULL						  // 指向返回值的指针，这里不使用
		);
	ReportError(hr, L"Failed to compile fx file!");

	// 创建ID3DEffect对象
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(),
		0,        // 不使用Effect标识
		device,  // 设备指针，用来创建Effect资源
		&pEffect  // 返回创建好的ID3DEffect对象
		);
	ReportError(hr, L"Failed to create effect!");
}
void D3DObject::BindTexture(LPCWSTR texPath, ID3D11Device* device){
	D3DX11CreateShaderResourceViewFromFile(device, texPath, NULL, NULL, &pTexture, NULL);
}
void D3DObject::BindVertice(Vertex* vertice, const UINT & vertexNum) {
	//-----成员变量设置-----
	pVertice = vertice;
	nVertexNum = vertexNum;
	nVertexStructSize = sizeof(Vertex);
}
void D3DObject::BindIndice(unsigned int* indice, const UINT & indexNum)
{
	pIndice = indice;
	nIndexNum = indexNum;
	nIndexStructSize = sizeof(unsigned int);
}
void D3DObject::CreateInputLayout(ID3D11Device* device){
	//-----获取Pass通道结构，视实际fx文件结构调整-----
	pTechnique = pEffect->GetTechniqueByName("TexTech");
	D3DX11_PASS_DESC passDesc;
	HRESULT hr = pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	ReportError(hr, L"Get Pass Failed!");

	//-----创建D3D设备能识别的输入布局-----
	hr = device->CreateInputLayout(
		layout,							// GameStandard定义的D3D11_INPUT_ELEMENT_DESC数组
		nInputLayoutDescElemNum,		// D3D11_INPUT_ELEMENT_DESC数组的元素个数
		passDesc.pIAInputSignature,		// 和实验4不同，这个参数传入的是Effect Pass描述的输入标识
		passDesc.IAInputSignatureSize,	// 和实验4不同，这个参数传入的是Effect Pass描述的输入标识的大小
		&pVertexLayout);				// 返回生成的输入布局对象
	ReportError(hr, L"创建Input Layout失败");
}
void D3DObject::CreateVertexBuffer(ID3D11Device* device){
	//-----顶点缓存结构-----
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				  // 缓存读写方式：默认
	vertexBufferDesc.ByteWidth = nVertexStructSize * nVertexNum;  // 设置缓存区域大小
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	      // 将这个缓存区域绑定到顶点缓存
	vertexBufferDesc.CPUAccessFlags = 0;						  // CPU访问标识符，0表示没有CPU访问
	vertexBufferDesc.MiscFlags = 0;							      // 其他项标识符，0表示不使用该项

	//-----用于初始化子资源的数据-----
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = pVertice; // 设置需要初始化的数据，即对应的顶点数组

	//-----创建D3D设备能识别的顶点缓存-----
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &initData, &pVertexBuffer);
	ReportError(hr, L"创建VertexBuffer失败");
}
void D3DObject::CreateIndexBuffer(ID3D11Device* device) {
	//-----索引缓存结构-----
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				  // 缓存读写方式：默认
	indexBufferDesc.ByteWidth = nIndexStructSize * nIndexNum;  // 设置缓存区域大小
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	      // 将这个缓存区域绑定到索引缓存
	indexBufferDesc.CPUAccessFlags = 0;						  // CPU访问标识符，0表示没有CPU访问
	indexBufferDesc.MiscFlags = 0;							      // 其他项标识符，0表示不使用该项

																  //-----用于初始化子资源的数据-----
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = pIndice; // 设置需要初始化的数据，即对应的顶点数组

								 //-----创建D3D设备能识别的索引缓存-----
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &initData, &pIndexBuffer);
	ReportError(hr, L"创建IndexBuffer失败");
}

void D3DObject::Draw(ID3D11DeviceContext* immediateContext, const CoordinateSystem & coord){
	// 设置坐标系缓存到Effect
	pEffect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(coord.world));            
	pEffect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&(coord.view));              
	pEffect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&(coord.projection)); 
	pEffect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&(coord.eyePosition));

	// 设置材质到Effect
	pEffect->GetVariableByName("Texture")->AsShaderResource()->SetResource(pTexture);

	// 设置创建好的输入布局到立即执行设备上下文
	immediateContext->IASetInputLayout(pVertexLayout);
	
	// 设置创建好的顶点缓存到立即执行设备上下文
	UINT nOffset = 0;
	immediateContext->IASetVertexBuffers(
		0,				    // 绑定的第一个输入槽
		1,				    // 顶点缓存的个数，这里为1个
		&pVertexBuffer,	    // 创建好的顶点缓存
		&nVertexStructSize,	// 跨度
		&nOffset		    // 缓存第一个元素到所用元素的偏移量
		);

	immediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// 指定图元类型为三角形
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// 将Pass通道应用到立即执行设备上下文
	pTechnique->GetPassByIndex(0)->Apply(0, immediateContext);
	
	pTechnique = pEffect->GetTechniqueByName("TexTech");
	D3DX11_PASS_DESC passDesc;
	HRESULT hr = pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	ReportError(hr, L"Get Pass Failed!");
	// 通过顶点缓存绘制
	immediateContext->DrawIndexed(nIndexNum, 0, 0);
}

float Car::CalSpeed(){
	if (carSpeed.velocity < MAX_SPEED)
		carSpeed.velocity = carSpeed.velocity + carAccele.accele;
	else if ((carSpeed.velocity>= MAX_SPEED))
		carSpeed.velocity = MAX_SPEED;

	return carSpeed.velocity;

}
void Car::CarYaw(float angle){
	
	XMMatrixRotationY(angle);

}





	
