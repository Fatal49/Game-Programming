#include "Platformer.hpp"

Platformer::Platformer(float w, float h, const char* name): Game(w, h, name), elapsed(0.0f),
lastFrameTicks(0.0f), totalTime(0.0f), pause(false), shader(nullptr), texShader(nullptr),
player1(nullptr), player2(nullptr), bullet(nullptr), be(nullptr) {
    if (!createWindow()) {
        printf("The SDL window could not be created. Call again.\n");
    } else {
        printf("SDL window is created\n");
    }
}

Platformer::Platformer(const char* name) : Game(name), elapsed(0.0f),
lastFrameTicks(0.0f), totalTime(0.0f), pause(false) {
    if (!createWindow()) {
        printf("The SDL window could not be created. Call again.\n");
    } else {
        printf("SDL window is created\n");
    }
}

Platformer::~Platformer() {
    if (shader) {
        printf("Freeing shader\n");
        delete shader;
    }
    
    if (texShader) {
        printf("Freeing textured shader\n");
        delete texShader;
    }
    
    if (player1)
        delete player1;
    
    if (player2)
        delete player2;
    
    if (bullet)
        delete bullet;
    
    if (be)
        delete be;
    
    SDL_Quit();
}

GLuint Platformer::LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void Platformer::setup() {
    // Set the viewport
    setViewPort();
    
    // Enable alpha blending & depth
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Load, compile & link the untextured shaders
    shader = new Shader("vertex.glsl", "fragment.glsl");
    
    // Load, compile & link the textured shaders
    texShader = new Shader("vertex_textured.glsl", "fragment_textured.glsl");
    
    // Loading font texture
    fontTexture = LoadTexture("assets/font1.png");
    
    // Set the game state
    gs = START_SCREEN;
    
    // Set the fading
    fading = FADING_OUT;
    
    // Set matrices & orthographic projection in each shader
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    shader->setModelMatrix(matrix);
    shader->setViewMatrix(view);
    shader->setProjectionMatrix(projection);
    
    texShader->setModelMatrix(matrix);
    texShader->setViewMatrix(view);
    texShader->setProjectionMatrix(projection);
    
    // Setup player 1
    player1 = new Rectangle(0.5f, 0.5f, true);
    player1->create();
    player1->LoadTexture("assets/sprite1.png");
    player1->translate(-1.777, 0.0f);
    player1->scale(1.5f, 1.5f);
    
    // Setup player 2
    player2 = new Rectangle(0.5f, 0.5f);
    player2->create();
    player2->translate(1.777f, 0.0f);
    player2->scale(1.5f, 1.5f);
    
    // Setup the Bullet Emitter
    be = new BulletEmitter(0.5f, 2.0f, 10, vec::vec2(0.06f, 0.0f));
    be->LoadTexture("assets/bullet.png");
    be->createBullets();
    be->position = vec::vec2(0.0f, 0.5f);
    be->scale(0.125f, 0.125f);
    be->rotate(degreesToRadians(-90.0f));
    
}

void Platformer::render() {
    // Set the color for the viewport depending on the game state
    switch (gs) {
        case START_SCREEN:
            glClearColor(black.x, black.y, black.z, black.w);
            break;
            
        case GAME:
            glClearColor(lightBlack.x, lightBlack.y, lightBlack.z, lightBlack.w);
            break;
            
        default:
            break;
    }
    
    // Clear the buffer with preset values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Call draw function
    draw();
    
    // Swap buffers
    swapBuffers();
}

void Platformer::draw() {
    switch (gs) {
        case START_SCREEN:
        {
            matrix.identity();
            matrix.Translate(-2.4f, 1.3f, 0.0f);
            matrix.Scale(2.5f, 3.0f, 1.0f);
            texShader->setModelMatrix(matrix);
            DrawText(texShader, fontTexture, "The Game to be Known", 0.1f, 0.0f, false);
            
            matrix.identity();
            matrix.Translate(-1.25f, 0.75f, 0.0f);
            matrix.Scale(1.5f, 1.75f, 1.0f);
            texShader->setModelMatrix(matrix);
            DrawText(texShader, fontTexture, "Press Enter to Start", 0.1f, 0.0f, true);
            
            matrix.identity();
            matrix.Translate(-0.5f, 0.2f, 0.0f);
            matrix.Scale(2.0f, 2.5f, 1.0f);
            texShader->setModelMatrix(matrix);
            DrawText(texShader, fontTexture, "Stage 1", 0.1f, 0.0f, false);
            
            matrix.identity();
            matrix.Translate(-0.5f, -0.5f, 0.0f);
            matrix.Scale(2.0f, 2.5f, 1.0f);
            texShader->setModelMatrix(matrix);
            DrawText(texShader, fontTexture, "Stage 2", 0.1f, 0.0f, false);
            
            matrix.identity();
            matrix.Translate(-0.5f, -1.2f, 0.0f);
            matrix.Scale(2.0f, 2.5f, 1.0f);
            texShader->setModelMatrix(matrix);
            DrawText(texShader, fontTexture, "Stage 3", 0.1f, 0.0f, false);
        } break;
        
        case GAME:
        {
            // Draw player1 using the correct shader
            if (player1->texture)
                player1->draw(texShader);
            else
                player1->draw(shader);
            
            // Draw player2 using the correct shader
            if (player2->texture)
                player2->draw(texShader);
            else
                player2->draw(shader);
            
            // Draw the bullets
            be->draw(texShader);
            
        } break;
            
        default:
            break;
    }
}

void Platformer::update() {
    // For annimation
    float ticks = (float) SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    totalTime += elapsed;
    float fixedElapsed = elapsed;
    
    switch (gs) {
        case START_SCREEN:
        {
            SDL_Event e;
            SDL_PollEvent(&e);
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_RETURN:
                            gs = GAME;              // Switch game modes
                        default:
                            break;
                    }
                    break;
                    
                default:
                    break;
            }
        } break;
            
        case GAME:
        {
            // Update every 60th of a second
            if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
                fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
            }
            
            while (fixedElapsed >= FIXED_TIMESTEP ) {
                fixedElapsed -= FIXED_TIMESTEP;
                
                if (!pause) {
                    player1->update(FIXED_TIMESTEP);
                    player2->update(FIXED_TIMESTEP);
                    be->update(FIXED_TIMESTEP);
                }
            }
            
            if (!pause) {
                player1->update(fixedElapsed);
                player2->update(fixedElapsed);
                be->update(fixedElapsed);
            }
            
            // Get key input
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            
            // Player 1 movement
            if (keys[SDL_SCANCODE_D]) {
                player1->translate(0.035f, 0.0f);
            }
            
            if (keys[SDL_SCANCODE_S]) {
                player1->translate(0.0f, -0.035f);
            }
            
            if (keys[SDL_SCANCODE_A]) {
                player1->translate(-0.035f, 0.0f);
            }
            
            if (keys[SDL_SCANCODE_W]) {
                player1->translate(0.0f, 0.035f);
            }
            
            
            // Player 2 movement
            if (keys[SDL_SCANCODE_RIGHT]) {
                player2->translate(0.035f, 0.0f);
            }
            
            if (keys[SDL_SCANCODE_DOWN]) {
                player2->translate(0.0f, -0.035f);
            }
            
            if (keys[SDL_SCANCODE_LEFT]) {
                player2->translate(-0.035f, 0.0f);
            }
            
            if (keys[SDL_SCANCODE_UP]) {
                player2->translate(0.0f, 0.035f);
            }
            
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        setPlayingFlag(false);
                    
                    if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        printf("I am in the while loop \n");
                        be->shootABullet();
                    }
                }
            }
        } break;
            
        default:
            break;
    }
}

void Platformer::DrawText(Shader *program, GLuint fontTexture,
                          std::string text, float size, float spacing, bool fade) {
    static float fadingTime = 0.0f;
    float texture_size = 1.0 / 16.0f;
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
    
    program->bind();
    
    GLuint fadeUniform = glGetUniformLocation(program->getProgramID(), "fade");
    glVertexAttribPointer(program->getPositionAttrib(), 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->getPositionAttrib());
    glVertexAttribPointer(program->getTexCoordAttrib(), 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->getTexCoordAttrib());
    
    if (fade) {
        switch (fading) {
            case FADING_OUT:
            {
                if (fadingTime <= degreesToRadians(FADE)) {
                    glUniform1f(fadeUniform, cosf(fadingTime));
                    fadingTime += elapsed;
                } else {
                    glUniform1f(fadeUniform, cosf(degreesToRadians(FADE)));
                    fading = FADING_IN;
                }
            } break;
                
            case FADING_IN:
            {
                if (fadingTime >= 0.0f) {
                    glUniform1f(fadeUniform, cosf(fadingTime));
                    fadingTime -= elapsed;
                } else {
                    fading = FADING_OUT;
                }
            } break;
                
            default:
                break;
        }
        
        
    } else
        glUniform1f(fadeUniform, 1.0f);
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, (int) text.size() * 6);
    
    glDisableVertexAttribArray(program->getPositionAttrib());
    glDisableVertexAttribArray(program->getTexCoordAttrib());
    
    program->unbind();
}

float Platformer::easeInOut(float from, float to, float time) {
    float tVal;
    if(time > 0.5) {
        float oneMinusT = 1.0f-((0.5f-time)*-2.0f);
        tVal = 1.0f - ((oneMinusT * oneMinusT * oneMinusT * oneMinusT *
                        oneMinusT) * 0.5f);
    } else {
        time *= 2.0;
        tVal = (time*time*time*time*time)/2.0;
    }
    return (1.0f-tVal)*from + tVal*to;
}

float Platformer::lerp(float from, float to, float time) {
    return (1.0-time)*from + time*to;
}




