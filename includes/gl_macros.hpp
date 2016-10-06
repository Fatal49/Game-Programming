#ifndef __GL_MACROS_HPP__
#define __GL_MACROS_HPP__

#include <math.h>
#include "vec.hpp"

struct Vertex {
    Vertex(vec::vec2 _position, vec::vec4 _color) : position(_position), color(_color) {}
    vec::vec2 position;
    vec::vec4 color;
};

#ifndef PI
#	define PI (float)3.14159265359
#endif

#ifndef RAND
#	define RAND (1.0 * rand() / (RAND_MAX))
#endif

#ifdef __APPLE__
    #define glGenVertexArrays glGenVertexArraysAPPLE
    #define glBindVertexArray glBindVertexArrayAPPLE
    #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

#endif /* __GL_MACROS_HPP__ defined */


