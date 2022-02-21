#pragma once

#include "components/simple_scene.h"

constexpr float period = 2.0f;

enum ENEMY_MODE {
	PATROL,
	ATTACK,
	DIE,
	DEAD
};

struct info {
	info(glm::vec3 _position, float _angleY) : position (_position), angleY(_angleY) {}
	glm::vec3 position;
	float angleY;
};

struct mesh_info {
	mesh_info(std::vector<std::string> _meshes, std::vector<glm::vec3> _scales,
		std::vector<glm::vec3> _position_from_center, std::string _shader_name)
		: meshes(_meshes), scales(_scales), position_from_center(_position_from_center),
		shader_name(_shader_name) {}
	std::vector<std::string> meshes;
	std::vector<glm::vec3> scales;
	std::vector<glm::vec3> position_from_center;
	std::string shader_name;
};

struct direction_info {
	direction_info(glm::vec3 _direction, float _speed) :
		direction(_direction), speed(_speed) {}
	direction_info() : direction(glm::vec3(0, 0, 0)), speed(0) {}
	glm::vec3 direction;
	float speed;
};

struct borders_info {
	borders_info(glm::vec3 _center) : cube_center(_center) {}
	borders_info() : cube_center(glm::vec3(-1)) {}
	glm::vec3 cube_center;
	float attackDistance = 5.0f;
	float patrolDistance = 3.0f;

	bool withinBorders(glm::vec3 position) {
		return (position.x >= cube_center.x - attackDistance) &&
			(position.x <= cube_center.x + attackDistance) &&
			(position.z >= cube_center.z - attackDistance) &&
			(position.z <= cube_center.z + attackDistance);
	}
	bool withinPatrolBorders(glm::vec3 position) {
		return (position.x >= cube_center.x - patrolDistance) &&
			(position.x <= cube_center.x + patrolDistance) &&
			(position.z >= cube_center.z - patrolDistance) &&
			(position.z <= cube_center.z + patrolDistance);
	}
};

struct Collider {
	Collider() :
		center(glm::vec3(-1)), size(glm::vec3(-1)) {}
	void update(glm::vec3 _center, glm::vec3 _size) {
		center = _center;
		size = _size;
	}
	glm::vec3 center;
	glm::vec3 size;
};

struct GameObject {
	GameObject(info _pos, mesh_info _meshInfo) :
		pos(_pos), meshInfo(_meshInfo), mode(PATROL) {}
	void setDirection(direction_info dir) {
		direction = dir;
	}
	void setBorders(borders_info bor) {
		borders = bor;
	}
	void updateCounter(float deltaTimeSeconds) {
		count += deltaTimeSeconds;
		if (count > period) {
			mode = DEAD;
		}
	}

	info pos;
	mesh_info meshInfo;
	Collider collider;
	direction_info direction; // doar pt inamici si proiectile

	// doar pt miscare inamici
	borders_info borders;
	int mode;
	float count = 0;
};