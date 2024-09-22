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

 // Размеры окна
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Камера
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Границы куба (AABB - Axis-Aligned Bounding Box)
glm::vec3 cubeMin(-0.5f, -0.5f, -0.5f);
glm::vec3 cubeMax(0.5f, 0.5f, 0.5f);

// Обработка ввода
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
    float yoffset = lastY - ypos; // Инвертировать y-координату
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    // Установка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Cube", NULL, NULL);
    if (!window) {
        std::cerr << "Ошибка создания окна" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Ошибка инициализации GLEW" << std::endl;
        return -1;
    }

    // Включение буфера глубины
    glEnable(GL_DEPTH_TEST);

    // Настройка шейдеров
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Создание куба
    Cube cube;

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Очистка экрана
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Установка матрицы проекции и вида
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Отрисовка куба
        cube.Draw(shader);

        // Обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождение ресурсов
    glfwTerminate();
    return 0;
}
