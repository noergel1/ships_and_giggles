#include "helpers.h"

float degreeToRadians(float _angle)
{
	return _angle * (M_PI / 180);
}

float radiansToDegree(float _angle)
{
	return _angle * (180 / M_PI);
}


unsigned int createVao(const std::vector<VertexData>& _vertices, const std::vector<unsigned int>& _indices)
{

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * _vertices.size(), &_vertices.at(0), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices.at(0), GL_STATIC_DRAW);

	glBindVertexArray(0);

	return VAO;
}

unsigned int createVao(const std::vector<VertexData>& _vertices)
{
	std::vector<unsigned int> indices(_vertices.size());
	std::iota(std::begin(indices), std::end(indices), 0); // Fill with consecutive ints up to _vertices.size() - 1

	unsigned int VAO = createVao(_vertices, indices);


	return VAO;
};