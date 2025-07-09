//
// Created by Matthew Halderman on 5/9/24.
//

#ifndef MATTSQUARED_GRAPHICS_VEHICLE_H
#define MATTSQUARED_GRAPHICS_VEHICLE_H


#include <memory>
#include "Object3D.h"
#include "UserInput.h"
#include "DynamicLight.h"

class Vehicle {
private:
    glm::vec3 inputDirection;
    UserInput& userInput;
    Object3D& body;

public:
    glm::vec3 frontLookat;
    glm::vec3 rearCamera;
    glm::vec3 velocity;
    glm::vec3 angVelocity;
    glm::vec3 rotation;
    glm::vec3 direction;
    glm::vec3 headlightPos;
    float accelPower = 0.0f;
    float accelTilt = 0.0f;
    float accelVelocity = 0.0f;
    float turnPower = 0.0f;
    float turnTilt = 0.0f;
    float turnVelocity = 0.0f;
    float followSpeed = 0.0f;
    float tireSpin = 0.0f;
    DynamicLight& headlights;
    DynamicLight& l_brakeLight;
    DynamicLight& r_brakeLight;

    float speed = 0.0f;
    float turnSpeed = 0.0f;
    Vehicle(Object3D &body, UserInput &userInput, DynamicLight &headlights, DynamicLight &lBrakeLight,
            DynamicLight &rBrakeLight);
    void Update(float deltaTime);

    void accelerate(float dt);

    void deccelerate(float dt);

    void brake(float dt);

    void turn(float dt);

    void reverse(float dt);

    void straighten(float dt);

    void SetLights(DynamicLight &headlights, DynamicLight &l_brakeLight, DynamicLight &r_brakeLight);

    void SetLights();

    void UpdateLights();
};


#endif //MATTSQUARED_GRAPHICS_VEHICLE_H
