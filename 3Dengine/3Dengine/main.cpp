/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Progress Team Corporation. All rights reserved.
 *  All files were created by the Progress Team, Progress Games, with the support of the Progress License.
 *  Copying files or code is prohibited by Copyright.
 *--------------------------------------------------------------------------------------------*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "cube.h"

 // ������� ����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ������� ���� (AABB - Axis-Aligned Bounding Box)
glm::vec3 cubeMin(-0.5f, -0.5f, -0.5f);
glm::vec3 cubeMax(0.5f, 0.5f, 0.5f);

// ��������� �����
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, cubeMin, cubeMax);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, cubeMin, cubeMax);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, cubeMin, cubeMax);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, cubeMin, cubeMax);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ������������� y-����������
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

int main() {
    // ������������� GLFW
    if (!glfwInit()) {
        std::cerr << "������ ������������� GLFW" << std::endl;
        return -1;
    }

    // ��������� ������ OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������� ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Cube", NULL, NULL);
    if (!window) {
        std::cerr << "������ �������� ����" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ������������� GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "������ ������������� GLEW" << std::endl;
        return -1;
    }

    // ��������� ������ �������
    glEnable(GL_DEPTH_TEST);

    // ��������� ��������
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // �������� ����
    Cube cube;

    // �������� ����
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // ������� ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ��������� ������� �������� � ����
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // ��������� ����
        cube.Draw(shader);

        // ��������� �������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ������������ ��������
    glfwTerminate();
    return 0;
}
