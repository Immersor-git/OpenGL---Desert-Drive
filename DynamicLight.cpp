//
// Created by Matthew Halderman on 5/6/24.
//

#include "DynamicLight.h"

#include <iostream>
#include <glm/glm.hpp>

void printMat4(const glm::mat4& mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

void DynamicLight::printLightSpaceMatrix() {
    printMat4(lightSpaceMatrix);
}

glm::mat4 DynamicLight::getLightSpaceMatrix() {
    return lightSpaceMatrix;
}

void DynamicLight::updateUniforms(ShaderProgram& shader) {
    //printMat4(lightSpaceMatrix);
    shader.setUniform("lights["+std::to_string(uniformIndex)+"]", lightSpaceMatrix);
}

void DynamicLight::setColor(glm::vec3 color) {
    lightSpaceMatrix[1] = glm::vec4(color, 0.0);
}

void DynamicLight::setRange(float range) {
    lightSpaceMatrix[2][1] = range;
}

void DynamicLight::setCutoffAngle(float degrees) {
    lightSpaceMatrix[2][2] = degrees;
}

void DynamicLight::setType(int type) {
    lightSpaceMatrix[2][0] = type;
}

void DynamicLight::setDirection(glm::vec3 lookVector) {
    lightSpaceMatrix[3] = glm::vec4(glm::normalize(lookVector), 0.0);
}

void DynamicLight::setPosition(glm::vec3 position) {
    lightSpaceMatrix[0] = glm::vec4(position, 0);
}

void DynamicLight::setUniformIndex(int index) {
    uniformIndex = index;
}

int DynamicLight::getUniformIndex() {
    return uniformIndex;
}

void DynamicLight::setLightSpaceMatrix(glm::mat4 matrix) {
    lightSpaceMatrix = matrix;
}