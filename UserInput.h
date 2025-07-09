//
// Created by Matthew Halderman on 5/9/24.
//

#ifndef MATTSQUARED_GRAPHICS_USERINPUT_H
#define MATTSQUARED_GRAPHICS_USERINPUT_H


class UserInput {
public:
    bool wPressed = false;
    bool sPressed = false;
    bool aPressed = false;
    bool dPressed = false;
    bool highBeams = false;
    int cameraView = 0;
    int forwardInput = 0;
    int sideInput = 0;
    void processInput(sf::Event event);
};


#endif //MATTSQUARED_GRAPHICS_USERINPUT_H
