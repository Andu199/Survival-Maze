#include "MazeGenerator.h"

using namespace std;

MazeGenerator::MazeGenerator()
{
}

MazeGenerator::~MazeGenerator()
{
}

void MazeGenerator::generateMaze(vector<GameObject>& obstacles,
	vector<GameObject>& entities) {

	setMatrix();
	int wall = rand() % 4;
	int pos = rand() % dimension + 1;
	stack<pair<int, int>> st;
	GameObject victory_wall = GameObject(
		info(glm::vec3(0), 0),
		mesh_info({ "" }, { glm::vec3(1) }, { glm::vec3(0) }, "VertexNormal")
	);
	switch (wall)
	{
	case 0:
		st.push({ 1, pos });
		matrix[1][pos].visited = true;
		matrix[1][pos].walls -= UP;
		victory_wall.pos.position = glm::vec3(cubeSize * pos, 0, cubeSize * 1);
		victory_wall.collider.update(victory_wall.pos.position + glm::vec3(0, 5, -5), glm::vec3(10, 10, 0.1f));
		obstacles.push_back(victory_wall);
		break;
	case 1:
		st.push({ pos, 1 });
		matrix[pos][1].visited = true;
		matrix[pos][1].walls -= LEFT;
		victory_wall.pos.position = glm::vec3(cubeSize * 1, 0, cubeSize * pos);
		victory_wall.collider.update(victory_wall.pos.position + glm::vec3(-5, 5, 0), glm::vec3(0.1f, 10, 10));
		obstacles.push_back(victory_wall);
		break;
	case 2:
		st.push({ dimension, pos });
		matrix[dimension][pos].visited = true;
		matrix[dimension][pos].walls -= DOWN;
		victory_wall.pos.position = glm::vec3(cubeSize * pos, 0, cubeSize * dimension);
		victory_wall.collider.update(victory_wall.pos.position + glm::vec3(0, 5, 5), glm::vec3(10, 10, 0.1f));
		obstacles.push_back(victory_wall);
		break;
	case 3:
		st.push({ pos, dimension });
		matrix[pos][dimension].visited = true;
		matrix[pos][dimension].walls -= RIGHT;
		victory_wall.pos.position = glm::vec3(cubeSize * dimension, 0, cubeSize * pos);
		victory_wall.collider.update(victory_wall.pos.position + glm::vec3(5, 5, 0), glm::vec3(0.1f, 10, 10));
		obstacles.push_back(victory_wall);
		break;
	default:
		break;
	}

	pair<int, int> curr_pos;
	while (!st.empty()) {
		curr_pos = st.top();
		pair<int, int> neighbor = getNeighbor(curr_pos);
		if (neighbor.first == -1) {
			st.pop();
		}
		else {
			matrix[neighbor.first][neighbor.second].visited = true;
			st.push(neighbor);
		}
	}
	addObstacles(obstacles);
	addRandomEntities(entities);
}

void MazeGenerator::setMatrix() {
	for (int i = 0; i < dimension + 2; ++i) {
		matrix[i][0].visited = true;
		matrix[i][dimension + 1].visited = true;
		matrix[0][i].visited = true;
		matrix[dimension + 1][i].visited = true;
	}
}

pair<int, int> MazeGenerator::getNeighbor(pair<int, int> act_pos) {
	if (matrix[act_pos.first][act_pos.second].walls == 0)
		return { -1, -1 };
	if (matrix[act_pos.first - 1][act_pos.second].visited &&
		matrix[act_pos.first + 1][act_pos.second].visited &&
		matrix[act_pos.first][act_pos.second - 1].visited &&
		matrix[act_pos.first][act_pos.second + 1].visited)
		return { -1, -1 };
	
	while (true) {
		if ((matrix[act_pos.first][act_pos.second].walls & UP) && (rand() % 4 == 0) &&
			!matrix[act_pos.first - 1][act_pos.second].visited) {
			matrix[act_pos.first][act_pos.second].walls -= UP;
			matrix[act_pos.first - 1][act_pos.second].walls -= DOWN;
			return { act_pos.first - 1, act_pos.second };
		}
		if ((matrix[act_pos.first][act_pos.second].walls & DOWN) && (rand() % 4 == 0) &&
			!matrix[act_pos.first + 1][act_pos.second].visited) {
			matrix[act_pos.first][act_pos.second].walls -= DOWN;
			matrix[act_pos.first + 1][act_pos.second].walls -= UP;
			return { act_pos.first + 1, act_pos.second };
		}
		if ((matrix[act_pos.first][act_pos.second].walls & LEFT) && (rand() % 4 == 0) &&
			!matrix[act_pos.first][act_pos.second - 1].visited) {
			matrix[act_pos.first][act_pos.second].walls -= LEFT;
			matrix[act_pos.first][act_pos.second - 1].walls -= RIGHT;
			return { act_pos.first, act_pos.second - 1 };
		}
		if ((matrix[act_pos.first][act_pos.second].walls & RIGHT) && (rand() % 4 == 0) &&
			!matrix[act_pos.first][act_pos.second + 1].visited) {
			matrix[act_pos.first][act_pos.second].walls -= RIGHT;
			matrix[act_pos.first][act_pos.second + 1].walls -= LEFT;
			return { act_pos.first, act_pos.second + 1 };
		}
	}
}

void MazeGenerator::addRandomEntities(std::vector<GameObject>& entities) {
	vector<glm::vec3> scales;
	vector<glm::vec3> disp;
	vector<string> meshes_player;

	// body
	meshes_player.push_back("playerBody");
	scales.push_back(glm::vec3(0.5f));
	disp.push_back(glm::vec3(0));

	//head
	meshes_player.push_back("playerHead");
	scales.push_back(glm::vec3(0.5f));
	disp.push_back(headDisp);

	// shoulders
	meshes_player.push_back("playerShoulder");
	meshes_player.push_back("playerShoulder");
	scales.push_back(glm::vec3(0.5f));
	scales.push_back(glm::vec3(0.5f));
	disp.push_back(shoulderDisp);
	disp.push_back(glm::vec3(-shoulderDisp.x, shoulderDisp.y, shoulderDisp.z));

	// hands
	meshes_player.push_back("playerHand");
	meshes_player.push_back("playerHand");
	scales.push_back(glm::vec3(0.5f));
	scales.push_back(glm::vec3(0.5f));
	disp.push_back(handDisp);
	disp.push_back(glm::vec3(-handDisp.x, handDisp.y, handDisp.z));

	// feet
	meshes_player.push_back("playerFoot");
	meshes_player.push_back("playerFoot");
	scales.push_back(glm::vec3(0.5f));
	scales.push_back(glm::vec3(0.5f));
	disp.push_back(footDisp);
	disp.push_back(glm::vec3(-footDisp.x, footDisp.y, footDisp.z));

	entities.push_back(GameObject(
	    info(glm::vec3(dimension / 2.0f * cubeSize, 1.25f,
			dimension / 2.0f * cubeSize), 0),
	    mesh_info(meshes_player, scales, disp, "VertexNormal")
	));

	int enemies_no = rand() % (max_enemies_no - min_enemies_no) + min_enemies_no;
	for (int k = 0; k < enemies_no; ++k) {
		int i = 0;
		int j = 0;
		while (i == 0 && j == 0) {
			i = rand() % dimension + 1;
			j = rand() % dimension + 1;
		}
		GameObject enemy = GameObject(
			info(glm::vec3(cubeSize * j, 0.45f, cubeSize * i - 3.0f), 0),
			mesh_info({ "enemy" }, { glm::vec3(0.03f) }, { glm::vec3(0) }, "VertexNormal")
		);
		enemy.setDirection(direction_info(glm::vec3(-1, 0, 0), enemySpeed));
		enemy.setBorders(borders_info(glm::vec3(cubeSize * j, 0.45f, cubeSize * i)));
		entities.push_back(enemy);
	}
}

void MazeGenerator::addObstacles(std::vector<GameObject>& obstacles) {

	obstacles.push_back(GameObject(
		info(glm::vec3((dimension + 1) / 2.0f * cubeSize, 0,
			(dimension + 1) / 2.0f * cubeSize), 0),
		mesh_info({ "floor" }, { glm::vec3(1) }, { glm::vec3(0, 0.05f, 0) }, "VertexNormal")
	));
	for (int i = 1; i < dimension + 1; ++i) {
		for (int j = 1; j < dimension + 1; ++j) {
			if (matrix[i][j].walls & UP) {
				GameObject wall = GameObject(
					info(glm::vec3(cubeSize * j, 0, cubeSize * i), RADIANS(90)),
					mesh_info({ "obstacle" }, { glm::vec3(1) }, { glm::vec3(5, 5, 0) }, "VertexNormal")
				);
				wall.collider.update(wall.pos.position + glm::vec3(0, 5, -5), glm::vec3(10, 10, 0.1f));
				obstacles.push_back(wall);
			}
			if (matrix[i][j].walls & RIGHT) {
				GameObject wall = GameObject(
					info(glm::vec3(cubeSize * j, 0, cubeSize * i), 0),
					mesh_info({ "obstacle" }, { glm::vec3(1) }, { glm::vec3(5, 5, 0) }, "VertexNormal")
				);
				wall.collider.update(wall.pos.position + glm::vec3(5, 5, 0), glm::vec3(0.5f, 10, 10));
				obstacles.push_back(wall);
			}
			if (matrix[i][j].walls & LEFT) {
				GameObject wall = GameObject(
					info(glm::vec3(cubeSize * j, 0, cubeSize * i), RADIANS(180)),
					mesh_info({ "obstacle" }, { glm::vec3(1) }, { glm::vec3(5, 5, 0) }, "VertexNormal")
				);
				wall.collider.update(wall.pos.position + glm::vec3(-5, 5, 0), glm::vec3(0.5f, 10, 10));
				obstacles.push_back(wall);
			}
			if (matrix[i][j].walls & DOWN) {
				GameObject wall = GameObject(
					info(glm::vec3(cubeSize * j, 0, cubeSize * i), RADIANS(270)),
					mesh_info({ "obstacle" }, { glm::vec3(1) }, { glm::vec3(5, 5, 0) }, "VertexNormal")
				);
				wall.collider.update(wall.pos.position + glm::vec3(0, 5, 5), glm::vec3(10, 10, 0.5f));
				obstacles.push_back(wall);
			}
		}
	}
}