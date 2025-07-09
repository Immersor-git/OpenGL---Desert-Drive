/**
This application renders a textured mesh that was loaded with Assimp.
*/

#include <glad/glad.h>
#include <iostream>

#include "Mesh3D.h"
#include "ShaderProgram.h"
#include "Scene.h"
#include "UserInput.h"
#include "Vehicle.h"
#include "SkyShading.h"

int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    Settings.majorVersion = 4;
    Settings.minorVersion = 1;
    Settings.attributeFlags = sf::ContextSettings::Attribute::Core;
	sf::Window window(sf::VideoMode{ 1600, 1200 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	// Initialize scene objects.
	auto scene = Scene::jeep();

	auto cameraPosition = glm::vec3(0,2,5);//glm::vec3(0, 0, 5);
	auto camera = glm::lookAt(cameraPosition, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	glm::mat4 perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);

	ShaderProgram& mainShader = scene.defaultShader;

	mainShader.activate();
	mainShader.setUniform("view", camera);
    mainShader.setUniform("projection", perspective);

    ShaderProgram skyShaderProgram = ShaderProgram::skyShading();
    skyShaderProgram.setUniform("view",glm::lookAt(glm::vec3(0,0,5),glm::vec3(0,0,0),glm::vec3(0,1,0)));
    skyShaderProgram.setUniform("projection",perspective);//glm::ortho(-1.0f,1.0f,-1.0f,1.0f,0.1f,100.0f));
    skyShaderProgram.setUniform("sunVector",glm::vec3(0,0,1));
    skyShaderProgram.setUniform("cameraVector",glm::vec3(0,0,-1));

    int skyVBO = SkyShading::InitializeWithQuad();
    //mainShader.setUniform("ambientColor",glm::vec3(0.3,0.3,0.3));
	// Ready, set, go!
	for (auto& animator : scene.animators) {
		animator.start();
	}

    UserInput input;

    auto l1 = DynamicLight(glm::mat4(1.0f));
    auto l2 = DynamicLight(glm::mat4(1.0f));
    auto l3 = DynamicLight(glm::mat4(1.0f));
    scene.lights.push_back(l1);
    scene.lights.push_back(l2);
    scene.lights.push_back(l3);
    auto square = Object3D({Mesh3D::square({})});
    square.grow(glm::vec3(3,3,3));
    Vehicle jeep(scene.objects[0], input, scene.lights[1], scene.lights[2], scene.lights[3]);
    int uIndex = 0;
    for (auto& light : scene.lights) {
        light.setUniformIndex(uIndex++);
        light.updateUniforms(mainShader);
    }

	bool running = true;
	sf::Clock c;
    float counter = 0.0f;
    DynamicLight& sun = scene.lights[0];

    bool shadingEnabled = true;
    unsigned int shadowMapFBO;
    unsigned int shadowMapWidth = 2048;
    unsigned int shadowMapHeight = 2048;
    unsigned int shadowMap;

    float near_plane = 0.1f, far_plane = 100.0f;
    float left = -25.0f, right = 25.0f, bottom = -25.0f, top = 25.0f;
    glm::mat4 shadowProjection = glm::ortho(left, right, bottom, top, near_plane, far_plane);
    if (shadingEnabled) {
        //mainShader.EnableShadowMap();
        glGenFramebuffers(1, &shadowMapFBO);

        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST); //GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST); //GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0+2);
        glBindTexture(GL_TEXTURE_2D,shadowMap);
    }
    mainShader.setUniform("shadowMap", 2);
    mainShader.setUniform("shadowProjection", shadowProjection);


    auto last = c.getElapsedTime();
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			} else {
                input.processInput(ev);
            }
		}
		
		auto now = c.getElapsedTime();
		auto diff = now - last;
		auto diffSeconds = diff.asSeconds();
		last = now;
		for (auto& animator : scene.animators) {
			animator.tick(diffSeconds);
		}
        //Jeep code
        jeep.Update(diffSeconds);
        //scene.objects[0].move(glm::vec3(input.sideInput,0,input.forwardInput)*diffSeconds*1.0f);
        //Some test code to move a spotlight around
        glm::vec3 lightPos = glm::vec3(sin(counter)*4,0+sin(counter*0.1234)*0.3,cos(counter)*4);
        //scene.lights[2].setPosition(jeep.headlightPos); //Moves the spotlight around
        //scene.lights[2].setDirection(normalize(jeep.direction - glm::vec3(0,0.3,0)));
        //scene.lights[2].updateUniforms(mainShader); //Call this whenever you change the light's properties
        //scene.objects[1].setPosition(lightPos); //Moves the tiger model to the light position
        counter += diff.asSeconds();
        jeep.headlights.updateUniforms(mainShader);
        jeep.l_brakeLight.updateUniforms(mainShader);
        jeep.r_brakeLight.updateUniforms(mainShader);
        cameraPosition = jeep.rearCamera;
        camera = glm::lookAt(cameraPosition, jeep.frontLookat, glm::vec3(0, 1, 0));
		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Shadows
        //sun.setDirection(glm::normalize(glm::vec3(5, -abs(sin(counter*0.25))*10,cos(counter*0.25)*10)));
        sun.updateUniforms(mainShader);
        if (shadingEnabled) {

            //Bounding sphere code//
            float boundingRadius = 25.0f;
            glm::vec3 cameraDirection = -glm::vec3(camera[0][2], camera[1][2], camera[2][2]);
            cameraDirection = glm::normalize(cameraDirection);
            glm::vec3 boundingCenter = cameraPosition + cameraDirection * boundingRadius*0.8f;
            //scene.objects[1].setPosition(boundingCenter);
            ///
            glm::mat4 sunMatrix = sun.getLightSpaceMatrix();
            glm::vec3 sunPosition = boundingCenter - glm::vec3(sunMatrix[3])*boundingRadius;
            glm::vec3 sunLookat = boundingCenter; //+ glm::direc;
            glm::mat4 lightView = glm::lookAt(sunPosition, sunLookat, glm::vec3(0,1,0));
            glm::mat4 lightSpaceMatrix = shadowProjection * lightView;//lightProjection * lightView;
            mainShader.setUniform("LightSpaceMatrix",lightSpaceMatrix);
            //skyShaderProgram.setUniform("sunVector",cameraPosition - glm::vec3(sunMatrix[3]));
            //mainShader.ShadowMapComplete();
            mainShader.setUniform("view", lightView);
            //mainShader.setUniform("viewPos", glm::lookAt(glm::vec3(0, 5, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)));
            mainShader.setUniform("includeLighting", false);
            //mainShader.RenderShadowMap(window);
            glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
            glBindTexture(GL_TEXTURE_2D,shadowMap);
            glViewport(0,0, shadowMapWidth, shadowMapHeight);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT);
            for (auto &o: scene.objects) {
                    o.render(window, mainShader);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //
            glViewport(0, 0, window.getSize().x, window.getSize().y);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mainShader.setUniform("view", camera);
        //mainShader.setUniform("viewPos", cameraPosition);
        //mainShader.setUniform("projection", perspective);
        mainShader.setUniform("includeLighting",true);
        //skyShaderProgram.setUniform("cameraVector",-glm::vec3(camera[0][2], camera[1][2], camera[2][2]));
        skyShaderProgram.setUniform("viewMatrix",camera);
        // your camera matrix
         // Render each object in the scene.
        SkyShading::RenderSky(skyVBO, mainShader,skyShaderProgram);
        for (auto& o : scene.objects) {
			o.render(window, mainShader);
		}
		window.display();
	}

	return 0;
}


