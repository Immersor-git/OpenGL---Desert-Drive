//
// Created by Matthew Halderman on 5/9/24.
//
#include <SFML/Window/Event.hpp>
#include "UserInput.h"
void UserInput::processInput(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.scancode == sf::Keyboard::Scancode::W) {
            //forward
            wPressed = true;
            forwardInput = 1;
        } else if (event.key.scancode == sf::Keyboard::Scancode::S) {
            //backward
            sPressed = true;
            forwardInput = -1;
        } else if (event.key.scancode == sf::Keyboard::Scancode::A) {
            //left
            aPressed = true;
            sideInput = -1;
        } else if (event.key.scancode == sf::Keyboard::Scancode::D) {
            //right
            dPressed = true;
            sideInput = 1;
        } else if (event.key.scancode == sf::Keyboard::Scancode::F) {
            highBeams = !highBeams;
        } else if (event.key.scancode == sf::Keyboard::Scancode::V) {
            cameraView = (cameraView + 1) % 5;
        } else if (event.key.scancode == sf::Keyboard::Scancode::N) {
            //timeOfDay = (timeOfDay + 1)%2;
        }

    } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.scancode == sf::Keyboard::Scancode::W) {
            //forward
            wPressed = false;
            forwardInput = sPressed ? -1 : 0;
        } else if (event.key.scancode == sf::Keyboard::Scancode::S) {
            //backward
            sPressed = false;
            forwardInput = wPressed ? 1 : 0;
        } else if (event.key.scancode == sf::Keyboard::Scancode::A) {
            //left
            aPressed = false;
            sideInput = dPressed ? 1 : 0;
        } else if (event.key.scancode == sf::Keyboard::Scancode::D) {
            //right
            dPressed = false;
            sideInput = aPressed ? -1 : 0;
        }
    }
}
#include "UserInput.h"
