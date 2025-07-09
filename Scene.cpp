#include "Scene.h"
#include "AssimpImport.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "BezierAnimation.h"


Scene Scene::jeep() {
    auto jeep = assimpLoad("../models/mil_jeep_fbx/chassis.fbx", true);

    jeep.addChild(assimpLoad("../models/mil_jeep_fbx/b_l_tire_centered.fbx", true));
    jeep.addChild(assimpLoad("../models/mil_jeep_fbx/b_r_tire_centered.fbx", true));
    jeep.addChild(assimpLoad("../models/mil_jeep_fbx/f_l_tire_centered.fbx", true));
    jeep.addChild(assimpLoad("../models/mil_jeep_fbx/f_r_tire_centered.fbx", true));

    jeep.getChild(2).move(glm::vec3(-295.31, 166.564, -142.697));
    jeep.getChild(3).move(glm::vec3(-295.31, 166.564, 142.697));
    jeep.getChild(4).move(glm::vec3(329.379, 170.609, -142.697));
    jeep.getChild(5).move(glm::vec3(329.379, 170.609, 142.697));

    jeep.move(glm::vec3(0, -1.25, 0));
    jeep.grow(glm::vec3(0.004, 0.004, 0.004));

    auto cactus = assimpLoad("../models/desert/Cactus.fbx", true);
    auto cactusMesh = cactus.getChild(0).getMesh(0);
    cactusMesh.addTexture(Texture::loadTexture("../models/desert/textures/tex_cactus01.png", "baseTexture"));
    cactus.move(glm::vec3(0, -0.5, 5));
    cactus.grow(glm::vec3(0.01, 0.01, 0.01));

    auto bird = assimpLoad("../models/bird/duck.fbx", true);
    bird.grow(glm::vec3(0.01, 0.01, 0.01));

    std::vector<Texture> groundtex = {Texture::loadTexture("../models/concrete.png","baseTexture"),Texture::loadTexture("../models/test_specular.jpg","specMap")};
    //std::vector<Mesh3D> ground = {Mesh3D::square(groundtex)};
    Object3D map = Object3D({Mesh3D::square(groundtex)});//assimpLoad("../models/racetrack/arena.obj", true);
    map.grow(glm::vec3(100,100,100));
    map.move(glm::vec3(0,-1,0));
    map.rotate(glm::vec3(glm::radians(-90.0f),0,0));

    std::vector<Texture> walltex = {Texture::loadTexture("../models/wall.jpg","baseTexture")};

    Object3D map2 = Object3D({Mesh3D::square(walltex)});//assimpLoad("../models/racetrack/arena.obj", true);
    map2.grow(glm::vec3(100,15,10));
    map2.move(glm::vec3(0,-1.2,50));
    map.rotate(glm::vec3(0,glm::radians(180.0),0));
    //map.rotate(glm::vec3(glm::radians(-90.0f),0,0));
    auto lightSource = assimpLoad("../models/tiger/scene.gltf", true);
    lightSource.setPosition(glm::vec3(-2,1,0));
    lightSource.grow(glm::vec3(0.01,0.01,0.01));


    glm::mat4 dirLight = glm::mat4(
            0,0 ,0,0, //Position (Direction for directional lights);
            1,1,0.9,0, //Color
            1,1,10,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0.5,-0.4,0,0 //LookAt (For spotlights and directionalLights)
    );

    /*glm::mat4 pointLight = glm::mat4(
            -5,0,0,0, //Position
            1,0,0,0, //Color
            2,25,0,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0,0,0,0 //LookAt (For spotlights)
    );

    /glm::mat4 spotLight = glm::mat4(
            1,0,5,0, //Position
            1,1,0.5,0, //Color
            3,70,45,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0,0,1,0 //LookAt (For spotlights)
    );

    glm::mat4 pointLight2 = glm::mat4(
            5,0,0,0, //Position
            0,1,0,0, //Color
            2,25,0,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0,0,0,0 //LookAt (For spotlights)
    )
     */

    std::vector<Object3D> objects;
    objects.push_back(std::move(jeep));
    objects.push_back(std::move(lightSource));
    objects.push_back(std::move(cactus));
    objects.push_back(std::move(bird));
    //objects.push_back(std::move(map));
    //objects.push_back(std::move(map2));
    std::vector<Texture> groundtex2 = {
            Texture::loadTexture("../models/desert_pbr_textures/desert-rocks1-albedo.png","baseTexture"),
            Texture::loadTexture("../models/desert_pbr_textures/desert-rocks1-Roughness.png","specMap"),
            Texture::loadTexture("../models/desert_pbr_textures/desert-rocks1-Height.png","heightMap"),
            Texture::loadTexture("../models/desert_pbr_textures/desert-rocks1-Normal-ogl.png","normalMap")};
    for (int x = -5; x < 5; x++) {
        for (int z = -5; z < 5; z++) {
            //,
            //int x = 0;
            //int z = 0;
                   //Texture::loadTexture("/Users/matthewhalderman/Downloads/mattsquared_graphics/models/desert_pbr_textures/desert-rocks1-Roughness.png","specMap")};
            Object3D groundPanel = Object3D({Mesh3D::square(groundtex2)});//assimpLoad("../models/racetrack/arena.obj", true);
            groundPanel.grow(glm::vec3(10,10,10));
            groundPanel.move(glm::vec3(x*10,-1,z*10));
            groundPanel.rotate(glm::vec3(glm::radians(90.0f),0,0));
            objects.push_back(std::move(groundPanel));

            auto cactus = assimpLoad("../models/desert/Cactus.fbx", true);
            float randHeight = pow((rand()%10)/10.0,2) * 3.0;
            cactus.move(glm::vec3(x*10 - 5 + rand()%10, -0.8 - randHeight*0.2, z*10 - 5 +rand()%10));
            cactus.grow(glm::vec3(0.0075 + 0.005*randHeight, 0.01 + 0.01 * randHeight, 0.0075 + 0.005*randHeight));
            objects.push_back(std::move(cactus));
        }
    }
    std::vector<DynamicLight> lights;
    lights.push_back(std::move(DynamicLight(dirLight)));
    //lights.push_back(std::move(DynamicLight(pointLight)));
    //lights.push_back(std::move(DynamicLight(spotLight)));
    //lights.push_back(std::move(DynamicLight(pointLight2)));
    //Animator animJeep;
    //animJeep.addAnimation(std::make_unique<RotationAnimation>(objects[0], 10, glm::vec3(0, 5.5, 0)));
    std::vector<Animator> animators;
    //animators.push_back(std::move(animJeep));
    Animator bezierBird;
    std::vector<glm::vec3> controlPoints = {
            glm::vec3(10, 0, 0),
            glm::vec3(10, 2.7, 1.6),
            glm::vec3(10, 0.2, 3.0),
            glm::vec3(10, 0.7, 4.5),
            glm::vec3(10, 0.1, 6.0),
            glm::vec3(10, 3.2, 5.8),
            glm::vec3(10, 3.2, 7.0),
            glm::vec3(10, 0.8, 8.0), //
            glm::vec3(10, 3.2, 7.0),
            glm::vec3(10, 3.2, 5.8),
            glm::vec3(10, 0.1, 6.0),
            glm::vec3(10, 0.7, 4.5),
            glm::vec3(10, 0.2, 3.0),
            glm::vec3(10, 2.7, 1.6),
            glm::vec3(10, 0, 0)
    };
    bezierBird.addAnimation(std::make_unique<BezierAnimation>(objects[3], 7, controlPoints));
    bezierBird.addAnimation(std::make_unique<BezierAnimation>(objects[3], 7, controlPoints));
    bezierBird.addAnimation(std::make_unique<BezierAnimation>(objects[3], 7, controlPoints));
    bezierBird.addAnimation(std::make_unique<BezierAnimation>(objects[3], 7, controlPoints));
    animators.push_back(std::move(bezierBird));

    return Scene {
            ShaderProgram::phongLighting(),
            std::move(objects),
            std::move(animators),
            std::move(lights)
    };
}

/**
 * @brief Constructs a scene of a tiger sitting in a boat, where the tiger is the child object
 * of the boat.
 * @return
 */
Scene Scene::lifeOfPi() {
    // This scene is more complicated; it has child objects, as well as animators.
    auto boat = assimpLoad("../models/boat/boat.fbx", true);
    boat.move(glm::vec3(0, -0.7, 0));
    boat.grow(glm::vec3(0.01, 0.01, 0.01));
    auto tiger = assimpLoad("../models/tiger/scene.gltf", true);
    tiger.move(glm::vec3(0, -5, 10));
    boat.addChild(std::move(tiger));

    // Because boat and tiger are local variables, they will be destroyed when this
    // function terminates. To prevent that, we need to move them into a vector, and then
    // move that vector as part of the return value.
    std::vector<Object3D> objects;
    objects.push_back(std::move(boat));

    // We want these animations to referenced the *moved* objects, which are no longer
    // in the variables named "tiger" and "boat". "boat" is now in the "objects" list at
    // index 0, and "tiger" is the index-1 child of the boat.
    Animator animBoat;
    animBoat.addAnimation(std::make_unique<RotationAnimation>(objects[0], 10, glm::vec3(0, 6.28, 0)));
    Animator animTiger;
    animTiger.addAnimation(std::make_unique<RotationAnimation>(objects[0].getChild(1), 10, glm::vec3(0, 0, 6.28)));

    // The Animators will be destroyed when leaving this function, so we move them into
    // a list to be returned.
    std::vector<Animator> animators;
    animators.push_back(std::move(animBoat));
    animators.push_back(std::move(animTiger));

    // Transfer ownership of the objects and animators back to the main.
    return Scene {
            ShaderProgram::phongLighting(),
            std::move(objects),
            std::move(animators),
            {}
    };
}

/**
 * @brief Constructs a scene of the textured Stanford bunny.
 */
Scene bunny() {
    auto bunny = assimpLoad("../models/bunny_textured.obj", true);
    bunny.grow(glm::vec3(9, 9, 9));
    bunny.move(glm::vec3(0.2, -1, 0));

    return Scene {
            ShaderProgram::phongLighting(),
            {bunny}
    };
}

/**
 * @brief  Demonstrates loading a square, oriented as the "floor", with a manually-specified texture
 * that does not come from Assimp.
 * @return
 */
Scene marbleSquare() {
    std::vector<Texture> textures = {
            Texture::loadTexture("../models/White_marble_03/Textures_4K/white_marble_03_4k_baseColor.tga", "baseTexture"),
    };

    auto mesh = Mesh3D::square(textures);
    auto square = Object3D(std::vector<Mesh3D>{mesh});
    square.grow(glm::vec3(5, 5, 5));
    square.rotate(glm::vec3(-3.14159 / 4, 0, 0));
    return Scene{
            ShaderProgram::phongLighting(),
            {square}
    };
}
