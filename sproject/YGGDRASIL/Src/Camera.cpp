#include "Camera.h"

//**********************************************************************************************
//FUNCTION:
void CCamera::processKeyboard(ECameraMovement vDirection, float vDeltaTime)
{
	float velocity = MovementSpeed * vDeltaTime;
	if (vDirection == ECameraMovement::FORWARD)
		Position += Front * velocity;
	if (vDirection == ECameraMovement::BACKWARD)
		Position -= Front * velocity;
	if (vDirection == ECameraMovement::LEFT)
		Position -= Right * velocity;
	if (vDirection == ECameraMovement::RIGHT)
		Position += Right * velocity;
	if (vDirection == ECameraMovement::UP)
		Position += Up * velocity;
	if (vDirection == ECameraMovement::DOWN)
		Position -= Up * velocity;
}

//**********************************************************************************************
//FUNCTION:
void CCamera::processMouseMovement(float vXOffset, float vYOffset, GLboolean vConstrainPitch)
{
	vXOffset *= MouseSensitivity;
	vYOffset *= MouseSensitivity;

	Yaw += vXOffset;
	Pitch += vYOffset;

	if (vConstrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	__updateCameraVectors();
}

//**********************************************************************************************
//FUNCTION:
void CCamera::processMouseScroll(float vYOffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= vYOffset * SCROLL_SENSITIVTY;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

//**********************************************************************************************
//FUNCTION:
void CCamera::__updateCameraVectors()
{
	glm::vec3 LocFront;
	LocFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	LocFront.y = sin(glm::radians(Pitch));
	LocFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(LocFront);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}