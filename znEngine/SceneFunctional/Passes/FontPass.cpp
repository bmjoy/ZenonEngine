#include "stdafx.h"

// Include
#include <Application.h>
#include <SceneFunctional\\Scene3D.h>
#include <SceneFunctional\\SceneNode3D.h>

// General
#include "FontPass.h"

// Additional
#include "UI\\Fonts\\Font.h"

CFontPass::CFontPass()
	: m_pRenderEventArgs(nullptr)
	, m_RenderDevice(_RenderDevice)
{

}

CFontPass::CFontPass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: m_pRenderEventArgs(nullptr)
	, m_Scene(scene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{

}

CFontPass::~CFontPass()
{
}


void CFontPass::PreRender(Render3DEventArgs& e)
{
	e.PipelineState = m_Pipeline.get();
	SetRenderEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}
}

void CFontPass::Render(Render3DEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(*this);
	}
}

void CFontPass::PostRender(Render3DEventArgs& e)
{
	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}

// Inherited from Visitor

bool CFontPass::Visit(std::shared_ptr<SceneNode3D> node)
{
	m_pRenderEventArgs->Node = node.operator->();

	const Camera* camera = GetRenderEventArgs().Camera;
	if (camera)
	{
		node->UpdateCamera(camera);

		PerObject perObjectData;
		perObjectData.Model = node->GetComponent<CTransformComponent>()->GetWorldTransfom();
		perObjectData.ModelView = camera->GetViewMatrix()       * perObjectData.Model;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;
		//perObjectData.Model = node.GetWorldTransfom();
		//perObjectData.ModelView = perObjectData.Model;
		//perObjectData.ModelViewProjection = camera->GetViewport().OrthoMatrix * perObjectData.ModelView;

		perObjectData.View = mat4(1.0f);
		perObjectData.Projection = camera->GetViewport().OrthoMatrix;

		// Update the constant buffer data
		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}

bool CFontPass::Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
    std::shared_ptr<CFontMesh> fontMesh = std::dynamic_pointer_cast<CFontMesh, IMesh>(Mesh);
    if (fontMesh)
    {
		return Mesh->Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}

bool CFontPass::Visit(std::shared_ptr<CLight3D> light)
{
	return false;
}

void CFontPass::UpdateViewport(Viewport _viewport)
{
	m_Pipeline->GetRasterizerState().SetViewport(_viewport);
}

//----------------------------------------------------------------------

void CFontPass::SetRenderEventArgs(Render3DEventArgs& e)
{
	m_pRenderEventArgs = &e;
}

Render3DEventArgs& CFontPass::GetRenderEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

//----------------------------------------------------------------------

std::shared_ptr<IRenderDevice> CFontPass::GetRenderDevice() const
{
	return m_RenderDevice.lock();
}

std::shared_ptr<PipelineState> CFontPass::GetPipelineState() const
{
	return m_Pipeline;
}