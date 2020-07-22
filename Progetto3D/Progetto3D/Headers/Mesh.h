#pragma once
#include <vector>
#include "common.h"
#include "Material.h"

class Mesh {
public:
	// Default Constructor
	Mesh()
	{

	}
	// Variable Set Constructor
	Mesh(std::vector<myCommon::Point>& _Vertices, std::vector<unsigned int>& _Indices)
	{
		vertices = _Vertices;
		indices = _Indices;
	}
	// Mesh Name
	std::string name;
	// myCommon::Point List
	std::vector<myCommon::Point> vertices;
	// Index List
	std::vector<unsigned int> indices;

	// Material
	Material material;
};