#include "../includes/sdl_window.hpp"
#include "../includes/sdl_app.hpp"
#include "../includes/ShaderProgram.hpp"
#include "../includes/Matrix.hpp"
#include "../includes/vec.hpp"
#include <SDL2_image/SDL_image.h>
#include <vector>

using namespace graphics;
using namespace graphics::vec;
using namespace std;

#define SHADER "./shaders/"

struct Vertex {
    Vertex(const vec2& pos) : position(pos) {}
    vec2 position;
};

class Game : public App {
private:

    // Vector for vertex positions
    vector<Vertex> positions;

    // Vector for texture coord
    vector<Vertex> texCoord;

    // MVP Matrix
    Matrix model;
    Matrix view;
    Matrix projection;

    // Shader program
    ShaderProgram* sp = nullptr;

    void init() {
        // Set the positions for the triangle
        positions.push_back(Vertex(vec2(0.5f, -0.5f)));
        positions.push_back(Vertex(vec2(0.0f, 0.5f)));
        positions.push_back(Vertex(vec2(-0.5f, -0.5f)));

        // Create, compile and link the shdaers
        sp = new ShaderProgram(SHADER"vertex.glsl", SHADER"fragment.glsl");

        // Set the Orthographic projection matrix
        projection.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

        glUseProgram(sp->programID);
    }

public:

    Game(Uint32 flags, const char* name = "demo", int w = 1280, int h = 800)
        : App(flags, name, w, h) { init(); }

    ~Game() { delete sp; }

    void onDraw() {
        // Set the matrices
        sp->setModelMatrix(model);
        sp->setViewMatrix(view);
        sp->setProjectionMatrix(projection);

        glVertexAttribPointer(sp->positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, &positions[0]);
        glEnableVertexAttribArray(sp->positionAttribute);

            // Draw the triangles
            glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(sp->positionAttribute);
    }

};

int main(int argc, char* argv[]) {

    Game a = Game(SDL_INIT_EVERYTHING, "Game Demo");
    a.start();

    return 0;
}
