#include "Arena.h"
void Arena::Draw()
{
	for (int i = 0; i < boxes.size(); i++)
	{
		boxes.at(i)->GetShader()->Use();
		boxes.at(i)->SetModelTransform(boxes.at(i)->GetModelTransform());
		boxes.at(i)->Draw();
	}
	for (int i = 0; i < points.size(); i++)
	{
		points.at(i)->GetShader()->Use();
		points.at(i)->SetModelTransform(points.at(i)->GetModelTransform());
		points.at(i)->Draw();
	}
	for (int i = 0; i < fruits.size(); i++)
	{
		fruits.at(i)->GetShader()->Use();
		fruits.at(i)->SetModelTransform(fruits.at(i)->GetModelTransform());
		fruits.at(i)->Draw();
	}
}


float Arena::GetBoxWidth()
{
	return boxWidth;
}

bool Arena::IsNavigatable(glm::vec3 Position)
{
	// Takes a position and determines whether or not it lies in the boundary of the map
	bool valid = false;
	float wiggleRoom = boxWidth/ 2.0f;
	// Only allow the player to move in the direction if it is within the pathing limits
	for (int i = 0; i < Pathing.size(); i++)
	{
		glm::vec3 curr = Pathing.at(i);
		float lowZ = curr.z - wiggleRoom;
		float highZ = curr.z + wiggleRoom;
		float lowX = curr.x - wiggleRoom;
		float highX = curr.x + wiggleRoom;


		if (Position.z >= lowZ && Position.z <= highZ && Position.x >= lowX && Position.x <= highX) {
			valid = true;
			break;
		}
	}
	return valid;
}

// Takes a position and returns the closest valid position
glm::vec3 Arena::GetNearestValidPosition(glm::vec3 Position)
{
	float distance = MAXINT;
	glm::vec3 result = glm::vec3(0,0,0);
	// Only allow the player to move in the direction if it is within the pathing limits
	for (int i = 0; i < Pathing.size(); i++)
	{
		glm::vec3 curr = Pathing.at(i);
		float d = glm::distance(curr, Position);

		if (d < distance) {
			distance = d;
			result = curr;
		}
	}
	return result;
}




std::vector<glm::vec3> Arena::GetPathing()
{
	return Pathing;
}

glm::vec3 Arena::GetPlayerInitialPosition()
{
	return PlayerInitialPosition;
}


std::vector<glm::vec3> Arena::GetGhostInitialPositions()
{
	return GhostInitialPositions;
}