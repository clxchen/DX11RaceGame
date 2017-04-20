#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
#pragma comment(lib,"assimp-vc120-mt.lib")

class Loader
{

private:
	std::vector<Vertex> vertices;
	UINT vertexCount;
	XMFLOAT3 boundBoxVertex[2];
	std::vector<unsigned int> indices;
	UINT indexCount;
public:
	Vertex* GetVertex()
	{
		return &vertices[0];
	}

	const UINT& GetVertexCount()
	{
		return vertexCount;
	}

	unsigned int* GetIndex()
	{
		return &indices[0];
	}

	const UINT& GetIndexCount()
	{
		return indexCount;
	}

	XMFLOAT3* GetBoundBoxVertex()
	{
		return boundBoxVertex;
	}

	void LoadCarFormFile(const std::string& pFile)                      //���복��
	{
		Assimp::Importer importer;// ����һ�������� 
								  // ʹ�õ���������ѡ����ģ���ļ� 
		const aiScene* scene = importer.ReadFile(pFile,
			aiProcess_CalcTangentSpace |            //�����־���Զ��������ߺ͸�����
			aiProcess_Triangulate |                 //�����־���Զ����ı�����ת��Ϊ������
			//aiProcess_JoinIdenticalVertices |       //�����־���Զ��ϲ���ͬ�Ķ���
			aiProcess_SortByPType);                 //�����־������ͬͼԪ���õ���ͬ��ģ����ȥ��ͼƬ���Ϳ����ǵ㡢ֱ�ߡ������ε�

		if (!scene)//������󣬻�ȡ������Ϣ��������Ӧ�Ĵ���
		{
			return;
		}
		else
		{
			for (unsigned int m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh* mesh = scene->mMeshes[m];

				if (!mesh->HasNormals() || !mesh->HasTextureCoords(0))
				{
					MessageBox(NULL, L"ģ���ļ���û������������߷�����Ϣ", L"Error", MB_OK);
					return;
				}
				vertexCount = mesh->mNumVertices;
				
				XMFLOAT3 min = XMFLOAT3(65535.0f, 65535.0f, 65535.0f);
				XMFLOAT3 max = XMFLOAT3(-65535.0f, -65535.0f, -65535.0f);
				for (unsigned int i = 0; i < vertexCount; ++i)
				{
					Vertex vertex = Vertex();
					vertex.Pos = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
					//��Χ����С��
					min.x = min.x > mesh->mVertices[i].x ? mesh->mVertices[i].x : min.x;
					min.y = min.y > mesh->mVertices[i].y ? mesh->mVertices[i].y : min.y;
					min.x = min.z > mesh->mVertices[i].z ? mesh->mVertices[i].z : min.z;
					//��Χ������
					max.x = max.x < mesh ->mVertices[i].x ? mesh ->mVertices[i].x : max.x;
					max.y = max.y < mesh ->mVertices[i].y ? mesh ->mVertices[i].y : max.y;
					max.x = max.z < mesh ->mVertices[i].z ? mesh ->mVertices[i].z : max.z;

					vertex.Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
					vertex.Tex = XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
					vertices.push_back(vertex);
				}
				boundBoxVertex[0] = min;
				boundBoxVertex[1] = max;
				indexCount = mesh->mNumFaces * 3;
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					const aiFace& Face = mesh->mFaces[i];
					indices.push_back(Face.mIndices[0]);
					indices.push_back(Face.mIndices[1]);
					indices.push_back(Face.mIndices[2]);
				}
			}
		}
	}

	void LoadTrackFormFile(const std::string& pFile)
	{
		Assimp::Importer importer;// ����һ�������� 
								  // ʹ�õ���������ѡ����ģ���ļ� 
		const aiScene* scene = importer.ReadFile(pFile,
			aiProcess_CalcTangentSpace |            //�����־���Զ��������ߺ͸�����
			aiProcess_Triangulate |                 //�����־���Զ����ı�����ת��Ϊ������
			//aiProcess_JoinIdenticalVertices |      //�����־���Զ��ϲ���ͬ�Ķ���
			aiProcess_SortByPType);                 //�����־������ͬͼԪ���õ���ͬ��ģ����ȥ��ͼƬ���Ϳ����ǵ㡢ֱ�ߡ������ε�

		if (!scene)//������󣬻�ȡ������Ϣ��������Ӧ�Ĵ���
		{
			return;
		}
		else
		{
			aiMesh* mesh = scene->mMeshes[0];

			if (!mesh->HasNormals() || !mesh->HasTextureCoords(0))
			{
				MessageBox(NULL, L"ģ���ļ���û������������߷�����Ϣ", L"Error", MB_OK);
				return;
			}
			int vertexCount = mesh->mNumVertices;
			std::vector<Vertex> vertices;
			for (int i = 0; i < vertexCount; ++i)
			{
				Vertex vertex = Vertex();
				vertex.Pos = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex.Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				vertex.Tex = XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				vertices.push_back(vertex);
			}
			indexCount = mesh->mNumFaces * 3;
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				const aiFace& Face = mesh->mFaces[i];
				indices.push_back(Face.mIndices[0]);
				indices.push_back(Face.mIndices[1]);
				indices.push_back(Face.mIndices[2]);
			}
		}
	}
};