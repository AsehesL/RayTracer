#include "GlobalResource.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/RenderBuffer.h"

static GlobalResource* gResource = nullptr;

void GlobalResource::Init(GLContext* glContext)
{
	if (gResource == nullptr)
		gResource = new GlobalResource(glContext);
}

void GlobalResource::Release()
{
	if (gResource != nullptr)
		delete gResource;
	gResource = nullptr;
}

Mesh* GlobalResource::GetCubeMesh()
{
	if (gResource == nullptr)
		return nullptr;
	if (!gResource->m_cubeMesh)
	{
		Mesh* cubeMesh = nullptr;
		cubeMesh = new Mesh(24, 36, gResource->m_glContext);

		cubeMesh->SetVertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0, 0, -1), Vector4(1, 0, 0, -1), Vector2(0, 0), 0);
		cubeMesh->SetVertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0, 0, -1), Vector4(1, 0, 0, -1), Vector2(0, 1), 1);
		cubeMesh->SetVertex(Vector3(0.5f, 0.5f, -0.5f), Vector3(0, 0, -1), Vector4(1, 0, 0, -1), Vector2(1, 1), 2);
		cubeMesh->SetVertex(Vector3(0.5f, -0.5f, -0.5f), Vector3(0, 0, -1), Vector4(1, 0, 0, -1), Vector2(1, 0), 3);

		cubeMesh->SetVertex(Vector3(0.5f, -0.5f, -0.5f), Vector3(1, 0, 0), Vector4(0, 0, 1, -1), Vector2(0, 0), 4);
		cubeMesh->SetVertex(Vector3(0.5f, 0.5f, -0.5f), Vector3(1, 0, 0), Vector4(0, 0, 1, -1), Vector2(0, 1), 5);
		cubeMesh->SetVertex(Vector3(0.5f, 0.5f, 0.5f), Vector3(1, 0, 0), Vector4(0, 0, 1, -1), Vector2(1, 1), 6);
		cubeMesh->SetVertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(1, 0, 0), Vector4(0, 0, 1, -1), Vector2(1, 0), 7);

		cubeMesh->SetVertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0, 0, 1), Vector4(-1, 0, 0, -1), Vector2(0, 0), 8);
		cubeMesh->SetVertex(Vector3(0.5f, 0.5f, 0.5f), Vector3(0, 0, 1), Vector4(-1, 0, 0, -1), Vector2(0, 1), 9);
		cubeMesh->SetVertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0, 0, 1), Vector4(-1, 0, 0, -1), Vector2(1, 1), 10);
		cubeMesh->SetVertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0, 0, 1), Vector4(-1, 0, 0, -1), Vector2(1, 0), 11);

		cubeMesh->SetVertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(-1, 0, 0), Vector4(0, 0, -1, -1), Vector2(0, 0), 12);
		cubeMesh->SetVertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3(-1, 0, 0), Vector4(0, 0, -1, -1), Vector2(0, 1), 13);
		cubeMesh->SetVertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3(-1, 0, 0), Vector4(0, 0, -1, -1), Vector2(1, 1), 14);
		cubeMesh->SetVertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3(-1, 0, 0), Vector4(0, 0, -1, -1), Vector2(1, 0), 15);

		cubeMesh->SetVertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(0, 0), 16);
		cubeMesh->SetVertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(0, 1), 17);
		cubeMesh->SetVertex(Vector3(0.5f, 0.5f, 0.5f), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(1, 1), 18);
		cubeMesh->SetVertex(Vector3(0.5f, 0.5f, -0.5f), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(1, 0), 19);

		cubeMesh->SetVertex(Vector3(0.5f, -0.5f, -0.5f), Vector3(0, -1, 0), Vector4(-1, 0, 0, -1), Vector2(0, 0), 20);
		cubeMesh->SetVertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0, -1, 0), Vector4(-1, 0, 0, -1), Vector2(0, 1), 21);
		cubeMesh->SetVertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0, -1, 0), Vector4(-1, 0, 0, -1), Vector2(1, 1), 22);
		cubeMesh->SetVertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0, -1, 0), Vector4(-1, 0, 0, -1), Vector2(1, 0), 23);

		cubeMesh->SetIndex(0, 0);
		cubeMesh->SetIndex(1, 1);
		cubeMesh->SetIndex(2, 2);
		cubeMesh->SetIndex(0, 3);
		cubeMesh->SetIndex(2, 4);
		cubeMesh->SetIndex(3, 5);

		cubeMesh->SetIndex(4, 6);
		cubeMesh->SetIndex(5, 7);
		cubeMesh->SetIndex(6, 8);
		cubeMesh->SetIndex(4, 9);
		cubeMesh->SetIndex(6, 10);
		cubeMesh->SetIndex(7, 11);

		cubeMesh->SetIndex(8, 12);
		cubeMesh->SetIndex(9, 13);
		cubeMesh->SetIndex(10, 14);
		cubeMesh->SetIndex(8, 15);
		cubeMesh->SetIndex(10, 16);
		cubeMesh->SetIndex(11, 17);

		cubeMesh->SetIndex(12, 18);
		cubeMesh->SetIndex(13, 19);
		cubeMesh->SetIndex(14, 20);
		cubeMesh->SetIndex(12, 21);
		cubeMesh->SetIndex(14, 22);
		cubeMesh->SetIndex(15, 23);

		cubeMesh->SetIndex(16, 24);
		cubeMesh->SetIndex(17, 25);
		cubeMesh->SetIndex(18, 26);
		cubeMesh->SetIndex(16, 27);
		cubeMesh->SetIndex(18, 28);
		cubeMesh->SetIndex(19, 29);

		cubeMesh->SetIndex(20, 30);
		cubeMesh->SetIndex(21, 31);
		cubeMesh->SetIndex(22, 32);
		cubeMesh->SetIndex(20, 33);
		cubeMesh->SetIndex(22, 34);
		cubeMesh->SetIndex(23, 35);

		gResource->m_meshObjectPool->Add(cubeMesh);
		gResource->m_cubeMesh = cubeMesh;
	}
	return gResource->m_cubeMesh;
}

Mesh* GlobalResource::GetSphereMesh()
{
	if (gResource == nullptr)
		return nullptr;
	if (!gResource->m_sphereMesh)
	{
		const int subdivision = 32;

		const double radius = 0.5f;

		const double deltaAngleHorizontal = 360.0 / subdivision;
		const double deltaAngleVertical = deltaAngleHorizontal * 0.5;
		
		const double uvDelta = 1.0 / subdivision;

		const int vertexCount = (subdivision - 1) * (subdivision + 1);
		const int indexCount = (subdivision - 2) * subdivision * 6;

		Mesh* sphereMesh = nullptr;
		sphereMesh = new Mesh(vertexCount + subdivision * 2, indexCount + subdivision * 6, gResource->m_glContext);

		int vindex = 0;
		for (int i = 0; i <= subdivision - 2; i++)
		{
			for (int j = 0; j <= subdivision; j++)
			{
				double y = radius * cos(deltaAngleVertical * (i + 1) * Math::DegToRad);
				double r = radius * sin(deltaAngleVertical * (i + 1) * Math::DegToRad);
				double x = r * cos(deltaAngleHorizontal * j * Math::DegToRad);
				double z = r * sin(deltaAngleHorizontal * j * Math::DegToRad);

				double u = uvDelta * (i + 1);
				double v = uvDelta * j;

				Vector3 tg = Vector3::Cross(Vector3(x,0,z), Vector3(0,1,0));
				tg.Normalize();

				Vector3 pos = Vector3(x, y, z);
				Vector3 nor;
				pos.GetNormalized(nor);
				sphereMesh->SetVertex(pos, vindex);
				sphereMesh->SetNormal(nor, vindex);
				sphereMesh->SetTangent(Vector4(tg.x,tg.y,tg.z,-1), vindex);
				sphereMesh->SetUV(Vector2(u, v), 0, vindex);

				vindex++;
			}
		}

		for (int i = 0; i < subdivision; i++)
		{
			double x = cos(deltaAngleHorizontal * (0.5f + i) * Math::DegToRad);
			double z = sin(deltaAngleHorizontal * (0.5f + i) * Math::DegToRad);

			Vector3 tg = Vector3::Cross(Vector3(x, 0, z), Vector3(0, 1, 0));
			tg.Normalize();

			sphereMesh->SetVertex(Vector3(0, radius, 0), vindex);
			sphereMesh->SetVertex(Vector3(0, -radius, 0), vindex + subdivision);
			sphereMesh->SetNormal(Vector3(0, 1, 0), vindex);
			sphereMesh->SetNormal(Vector3(0, -1, 0), vindex + subdivision);
			sphereMesh->SetTangent(Vector4(tg.x, tg.y, tg.z, -1), vindex);
			sphereMesh->SetTangent(Vector4(tg.x, tg.y, tg.z, -1), vindex + subdivision);
			sphereMesh->SetUV(Vector2(uvDelta * i + uvDelta * 0.5f, 0.0f), 0, vindex);
			sphereMesh->SetUV(Vector2(uvDelta * i + uvDelta * 0.5f, 1.0f), 0, vindex + subdivision);

			vindex++;
		}

		int tindex = 0;
		for (int i = 0; i < subdivision - 2; i++)
		{
			for (int j = 0; j < subdivision; j++)
			{
				int index0 = i * (subdivision + 1) + j;
				int index1 = i * (subdivision + 1) + j + 1;
				int index2 = (i + 1) * (subdivision + 1) + j;
				int index3 = (i + 1) * (subdivision + 1) + j + 1;

				sphereMesh->SetIndex(index0, tindex);
				sphereMesh->SetIndex(index1, tindex + 1);
				sphereMesh->SetIndex(index2, tindex + 2);
				sphereMesh->SetIndex(index2, tindex + 3);
				sphereMesh->SetIndex(index1, tindex + 4);
				sphereMesh->SetIndex(index3, tindex + 5);
				tindex += 6;
			}
		}
		for (int i = 0; i < subdivision; i++)
		{
			sphereMesh->SetIndex(i, tindex);
			sphereMesh->SetIndex(vertexCount + i, tindex + 1);
			sphereMesh->SetIndex(i + 1, tindex + 2);
			sphereMesh->SetIndex(vertexCount - (subdivision + 1) + i, tindex + 3);
			sphereMesh->SetIndex(vertexCount - (subdivision + 1) + 1 + i, tindex + 4);
			sphereMesh->SetIndex(vertexCount + subdivision + i, tindex + 5);
			tindex += 6;
		}

		gResource->m_meshObjectPool->Add(sphereMesh);
		gResource->m_sphereMesh = sphereMesh;
	}
	return gResource->m_sphereMesh;
}

Mesh* GlobalResource::GetPlaneMesh()
{
	if (gResource == nullptr)
		return nullptr;
	if (!gResource->m_planeMesh)
	{
		Mesh* planeMesh = nullptr;
		planeMesh = new Mesh(4, 6, gResource->m_glContext);

		planeMesh->SetVertex(Vector3(-5, 0, -5), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(0, 1), 0);
		planeMesh->SetVertex(Vector3(-5, 0, 5), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(0, 0), 1);
		planeMesh->SetVertex(Vector3(5, 0, 5), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(1, 0), 2);
		planeMesh->SetVertex(Vector3(5, 0, -5), Vector3(0, 1, 0), Vector4(1, 0, 0, -1), Vector2(1, 1), 3);

		planeMesh->SetIndex(0, 0);
		planeMesh->SetIndex(1, 1);
		planeMesh->SetIndex(2, 2);
		planeMesh->SetIndex(0, 3);
		planeMesh->SetIndex(2, 4);
		planeMesh->SetIndex(3, 5);

		gResource->m_meshObjectPool->Add(planeMesh);
		gResource->m_planeMesh = planeMesh; 
	}
	return gResource->m_planeMesh;
}

Mesh* GlobalResource::GetScreenQuadMesh()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_screenQuadMesh == nullptr)
	{
		Mesh* screenQuad = nullptr;
		screenQuad = new Mesh(4, 6, gResource->m_glContext);

		screenQuad->SetVertex(Vector3(-1, -1, 0), Vector3(0, 0, 1), Vector4(0, 0, 0, 0), Vector2(0, 1), 0);
		screenQuad->SetVertex(Vector3(-1, 1, 0), Vector3(0, 0, 1), Vector4(0, 0, 0, 0), Vector2(0, 0), 1);
		screenQuad->SetVertex(Vector3(1, 1, 0), Vector3(0, 0, 1), Vector4(0, 0, 0, 0), Vector2(1, 0), 2);
		screenQuad->SetVertex(Vector3(1, -1, 0), Vector3(0, 0, 1), Vector4(0, 0, 0, 0), Vector2(1, 1), 3);

		screenQuad->SetIndex(0, 0);
		screenQuad->SetIndex(1, 1);
		screenQuad->SetIndex(2, 2);
		screenQuad->SetIndex(0, 3);
		screenQuad->SetIndex(2, 4);
		screenQuad->SetIndex(3, 5);

		gResource->m_meshObjectPool->Add(screenQuad);
		gResource->m_screenQuadMesh = screenQuad;
	}
	return gResource->m_screenQuadMesh;
}

Mesh* GlobalResource::GetSkySphereMesh()
{
	if (gResource == nullptr)
		return nullptr;
	if (!gResource->m_skySphereMesh)
	{
		const int horizontalSubdivision = 24;
		const int verticalSubdivision = 17;

		const int vertexCount = horizontalSubdivision * verticalSubdivision + 2;
		const int indexCount = (verticalSubdivision - 1) * horizontalSubdivision * 6 + horizontalSubdivision * 2 * 3;

		const double vdelta = 180.0 / (verticalSubdivision + 1) * Math::DegToRad;
		const double hdelta = 360.0 / horizontalSubdivision * Math::DegToRad;

		Mesh* skySphere = nullptr;
		skySphere = new Mesh(vertexCount, indexCount, gResource->m_glContext);

		for (int i = 1; i <= verticalSubdivision; i++)
		{
			for (int j = 0; j < horizontalSubdivision; j++)
			{
				double cvangle = i * vdelta;
				double changle = j * hdelta;

				double y = cos(cvangle);

				double r = sin(cvangle);

				double x = r * cos(changle);
				double z = r * sin(changle);

				skySphere->SetVertex(Vector3(x, y, z), (i - 1) * horizontalSubdivision + j);

				int index = (i - 1) * horizontalSubdivision + j;
				if (i != verticalSubdivision)
				{
					if (j != horizontalSubdivision - 1)
					{
						skySphere->SetIndex((i - 1) * horizontalSubdivision + j, index * 6);
						skySphere->SetIndex((i) * horizontalSubdivision + j, index * 6 + 1);
						skySphere->SetIndex((i - 1) * horizontalSubdivision + j + 1, index * 6 + 2);

						skySphere->SetIndex((i - 1) * horizontalSubdivision + j + 1, index * 6 + 3);
						skySphere->SetIndex((i) * horizontalSubdivision + j, index * 6 + 4);
						skySphere->SetIndex((i) * horizontalSubdivision + j + 1, index * 6 + 5);
					}
					else
					{
						skySphere->SetIndex((i - 1) * horizontalSubdivision + j, index * 6);
						skySphere->SetIndex((i)*horizontalSubdivision + j, index * 6 + 1);
						skySphere->SetIndex((i - 1) * horizontalSubdivision, index * 6 + 2);

						skySphere->SetIndex((i - 1) * horizontalSubdivision, index * 6 + 3);
						skySphere->SetIndex((i)*horizontalSubdivision + j, index * 6 + 4);
						skySphere->SetIndex((i)*horizontalSubdivision, index * 6 + 5);
					}
				}
			}
		}

		skySphere->SetVertex(Vector3(0, 1, 0), verticalSubdivision * horizontalSubdivision);
		skySphere->SetVertex(Vector3(0, -1, 0), verticalSubdivision * horizontalSubdivision + 1);

		for (int i = 0; i < horizontalSubdivision; i++)
		{
			if (i != horizontalSubdivision - 1)
			{
				skySphere->SetIndex(verticalSubdivision * horizontalSubdivision, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6);
				skySphere->SetIndex(i, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 1);
				skySphere->SetIndex(i + 1, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 2);

				skySphere->SetIndex(verticalSubdivision * horizontalSubdivision + 1, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 3);
				skySphere->SetIndex((verticalSubdivision - 1) * horizontalSubdivision + i + 1, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 4);
				skySphere->SetIndex((verticalSubdivision - 1) * horizontalSubdivision + i, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 5);
			}
			else
			{
				skySphere->SetIndex(verticalSubdivision * horizontalSubdivision, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6);
				skySphere->SetIndex(i, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 1);
				skySphere->SetIndex(0, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 2);

				skySphere->SetIndex(verticalSubdivision * horizontalSubdivision + 1, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 3);
				skySphere->SetIndex((verticalSubdivision - 1) * horizontalSubdivision, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 4);
				skySphere->SetIndex((verticalSubdivision - 1) * horizontalSubdivision + i, (verticalSubdivision - 1) * horizontalSubdivision * 6 + i * 6 + 5);
			}
		}

		gResource->m_meshObjectPool->Add(skySphere);
		gResource->m_skySphereMesh = skySphere;
	}
	return gResource->m_skySphereMesh;
}

Mesh* GlobalResource::CreateMesh(unsigned int vertexCount, unsigned int indexCount, int& meshID)
{
	meshID = -1;
	if (gResource == nullptr)
		return nullptr;
	Mesh* mesh = new Mesh(vertexCount, indexCount, gResource->m_glContext);
	meshID = gResource->m_meshObjectPool->Add(mesh);
	return mesh;
}

Texture* GlobalResource::GetBlackTexture()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_blackTexture == nullptr)
	{
		Texture* blackTexture = CreateTexture(1, 1);
		blackTexture->SetPixel(0, 0, Color(0,0,0,1));
		blackTexture->ApplyData();

		gResource->m_blackTexture = blackTexture;
	}
	return gResource->m_blackTexture;
}

Texture* GlobalResource::GetWhiteTexture()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_whiteTexture == nullptr)
	{
		Texture* whiteTexture = CreateTexture(1, 1);
		whiteTexture->SetPixel(0, 0, Color(1, 1, 1, 1));
		whiteTexture->ApplyData();

		gResource->m_whiteTexture = whiteTexture;
	}
	return gResource->m_whiteTexture;
}

Texture* GlobalResource::GetDefaultBumpMap()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_defaultBumpMap == nullptr)
	{
		Texture* bumpMap = CreateTexture(1, 1);
		bumpMap->SetPixel(0, 0, Color(0.5, 0.5, 1, 1));
		bumpMap->ApplyData();

		gResource->m_defaultBumpMap = bumpMap;
	}
	return gResource->m_defaultBumpMap;
}

RenderTexture* GlobalResource::GetPreIntegratedBRDFTexture()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_preIntegratedBRDFTexture == nullptr)
	{
		if (gResource->m_integrateBRDFShader == nullptr)
		{
			IntegrateBRDFShader* integratedShader = CreateShader<IntegrateBRDFShader>();
			gResource->m_integrateBRDFShader = integratedShader;
		}
		Mesh* screenQuadMesh = GetScreenQuadMesh();

		RenderTexture* renderTexture = new RenderTexture(256,256,false,gResource->m_glContext);
		gResource->m_preIntegratedBRDFTexture = renderTexture;


		gResource->m_glContext->SetRenderBuffer(renderTexture->GetColorBuffer(), renderTexture->GetDepthBuffer());

		gResource->m_glContext->SetViewport(0, 0, renderTexture->GetWidth(), renderTexture->GetHeight());

		gResource->m_glContext->ClearColor(0, 0, 0, 0);
		gResource->m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);

		if (screenQuadMesh && gResource->m_integrateBRDFShader && renderTexture)
		{
			gResource->m_integrateBRDFShader->SetRenderSize(renderTexture->GetWidth(), renderTexture->GetHeight(), renderTexture->GetWidth(), renderTexture->GetHeight());
			
			if (gResource->m_integrateBRDFShader->Execute())
			{
				screenQuadMesh->Commit();
			}
		}
	}
	return gResource->m_preIntegratedBRDFTexture;
}

bool GlobalResource::RemoveShader(int shaderID)
{
	if (gResource == nullptr)
		return false;
	return gResource->m_shaderObjectPool->Delete(shaderID);
}

bool GlobalResource::RemoveComputeShader(int computeShaderID)
{
	if (gResource == nullptr)
		return false;
	return gResource->m_computeShaderObjectPool->Delete(computeShaderID);
}

Texture* GlobalResource::CreateTexture(unsigned int width, unsigned int height, int& textureID)
{
	textureID = -1;
	if (gResource == nullptr)
		return nullptr;
	Texture* texture = new Texture(width, height, gResource->m_glContext);
	textureID = gResource->m_texturePool->Add(texture);
	return texture;
}

Texture* GlobalResource::CreateTexture(unsigned int width, unsigned int height)
{
	int textureID = -1;
	return CreateTexture(width, height, textureID);
}

Texture* GlobalResource::CreateTexture(const char* path, bool isLinear, int& textureID)
{
	textureID = -1;
	if (gResource == nullptr)
		return nullptr;
	Texture* texture = new Texture(path, isLinear, gResource->m_glContext);
	textureID = gResource->m_texturePool->Add(texture);
	return texture;
}

Texture* GlobalResource::CreateTexture(const char* path, bool isLinear)
{
	int textureID = -1;
	return CreateTexture(path, isLinear, textureID);
}

bool GlobalResource::RemoveTexture(int textureID)
{
	if (gResource == nullptr)
		return false;
	return gResource->m_texturePool->Delete(textureID);
}

Mesh* GlobalResource::GetMesh(int meshID)
{
	if (gResource == nullptr)
		return nullptr;
	return gResource->m_meshObjectPool->Get(meshID);
}

bool GlobalResource::RemoveMesh(int meshID)
{
	if (gResource == nullptr)
		return false;
	return gResource->m_meshObjectPool->Delete(meshID);
}

Texture* GlobalResource::GetTexture(int textureID)
{
	if (gResource == nullptr)
		return nullptr;
	return gResource->m_texturePool->Get(textureID);
}

ScreenQuadShader* GlobalResource::GetScreenQuadShader()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_screenQuadShader == nullptr)
	{
		ScreenQuadShader* screenQuadShader = new ScreenQuadShader(gResource->m_glContext);

		gResource->m_shaderObjectPool->Add(screenQuadShader);

		gResource->m_screenQuadShader = screenQuadShader;
	}
	return gResource->m_screenQuadShader;
}

LinearToGammaShader* GlobalResource::GetLinearToGammaShader()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_linearToGammaShader == nullptr)
	{
		LinearToGammaShader* linearToGammaShader = new LinearToGammaShader(gResource->m_glContext);

		gResource->m_shaderObjectPool->Add(linearToGammaShader);

		gResource->m_linearToGammaShader = linearToGammaShader;
	}
	return gResource->m_linearToGammaShader;
}

ErrorShader* GlobalResource::GetErrorShader()
{
	if (gResource == nullptr)
		return nullptr;
	if (gResource->m_errorShader == nullptr)
	{
		ErrorShader* errorShader = new ErrorShader(gResource->m_glContext);

		gResource->m_shaderObjectPool->Add(errorShader);

		gResource->m_errorShader = errorShader;
	}
	return gResource->m_errorShader;
}

GlobalResource::GlobalResource(GLContext* glContext)
{
	m_shaderObjectPool = new ObjectPool<ShaderBase>();
	m_computeShaderObjectPool = new ObjectPool<ComputeShaderBase>();
	m_meshObjectPool = new ObjectPool<Mesh>();
	m_texturePool = new ObjectPool<Texture>();

	m_cubeMesh = nullptr;
	m_sphereMesh = nullptr;
	m_planeMesh = nullptr;
	m_skySphereMesh = nullptr;
	m_screenQuadMesh = nullptr;

	m_screenQuadShader = nullptr;
	m_integrateBRDFShader = nullptr;
	m_linearToGammaShader = nullptr;
	m_errorShader = nullptr;

	m_blackTexture = nullptr;
	m_whiteTexture = nullptr;
	m_defaultBumpMap = nullptr;

	m_preIntegratedBRDFTexture = nullptr;

	m_glContext = glContext;
}

GlobalResource::~GlobalResource()
{
	delete m_shaderObjectPool;
	delete m_computeShaderObjectPool;
	delete m_meshObjectPool;
	delete m_texturePool;

	if (m_preIntegratedBRDFTexture)
		delete m_preIntegratedBRDFTexture;
	m_preIntegratedBRDFTexture = nullptr;

	m_cubeMesh = nullptr;
	m_sphereMesh = nullptr;
	m_planeMesh = nullptr;
	m_skySphereMesh = nullptr;
	m_screenQuadMesh = nullptr;

	m_screenQuadShader = nullptr;
	m_integrateBRDFShader = nullptr;
	m_linearToGammaShader = nullptr;
	m_errorShader = nullptr;

	m_blackTexture = nullptr;
	m_whiteTexture = nullptr;
	m_defaultBumpMap = nullptr;
}

GlobalResource* GlobalResource::GetInstance()
{
	return gResource;
}
