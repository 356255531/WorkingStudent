#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "lodepng.h"

void InitGraphics();
void ReleaseGraphics();
void BeginFrame();
void EndFrame();

class GfxShader
{
    GLchar* Src;
    GLuint Id;
    GLuint GlShaderType;

public:

    GfxShader() : Src(NULL), Id(0), GlShaderType(0) {}
    ~GfxShader() { if(Src) delete[] Src; }

    bool LoadVertexShader(const char* filename);
    bool LoadFragmentShader(const char* filename);
    GLuint GetId() { return Id; }
};

class GfxProgram
{
    GfxShader* VertexShader;
    GfxShader* FragmentShader;
    GLuint Id;

public:

    GfxProgram() {}
    ~GfxProgram() {}

    bool Create(GfxShader* vertex_shader, GfxShader* fragment_shader);
    GLuint GetId() { return Id; }
};

class GfxTexture
{
    int Width;
    int Height;
    GLuint Id;
    bool IsRGBA;

    GLuint FramebufferId;
public:

    GfxTexture() : Width(0), Height(0), Id(0), FramebufferId(0) {}
    ~GfxTexture() {}

    bool CreateRGBA(int width, int height, const void* data = NULL);
    bool CreateGreyScale(int width, int height, const void* data = NULL);
    bool GenerateFrameBuffer();
    void SetPixels(const void* data);
    GLuint GetId() { return Id; }
    GLuint GetFramebufferId() { return FramebufferId; }
    int GetWidth() {return Width;}
    int GetHeight() {return Height;}
    void Save(const char* fname);
    void* ret_img();
};

void SaveFrameBuffer(const char* fname);

void DrawTextureRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawYUVTextureRect(
    GfxTexture* ytexture,
    GfxTexture* utexture,
    GfxTexture* vtexture,
    float x0, 
    float y0,
    float x1, 
    float y1,
    GfxTexture* render_target
);
void DrawSobelRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawBlurRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
//New created draw fnction
void DrawAdd3x3WindowRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawHarrisRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawNonMaxSupRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target,
    float threshold=0.06
);


// KLT algorithm
void DrawTexturePlusDiffRect(    
    GfxTexture* texture_1,
    GfxTexture* texture_2,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawSimpleSobelRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawGradientRect(
    GfxTexture* texture,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
void DrawTextureMultiRect(    
    GfxTexture* texture_1,
    GfxTexture* texture_2,
    float x0,
    float y0,
    float x1,
    float y1,
    GfxTexture* render_target
);
#endif
