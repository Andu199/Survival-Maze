#include "Generator.h"

using namespace std;
using namespace m1;

Generator::Generator()
{
}

Generator::~Generator()
{
}

void Generator::generate(std::unordered_map<std::string, Mesh*>& meshes, std::string path) {
	GeneratePlayer(meshes);
	GenerateEnemy(meshes, path);
	GenerateObstacle(meshes);
	GenerateFloor(meshes);
	GenerateProjectile(meshes, path);
	meshes["health"] = object2D::CreateSquare("health", glm::vec3(-1, 0, 0), 2, glm::vec3(1, 0, 0), true);
	meshes["time"] = object2D::CreateSquare("time", glm::vec3(-1, 0, 0), 2, glm::vec3(1, 1, 0), true);
	meshes["empty"] = object2D::CreateSquare("empty", glm::vec3(-1, 0, 0), 2, glm::vec3(1, 0, 1), true);
}

void Generator::GeneratePlayer(std::unordered_map<std::string, Mesh*>& meshes) {
	// body
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;
	GenerateParallelepiped(vertices, indices, playerBody, body_shoulderColor);
	meshes["playerBody"] = new Mesh("playerBody");
	meshes["playerBody"]->InitFromData(vertices, indices);

	// head
	vertices = {};
	indices = {};
	GenerateParallelepiped(vertices, indices, playerHead, head_handColor);
	meshes["playerHead"] = new Mesh("playerHead");
	meshes["playerHead"]->InitFromData(vertices, indices);

	// hand
	vertices = {};
	indices = {};
	GenerateParallelepiped(vertices, indices, playerHand, head_handColor);
	meshes["playerHand"] = new Mesh("playerHand");
	meshes["playerHand"]->InitFromData(vertices, indices);

	// shoulder
	vertices = {};
	indices = {};
	GenerateParallelepiped(vertices, indices, playerShoulder, body_shoulderColor);
	meshes["playerShoulder"] = new Mesh("playerShoulder");
	meshes["playerShoulder"]->InitFromData(vertices, indices);

	// foot
	vertices = {};
	indices = {};
	GenerateParallelepiped(vertices, indices, playerFoot, footColor);
	meshes["playerFoot"] = new Mesh("playerFoot");
	meshes["playerFoot"]->InitFromData(vertices, indices);
}

void Generator::GenerateEnemy(std::unordered_map<std::string, Mesh*>& meshes, std::string path) {
	Mesh* mesh = new Mesh("enemy");
	mesh->LoadMesh(PATH_JOIN(path, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
	meshes[mesh->GetMeshID()] = mesh;
}

void Generator::GenerateObstacle(std::unordered_map<std::string, Mesh*>& meshes) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;
	GenerateParallelepiped(vertices, indices, glm::vec3(cubeSmallSize, cubeSize, cubeSize), obstacleColor);

	meshes["obstacle"] = new Mesh("obstacle");
	meshes["obstacle"]->InitFromData(vertices, indices);
}

void Generator::GenerateFloor(std::unordered_map<std::string, Mesh*>& meshes) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;
	GenerateParallelepiped(vertices, indices, glm::vec3(floorSize, cubeSmallSize, floorSize), floorColor);

	meshes["floor"] = new Mesh("floor");
	meshes["floor"]->InitFromData(vertices, indices);
}

void Generator::GenerateProjectile(std::unordered_map<std::string, Mesh*>& meshes, std::string path) {
	Mesh* mesh = new Mesh("projectile");
	mesh->LoadMesh(PATH_JOIN(path, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;
}

void Generator::GenerateParallelepiped(std::vector<VertexFormat>& vertices,
	std::vector<unsigned int>& indices, glm::vec3 sides, glm::vec3 color) {

	vertices.insert(vertices.end(), {
		VertexFormat(glm::vec3(-sides.x / 2, -sides.y / 2, sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(sides.x / 2, -sides.y / 2, sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(-sides.x / 2, sides.y / 2, sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(sides.x / 2, sides.y / 2, sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(-sides.x / 2, -sides.y / 2, -sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(sides.x / 2, -sides.y / 2, -sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(-sides.x / 2, sides.y / 2, -sides.z / 2), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(sides.x / 2, sides.y / 2, -sides.z / 2), glm::vec3(0, 1, 0), color),
		});
	indices.insert(indices.end(), {
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4
		});
}