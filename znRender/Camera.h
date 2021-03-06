#pragma once

#include "Ray.h"
#include "RenderEvents.h"
#include "Viewport.h"

class Camera : public Object
{
public:
	enum class Space
	{
		Local,
		World
	};

public:
	Camera();
	virtual ~Camera() {};

	// TODO: Viewports belong in the rasterizer state.. not the camera. Set view port parameters
	void SetViewport(const Viewport& viewport);
	const Viewport& GetViewport() const;

	const Frustum& GetFrustum() const;

	void SetProjectionRH(float fovy, float aspect, float zNear, float zFar);
	void SetProjectionLH(float fovy, float aspect, float zNear, float zFar);
    void SetOrthographic(float left, float right, float top, float bottom);

	float GetNearClipPlane() const;
	float GetFarClipPlane() const;


	// Translate
	void TranslateX(float x, Space space = Space::Local);
	void TranslateY(float y, Space space = Space::Local);
	void TranslateZ(float z, Space space = Space::Local);

	void SetTranslate(cvec3 translate);
	vec3 GetTranslation() const;

    void DoMoveFront(float Value = 1.0f);
    void DoMoveBack(float Value = 1.0f);
    void DoMoveLeft(float Value = 1.0f);
    void DoMoveRight(float Value = 1.0f);

	// Rotate
    void SetYaw(float Yaw);
    void AddYaw(float Yaw);

    void SetPitch(float Pitch);
    void AddPitch(float Pitch);

    glm::vec3 GetDirection() const;

	bool IsDirty() const;
	
	// Matrices
	mat4 GetViewMatrix() const;
	mat4 GetProjectionMatrix() const;
	mat4 GetViewProjectionInverseMatrix() const; // Get the view projection inverse matrix (useful for picking)

protected:
    virtual void UpdateDirections();

	virtual void UpdateViewMatrix();
	virtual void UpdateViewProjectionInverse();

	Viewport    m_Viewport;
	Frustum     m_Frustum;

	// Projection parameters
	float       m_fVFOV;
	float       m_fAspect;
	float       m_fNear;
	float       m_fFar;

	// World space parameters
	vec3        m_Translate;

    vec3        m_Front;
    vec3        m_Right;
    vec3        m_Up;

    float       m_Yaw_X;
    float       m_Pitch_Y;

	mat4        m_ViewMatrix;
	mat4        m_ProjectionMatrix;
	mat4	    m_ViewProjectionInverse;

	bool        m_bViewDirty;
	bool		m_bViewProjectionInverseDirty;
};