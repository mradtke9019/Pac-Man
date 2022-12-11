#include "Arena.h"
void Arena::Draw()
{
	for (int i = 0; i < lightModels.size(); i++)
	{
		lightModels[i].model->GetShader()->Use();
		lightModels[i].model->SetModelTransform(lightModels[i].Transform);
		lightModels[i].model->Draw();
	}
}

std::vector<std::vector<bool>>* Arena::GetValidPathing()
{
	return &ValidPathing;
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



glm::vec2 Arena::WorldToGrid(glm::vec3 Position)
{
	float distance = MAXINT;
	glm::vec2 result = glm::vec2(MAXINT, MAXINT);
	// Only allow the player to move in the direction if it is within the pathing limits
	for (int i = 0; i < Pathing.size(); i++)
	{
		glm::vec3 curr = Pathing.at(i);
		float d = glm::distance(curr, Position);

		if (d < distance) {
			distance = d;
			result = PathIndexes.at(i);
		}
	}
	return result;
}

glm::vec3 Arena::GetPlayerInitialPosition()
{
	return PlayerInitialPosition;
}


std::vector<glm::vec3> Arena::GetGhostInitialPositions()
{
	return GhostInitialPositions;
}


bool Arena::Collision(glm::vec3 p1, glm::vec3 p2)
{
	return WorldToGrid(p1) == WorldToGrid(p2);
}



std::vector<LightModel>* Arena::GetArenaObjects()
{
	return &lightModels;
}


std::vector<glm::vec3>* Arena::GetTeleportPoints()
{
	return &teleport;
}