#ifndef COMMON
#define COMMON

#include <string>

namespace myCommon {

	constexpr float PI = 3.14159265358979323846f;

	enum class DrawMode { points, lines, lines_strip, lines_loop, triangles, triangles_fan, triangles_strip };

	// Structure: Vector2
	//
	// Description: A 2D Vector that Holds Positional Data
	struct vec2
	{
		// Default Constructor
		vec2()
		{
			x = 0.0f;
			y = 0.0f;
		}
		// Variable Set Constructor
		vec2(float x_, float y_)
		{
			x = x_;
			y = y_;
		}
		vec2& operator = (vec2 const& other) {
			this->x = other.x;
			this->y = other.y;
			return *this;
		}
		vec2& operator += (vec2 const& other) {
			this->x += other.x;
			this->y += other.y;
			return *this;
		}

		vec2& operator *= (vec2 const& other) {
			this->x *= other.x;
			this->y *= other.y;
			return *this;
		}
		// Bool Equals Operator Overload
		bool operator==(const vec2& other) const
		{
			return (this->x == other.x && this->y == other.y);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const vec2& other) const
		{
			return !(this->x == other.x && this->y == other.y);
		}
		// Addition Operator Overload
		vec2 operator+(const vec2& right) const
		{
			return vec2(this->x + right.x, this->y + right.y);
		}
		// Subtraction Operator Overload
		vec2 operator-(const vec2& right) const
		{
			return vec2(this->x - right.x, this->y - right.y);
		}
		// Float Multiplication Operator Overload
		vec2 operator*(const float& other) const
		{
			return vec2(this->x * other, this->y * other);
		}

		// Positional Variables
		float x;
		float y;
	};

	// Structure: Vector3
	//
	// Description: A 3D Vector that Holds Positional Data
	struct vec3 {
		// Default Constructor
		vec3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		// Variable Set Constructor
		vec3(float x_, float y_, float z_)
		{
			x = x_;
			y = y_;
			z = z_;
		}

		vec3& operator = (vec3 const& other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;
		}
		vec3& operator += (vec3 const& other) {
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		vec3& operator *= (vec3 const& other) {
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;
		}
		// Bool Equals Operator Overload
		bool operator==(const vec3& other) const
		{
			return (this->x == other.x && this->y == other.y && this->z == other.z);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const vec3& other) const
		{
			return !(this->x == other.x && this->y == other.y && this->z == other.z);
		}
		// Addition Operator Overload
		vec3 operator+(const vec3& right) const
		{
			return vec3(this->x + right.x, this->y + right.y, this->z + right.z);
		}
		// Subtraction Operator Overload
		vec3 operator-(const vec3& right) const
		{
			return vec3(this->x - right.x, this->y - right.y, this->z - right.z);
		}
		// Float Multiplication Operator Overload
		vec3 operator*(const float& other) const
		{
			return vec3(this->x * other, this->y * other, this->z * other);
		}
		// Float Division Operator Overload
		vec3 operator/(const float& other) const
		{
			return vec3(this->x / other, this->y / other, this->z / other);
		}

		// Positional Variables
		float x;
		float y;
		float z;
	};

	struct vec4 {
		float x, y, z, w;
	};

	struct Point {
		vec3 coords;
		vec3 normal;
		vec2 uvCoords;
	};

	float const signAngleCos(float grad);

	float const signAngleSin(float grad);

	float degToRad(float const& grad);

	std::string const vecToString(vec3 vec, std::string name);

}
#endif
