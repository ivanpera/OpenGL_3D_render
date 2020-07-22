//// OBJ_Loader.h - A Single Header OBJ Model Loader
//// Credits: Robert Smith https://github.com/Bly7/OBJ-Loader
#pragma once

#include "common.h"

// Iostream - STD I/O Library
#include <iostream>

// Vector - STD Vector/Array Library
#include <vector>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>

// Math.h - STD math Library
#include <math.h>
#include "Mesh.h"
#include "Material.h"

// Print progress to console while loading (large models)
#define OBJL_CONSOLE_OUTPUT

// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace objl
{
	// Namespace: Math
	//
	// Description: The namespace that holds all of the math
	//	functions need for OBJL
	namespace math
	{
		// myCommon::vec3 Cross Product
		myCommon::vec3 CrossV3(const myCommon::vec3 a, const myCommon::vec3 b);

		// myCommon::vec3 Magnitude Calculation
		float MagnitudeV3(const myCommon::vec3 in);

		// myCommon::vec3 DotProduct
		float DotV3(const myCommon::vec3 a, const myCommon::vec3 b);

		// Angle between 2 myCommon::vec3 Objects
		float AngleBetweenV3(const myCommon::vec3 a, const myCommon::vec3 b);

		// Projection Calculation of a onto b
		myCommon::vec3 ProjV3(const myCommon::vec3 a, const myCommon::vec3 b);
	}

	// Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm
	{
		// myCommon::vec3 Multiplication Opertor Overload
		myCommon::vec3 operator*(const float& left, const myCommon::vec3& right);

		// A test to see if P1 is on the same side as P2 of a line segment ab
		bool SameSide(myCommon::vec3 p1, myCommon::vec3 p2, myCommon::vec3 a, myCommon::vec3 b);

		// Generate a cross produect normal for a triangle
		myCommon::vec3 GenTriNormal(myCommon::vec3 t1, myCommon::vec3 t2, myCommon::vec3 t3);

		// Check to see if a myCommon::vec3 Point is within a 3 myCommon::vec3 Triangle
		bool inTriangle(myCommon::vec3 point, myCommon::vec3 tri1, myCommon::vec3 tri2, myCommon::vec3 tri3);

		// Split a String into a string array at a given token
		inline void split(const std::string& in,
			std::vector<std::string>& out,
			std::string token);

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string& in);

		// Get first token of string
		inline std::string firstToken(const std::string& in);

		// Get element at given index position
		template <class T>
		inline const T& getElement(const std::vector<T>& elements, std::string& index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
	}

	// Class: Loader
	//
	// Description: The OBJ Model Loader
	class Loader
	{
	public:
		// Default Constructor
		Loader()
		{

		}
		~Loader()
		{
			LoadedMeshes.clear();
		}

		// Load a file into the loader
		//
		// If file is loaded return true
		//
		// If the file is unable to be found
		// or unable to be loaded return false
		bool LoadFile(std::string Path);

		// Loaded Mesh Objects
		std::vector<Mesh> LoadedMeshes;
		// Loaded myCommon::Point Objects
		std::vector<myCommon::Point> LoadedVertices;
		// Loaded Index Positions
		std::vector<unsigned int> LoadedIndices;
		// Loaded Material Objects
		std::vector<Material> LoadedMaterials;

	private:
		// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
		void GenVerticesFromRawOBJ(std::vector<myCommon::Point>& oVerts,
			const std::vector<myCommon::vec3>& iPositions,
			const std::vector<myCommon::vec2>& iTCoords,
			const std::vector<myCommon::vec3>& iNormals,
			std::string icurline);

		// Load Materials from .mtl file
		bool LoadMaterials(std::string path);
		// Triangulate a list of vertices into a face by printing
		//	inducies corresponding with triangles within it
		void PointTriangulation(std::vector<unsigned int>& oIndices,
			const std::vector<myCommon::Point>& iVerts);
		
	};
}

