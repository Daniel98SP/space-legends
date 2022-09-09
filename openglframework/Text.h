#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <map>
#include <string>

using namespace std;

class Text {
public:
    static int initClass();
    static void renderText(string text, glm::vec2 position, float scale, glm::vec3 color);
    static glm::vec2 getTextSize(string text);

    struct Character {
        unsigned int textureId;  // ID handle of the glyph texture
        glm::ivec2   size;       // Size of glyph
        glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
        unsigned int advance;    // Offset to advance to next glyph
    };

    static map<char, Character> characters;

private:
    static GLuint vao;
    static GLuint vbo;
};
