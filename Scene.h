#ifndef MATTSQUARED_GRAPHICS_SCENE_H
#define MATTSQUARED_GRAPHICS_SCENE_H


#include <vector>
#include "ShaderProgram.h"
#include "Object3D.h"
#include "Animator.h"
#include "DynamicLight.h"

class Scene {
public:
    ShaderProgram defaultShader;
    std::vector<Object3D> objects;
    std::vector<Animator> animators;
    std::vector<DynamicLight> lights;

    Scene(ShaderProgram &&defaultShader, std::vector<Object3D> &&objects, std::vector<Animator> &&animators, std::vector<DynamicLight> &&lights)
        : defaultShader(defaultShader), objects(std::move(objects)), animators(std::move(animators)), lights(std::move(lights)) {}
    Scene(ShaderProgram &&shader, const Object3D& object)
        : defaultShader(shader), objects(std::vector<Object3D>{object}) {}

    static Scene jeep();
    static Scene lifeOfPi();
    static Scene bunny();
    static Scene marbleSquare();
};


#endif //MATTSQUARED_GRAPHICS_SCENE_H
