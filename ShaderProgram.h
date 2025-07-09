#pragma once
#include <glm/ext.hpp>
#include <string>
#include <SFML/Window.hpp>

class ShaderProgram {

public:
    uint32_t m_programId;
	ShaderProgram();
	void load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void activate();

	void setUniform(const std::string& uniformName, bool value);
	void setUniform(const std::string& uniformName, int32_t value);
	void setUniform(const std::string& uniformName, float_t value);
	void setUniform(const std::string& uniformName, const glm::vec2& value);
	void setUniform(const std::string& uniformName, const glm::vec3& value);
	void setUniform(const std::string& uniformName, const glm::vec4& value);
	void setUniform(const std::string& uniformName, const glm::mat2& value);
	void setUniform(const std::string& uniformName, const glm::mat3& value);
	void setUniform(const std::string& uniformName, const glm::mat4& value);

    static ShaderProgram phongLighting();
    static ShaderProgram textureMapping();

    void RenderShadowMap(sf::Window &window);

    void ShadowMapComplete();

    void EnableShadowMap(sf::Window &window);

    void EnableShadowMap();

    void setUniform(const std::string &uniformName, uint32_t value);

    void RenderShadowMap();

    static ShaderProgram skyShading();
};