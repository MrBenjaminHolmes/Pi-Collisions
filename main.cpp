#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <array>
#include <ctime> 
#include <iostream>
#include <cstdlib>



class Object {
public:
    std::vector<float> position;
    std::vector<float> velocity;
    float mass;
    Object(std::vector<float> position, std::vector<float> velocity, float mass = 0.0f) {
        this->position = position;
        this->velocity = velocity;
        this->mass = mass;
    }
    
    void draw()
    {
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
};

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
        Object(std::vector<float>{400.0f,560.0f},std::vector<float>{-4.0f, 0.0f},15.0f)
        

    };

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);
        for (auto& obj : objs) {
            obj.updatePos();
            obj.draw();

            if (obj.position[0] - 30 <= 0) {
                obj.position[0] = 30;  
                obj.velocity[0] = -obj.velocity[0];
            }
        }
  

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

