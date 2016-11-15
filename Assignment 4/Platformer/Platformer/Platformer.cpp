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
    
    if (ss)
        delete ss;
    
    SDL_Quit();
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
    
    // Load Sprite sheet
    spriteSheetID = LoadTexture("sprites/sprites.png");
    
    // Get entities from sprite sheet
    ss = new SpriteSheet("sprites/sprites.xml", 128, 64);
    ss->extractSpriteInfo();
    ss->fillEntities(entities, spriteSheetID, 0.1);

    // Setup mario
    entities[1].translate(0.3f, 0.1f);
    entities[1].mass = 1.0f;
    entities[1].velocity_x = 1.0f;
    entities[1].velocity_y = 0.6f;
    entities[1].acceleration_x = 20.0f;
    entities[1].setUpCollBox();
}

void Platformer::render() {
    // Set the color for the viewport
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Clear the buffer with preset values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Call draw function
    draw();
    
    // Swap buffers
    swapBuffers();
}

void Platformer::draw() {
    entities[1].render(shader);
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
        entities[1].update(FIXED_TIMESTEP);
        
        // Poll events
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        
        if (keys[SDL_SCANCODE_RIGHT]) {
            entities[1].moveRight(FIXED_TIMESTEP);
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            entities[1].moveLeft(FIXED_TIMESTEP);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            entities[1].moveDown(FIXED_TIMESTEP);
        }
        if (keys[SDL_SCANCODE_UP]) {
            entities[1].moveUp(FIXED_TIMESTEP);
        }
        if (keys[SDL_SCANCODE_SPACE]) {
            entities[1].jump(FIXED_TIMESTEP);
        }
    }
}













