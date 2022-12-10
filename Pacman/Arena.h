#pragma once
#include <fstream>
#include "Model.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#pragma once

enum Type {Point, Box, Fruit, Empty};

struct LightModel {
	glm::mat4 Transform;
	glm::vec3 Position;
	Model* model;
	Type type;
};

class Arena
{
private:
	Model box;
	Model point;
	Model fruit;
	std::vector<LightModel> lightModels;
	std::vector<glm::vec3> teleport;
	Shader* shader;
	float boxWidth;
	std::string modelFile;
	std::string pointModelFile;
	glm::vec3 PlayerInitialPosition;
	std::vector<glm::vec3> GhostInitialPositions;
	std::vector<glm::vec3> Pathing;
	std::vector<glm::vec2> PathIndexes;
	std::vector<std::vector<bool>> ValidPathing;
	std::vector<std::string> arenaTxt;

	void LoadArenaFile(std::string file)
	{
		arenaTxt = std::vector<std::string>();
		std::ifstream is(file);
		std::string str;
		while (getline(is, str))
		{
			arenaTxt.push_back(str);
		}
	}

	void ParseArenaFile(Shader* shader)
	{
		glm::vec3 offset = glm::vec3(-1.0f * boxWidth * (float)arenaTxt.at(0).size() / 2.0f , 0.0f, -1.0f * boxWidth * (float)arenaTxt.size() / 2.0f);
		for (int i = 0; i < arenaTxt.size(); i++)
		{
			ValidPathing.push_back(std::vector<bool>());
			for (int j = 0; j < arenaTxt.at(i).size(); j++)
			{
				bool valid = false;
				LightModel m = LightModel();

				glm::vec3 coordinate = glm::vec3(j * boxWidth, 0, i * boxWidth) + offset;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), coordinate);
				m.type = Empty;
				m.Position = coordinate;
				m.Transform = transform;

				if (arenaTxt.at(i).at(j) == '/') {
					m.model = &box;
					m.type = Box;
				}
				else {
					if (arenaTxt.at(i).at(j) == 'P')
					{
						PlayerInitialPosition = coordinate;
					}
					else if (arenaTxt.at(i).at(j) == 'G')
					{
						GhostInitialPositions.push_back(coordinate);
					}
					else if (arenaTxt.at(i).at(j) == '.')
					{
						m.model = &point;
						m.type = Point;
					}
					else if (arenaTxt.at(i).at(j) == 'F')
					{
						m.model = &fruit;
						m.type = Fruit;
					}
					else if (arenaTxt.at(i).at(j) == 'T')
					{
						teleport.push_back(coordinate);
					}

					Pathing.push_back(coordinate);
					PathIndexes.push_back(glm::vec2(j, i));
					valid =true;
				}
				if (m.type != Empty)
				{
					lightModels.push_back(m);
				}
				ValidPathing.at(i).push_back(valid);
			}
		}
	}


public:
	Arena(std::string arenaFile, Shader* shader)
		: boxWidth(7.0f), modelFile("./box.obj"), pointModelFile("./point.obj")
	{
		box = Model("./box.obj", glm::vec3(0,0,0), shader, glm::vec3(0.0, 0.0, 1.0));
		point = Model("./point.obj", glm::vec3(0, 0, 0), shader, glm::vec3(1.0, 1.0, 0.0));;
		fruit = Model("./fruit.obj", glm::vec3(0, 0, 0), shader, glm::vec3(1.0, 1.0, 0.0));;
		LoadArenaFile(arenaFile);
		ParseArenaFile(shader);
	}
	~Arena()
	{

	}

	void Reset()
	{

	}

	std::vector<std::vector<bool>>* GetValidPathing();

	float GetBoxWidth();

	bool IsNavigatable(glm::vec3 Position);

	glm::vec3 GetNearestValidPosition(glm::vec3 Position);


	// Accepts a position and converts it to the 2D arena grid coordinate
	glm::vec2 WorldToGrid(glm::vec3 Position);
	glm::vec3 GetPlayerInitialPosition();
	std::vector<glm::vec3> GetGhostInitialPositions();
	bool Collision(glm::vec3 p1, glm::vec3 p2);
	void Draw();
	std::vector<LightModel>* GetArenaObjects();
	std::vector<glm::vec3>* GetTeleportPoints();
};