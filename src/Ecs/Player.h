#pragma once
#include <glm\glm.hpp>
//#include "../Shader/Shader.h"
#include "BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Headers/Config.h"

class PlayerModel : public BaseModel {  // player cube will change later for obj model 
public:
    GLuint VAO, VBO;

    // ################# New ######################
    //float yaw = -90.0f; // looking down -Z
    //float pitch = 0.0f;
    //float roll = 0.0f;

    //

    //void UpdateModelMatrix() {
    //    glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
    //    glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0)); // yaw
    //    glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1, 0, 0)); // pitch
    //    glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(roll), glm::vec3(0, 0, 1)); // roll
    //    glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
    //    modelMatrix = T * (Ry * Rx * Rz) * S;
    //}

    // Local axes from yaw/pitch (for movement)
    //inline glm::vec3 Forward() const {
    //    glm::vec3 f;
    //    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); // ### ERROR HERE line 29
    //    f.y = sin(glm::radians(pitch));
    //    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //    return glm::normalize(f);
    //}
    //inline glm::vec3 RightAxis() const {
    //    return glm::normalize(glm::cross(Forward(), glm::vec3(0, 1, 0)));
    //}
    //inline glm::vec3 UpAxis() const {
    //    return glm::normalize(glm::cross(RightAxis(), Forward()));
    //}
    // ################## end new ########################

    PlayerModel(int idx, const std::string& name, int PlayerIdx) {
        index = idx;             // list index
        objectName = name;       // cube name
        objectIndex = PlayerIdx;         // start index for a cube
        objectTypeID = MAIN_PLAYER; // Example type ID for Cube set in config = 1

        position = glm::vec3(0.0f, 0.0f, 0.0f); // Initial position
        scale = glm::vec3(1.0f, 1.0f, 1.0f);    // Initial scale
        modelMatrix = glm::mat4(1.0f);


        GLfloat vertices[] = { //  Normal         Tex cords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f

        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture location 1
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)3);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //glEnableVertexAttribArray(0);
    }

    ~PlayerModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void DrawPlayer() {

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
private:
};
