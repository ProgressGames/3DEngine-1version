/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Progress Team Corporation. All rights reserved.
 *  All files were created by the Progress Team, Progress Games, with the support of the Progress License.
 *  Copying files or code is prohibited by Copyright.
 *--------------------------------------------------------------------------------------------*/
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f) {
        Position = position;
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw = -90.0f;
        Pitch = 0.0f;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(CameraMovement direction, const glm::vec3& cubeMin, const glm::vec3& cubeMax) {
        float velocity = MovementSpeed * 0.01f;
        glm::vec3 newPosition = Position;

        if (direction == FORWARD)
            newPosition += Front * velocity;
        if (direction == BACKWARD)
            newPosition -= Front * velocity;
        if (direction == LEFT)
            newPosition -= Right * velocity;
        if (direction == RIGHT)
            newPosition += Right * velocity;

        // Проверка коллизии
        if (!checkCollision(newPosition, cubeMin, cubeMax)) {
            Position = newPosition;
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

    bool checkCollision(const glm::vec3& newPosition, const glm::vec3& cubeMin, const glm::vec3& cubeMax) {
        return (newPosition.x >= cubeMin.x && newPosition.x <= cubeMax.x) &&
            (newPosition.y >= cubeMin.y && newPosition.y <= cubeMax.y) &&
            (newPosition.z >= cubeMin.z && newPosition.z <= cubeMax.z);
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
