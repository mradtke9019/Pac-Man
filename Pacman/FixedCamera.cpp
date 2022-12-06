#include "FixedCamera.h"

FixedCamera::FixedCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	Position = position;// glm::vec3(0.0f, 3.0f, 3.0f);
	Target = target;
	Up = up;

	View = glm::lookAt(Position, Target, Up);
};

glm::vec3 FixedCamera::GetPosition()
{
	return Position;
}

glm::mat4* FixedCamera::GetViewTransform()
{
	return &View;
};

void FixedCamera::SetPosition(glm::vec3 position)
{
	Position = position;
};

void FixedCamera::SetTarget(glm::vec3 target)
{
	Target = target;
};

glm::vec3 FixedCamera::GetTarget()
{
	return Target;
};
