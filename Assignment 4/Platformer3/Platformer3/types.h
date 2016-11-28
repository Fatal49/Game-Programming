//
//  types.h
//  Platformer
//
//  Created by Hassani Senior on 11/5/16.
//  Copyright © 2016 Hassani Senior. All rights reserved.
//

#ifndef __TYPES_H__
#define __TYPES_H__

#include "vec.h"

// entity
enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN };
enum EntityCollPlacement { TOP, RIGHT, BOTTOM, LEFT };
struct collInfo {
    EntityCollPlacement ecp;
    vec::vec2 collEntityPos;
};

// Structure for sprites information
struct spriteInfo {
    spriteInfo() {}
    spriteInfo(const spriteInfo& rhs) : name(rhs.name), u(rhs.u), v(rhs.v),
    width(rhs.width), height(rhs.height) {
        for (int i = 0; i < 12; i++)
            texCoords[i] = rhs.texCoords[i];
        
        for (int i = 0; i < 12; i++)
            vertices[i] = rhs.vertices[i];
    }
    
    std::string name;
    GLfloat texCoords[12];
    GLfloat vertices[12];
    float u = 0.0f, v = 0.0f;
    float width = 0.0f, height = 0.0f;
};

// Collision box for entities
struct AABB {
    vec::vec2 center;
    vec::vec2 halfSize;
};


#endif /* __TYPES_H__ defined */
