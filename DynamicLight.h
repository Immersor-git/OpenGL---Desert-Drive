//
// Created by Matthew Halderman on 5/6/24.
//

#ifndef MATTSQUARED_GRAPHICS_DYNAMICLIGHT_H
#define MATTSQUARED_GRAPHICS_DYNAMICLIGHT_H


#include <glm/glm.hpp>
#include "ShaderProgram.h"

class DynamicLight {
private:
    glm::mat4 lightSpaceMatrix;
    int uniformIndex = 0;
public:
    explicit DynamicLight(glm::mat4 lightSpaceMatrix) : lightSpaceMatrix(lightSpaceMatrix) {};

    void updateUniforms(ShaderProgram &shader);

    void setColor(glm::vec3 color);

    void setRange(float range);

    void setType(int type);

    void setDirection(glm::vec3 lookVector);

    void setPosition(glm::vec3 position);

    void setUniformIndex(int index);

    int getUniformIndex();

    void setLightSpaceMatrix(glm::mat4 matrix);

    void printLightSpaceMatrix();

    void setCutoffAngle(float degrees);

    glm::mat4 getLightSpaceMatrix();
};


#endif //MATTSQUARED_GRAPHICS_DYNAMICLIGHT_H
