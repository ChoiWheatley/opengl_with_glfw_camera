#pragma once
#include <memory>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Node.h"

class ShaderI;
class TextureI;
class VertexI;

class Mesh : public Node
{
public: // interfaces
	virtual glm::mat4 getCoordSpace() override;
	int getVao();

public: // constructor
	Mesh(std::shared_ptr<ShaderI> shader,
		std::shared_ptr<TextureI> texture,
		std::shared_ptr<VertexI> vertex);

private: // member
	std::shared_ptr<ShaderI> shader;
	std::shared_ptr<TextureI> texture;
	std::shared_ptr<VertexI> vertex;
};

// shader를 관리하는 객체의 역할을 정의하는 인터페이스.
class ShaderI
{
public:
	virtual ~ShaderI() = default;
	// interfaces
	/**
	 * \brief Shader code --> Shader program
	 */
	virtual void useShaderProgram() const = 0;
	/**
	 * \brief change uniform value
	 * \param name string literal pointing GLSL's uniform variable
	 * \param value value
	 */
	virtual void setUniformValue(const std::string& name, const glm::mat4& value) const = 0;
	virtual void setUniformValue(const std::string& name, const glm::vec3& value) const = 0;
	virtual void setUniformValue(const std::string& name, const glm::vec2& value) const = 0;
	virtual void setUniformValue(const std::string& name, float value) const = 0;
	virtual void setUniformValue(const std::string& name, int value) const = 0;
	virtual void setUniformValue(const std::string& name, bool value) const = 0;
};

class TextureI
{
public: // interfaces
	virtual const char* getTextureImg() const = 0;
};

/**
 * \brief raw vertex, tex_coord 데이터를 vbo에 담아 vao에 리턴하기,
 * vertex shader layout 번호와 vbo를 알맞게 가리키게 만들기
 */
class VertexI
{
public: // interfaces
	virtual void setVertexAttribute(GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) = 0;

};
