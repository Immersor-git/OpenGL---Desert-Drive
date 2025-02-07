/**
This application renders a textured mesh that was loaded with Assimp.
*/

#include <glad/glad.h>

#include "Mesh3D.h"
#include "ShaderProgram.h"
#include "Scene.h"

int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    Settings.majorVersion = 4;
    Settings.minorVersion = 1;
    Settings.attributeFlags = sf::ContextSettings::Attribute::Core;
	sf::Window window(sf::VideoMode{ 1200, 800 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	// Initialize scene objects.
	auto scene = Scene::jeep();

	auto cameraPosition = glm::vec3(0, 0, 5);
	auto camera = glm::lookAt(cameraPosition, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	auto perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);

	ShaderProgram& mainShader = scene.defaultShader;
	mainShader.activate();
	mainShader.setUniform("view", camera);
    mainShader.setUniform("projection", perspective);
    mainShader.setUniform("directionalLight", normalize(glm::vec3(-1,-1,-1)));
    mainShader.setUniform("ambientColor",glm::vec3(0.3,0.3,0.3));
    mainShader.setUniform("normalTexFader",0.5f);
    mainShader.setUniform("texNormalFader",0.5f);
	// Ready, set, go!
	for (auto& animator : scene.animators) {
		animator.start();
	}
	bool running = true;
	sf::Clock c;
    float counter = 0.0f;

	auto last = c.getElapsedTime();
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
		}
		
		auto now = c.getElapsedTime();
		auto diff = now - last;
		auto diffSeconds = diff.asSeconds();
		last = now;
		for (auto& animator : scene.animators) {
			animator.tick(diffSeconds);
		}

        counter += diff.asSeconds();

        mainShader.setUniform("texNormalFader",glm::vec4((sin(counter)+1.0f)*0.5f));
        mainShader.setUniform("directionalLight", normalize(glm::vec3(sin(counter*0.1),cos(counter*0.1),0)));
		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render each object in the scene.
		for (auto& o : scene.objects) {
			o.render(window, mainShader);
		}
		window.display();
	}

	return 0;
}


