#include "Platformer.hpp"

Platformer::Platformer(float w, float h, const char* name): Game(w, h, name) {
    if (!createWindow()) {
        printf("The SDL window could not be created. Call again.\n");
    } else {
        printf("SDL window is created\n");
        init();
    }
}

Platformer::Platformer(const char* name) : Game(name) {
    if (!createWindow()) {
        printf("The SDL window could not be created. Call again.\n");
    } else {
        printf("SDL window is created\n");
        init();
    }
}

Platformer::~Platformer() {
    if (shader) {
        printf("Freeing shader\n");
        delete shader;
    }
    
    if (ts)
        delete ts;
    
    if (t)
        delete t;
    
    SDL_Quit();
}

void Platformer::init() {
    // Static entities
    for (int i = 0; i < 277; i++) {
        if (i == 39 || i == 85 || i == 129 || i == 134 || i == 141 || i == 161)
            continue;
        staticEntities.push_back(i);
    }
}

GLuint Platformer::LoadTexture(const char *image_path) {
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

int Platformer::findEntity(int n) const {
    for (int i = 0; i < tiles.size(); i++)
        if (tiles[i].num == n)
            return i;
    
    return -1;
}

void Platformer::setStaticEntities() {
    for (int i = 0; i < staticEntities.size(); i++)
        tiles[staticEntities[i]].e.isStatic = true;
}

void Platformer::setup() {
    // Set the viewport
    setViewPort();
    
    // Enable alpha blending & depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Load, compile & link the shaders
    shader = new Shader("vertex_textured.glsl", "fragment_textured.glsl");
    
    // Set matrices & orthographic projection
    shader->setModelMatrix(model);
    shader->setViewMatrix(view);
    shader->setProjectionMatrix(projection);
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    textureID = LoadTexture("sprites/dirt-tiles.png");
    t = new Tiles(0.04f, 24, 16, 95, 20, textureID);
    t->readInLevel("sprites/level2.txt", map, entities);
    t->init(map);
    
    projection.Translate(0.0f, -245.0f, 0.0f);
//    model.Translate(-34.0f, -20.0f, 0.0f);
//    view.Scale(9.5f, 9.5f, 1.0f);
    shader->setProjectionMatrix(projection);
    // (1.0f / 24.0f) * (1.0f / 16.0f)
    
//    SPRITE_COUNT_X = 24.0f;
//    SPRITE_COUNT_Y = 16.0f;
//    TILE_WIDTH = 1.0f / SPRITE_COUNT_X;
//    TILE_HEIGHT = 1.0f / SPRITE_COUNT_Y;
//    
//    ts = new TileSheet("sprites/dirt-tiles.png", 384.0f, 256.0f, 16.0f, 16.0f, 16, 24, 0.04f);
//    ts->loadTiles(tiles);
//    ts->getTileIndex("sprites/dirt-tiles_map.csv", tiles);
//    ts->buildMap("sprites/level.txt", map, tiles, entities);
//    
//    setStaticEntities();
    
//    for (int y = 0; y < map.size(); y++) {
//        for (int x = 0; x < map[0].size(); x++) {
//        }
//    }
}

void Platformer::render() {
    // Set the color for the viewport
    // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Clear the buffer with preset values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Call draw function
    draw();
    
    // Swap buffers
    swapBuffers();
}

void Platformer::draw() {
//    for (size_t y = 0; y < map.size(); y++) {
//        for (size_t x = 0; x < map[0].size(); x++) {
//            if (map[y][x] != 0) {
//                printf("I am being drawn -> %u\n", map[y][x]);
//                t->draw(shader, map[y][x]);
//            }
//        }
//    }
    t->draw(shader);
    
//    for (size_t y = 0; y < 2; y++) {
//        for (size_t x = 0; x < 2; x++) {
//            if (map[y][x] != 1000) {
//                vec::vec2 pos = vec::vec2(-1 + (x * TILE_WIDTH), 1 - (y * TILE_HEIGHT));
//                tiles[map[y][x]].e.position = pos;
//                tiles[map[y][x]].e.render(shader);
//            }
//        }
//    }
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
//        tiles[DRAW_E].e.update(FIXED_TIMESTEP);
    }
    
//    tiles[DRAW_E].e.update(fixedElapsed);
    
    // Handle input
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_RIGHT:
                    printf("The right key was pressed       :)\n");
//                    tiles[DRAW_E].e.moveRight(fixedElapsed);
                    break;
                default:
                    break;
            }
            break;
            
        case SDL_KEYUP:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_SPACE:
//                    tiles[DRAW_E].e.jump(fixedElapsed);
                    break;
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
}













