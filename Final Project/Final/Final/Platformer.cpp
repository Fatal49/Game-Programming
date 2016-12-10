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
    
    if (player1)
        delete player1;
    
    if (player2)
        delete player2;
    
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
    shader = new Shader("vertex.glsl", "fragment.glsl");
    
    // Set matrices & orthographic projection
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    shader->setModelMatrix(model);
    shader->setViewMatrix(view);
    shader->setProjectionMatrix(projection);
    
    // Setup player 1
    player1 = new Rectangle(0.5f, 0.5f);
    player1->create();
    player1->translate(-1.777, 0.0f);
    player1->scale(1.5f, 1.5f);
    
    // Setup player 2
    player2 = new Rectangle(0.5f, 0.5f);
    player2->create();
    player2->translate(1.777f, 0.0f);
    player2->scale(1.5f, 1.5f);
}

void Platformer::render() {
    // Set the color for the viewport
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);       // Light black color
    
    // Clear the buffer with preset values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Call draw function
    draw();
    
    // Swap buffers
    swapBuffers();
}

void Platformer::draw() {
    player1->draw(shader);
    player2->draw(shader);
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
        
        if (!pause) {
            player1->update(FIXED_TIMESTEP);
            player2->update(FIXED_TIMESTEP);
        }
    }
    
    if (!pause) {
        player1->update(fixedElapsed);
        player2->update(fixedElapsed);
    }
    
    // Handle input
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
    
    
    // Player 1 movement
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
    SDL_PollEvent(&e);
    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {

                default:
                    break;
            }
            break;
            
        case SDL_KEYUP:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_SPACE:
                    pause = true;
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
}






