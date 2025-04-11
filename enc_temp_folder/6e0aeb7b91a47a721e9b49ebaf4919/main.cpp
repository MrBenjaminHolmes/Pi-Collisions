#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <array>
#include <ctime> 
#include <iostream>
#include <cstdlib>

int collisionCount = 0;

class Object {
public:
    std::vector<float> position;
    std::vector<float> velocity;
    float mass;
    bool hasCollided;  // Add a collision flag
    Object(std::vector<float> position, std::vector<float> velocity, float mass = 0.0f)
        : position(position), velocity(velocity), mass(mass), hasCollided(false) {
    }

    void draw() {
        double halfside = 30;
        glColor3d(1, 1, 1);
        glBegin(GL_POLYGON);
        glVertex2d(position[0] + halfside, position[1] + halfside);
        glVertex2d(position[0] + halfside, position[1] - halfside);
        glVertex2d(position[0] - halfside, position[1] - halfside);
        glVertex2d(position[0] - halfside, position[1] + halfside);
        glEnd();
    }

    void updatePos() {
        this->position[0] += velocity[0];
        this->position[1] += velocity[1];
    }

    void resetCollisionFlag() {
        hasCollided = false;
    }
};

bool checkCollision(const Object& a, const Object& b, float halfside = 30.0f) {
    return std::abs(a.position[0] - b.position[0]) < 2 * halfside;
}

void resolveCollisions(Object& a, Object& b) {
    float v1x_new = ((a.mass - b.mass) * a.velocity[0] + 2 * b.mass * b.velocity[0]) / (a.mass + b.mass);
    float v2x_new = ((b.mass - a.mass) * b.velocity[0] + 2 * a.mass * a.velocity[0]) / (a.mass + b.mass);
    a.velocity[0] = v1x_new;
    b.velocity[0] = v2x_new;
    collisionCount++;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    int screenX = 1200;
    int screenY = 600;
    GLFWwindow* window = glfwCreateWindow(screenX, screenY, "2D PI SIM", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, screenX, screenY);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glOrtho(0, screenX, screenY, 0, -1, 1);
    std::vector<Object> objs = {
        Object(std::vector<float>{400.0f, 560.0f}, std::vector<float>{0.0f, 0.0f}, 1.0f),
        Object(std::vector<float>{700.0f, 560.0f}, std::vector<float>{-1.0f, 0.0f}, 1000.0f)
    };

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Reset collision flags at the start of each frame
        for (auto& obj : objs) {
            obj.resetCollisionFlag();
        }

        for (auto& obj : objs) {
            obj.updatePos();
            obj.draw();

            // Detect boundary collisions
            if (obj.position[0] - 30 <= 0) {
                obj.position[0] = 30;
                obj.velocity[0] = -obj.velocity[0];
                collisionCount++;
                std::cout << "Collision Count = " << collisionCount << std::endl;
            }

            // Check for object-to-object collisions
            for (auto& otherObj : objs) {
                if (&obj != &otherObj && checkCollision(obj, otherObj)) {
                    if (!obj.hasCollided && !otherObj.hasCollided) {
                        resolveCollisions(obj, otherObj);
                        obj.hasCollided = true;
                        otherObj.hasCollided = true;
                        std::cout << "Collision Count = " << collisionCount << std::endl;
                    }
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
