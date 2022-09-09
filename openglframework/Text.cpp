#include "Text.h"
#include <iostream>
#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Global.h"
#include FT_FREETYPE_H

using namespace std;
using namespace Global;

map<char, Text::Character > Text::characters;
GLuint Text::vao;
GLuint Text::vbo;

int Text::initClass() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/BowlbyOneSC-Regular.ttf", 0, &face)) {
        return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 200);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    return 0;
}

void Text::renderText(std::string text, glm::vec2 position, float scale, glm::vec3 color) {
    shader.setUniformMatrix4f("modelview", glm::mat4(1.f));
    shader.setUniformInt("textRendering", 1);
    shader.setUniform3f("textColor", color.r, color.g, color.b);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character character = characters[*c];

        float xpos = position.x + character.bearing.x * scale;
        float ypos = position.y - (character.size.y - character.bearing.y) * scale;
        float w = character.size.x * scale;
        float h = character.size.y * scale;

        float vertices[24] = {
            xpos,     ypos + h,   0.0f, 1.0f,
            xpos,     ypos,       0.0f, 0.0f,
            xpos + w, ypos,       1.0f, 0.0f,

            xpos,     ypos + h,   0.0f, 1.0f,
            xpos + w, ypos,       1.0f, 0.0f,
            xpos + w, ypos + h,   1.0f, 1.0f
        };

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, character.textureId);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(shader.bindVertexAttribute("position", 2, 4 * sizeof(float), 0));
        glEnableVertexAttribArray(shader.bindVertexAttribute("textureCoordinate", 2, 4 * sizeof(float), (void*)(2 * sizeof(float))));

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_TEXTURE_2D);

        position.x += (character.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    shader.setUniformInt("textRendering", 0);
}

glm::vec2 Text::getTextSize(string text) {
    glm::vec2 size(0.f);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character character = characters[*c];
        size.x += character.bearing.x + (character.advance >> 6);
        if (size.y < character.size.y) {
            size.y = character.size.y;
        }
    }

    return size;
}