#include "spaceship.h"
#include <iostream>

Spaceship::Spaceship(const ShaderPtr& shader)
    : shader(shader) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); // Generate EBO

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Spaceship::~Spaceship() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO); // delete EBO
}

void Spaceship::draw(float xOffset, float yOffset) {
    float updatedVertices[72] = {
        // top part
        0.0f + xOffset, 0.15f + yOffset, 0.0f,
        -0.05f + xOffset, 0.05f + yOffset, 0.0f,
        0.05f + xOffset, 0.05f + yOffset, 0.0f,

        // Left of centre
        -0.05f + xOffset, 0.05f + yOffset, 0.0f,
        -0.1f + xOffset, -0.05f + yOffset, 0.0f,
        -0.05f + xOffset, -0.1f + yOffset, 0.0f,

        // Right side of centre
        0.05f + xOffset, 0.05f + yOffset, 0.0f,
        0.1f + xOffset, -0.05f + yOffset, 0.0f,
        0.05f + xOffset, -0.1f + yOffset, 0.0f,

        // bottom part
        -0.05f + xOffset, -0.1f + yOffset, 0.0f,
        0.05f + xOffset, -0.1f + yOffset, 0.0f,
        -0.05f + xOffset, -0.15f + yOffset, 0.0f,
        0.05f + xOffset, -0.15f + yOffset, 0.0f,
        -0.05f + xOffset, -0.2f + yOffset, 0.0f,
        0.05f + xOffset, -0.2f + yOffset, 0.0f,
        -0.025f + xOffset, -0.25f + yOffset, 0.0f,
        0.025f + xOffset, -0.25f + yOffset, 0.0f,

        // left wing
        -0.1f + xOffset, -0.05f + yOffset, 0.0f,
        -0.15f + xOffset, -0.15f + yOffset, 0.0f,
        -0.05f + xOffset, -0.1f + yOffset, 0.0f,

        // right wing
        0.1f + xOffset, -0.05f + yOffset, 0.0f,
        0.15f + xOffset, -0.15f + yOffset, 0.0f,
        0.05f + xOffset, -0.1f + yOffset, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(updatedVertices), updatedVertices);

    shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 33, GL_UNSIGNED_INT, 0); // 使用 glDrawElements 绘制
}
