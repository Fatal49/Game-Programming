#include "Platformer.hpp"

Platformer::Platformer(float w, float h, const char* name): Game(w, h, name) {
    if (!createWindow()) {
        printf("The SDL window could not be created. Call again.\n");
    } else {
        printf("SDL window is created\n");
    }
}

Platformer::Platformer(const char* name) : Game(name) {
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
    
    if (tile)
        delete tile;
      
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

bool Platformer::processEvents() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // Close the window on any SDL_QUIT event
            case SDL_QUIT:
                printf("SDL window is closing\n");
                return false;
                break;
            
            // Handle Keyboard input related to the window
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        printf("SDL window is closing\n");
                        return false;
                        break;
                    default:
                        break;
                }
                break;
                
            default:
                break;
        }
    }
    
    return true;
}

void Platformer::setup() {
    // Set the viewport
    setViewPort();
    
    // Enable alpha blending & depth
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Load, compile & link the shaders
    shader = new Shader("vertex_textured.glsl", "fragment_textured.glsl");
    
    // Set matrices & orthographic projection
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    model.Translate(-3.55f, 2.0f, 0.0f);
    shader->setModelMatrix(model);
    shader->setViewMatrix(view);
    shader->setProjectionMatrix(projection);
    
    // Create the map
//    int w = int(getWidth() / TILE_WIDTH);
//    int h = int(getHeight() / TILE_HEIGHT);
//    float TS_WIDTH = 7.1f / w;
//    float TS_HEIGHT = 4.0f / h;
//    
//    for (size_t y = 0; y < h; y++) {
//        for (size_t x = 0; x < w; x++) {
//            if (x != 31) {
//                vertices.insert(vertices.end(), {
//                    x * TS_WIDTH, y * -TS_HEIGHT,                               // Top Left
//                    x * TS_WIDTH, (y * -TS_HEIGHT) - TS_HEIGHT,                 // Bottom Left
//                    (x * TS_WIDTH) + TS_WIDTH, y * -TS_HEIGHT,                  // Top Right
//                    x * TS_WIDTH, (y * -TS_HEIGHT) - TS_HEIGHT,                 // Bottom Left
//                    (x * TS_WIDTH) + TS_WIDTH, (y * -TS_HEIGHT) - TS_HEIGHT,    // Bottom Right
//                    (x * TS_WIDTH) + TS_WIDTH, y * -TS_HEIGHT,                  // Top Right
//                });
//
//            }
//        }
//    }
    
//    vertices.push_back(-0.5f);                // Top Left
//    vertices.push_back(0.5f);
//    
//    vertices.push_back(-0.5f);                // Bottom Left
//    vertices.push_back(-0.5f);
//    
//    vertices.push_back(0.5f);                 // Top Right
//    vertices.push_back(0.5f);
//    
//    vertices.push_back(-0.5f);                // Bottom Left
//    vertices.push_back(-0.5f);
//    
//    vertices.push_back(0.5f);                 // Bottom Right
//    vertices.push_back(-0.5f);
//    
//    vertices.push_back(0.5f);                 // Top Right
//    vertices.push_back(0.5f);
    
    
    
//    vertices.push_back(0.0f);
//    vertices.push_back(0.0f);
//    
//    vertices.push_back(0.0f);
//    vertices.push_back(-1.0f / 12.0f);
//    
//    vertices.push_back(2.0f / 21.0f);
//    vertices.push_back(0.0f);
//    
//    vertices.push_back(0.0f);
//    vertices.push_back(-1.0f / 12.0f);
//    
//    vertices.push_back(2.0f / 21.0f);
//    vertices.push_back(-1.0f / 12.0f);
//    
//    vertices.push_back(2.0f / 21.0f);
//    vertices.push_back(0.0f);
    
    // Texture coordinates
//    for (size_t i = 0; i < w * h; i++) {
//        texCoords.insert(texCoords.end(), {
//            0, 0,
//            0, 1,
//            1, 0,
//            0, 1,
//            1, 1,
//            1, 0
//        });
//    }
    
//    float texWidth = 1.0f / SPRITE_COUNT_X;
//    float texHeight = 1.0f / SPRITE_COUNT_Y;
//    int n = 165;
//    int x = (n % 24)
//    int y = 1;
//    
//    for (size_t i = 0; i < w * h; i++) {
//        texCoords.insert(texCoords.end(), {
//            x * texWidth, y * texHeight,                                    // Top Left
//            x * texWidth, (y * texHeight) + texHeight,                      // Bottom Left
//            (x * texWidth) + texWidth, y * texHeight,                       // Top Right
//            x * texWidth, (y * texHeight) + texHeight,                      // Bottom Left
//            (x * texWidth) + texWidth, (y * texHeight) + texHeight,         // Bottom Right
//            (x * texWidth) + texWidth, y * texHeight,                       // Top Right
//        });
//    }
    
    
//    texCoords[0] = 0;
//    texCoords[1] = 0;
//    
//    texCoords[2] = 0;
//    texCoords[3] = 1;
//    
//    texCoords[4] = 1;
//    texCoords[5] = 0;
//    
//    texCoords[6] = 0;
//    texCoords[7] = 1;
//    
//    texCoords[8] = 1;
//    texCoords[9] = 1;
//    
//    texCoords[10] = 1;
//    texCoords[11] = 0;
    
    // Load the textures
//    tex1 = LoadTexture("assets/grassCenterBlock.png");
//    tex1 = LoadTexture("assets/dirt-tiles.png");
    
    // Create the Tile
    tile = new Tile(getWidth(), getHeight(), 16.0f, 16.0f, 7.1f, 4.0f);
    tile->readFlareMap("assets/level.txt");
    tile->loadTexture("assets/dirt-tiles.png");
    tile->useMapData(24.0f, 16.0f);    
}

void Platformer::render() {
    // Set the color for the viewport    
    glClearColor(122.0f / 256.0f, 224.0f / 256.0f, 241.0f / 255.0f, 1.0f);
    
    // Clear the buffer with preset values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Call draw function
    draw();
    
    // Swap buffers
    swapBuffers();
}

void Platformer::draw() {
//    glBindTexture(GL_TEXTURE_2D, tex1);
//    
//    // Draw sprite
//    glVertexAttribPointer(shader->getPositionAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
//    glEnableVertexAttribArray(shader->getPositionAttrib());
//    glVertexAttribPointer(shader->getTexCoordAttrib(), 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
//    glEnableVertexAttribArray(shader->getTexCoordAttrib());
//    
//    glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
//    
//    glDisableVertexAttribArray(shader->getPositionAttrib());
//    glDisableVertexAttribArray(shader->getTexCoordAttrib());
    
    tile->draw(shader);
}

void Platformer::update() {
    // For annimation
    float ticks = (float) SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    angle += elapsed;
    float fixedElapsed = elapsed;
    
    // Update every 60th of a second
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    
    while (fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
    }
    

    
    // Handle input
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_RIGHT:
                    

                    break;
                default:
                    break;
            }
            break;
            
        case SDL_KEYUP:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_SPACE:

                    break;
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
}













