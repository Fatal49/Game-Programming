#ifndef __TYPES_H__
#define __TYPES_H__

#include "vec.h"

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

struct Vertex {
    Vertex() {}
    Vertex(vec::vec2 p, vec::vec4 _c) : position(p), c(_c) {}
    vec::vec2 position = vec::vec2(0.0f, 0.0f);
    vec::vec4 c = vec::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};

struct box {
    vec::vec2 topLeft;
    vec::vec2 topRight;
    vec::vec2 bottomLeft;
    vec::vec2 bottomRight;
};

enum type { PLAYER, GHOST, ANIMATION };

enum life_state { DEAD, ALIVE };

#endif /* __TYPES_H__ defined */