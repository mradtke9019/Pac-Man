#pragma once
#include <fstream>
#include "Model.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#pragma once
class Arena
{
private:
	std::vector<Model*> boxes;
	std::vector<Model*> points;
	std::vector<Model*> fruits;
	Shader* shader;
	float boxWidth;
	std::string modelFile;
	std::string pointModelFile;
	glm::vec3 PlayerInitialPosition;
	std::vector<glm::vec3> GhostInitialPositions;
	std::vector<glm::vec3> Pathing;
	std::vector<glm::vec2> PathIndexes;

	void ParseArenaFile(std::string file, Shader* shader)
	{
		std::vector<std::string> arenaTxt;
		std::ifstream is(file);
		std::string str;
		while (getline(is, str))
		{
			arenaTxt.push_back(str);
		}
		glm::vec3 offset = glm::vec3(-1.0f * boxWidth * (float)arenaTxt.at(0).size() / 2.0f , 0.0f, -1.0f * boxWidth * (float)arenaTxt.size() / 2.0f);
		for (int i = 0; i < arenaTxt.size(); i++)
		{
			for (int j = 0; j < arenaTxt.at(i).size(); j++)
			{
				if (arenaTxt.at(i).at(j) == '/') {
					boxes.push_back(new Model(modelFile, glm::vec3(j * boxWidth, 0, i * boxWidth) + offset, shader));
				}
				else {
					glm::vec3 coordinate = glm::vec3(j * boxWidth, 0, i * boxWidth) + offset;
					if (arenaTxt.at(i).at(j) == 'P')
						PlayerInitialPosition = coordinate;
					else if (arenaTxt.at(i).at(j) == 'G')
						GhostInitialPositions.push_back(coordinate);
					else if (arenaTxt.at(i).at(j) == '.')
						points.push_back(new Model(pointModelFile, coordinate, shader));
					else if (arenaTxt.at(i).at(j) == 'F')
						fruits.push_back(new Model("./fruit.obj", coordinate, shader));

					Pathing.push_back(coordinate);
					PathIndexes.push_back(glm::vec2(i, j));
				}
			}
		}
	}


public:
	Arena(std::string arenaFile, Shader* shader)
		: boxWidth(7.0f), modelFile("./box.obj"), pointModelFile("./point.obj")
	{
		ParseArenaFile(arenaFile,shader);
	}

	float GetBoxWidth();

	bool IsNavigatable(glm::vec3 Position);

	glm::vec3 GetNearestValidPosition(glm::vec3 Position);

	std::vector<glm::vec3> GetPathing();

	std::vector<glm::vec2> GetPathIndexes();

	glm::vec3 GetPlayerInitialPosition();

	std::vector<glm::vec3> GetGhostInitialPositions();

	void Draw();
};