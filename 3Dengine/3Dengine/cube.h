/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Progress Team Corporation. All rights reserved.
 *  All files were created by the Progress Team, Progress Games, with the support of the Progress License.
 *  Copying files or code is prohibited by Copyright.
 *--------------------------------------------------------------------------------------------*/
#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include "shader.h"

class Cube {
public:
    GLuint VAO, VBO;

    Cube() {
        float vertices[] = {
            // Передняя грань
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            // Задняя грань
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            // Левая грань
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            // Правая грань
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

             // Верхняя грань
             -0.5f,  0.5f, -0.5f,
             -0.5f,  0.5f,  0.5f,
              0.5f,  0.5f,  0.5f,
              0.5f,  0.5f,  0.5f,
              0.5f,  0.5f, -0.5f,
             -0.5f,  0.5f, -0.5f,

             // Нижняя грань
             -0.5f, -0.5f, -0.5f,
              0.5f, -0.5f, -0.5f,
              0.5f, -0.5f,  0.5f,
              0.5f, -0.5f,  0.5f,
             -0.5f, -0.5f,  0.5f,
             -0.5f, -0.5f, -0.5f
        };

        // Создание VAO и VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw(Shader& shader) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    ~Cube() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif