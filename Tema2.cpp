#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    Generator* generator = new Generator();
    generator->generate(meshes, window->props.selfDir);
    delete generator;

    MazeGenerator* mazeGenerator = new MazeGenerator();
    mazeGenerator->generateMaze(obstacles, entities);
    delete mazeGenerator;

    renderCameraTarget = true;
    camera = new m1::Camera();
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Shader* shader = new Shader("VertexDisplacement");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    window->DisablePointer();
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema2::UpdatePositions(float deltaTimeSeconds) {
    auto projectile = projectiles.begin();
    while (projectile != projectiles.end()) {
        projectile->pos.position += projectile->direction.direction *
            projectile->direction.speed * deltaTimeSeconds;
        projectile++;
    }

    auto enemy = entities.begin();
    enemy++;
    while (enemy != entities.end()) {
        if (enemy->mode == DIE) {
            enemy++;
            continue;
        }
        // daca player-ul se afla in patratul lor.
        if (enemy->borders.withinBorders(entities[0].pos.position)) {
            enemy->mode = ATTACK;
            glm::vec3 direction = entities[0].pos.position - enemy->pos.position;
            direction = glm::normalize(glm::vec3(direction.x, 0, direction.z));
            enemy->direction.direction = direction;
            enemy->pos.angleY =
                glm::acos(glm::dot(glm::vec3(-1, 0, 0), enemy->direction.direction));
            enemy->pos.position += enemy->direction.direction *
                enemy->direction.speed * deltaTimeSeconds;
            
        }
        else {
            if (enemy->mode == ATTACK) {
                enemy->mode = PATROL;
                // reparam pozitia.
                enemy->pos.position =
                    enemy->borders.cube_center + glm::vec3(0, 0, -enemy->borders.patrolDistance);
                enemy->pos.angleY = 0;
                enemy->direction.direction = glm::vec3(-1, 0, 0);
            }
            enemy->pos.position += enemy->direction.direction *
                enemy->direction.speed * deltaTimeSeconds;
            // daca au iesit din patratul lor de patrulare
            if (!enemy->borders.withinPatrolBorders(enemy->pos.position)) {
                if (enemy->direction.direction == glm::vec3(-1, 0, 0)) {
                    enemy->pos.position.x =
                        enemy->borders.cube_center.x - enemy->borders.patrolDistance;
                }
                else if (enemy->direction.direction == glm::vec3(0, 0, -1)) {
                    enemy->pos.position.z =
                        enemy->borders.cube_center.z - enemy->borders.patrolDistance;
                }
                else if (enemy->direction.direction == glm::vec3(1, 0, 0)) {
                    enemy->pos.position.x =
                        enemy->borders.cube_center.x + enemy->borders.patrolDistance;
                }
                else {
                    enemy->pos.position.z =
                        enemy->borders.cube_center.z + enemy->borders.patrolDistance;
                }
                enemy->direction.direction =
                    glm::cross(enemy->direction.direction, glm::vec3(0, -1, 0));
                enemy->pos.angleY = 
                    glm::acos(glm::dot(glm::vec3(-1, 0, 0), enemy->direction.direction));
            }
        }
        enemy++;
    }
}

bool Tema2::CheckCollision(const Collider& c1, const Collider& c2, GameObject* player,
    float deltaTimeSeconds) {

    glm::vec2 deltaX = glm::vec2(c2.center.x + c2.size.x / 2 - (c1.center.x - c1.size.x / 2),
        c1.center.x + c1.size.x / 2 - (c2.center.x - c2.size.x / 2));
    glm::vec2 deltaY = glm::vec2(c2.center.y + c2.size.y / 2 - (c1.center.y - c1.size.y / 2),
        c1.center.y + c1.size.y / 2 - (c2.center.y - c2.size.y / 2));
    glm::vec2 deltaZ = glm::vec2(c2.center.z + c2.size.z / 2 - (c1.center.z - c1.size.z / 2),
        c1.center.z + c1.size.z / 2 - (c2.center.z - c2.size.z / 2));

    if (deltaX.x > 0 && deltaX.y > 0 &&
        deltaY.x > 0 && deltaY.y > 0 &&
        deltaZ.x > 0 && deltaZ.y > 0) {

        if (player) {
            float sgnX;
            float sgnZ;
            float minDeltaX, minDeltaZ;
            if (deltaX.x > deltaX.y) {
                minDeltaX = deltaX.y;
                sgnX = -1;
            }
            else {
                minDeltaX = deltaX.x;
                sgnX = 1;
            }
            if (deltaZ.x > deltaZ.y) {
                minDeltaZ = deltaZ.y;
                sgnZ = -1;
            }
            else {
                minDeltaZ = deltaZ.x;
                sgnZ = 1;
            }

            if (minDeltaX < minDeltaZ)
                player->pos.position.x -= sgnX * deltaTimeSeconds * playerSpeed;
            else
                player->pos.position.z -= sgnZ * deltaTimeSeconds * playerSpeed;
        }
        return true;
    }
    return false;
}

void Tema2::Collide(float deltaTimeSeconds) {
    float playerAngleY = entities[0].pos.angleY;
    entities[0].collider.update(
        entities[0].pos.position,
        glm::vec3(playerSize.x * abs(cos(playerAngleY)) + playerSize.z * abs(sin(playerAngleY)),
            playerSize.y,
            playerSize.x * abs(sin(playerAngleY)) + playerSize.z * abs(cos(playerAngleY)))
    );
    // perete/player, perete/proiectil.
    auto wall = obstacles.begin();
    if (CheckCollision(entities[0].collider, wall->collider, NULL, deltaTimeSeconds)) {
        cout << "\n\n\nYou WON! Congrats\n\n\n";
        exit(0);
    }
    wall += 2; // fara floor
    while (wall != obstacles.end()) {
        CheckCollision(wall->collider, entities[0].collider, &entities[0], deltaTimeSeconds);
        auto projectile = projectiles.begin();
        while (projectile != projectiles.end()) {
            if (CheckCollision(projectile->collider, wall->collider, NULL, deltaTimeSeconds)) {
                projectile = projectiles.erase(projectile);
            }
            else {
                projectile++;
            }
        }
        wall++;
    }

    // inamic/player, inamic/proiectil
    auto enemy = entities.begin();
    enemy++;
    while (enemy != entities.end()) {
        if (enemy->mode == DIE) {
            enemy->updateCounter(deltaTimeSeconds);
            if (enemy->mode == DEAD) {
                enemy = entities.erase(enemy);
            }
            else {
                enemy++;
            }
            continue;
        }
        enemy->collider.update(enemy->pos.position, enemySize);
        if (CheckCollision(entities[0].collider, enemy->collider, NULL, deltaTimeSeconds)) {
            enemy = entities.erase(enemy);
            playerHealth -= 25;
            if (playerHealth <= 0) {
                cout << "\n\n\nYou DIED! Game Over\n\n\n";
                exit(0);
            }
        }
        else {
            auto projectile = projectiles.begin();
            while (projectile != projectiles.end()) {
                projectile->collider.update(projectile->pos.position, projectileSize);
                if (CheckCollision(enemy->collider, projectile->collider, NULL, deltaTimeSeconds)) {
                    enemy->mode = DIE;
                    enemy->meshInfo.shader_name = "VertexDisplacement";
                    timer -= 5.0f; // castigi timp
                    projectile = projectiles.erase(projectile);
                    break;
                }
                else {
                    projectile++;
                }
            }
            enemy++;
        }
    }
}

void Tema2::UpdateCameras() {
    distanceFromPlayer = glm::vec3(
        distance * sin(entities[0].pos.angleY) * sin(cameraXoZangle + AI_MATH_HALF_PI),
        distance * cos(cameraXoZangle + AI_MATH_HALF_PI),
        distance * cos(entities[0].pos.angleY) * sin(cameraXoZangle + AI_MATH_HALF_PI));
    camera->Set(entities[0].pos.position + distanceFromPlayer,
        entities[0].pos.position, glm::vec3(0, 1, 0));
}

void Tema2::DrawScene() {
    auto resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    for (auto object : obstacles) {
        glm::mat4 modelMatrix = glm::mat4(1);
        for (int i = 0; i < object.meshInfo.meshes.size(); ++i) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, object.pos.position);
            modelMatrix = glm::rotate(modelMatrix, object.pos.angleY, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, object.meshInfo.scales[i]);
            modelMatrix = glm::translate(modelMatrix, object.meshInfo.position_from_center[i]);
            RenderMesh(meshes[object.meshInfo.meshes[i]], shaders[object.meshInfo.shader_name.c_str()], modelMatrix);
        }
    }

    int first_index = 0;
    if (!renderCameraTarget)
        first_index++;
    for (int j = first_index; j < entities.size(); ++j) {
        glm::mat4 modelMatrix = glm::mat4(1);
        for (int i = 0; i < entities[j].meshInfo.meshes.size(); ++i) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, entities[j].pos.position);
            modelMatrix = glm::rotate(modelMatrix, entities[j].pos.angleY, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, entities[j].meshInfo.scales[i]);
            modelMatrix = glm::translate(modelMatrix, entities[j].meshInfo.position_from_center[i]);
            RenderMesh(meshes[entities[j].meshInfo.meshes[i]], shaders[entities[j].meshInfo.shader_name.c_str()], modelMatrix);
        }
    }
    
    for (auto projectile : projectiles) {
        glm::mat4 modelMatrix = glm::mat4(1);
        for (int i = 0; i < projectile.meshInfo.meshes.size(); ++i) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, projectile.pos.position);
            modelMatrix = glm::rotate(modelMatrix, projectile.pos.angleY, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, projectile.meshInfo.scales[i]);
            modelMatrix = glm::translate(modelMatrix, projectile.meshInfo.position_from_center[i]);
            RenderMesh(meshes[projectile.meshInfo.meshes[i]], shaders[projectile.meshInfo.shader_name.c_str()], modelMatrix);
        }
    }
}

void Tema2::DrawBars() {
    auto resolution = window->GetResolution();

    glViewport(0, resolution.y * 0.95, resolution.x * 0.4f, resolution.y * 0.05f);
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(-1, 0);
    float scale = playerHealth / playerMaxHealth;
    modelMatrix *= transform2D::Scale(scale, 1);
    modelMatrix *= transform2D::Translate(1, 0);
    RenderMesh2DObject(meshes["health"], modelMatrix, glm::vec3(1, 0, 0));
    RenderMesh2DObject(meshes["empty"], glm::mat3(1), glm::vec3(1, 0, 1));

    glViewport(resolution.x * 0.6f, resolution.y * 0.95, resolution.x * 0.4f, resolution.y * 0.05f);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(-1, 0);
    scale = (maxTime - timer) / maxTime;
    modelMatrix *= transform2D::Scale(scale, 1);
    modelMatrix *= transform2D::Translate(1, 0);
    RenderMesh2DObject(meshes["time"], modelMatrix, glm::vec3(1, 1, 0));
    RenderMesh2DObject(meshes["empty"], glm::mat3(1), glm::vec3(1, 0, 1));
}


void Tema2::Update(float deltaTimeSeconds)
{
    timer += deltaTimeSeconds;
    if (timer > maxTime) {
        cout << "\n\n\nTime's up! Game Over\n\n\n";
        exit(0);
    }
    UpdatePositions(deltaTimeSeconds);
    Collide(deltaTimeSeconds);
    UpdateCameras();
    DrawBars();
    DrawScene();
}


void Tema2::FrameEnd()
{

}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    if (strcmp(shader->GetName(), "VertexDisplacement") == 0) {
        int loc_time = glGetUniformLocation(shader->program, "time");
        glUniform1f(loc_time, Engine::GetElapsedTime());
    }
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderMesh2DObject(Mesh* mesh, const glm::mat3& modelMatrix, const glm::vec3& color)
{
    Shader* shader = shaders.at("Color");

    if (!mesh || !shader || !shader->program)
        return;

    glm::mat3 mm = modelMatrix;
    glm::mat4 model = glm::mat4(
        mm[0][0], mm[0][1], mm[0][2], 0.f,
        mm[1][0], mm[1][1], mm[1][2], 0.f,
        0.f, 0.f, mm[2][2], 0.f,
        mm[2][0], mm[2][1], 0.f, 1.f);

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(shader->GetUniformLocation("color"), color.r, color.g, color.b);

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        entities[0].pos.position.z -= playerSpeed * deltaTime
            * cos(entities[0].pos.angleY);
        entities[0].pos.position.x -= playerSpeed * deltaTime
            * sin(entities[0].pos.angleY);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        entities[0].pos.position.z += playerSpeed * deltaTime
            * sin(entities[0].pos.angleY);
        entities[0].pos.position.x -= playerSpeed * deltaTime
            * cos(entities[0].pos.angleY);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        entities[0].pos.position.z += playerSpeed * deltaTime
            * cos(entities[0].pos.angleY);
        entities[0].pos.position.x += playerSpeed * deltaTime
            * sin(entities[0].pos.angleY);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        entities[0].pos.position.z -= playerSpeed * deltaTime
            * sin(entities[0].pos.angleY);
        entities[0].pos.position.x += playerSpeed * deltaTime
            * cos(entities[0].pos.angleY);
    }
    if (window->KeyHold(GLFW_KEY_LEFT_CONTROL)) {
        renderCameraTarget = false;
        distance = 0.01f;
    }
    else {
        renderCameraTarget = true;
        distance = 4.0f;
        if (cameraXoZangle > AI_MATH_HALF_PI / 6)
            cameraXoZangle = AI_MATH_HALF_PI / 6;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    entities[0].pos.angleY -= deltaX * cameraSensivity;
    cameraXoZangle -= deltaY * cameraSensivity;

    if (!renderCameraTarget) {
        if (cameraXoZangle < -AI_MATH_HALF_PI / 2)
            cameraXoZangle = -AI_MATH_HALF_PI / 2;
        if (cameraXoZangle > AI_MATH_HALF_PI / 2)
            cameraXoZangle = AI_MATH_HALF_PI / 2;
    }
    else {
        if (cameraXoZangle < -AI_MATH_HALF_PI / 2)
            cameraXoZangle = -AI_MATH_HALF_PI / 2;
        if (cameraXoZangle > AI_MATH_HALF_PI / 6)
            cameraXoZangle = AI_MATH_HALF_PI / 6;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (!renderCameraTarget) {
        if (button == GLFW_MOUSE_BUTTON_2) {
            GameObject projectile = GameObject(
                entities[0].pos,
                mesh_info({ "projectile" }, { glm::vec3(0.1f) }, { glm::vec3(0) },
                    "VertexNormal")
            );
            projectile.setDirection(direction_info(camera->forward, projectileSpeed));
            projectiles.push_back(projectile);
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
