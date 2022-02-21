#pragma once

#include "components/simple_scene.h"
#include "GameObject.h"


#include <iostream>
#include <vector>
#include <stack>

constexpr int UP = 0x1000;
constexpr int DOWN = 0x0100;
constexpr int LEFT = 0x0010;
constexpr int RIGHT = 0x0001;

struct Cell {
	Cell() : walls(0x1111), visited(false) {}
	int walls;
	bool visited;
};

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();
	void generateMaze(std::vector<GameObject>&, std::vector<GameObject>&);

private:
	std::pair<int, int> getNeighbor(std::pair<int, int>);
	void setMatrix();
	void addObstacles(std::vector<GameObject>&);
	void addRandomEntities(std::vector<GameObject>&);
	int dimension = 10;
	int cubeSize = 10;
	std::vector<std::vector<Cell>> matrix =
		std::vector<std::vector<Cell>>(dimension + 2, std::vector<Cell>(dimension + 2, Cell()));

	glm::vec3 handDisp = glm::vec3(0.77f, -0.75f, 0);
	glm::vec3 shoulderDisp = glm::vec3(0.77f, 0.25f, 0);
	glm::vec3 footDisp = glm::vec3(0.3f, -1.77f, 0);
	glm::vec3 headDisp = glm::vec3(0, 1.27f, 0);

	int max_enemies_no = 10;
	int min_enemies_no = 3;
	float enemySpeed = 6.0f;
};