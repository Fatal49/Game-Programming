#include "Game.h"

Game::Game(Uint32 flags, const char* name, int w, int h)
: App(flags, name, w, h), width(w), height(h) { init(); }

Game::~Game() {
    if (!shader)
        delete shader;
}

void Game::init() {
    
    // Load, compile & link the shaders
    shader = new Shader(SHADERS"vertex_textured.glsl", SHADERS"fragment_textured.glsl");
    
    // Set matrices & orthographic projection
    shader->setModelMatrix(model);
    shader->setViewMatrix(view);
    shader->setProjectionMatrix(projection);
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    // Id for sprite sheet
    GLuint spriteSheetID = LoadTexture(IMAGE"sprites.png");
    
    // Load sprites
    invaders.push_back(Entity(spriteSheetID, 202.0f/512.0f, 139.0f/256.0f, 72.0f/512.0f,
                              67.0f/256.0f, 0.1f, GHOST, ALIVE));
    invaders.push_back(Entity(spriteSheetID, 276.0f/512.0f, 139.0f/256.0f, 70.0f/512.0f,
                              67.0f/256.0f, 0.1f, GHOST, ALIVE));
    invaders.push_back(Entity(spriteSheetID, 202.0f/512.0f, 70.0f/256.0f, 94.0f/512.0f,
                              67.0f/256.0f, 0.1f, GHOST, ALIVE));
    invaders.push_back(Entity(spriteSheetID, 108.0f/512.0f, 90.0f/256.0f, 92.0f/512.0f,
                              66.0f/256.0f, 0.1f, GHOST, ALIVE));
    invaders.push_back(Entity(spriteSheetID, 202.0f/512.0f, 0.0f, 98.0f/512.0f,
                              68.0f/256.0f, 0.1f, GHOST, ALIVE));
    invaders.push_back(Entity(spriteSheetID, 0.0f, 158.0f/256.0f, 100.0f/512.0f,
                              66.0f/256.0f, 0.1f, GHOST, ALIVE));
    invaders.push_back(Entity(spriteSheetID, 0.0f, 0.0f, 200.0f/512.0f,
                              88.0f/256.0f, 0.1f, GHOST, ALIVE));
    bullets.push_back(Entity(spriteSheetID, 302.0f/512.0f, 0.0f, 24.0f/512.0f, 97.0f/256.0f, 0.18f,
                    ANIMATION, DEAD));
    destruction = Entity(spriteSheetID, 102.0f/512.0f, 158.0f/256.0f, 98.0f/512.0f, 58.0f/256.0f, 0.1f,
                         ANIMATION, DEAD);
    player = Entity(spriteSheetID, 0.0f, 90.0f/256.0f, 106.0f/512.0f, 66.0f/256.0f, 0.1f,
                    PLAYER, ALIVE);
    
    fontTexture = LoadTexture(IMAGE"font1.png");
    
    // Setup the scene
    setupScene();
}

void Game::setupScene() {
    // Setup the draw Box
    db.topLeft = vec::vec2(-1.0f * 0.6f, 1.0f * 0.9f);
    db.topRight = vec::vec2(1.0f * 0.6f, 1.0f * 0.9f);
    db.bottomLeft = vec::vec2(-1.0f * 0.6f, -1.0f * 0.1f);
    db.bottomRight = vec::vec2(1.0f * 0.6f, -1.0f * 0.1f);
    
    // Setup the window box for collision detection
    window.topLeft = vec::vec2(-1.0f, 1.0f);
    window.topRight = vec::vec2(1.0f, 1.0f);
    window.bottomLeft = vec::vec2(-1.0f, -1.0f);
    window.bottomRight = vec::vec2(1.0f, -1.0f);
    
    // Calculate OpenGL coordinates
    float x_dis = (fabs(db.topLeft.x) + fabs(db.topRight.x)) / MAX_SPRITES_COL;
    float y_dis = (fabs(db.topRight.y) + fabs(db.bottomRight.y)) / MAX_SPRITES_ROW;
    
    // Which sprite is drawn in which row
    std::vector<std::size_t> rows = { 2, 4, 3, 5, 5 };
    
    // Set up the invaders intial positions
    for (std::size_t i = 0; i < rows.size(); i++) {
        std::vector<Entity> vec;
        for (std::size_t index = 0; index < MAX_SPRITES_COL; index++) {
            Entity temp(invaders[rows[i]]);
            vec::vec2 iPos;
            iPos.x = db.topLeft.x + (x_dis * index);
            iPos.y = db.topLeft.y - (y_dis * i);
            temp.setPosition(iPos);
            temp.velocity = vec::vec2(1.5f, 0.0f);
            vec.push_back(temp);
        }
        invaderGrid.push_back(vec);
    }

    // Setup the players intial position
    player.setPosition(vec::vec2(0.0f, -0.92f));
    
    // Setup the bullets
    bullets[0].setPosition(vec::vec2(0.0f, -3.0f));
    bullets[0].velocity = vec::vec2(0.0f, 3.0f);
    bullets[0].ls = DEAD;
    bulletHalfHeight = (bullets[0].sprite.height * 0.5 ) / 4.0f;
    for (int i = 1; i < MAX_BULLETS; i++)
        bullets.push_back(Entity(bullets[0]));
}

GLuint Game::LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void Game::update(float elapsed, eventParse ep) {
    switch (gs) {
        case TITLE_SCREEN:
            if (ep.sc == SDL_SCANCODE_RETURN)
                gs = GAME;
            break;
            
        case GAME:
            for (int y = 0; y < invaderGrid.size(); y++) {
                // Collision Detection
                if (invaderGrid[y].size() == 0)
                    continue;
                if (invaderGrid[y].size() == 1) {
                    invaderGrid[y][0].detectCollision(window);
                    if (invaderGrid[y][0].collisionLeft) {
                        invaderGrid[y][0].translateModel(0.0f, -0.01f, 0.0f);
                        invaderGrid[y][0].velocity *= -1;
                    }
                    if (invaderGrid[y][0].collisionRight) {
                        invaderGrid[y][0].translateModel(0.0f, -0.01f, 0.0f);
                        invaderGrid[y][0].velocity *= -1;
                    }
                    
                }
                if (invaderGrid[y].size() > 1) {
                    Entity f = invaderGrid[y].front();
                    Entity b = invaderGrid[y].back();
                    
                    f.detectCollision(window);
                    b.detectCollision(window);
                    
                    if (f.collisionLeft) {
                        for (int i = 0; i < invaderGrid[y].size(); i++) {
                            invaderGrid[y][i].translateModel(0.0f, -0.01f, 0.0f);
                            invaderGrid[y][i].velocity *= -1;
                        }
                    }
                    if (b.collisionRight)
                        for (int i = 0; i < invaderGrid[y].size(); i++) {
                            invaderGrid[y][i].translateModel(0.0f, -0.01f, 0.0f);
                            invaderGrid[y][i].velocity *= -1;
                        }
                }
                
                // Update all invaders
                for (int x = 0; x < invaderGrid[y].size(); x++) {
                    invaderGrid[y][x].update(elapsed);
                }
                
                // Collision detection and update for player
                player.detectCollision(window);
                if (ep.sc == SDL_SCANCODE_RIGHT) {                      // Move right
                    if (!player.collisionRight) {
                        printf("Hello, there\n");
                        player.velocity = vec::vec2(8.5f, 0.0f);
                        player.update(elapsed);
                        ep.zeroOut();
                    }
                } else if (ep.sc == SDL_SCANCODE_LEFT) {                       // Move left
                    if (!player.collisionLeft) {
                        player.velocity = vec::vec2(-8.5f, 0.0f);
                        player.update(elapsed);
                        ep.zeroOut();
                    }
                } else if (ep.sc == SDL_SCANCODE_SPACE) {           // Shoots
                    printf("Here I am\n");
                    vec::vec4 pcp = player.currentPosition();
                    pcp.y += (bulletHalfHeight + (player.sprite.height * 0.5) - 0.035);
                    for (int i = 0; i < bullets.size(); i++)
                        if (bullets[i].ls == DEAD) {
                            bullets[i].ls = ALIVE;
                            bullets[i].setPosition(vec::vec2(pcp.x, pcp.y));
                            //  bullets[i].velocity = vec::vec2(5.0f, 1.2f);
                            bullets[i].update(elapsed);
                            break;
                        }
                    ep.zeroOut();
                } else if (ep.sc == SDL_SCANCODE_RETURN) {
                    gs = GAME;
                    ep.zeroOut();
                }
                
            }
            break;
            
        default:
            break;
    }
}

void Game::DrawText(Shader *program, GLuint fontTexture, std::string text, float size, float spacing) {
    float texture_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        });
    }
    
    glUseProgram(program->getProgramID());
    glVertexAttribPointer(program->getPositionAttrib(), 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->getPositionAttrib());
    glVertexAttribPointer(program->getTexCoordAttrib(), 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->getTexCoordAttrib());
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, (int) text.size() * 6);
    glDisableVertexAttribArray(program->getPositionAttrib());
    glDisableVertexAttribArray(program->getTexCoordAttrib());
}

void Game::onDraw(const eventParse& ep, float elapsed, float angle) {
    float fixedElapsed = elapsed;
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        update(FIXED_TIMESTEP, ep);
    }
    
    switch (gs) {
        case TITLE_SCREEN:
            model.Translate(-0.45f, 0.3f, 0.0f);
            model.Scale(1.0f, 1.4f, 1.0f);
            shader->setModelMatrix(model);
            model.identity();
            DrawText(shader, fontTexture, "Press Enter to Start", 0.05f, 0.0f);
            
            model.Translate(-0.63f, -0.1f, 0.0f);
            model.Scale(2.0f, 3.0f, 1.0f);
            shader->setModelMatrix(model);
            model.identity();
            DrawText(shader, fontTexture, "Space Invaders", 0.05f, 0.0f);
            break;
        case GAME:
            // Render the scene
            for (int y = 0; y < invaderGrid.size(); y++)
                for (int x = 0; x < invaderGrid[0].size(); x++)
                    invaderGrid[y][x].draw(shader);
            
            player.draw(shader);
            
            for (int i = 0; i < bullets.size(); i++)
                bullets[i].draw(shader);
            break;
            
        default:
            break;
    }
}






