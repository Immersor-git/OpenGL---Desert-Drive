#ifndef MATTSQUARED_GRAPHICS_BEZIERANIMATION_H
#define MATTSQUARED_GRAPHICS_BEZIERANIMATION_H

#include <vector>
#include <glm/glm.hpp>
#include "Animation.h"
#include <iostream>

class BezierAnimation : public Animation {
private:
    std::vector<glm::vec3> m_controlPoints;
    std::vector<glm::vec3> m_bezierPoints;
    float m_t = 0.0f;
    float m_elapsedTime = 1.0f;
    float m_duration;

void applyAnimation(float_t dt) override {
    m_elapsedTime += dt;
    m_t = m_elapsedTime / m_duration;
    if (m_t > 1.0f) {
        m_t = 1.0f;
    }
    glm::vec3 newPosition = getBezierPoint(m_t);
    object().setPosition(newPosition);
}

glm::vec3 calulateBezierPoint(float t) {
    int numControlPoints = m_controlPoints.size();

}

glm::vec3 getBezierPoint(float t) {
    m_bezierPoints = m_controlPoints;
    while (m_bezierPoints.size() > 1) {
        std::vector<glm::vec3> newPoints;
        for (size_t i = 0; i < m_bezierPoints.size() - 1; i++) {
            newPoints.push_back(m_bezierPoints[i] * (1 - t) + m_bezierPoints[i + 1] * t);
        }
        m_bezierPoints = newPoints;
    }
    return m_bezierPoints[0];
}
public:
    BezierAnimation(Object3D& object, float_t duration, const std::vector<glm::vec3>& controlPoints) :
        Animation(object, duration), m_duration(duration), m_controlPoints(controlPoints) {}
};

#endif //MATTSQUARED_GRAPHICS_BEZIERANIMATION_H
