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
    
    if (rec3)
        delete rec3;
      
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
    rec1->velocity = vec::vec2(0.5f, -0.1f);
    
    // Set position of second rectangle
    rec2 = new Rectangle(0.5f, 0.5f);
    rec2->create();
    rec2->translate(2.0f, -0.5f);
    rec2->scale(1.5f, 1.5f);
    rec2->velocity = vec::vec2(0.0f, 0.2f);
    
    // Set position of third rectangle
    rec3 = new Rectangle(0.8f, 0.6f);
    rec3->create();
    rec3->translate(0.0f, 1.5f);
    rec3->scale(1.1f, 1.1f);
    rec3->velocity = vec::vec2(0.0f, -0.3f);
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
    rec3->draw(shader);
}

void Platformer::update() {
    float ticks = (float) SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    float fixedElapsed = elapsed;
    
    // Update every 60th of a second
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    
    while (fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        
        if (!pause) {
            rec2->rotate(FIXED_TIMESTEP);
            
            rec1->update(FIXED_TIMESTEP);
            rec2->update(FIXED_TIMESTEP);
            rec3->update(FIXED_TIMESTEP);
        }
    }
    
    if (!pause) {
        rec2->rotate(fixedElapsed);
        
        rec1->update(fixedElapsed);
        rec2->update(fixedElapsed);
        rec3->update(fixedElapsed);
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

void Platformer::checkCollisions() {
    std::vector<vec::vec2> rec1Points = rec1->getPoints();
    std::vector<vec::vec2> rec2Points = rec2->getPoints();
    std::vector<vec::vec2> rec3Points = rec3->getPoints();
    int maxChecks = 10;
    vec::vec2 n;
    
    while (checkSATCollision(rec1Points, rec2Points) && maxChecks > 0) {
        vec::vec2 p1 = rec1->getCenter();
        vec::vec2 p2 = rec2->getCenter();
        
        n = vec::vec2(p1.x - p2.x, p1.y - p2.y);
        n = vec::normalize(n);
        
        vec::vec2 v = rec1->velocity - (2 * (vec::dot(rec1->velocity, n)) * n);
        if (v.x < 0 || v.y < 0) {
            v.x = fabsf(v.x);
            v.y = fabsf(v.y);
        }
        
        rec1->velocity = v;
        rec2->velocity = (v *= -1);
        
        maxChecks -= 1;
    }
    maxChecks = 10;
    
    while (checkSATCollision(rec1Points, rec3Points) && maxChecks > 0) {
        vec::vec2 p1 = rec1->getCenter();
        vec::vec2 p2 = rec3->getCenter();
        
        n = vec::vec2(p1.x - p2.x, p1.y - p2.y);
        n = vec::normalize(n);
        
        vec::vec2 v = rec1->velocity - (2 * (vec::dot(rec1->velocity, n)) * n);
        if (v.x < 0 && v.y < 0) {
            v.x = fabsf(v.x);
            v.y = fabsf(v.y);
        }
        
        rec1->velocity = v;
        rec3->velocity = (v *= -1);
        
        maxChecks -= 1;
    }
    maxChecks = 10;
    
    while (checkSATCollision(rec2Points, rec3Points) && maxChecks > 0) {
        vec::vec2 p1 = rec2->getCenter();
        vec::vec2 p2 = rec3->getCenter();
        
        n = vec::vec2(p1.x - p2.x, p1.y - p2.y);
        n = vec::normalize(n);
        
        vec::vec2 v = rec2->velocity - (2 * (vec::dot(rec2->velocity, n)) * n);
        if (v.x < 0 && v.y < 0) {
            v.x = fabsf(v.x);
            v.y = fabsf(v.y);
        }
        
        rec2->velocity = v;
        rec3->velocity = (v *= -1);
        
        maxChecks -= 1;
    }
    maxChecks = 10;
}

void Platformer::checkCollisionWithWall(Rectangle* rec, float opengl_x, float opengl_y) {
    std::vector<vec::vec2> points = rec->getPoints();
    float maxX = opengl_x * 0.5f;
    float maxY = opengl_y * 0.5f;
    bool touching = false;
    
    for (size_t i = 0; i < points.size(); i++) {
        if (fabsf(points[i].x) >= maxX) {
            touching =  true;
            break;
        }
        if (fabsf(points[i].y) >= maxY) {
            touching = true;
            break;
        }
    }
    
    if (touching) {
        
    }
}

bool Platformer::testSATSeparationForEdge(float edgeX, float edgeY,
                                         const std::vector<vec::vec2> &points1,
                                         const std::vector<vec::vec2> &points2) {
    float normalX = -edgeY;
    float normalY = edgeX;
    float len = sqrtf(normalX*normalX + normalY*normalY);
    normalX /= len;
    normalY /= len;
    
    std::vector<float> e1Projected;
    std::vector<float> e2Projected;
    
    for(int i=0; i < points1.size(); i++) {
        e1Projected.push_back(points1[i].x * normalX + points1[i].y * normalY);
    }
    for(int i=0; i < points2.size(); i++) {
        e2Projected.push_back(points2[i].x * normalX + points2[i].y * normalY);
    }
    
    std::sort(e1Projected.begin(), e1Projected.end());
    std::sort(e2Projected.begin(), e2Projected.end());
    
    float e1Min = e1Projected[0];
    float e1Max = e1Projected[e1Projected.size()-1];
    float e2Min = e2Projected[0];
    float e2Max = e2Projected[e2Projected.size()-1];
    float e1Width = fabs(e1Max-e1Min);
    float e2Width = fabs(e2Max-e2Min);
    float e1Center = e1Min + (e1Width/2.0);
    float e2Center = e2Min + (e2Width/2.0);
    float dist = fabs(e1Center-e2Center);
    float p = dist - ((e1Width+e2Width)/2.0);
    
    if(p < 0) {
        return true;
    }
    return false;
}

bool Platformer::checkSATCollision(const std::vector<vec::vec2> &e1Points,
                                  const std::vector<vec::vec2> &e2Points) {
    for(int i=0; i < e1Points.size(); i++) {
        float edgeX, edgeY;
        
        if(i == e1Points.size()-1) {
            edgeX = e1Points[0].x - e1Points[i].x;
            edgeY = e1Points[0].y - e1Points[i].y;
        } else {
            edgeX = e1Points[i+1].x - e1Points[i].x;
            edgeY = e1Points[i+1].y - e1Points[i].y;
        }
        
        bool result = testSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points);
        if(!result) {
            return false;
        }
    }
    for(int i=0; i < e2Points.size(); i++) {
        float edgeX, edgeY;
        
        if(i == e2Points.size()-1) {
            edgeX = e2Points[0].x - e2Points[i].x;
            edgeY = e2Points[0].y - e2Points[i].y;
        } else {
            edgeX = e2Points[i+1].x - e2Points[i].x;
            edgeY = e2Points[i+1].y - e2Points[i].y;
        }
        bool result = testSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points);
        if(!result) {
            return false;
        }
    }
    
    printf("I am here :)\n");
    return true;
}

