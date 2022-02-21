#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/lab3/object2D.h"


class Generator {
public:
	Generator();
	~Generator();
	void generate(std::unordered_map<std::string, Mesh*>&, std::string);
private:
	void GeneratePlayer(std::unordered_map<std::string, Mesh*>&);
	void GenerateEnemy(std::unordered_map<std::string, Mesh*>&, std::string);
	void GenerateObstacle(std::unordered_map<std::string, Mesh*>&);
	void GenerateFloor(std::unordered_map<std::string, Mesh*>&);
	void GenerateProjectile(std::unordered_map<std::string, Mesh*>&, std::string);
	void GenerateParallelepiped(std::vector<VertexFormat>& vertices,
		std::vector<unsigned int>& indices, glm::vec3 sides, glm::vec3 color);

	glm::vec3 playerBody = glm::vec3(1, 2, 0.5f);
	glm::vec3 body_shoulderColor = glm::vec3(0.423, 0.803, 0.917);
	glm::vec3 playerHand = glm::vec3(0.5f);
	glm::vec3 playerShoulder = glm::vec3(0.5f, 1.5f, 0.5f);
	glm::vec3 playerHead = glm::vec3(0.5f);
	glm::vec3 head_handColor = glm::vec3(0.925, 0.909, 0.537);
	glm::vec3 playerFoot = glm::vec3(0.4f, 1.5f, 0.4f);
	glm::vec3 footColor = glm::vec3(0, 0, 1);

	float cubeSize = 10;
	float cubeSmallSize = 0.1f;
	float floorSize = 100;
	glm::vec3 obstacleColor = glm::vec3(0.168, 0.294, 0.149);
	glm::vec3 floorColor = glm::vec3(0.266, 0.207, 0.294);
};