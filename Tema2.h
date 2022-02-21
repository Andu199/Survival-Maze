#pragma once

#include "Generator.h"
#include "GameObject.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/MazeGenerator.h"
#include "lab_m1/lab3/transform2D.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;

        void UpdatePositions(float deltaTimeSeconds);
        bool CheckCollision(const Collider& c1, const Collider& c2, GameObject*, float deltaTimeSeconds);
        void Collide(float deltaTimeSeconds);
        void UpdateCameras();

        void UpdateViewport(glm::vec2 corner, glm::vec2 size);
        void DrawScene();
        void DrawBars();

        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderMesh2DObject(Mesh* mesh, const glm::mat3& modelMatrix, const glm::vec3& color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        m1::Camera* camera;
        float cameraSensivity = 0.002f;
        float distance = 4.0f;
        float cameraXoZangle = 0;
        glm::vec3 distanceFromPlayer = glm::vec3(0, 1.25f, distance);
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        float maxTime = 120.0f;
        float timer = 0;

        std::vector<GameObject> entities;
        std::vector<GameObject> obstacles;
        std::vector<GameObject> projectiles;
        float playerSpeed = 4.0f;
        float playerMaxHealth = 100;
        float playerHealth = playerMaxHealth;
        float projectileSpeed = 40.0f;

        glm::vec3 playerSize = glm::vec3(1.02f, 4.04f, 0.25f);
        glm::vec3 enemySize = glm::vec3(1);
        glm::vec3 projectileSize = glm::vec3(0.15f);

    };
}   // namespace m1
