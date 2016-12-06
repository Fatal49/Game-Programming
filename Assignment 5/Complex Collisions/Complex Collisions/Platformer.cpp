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
    
    if (rec1)
        delete rec1;
    
    if (rec2)
        delete rec2;
      
    SDL_Quit();
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
    glLineWidth(5.0f);
    
    // Load, compile & link the shaders
    shader = new Shader("vertex.glsl", "fragment.glsl");
    
    // Set matrices & orthographic projection
    projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    shader->setModelMatrix(model);
    shader->setViewMatrix(view);
    shader->setProjectionMatrix(projection);
    
    // Set position of first rectangle
    rec1 = new Rectangle(0.25f, 0.5f);
    rec1->create();
    rec1->translate(-2.0f, 0.5f);
    rec1->scale(1.5f, 1.5f);
    rec1->setVelocity(vec::vec2(0.3f, -0.1f));
    
    // Set position of second rectangle
    rec2 = new Rectangle(0.5f, 0.5f);
    rec2->create();
    rec2->translate(2.0f, -0.5f);
    rec2->scale(1.5f, 1.5f);
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
    rec1->draw(shader);
    rec2->draw(shader);
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
            rec2->rotate(sinf(FIXED_TIMESTEP));
            
            rec1->update(FIXED_TIMESTEP);
            rec2->update(FIXED_TIMESTEP);
            
            if (rec1->checkCollision(rec2->getPoints())) {
                rec1->setVelocity(vec::vec2(-0.3f, -0.1f));
                printf("There Colliding  1 :)\n");
            }
        }
    }
    
    if (!pause) {
        rec2->rotate(sinf(fixedElapsed));
        
        rec1->update(fixedElapsed);
        rec2->update(fixedElapsed);
        
        if (rec1->checkCollision(rec2->getPoints())) {
            rec1->setVelocity(vec::vec2(-0.3f, -0.1f));
            printf("There Colliding  2 :)\n");
        }
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
                    if (pause)
                        pause = false;
                    else
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




