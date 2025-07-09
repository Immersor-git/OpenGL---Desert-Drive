#include <iostream>
#include "Mesh3D.h"
#include <glad/glad.h>

using std::vector;
using sf::Vector2u;
using glm::mat4;
using glm::vec4;

glm::vec3 calculateTangent(const glm::vec3& normal1, const glm::vec3& normal2, const glm::vec3& normal3,
                           const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3) {
    // Calculate the two vectors for this face
    glm::vec3 edge1 = normal2 - normal1;
    glm::vec3 edge2 = normal3 - normal1;

    // Calculate the two texture coordinate vectors for this face
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    // Calculate the reciprocal of the determinant of the matrix of the above vectors
    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    // Use the reciprocal and the vectors to calculate the tangent
    glm::vec3 tangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = glm::normalize(tangent);

    return tangent;
}
//

Mesh3D::Mesh3D(std::vector<Vertex3D>&& vertices, std::vector<uint32_t>&& faces,
	Texture texture) 
	: Mesh3D(std::move(vertices), std::move(faces), std::vector<Texture>{texture}) {
}

Mesh3D::Mesh3D(std::vector<Vertex3D>&& vertices, std::vector<uint32_t>&& faces, std::vector<Texture>&& textures)
 : m_vertexCount(vertices.size()), m_faceCount(faces.size()), m_textures(textures) {
   /*
    for (int fId = 0; fId < m_faceCount-2; fId+=3) {
        u_int f1 = faces[fId];
        u_int f2 = faces[fId+1];
        u_int f3 = faces[fId+2];

        glm::vec3 norm1 = glm::vec3(vertices[f1].nx, vertices[f1].ny, vertices[f1].nz);
        glm::vec3 norm2 = glm::vec3(vertices[f2].nx, vertices[f2].ny, vertices[f2].nz);
        glm::vec3 norm3 = glm::vec3(vertices[f3].nx, vertices[f3].ny, vertices[f3].nz);

        glm::vec2 uv1 = glm::vec2(vertices[f1].u, vertices[f1].v);
        glm::vec2 uv2 = glm::vec2(vertices[f2].u, vertices[f2].v);
        glm::vec2 uv3 = glm::vec2(vertices[f3].u, vertices[f3].v);

        glm::vec3 tangent = calculateTangent(norm1, norm2, norm3, uv1, uv2, uv3);
        Vertex3D vertice1 = vertices[f1];
        Vertex3D vertice2 = vertices[f2];
        Vertex3D vertice3 = vertices[f3];

        //vertice1.setTangent(tangent);
        //vertice2.setTangent(tangent);
        //vertice3.setTangent(tangent);

        //vertices[f1] = std::move(vertice1);
        //vertices[f2] = std::move(vertice2);
        //vertices[f3] = std::move(vertice3);
    }
    */
	// Generate a vertex array object on the GPU.
	glGenVertexArrays(1, &m_vao);
	// "Bind" the newly-generated vao, which makes future functions operate on that specific object.
	glBindVertexArray(m_vao);

	// Generate a vertex buffer object on the GPU.
	uint32_t vbo;
	glGenBuffers(1, &vbo);

	// "Bind" the newly-generated vbo, which makes future functions operate on that specific object.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// This vbo is now associated with m_vao.
	// Copy the contents of the vertices list to the buffer that lives on the GPU.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices[0], GL_STATIC_DRAW);

	// Inform OpenGL how to interpret the buffer. Each vertex now has TWO attributes; a position and a color.
	// Atrribute 0 is position: 3 contiguous floats (x/y/z)...
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex3D), 0);
	glEnableVertexAttribArray(0);

	// Attribute 1 is normal (nx, ny, nz): 3 contiguous floats, starting 12 bytes after the beginning of the vertex.
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex3D), (void*)12);
	glEnableVertexAttribArray(1);

	// Attribute 2 is texture coordinates (u, v): 2 contiguous floats, starting 24 bytes after the beginning of the vertex.
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex3D), (void*)24);
	glEnableVertexAttribArray(2);

	// Generate a second buffer, to store the indices of each triangle in the mesh.
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(uint32_t), &faces[0], GL_STATIC_DRAW);

	// Unbind the vertex array, so no one else can accidentally mess with it.
	glBindVertexArray(0);
// Render commands will no longer render to the screen.

}

void Mesh3D::addTexture(Texture texture)
{
	m_textures.push_back(texture);
}

void Mesh3D::render(sf::Window& window, ShaderProgram& program) const {
	// Activate the mesh's vertex array.
	glBindVertexArray(m_vao);
    bool hasHeightmap = false;
	for (auto i = 0; i < m_textures.size(); i++) {
		program.setUniform(m_textures[i].samplerName, i + 4);
        if (m_textures[i].samplerName == "heightMap") hasHeightmap = true;
		glActiveTexture(GL_TEXTURE4 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].textureId);
	}
    //if (hasHeightmap) std::cout<<"Heightmap render \n";
    program.setUniform("hasHeightmap", hasHeightmap);
	// Draw the vertex array, using its "element buffer" to identify the faces.
	glDrawElements(GL_TRIANGLES, m_faceCount, GL_UNSIGNED_INT, nullptr);
	// Deactivate the mesh's vertex array and texture.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Mesh3D Mesh3D::square(const std::vector<Texture> &textures) {
	return Mesh3D(
		{ 
		  { 0.5, 0.5, 0, 0, 0, -1, 1, 0 },    // TR
		  { 0.5, -0.5, 0, 0, 0, -1, 1, 1 },   // BR
		  { -0.5, -0.5, 0, 0, 0, -1, 0, 1 },  // BL
		  { -0.5, 0.5, 0, 0, 0, -1, 0, 0 },   // TL
		}, 
		{ 
			3, 1, 2,
			0, 1, 3,
		},
		std::vector<Texture>(textures)
	);
}

Mesh3D Mesh3D::triangle(Texture texture) {
	return Mesh3D(
		{ { -0.5, -0.5, 0., 0, 0, 1, 0., 1. },
		  { -0.5, 0.5, 0, 0, 0, 1, 0., 0. },
		  { 0.5, 0.5, 0, 0, 0, 1, 1, 0 } },
		{ 2, 1, 0 },
		texture
	);
}

Mesh3D Mesh3D::cube(Texture texture) {
	std::vector<Vertex3D> verts = {
		///*BUR*/{ 0.5, 0.5, -0.5,  0, 0},
		///*BUL*/{ -0.5, 0.5, -0.5, 0, 0},
		///*BLL*/{ -0.5, -0.5, -0.5, 1.0, 0 },
		///*BLR*/{ 0.5, -0.5, -0.5, 0, 1.0},
		///*FUR*/{ 0.5, 0.5, 0.5, 1.0, 0},
		///*FUL*/{-0.5, 0.5, 0.5, 1.0, 1.0},
		///*FLL*/{-0.5, -0.5, 0.5, 0, 1.0},
		///*FLR*/{0.5, -0.5, 0.5, 1.0, 1.0}
	};
	std::vector<uint32_t> tris = {
		0, 1, 2,
		0, 2, 3,
		4, 0, 3,
		4, 3, 7,
		5, 4, 7,
		5, 7, 6,
		1, 5, 6,
		1, 6, 2,
		4, 5, 1,
		4, 1, 0,
		2, 6, 7,
		2, 7, 3
	};

	return Mesh3D({}, {}, texture);
}
