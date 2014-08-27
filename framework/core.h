/*
 * framework/core.h
 *
 *      Author: MMW
 */
#ifndef CORE_H_
#define CORE_H_

#define DEBUG true

#define PI 3.14159

#define GLM_FORCE_RADIANS

/* Number of bone influences per vertex for vertex weighting. */
#define INFLUENCES      4
/* Number of bones in armature. */
#define TRANSFORMATIONS 8
/* Number of morph targets the shader supports. */
#define MORPHLIMIT      2

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>

#include <memory>

#include <algorithm>
#include <vector>
#include <map>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Exceptions.h"

/* Use "std" namespace for "printf" for quick debugging. */
using std::printf;
using std::cout;
using std::endl;

/* Data types */
typedef uint Index;
typedef std::string String;
typedef glm::vec4 Vec4;
typedef glm::vec3 Vec3;
typedef glm::vec2 Vec2;
typedef glm::mat4 Mat4;
typedef glm::quat Quat;
typedef std::vector<glm::quat> QuatArray;
typedef std::vector<Vec3> Vec3Array;
typedef std::vector<Vec2> Vec2Array;
typedef std::vector<glm::mat4> Mat4Array;
typedef std::vector<Index> IndexArray;
typedef std::map<Index, glm::vec3> MapIndexVec3;

#endif /* CORE_H_ */
