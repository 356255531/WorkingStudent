#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "bcm_host.h"
#include "graphics.h"

#define check() assert(glGetError() == 0)

uint32_t GScreenWidth;
uint32_t GScreenHeight;
EGLDisplay GDisplay;
EGLSurface GSurface;
EGLContext GContext;

// Define vertex shader
GfxShader GSimpleVS;

// Define fragment shader
GfxShader GSimpleFS;
GfxShader GYUVFS;
GfxShader GBlurFS;
GfxShader GSimpleSobelFS;
GfxShader GSobelFS;
GfxShader GWindowFS;
GfxShader GHarrisFS;
GfxShader GNonMaxSupFS;

GfxShader GPlusFS;
GfxShader GDiffFS;
GfxShader GMultiFS;

// Define GL Program
GfxProgram GSimpleProg;
GfxProgram GYUVProg;
GfxProgram GBlurProg;
GfxProgram GSimpleSobelProg;
GfxProgram GSobelProg;
GfxProgram GWindowProg;
GfxProgram GHarrisProg;
GfxProgram GNonMaxSupProg;

GfxProgram GPlusProg;
GfxProgram GDiffProg;
GfxProgram GMultiProg;

GLuint GQuadVertexBuffer;

void InitGraphics()
{
	bcm_host_init();
	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;

	static EGL_DISPMANX_WINDOW_T nativewindow;

	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	static const EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	static const EGLint context_attributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLConfig config;

	// get an EGL display connection
	GDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(GDisplay!=EGL_NO_DISPLAY);
	check();

	// initialize the EGL display connection
	result = eglInitialize(GDisplay, NULL, NULL);
	assert(EGL_FALSE != result);
	check();

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(GDisplay, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);
	check();

	// get an appropriate EGL frame buffer configuration
	result = eglBindAPI(EGL_OPENGL_ES_API);
	assert(EGL_FALSE != result);
	check();

	// create an EGL rendering context
	GContext = eglCreateContext(GDisplay, config, EGL_NO_CONTEXT, context_attributes);
	assert(GContext!=EGL_NO_CONTEXT);
	check();

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */, &GScreenWidth, &GScreenHeight);
	assert( success >= 0 );

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = GScreenWidth;
	dst_rect.height = GScreenHeight;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = GScreenWidth << 16;
	src_rect.height = GScreenHeight << 16;

	dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
	dispman_update = vc_dispmanx_update_start( 0 );

	dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
		0/*layer*/, &dst_rect, 0/*src*/,
		&src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

	nativewindow.element = dispman_element;
	nativewindow.width = GScreenWidth;
	nativewindow.height = GScreenHeight;
	vc_dispmanx_update_submit_sync( dispman_update );

	check();

	GSurface = eglCreateWindowSurface( GDisplay, config, &nativewindow, NULL );
	assert(GSurface != EGL_NO_SURFACE);
	check();

	// connect the context to the surface
	result = eglMakeCurrent(GDisplay, GSurface, GSurface, GContext);
	assert(EGL_FALSE != result);
	check();

	// Set background color and clear buffers
	glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT );

	// Load the vertex shader
	GSimpleVS.LoadVertexShader("VertexShader/simplevertshader.glsl");

	// Load the fragment shader
	GSimpleFS.LoadFragmentShader("FragmentShader/simplefragshader.glsl");
	GYUVFS.LoadFragmentShader("FragmentShader/yuvfragshader.glsl");
	GBlurFS.LoadFragmentShader("FragmentShader/blurfragshader.glsl");
	GSimpleSobelFS.LoadFragmentShader("FragmentShader/simplesobelfragshader.glsl");
	GSobelFS.LoadFragmentShader("FragmentShader/sobelfragshader.glsl");
	GWindowFS.LoadFragmentShader("FragmentShader/windowfragshader.glsl");
	GHarrisFS.LoadFragmentShader("FragmentShader/harrisfragshader.glsl");
	GNonMaxSupFS.LoadFragmentShader("FragmentShader/nonmaxsupfragshader_visual.glsl");

	GPlusFS.LoadFragmentShader("FragmentShader/plusfragshader.glsl");
	GDiffFS.LoadFragmentShader("FragmentShader/difffragshader.glsl");
	GMultiFS.LoadFragmentShader("FragmentShader/multifragshader.glsl");


	// Load the GL program
	GSimpleProg.Create(&GSimpleVS, &GSimpleFS);
	GYUVProg.Create(&GSimpleVS, &GYUVFS);
	GBlurProg.Create(&GSimpleVS, &GBlurFS);
	GSimpleSobelProg.Create(&GSimpleVS, &GSimpleSobelFS);
	GSobelProg.Create(&GSimpleVS, &GSobelFS);
	GWindowProg.Create(&GSimpleVS, &GWindowFS);
	GHarrisProg.Create(&GSimpleVS, &GHarrisFS);
	GNonMaxSupProg.Create(&GSimpleVS, &GNonMaxSupFS);

	GPlusProg.Create(&GSimpleVS, &GPlusFS);
	GDiffProg.Create(&GSimpleVS, &GDiffFS);
	GMultiProg.Create(&GSimpleVS, &GMultiFS);

	check();

	//create an ickle vertex buffer
	static const GLfloat quad_vertex_positions[] = {
		0.0f, 0.0f,	1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenBuffers(1, &GQuadVertexBuffer);
	check();
	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_positions), quad_vertex_positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	check();
}

void BeginFrame()
{
	// Prepare viewport
	glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	check();

	// Clear the background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	check();
}

void EndFrame()
{
	eglSwapBuffers(GDisplay, GSurface);
	check();
}

void ReleaseGraphics()
{

}

// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void printShaderInfoLog(GLint shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog : " << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}
}

bool GfxShader::LoadVertexShader(const char* filename)
{
	//cheeky bit of code to read the whole file into memory
	assert(!Src);
	FILE* f = fopen(filename, "rb");
	assert(f);
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	Src = new GLchar[sz + 1];
	fread(Src, 1, sz, f);
	Src[sz] = 0; //null terminate it!
	fclose(f);

	//now create and compile the shader
	GlShaderType = GL_VERTEX_SHADER;
	Id = glCreateShader(GlShaderType);
	glShaderSource(Id, 1, (const GLchar**)&Src, 0);
	glCompileShader(Id);
	check();

	//compilation check
	GLint compiled;
	glGetShaderiv(Id, GL_COMPILE_STATUS, &compiled);
	if (compiled == 0)
	{
		printf("Failed to compile vertex shader %s:\n%s\n", filename, Src);
		printShaderInfoLog(Id);
		glDeleteShader(Id);
		return false;
	}
	else
	{
		printf("Compiled vertex shader %s:\n%s\n", filename, Src);
	}

	return true;
}

bool GfxShader::LoadFragmentShader(const char* filename)
{
	//cheeky bit of code to read the whole file into memory
	assert(!Src);
	FILE* f = fopen(filename, "rb");
	assert(f);
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	Src = new GLchar[sz + 1];
	fread(Src, 1, sz, f);
	Src[sz] = 0; //null terminate it!
	fclose(f);

	//now create and compile the shader
	GlShaderType = GL_FRAGMENT_SHADER;
	Id = glCreateShader(GlShaderType);
	glShaderSource(Id, 1, (const GLchar**)&Src, 0);
	glCompileShader(Id);
	check();

	//compilation check
	GLint compiled;
	glGetShaderiv(Id, GL_COMPILE_STATUS, &compiled);
	if (compiled == 0)
	{
		printf("Failed to compile fragment shader %s:\n%s\n", filename, Src);
		printShaderInfoLog(Id);
		glDeleteShader(Id);
		return false;
	}
	else
	{
		printf("Compiled fragment shader %s:\n%s\n", filename, Src);
	}

	return true;
}

bool GfxProgram::Create(GfxShader* vertex_shader, GfxShader* fragment_shader)
{
	VertexShader = vertex_shader;
	FragmentShader = fragment_shader;
	Id = glCreateProgram();
	glAttachShader(Id, VertexShader->GetId());
	glAttachShader(Id, FragmentShader->GetId());
	glLinkProgram(Id);
	check();
	printf("Created program id %d from vs %d and fs %d\n", GetId(), VertexShader->GetId(), FragmentShader->GetId());

	// Prints the information log for a program object
	char log[1024];
	glGetProgramInfoLog(Id, sizeof log, NULL, log);
	printf("%d:program:\n%s\n", Id, log);

	return true;
}

void DrawYUVTextureRect(GfxTexture* ytexture, GfxTexture* utexture, GfxTexture* vtexture, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GYUVProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GYUVProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GYUVProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GYUVProg.GetId(), "tex0"), 0);
	glUniform1i(glGetUniformLocation(GYUVProg.GetId(), "tex1"), 1);
	glUniform1i(glGetUniformLocation(GYUVProg.GetId(), "tex2"), 2);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ytexture->GetId());	check();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, utexture->GetId());	check();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, vtexture->GetId());	check();
	glActiveTexture(GL_TEXTURE0);

	GLuint loc = glGetAttribLocation(GYUVProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawSimpleSobelRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GSimpleProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GSimpleProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GSimpleProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GSimpleProg.GetId(), "tex"), 0);
	glUniform2f(glGetUniformLocation(GSimpleProg.GetId(), "texelsize"), 1.f / texture->GetWidth(), 1.f / texture->GetHeight());
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());		check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);						check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); 			check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawSobelRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GSobelProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GSobelProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GSobelProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GSobelProg.GetId(), "tex"), 0);
	glUniform2f(glGetUniformLocation(GSobelProg.GetId(), "texelsize"), 1.f / texture->GetWidth(), 1.f / texture->GetHeight());
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());		check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);						check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); 			check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawBlurRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GBlurProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GBlurProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GBlurProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GBlurProg.GetId(), "tex"), 0);
	glUniform2f(glGetUniformLocation(GBlurProg.GetId(), "texelsize"), 1.f / texture->GetWidth(), 1.f / texture->GetHeight());
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());	check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawAdd3x3WindowRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target) {
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GWindowProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GWindowProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GWindowProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GWindowProg.GetId(), "tex"), 0);
	glUniform2f(glGetUniformLocation(GWindowProg.GetId(), "texelsize"), 1.f / texture->GetWidth(), 1.f / texture->GetHeight());
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());	check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawHarrisRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target) {
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GHarrisProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GHarrisProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GHarrisProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GHarrisProg.GetId(), "tex"), 0);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());	check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawNonMaxSupRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target, float threshold)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GNonMaxSupProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GNonMaxSupProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GNonMaxSupProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GNonMaxSupProg.GetId(), "tex"), 0);
	glUniform1f(glGetUniformLocation(GNonMaxSupProg.GetId(), "threshold"), threshold);
	glUniform2f(glGetUniformLocation(GNonMaxSupProg.GetId(), "texelsize"), 1.f / texture->GetWidth(), 1.f / texture->GetHeight());
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());		check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);						check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); 			check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}
void DrawTextureRect(GfxTexture* texture, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GSimpleProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GSimpleProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GSimpleProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GSimpleProg.GetId(), "tex"), 0);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glBindTexture(GL_TEXTURE_2D, texture->GetId());	check();

	GLuint loc = glGetAttribLocation(GSimpleProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawTexturePlusRect(GfxTexture* tex_1, GfxTexture* tex_2, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GPlusProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GPlusProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GPlusProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GPlusProg.GetId(), "tex0"), 0);
	glUniform1i(glGetUniformLocation(GPlusProg.GetId(), "tex1"), 1);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_1->GetId());	check();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_2->GetId());	check();
	glActiveTexture(GL_TEXTURE0);

	GLuint loc = glGetAttribLocation(GPlusProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawTextureDiffRect(GfxTexture* tex_1, GfxTexture* tex_2, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GDiffProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GDiffProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GDiffProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GDiffProg.GetId(), "tex0"), 0);
	glUniform1i(glGetUniformLocation(GDiffProg.GetId(), "tex1"), 1);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_1->GetId());	check();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_2->GetId());	check();
	glActiveTexture(GL_TEXTURE0);

	GLuint loc = glGetAttribLocation(GDiffProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

void DrawTextureMultiRect(GfxTexture* tex_1, GfxTexture* tex_2, float x0, float y0, float x1, float y1, GfxTexture* render_target)
{
	if (render_target)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, render_target->GetFramebufferId());
		glViewport ( 0, 0, render_target->GetWidth(), render_target->GetHeight() );
		check();
	}

	glUseProgram(GMultiProg.GetId());	check();

	glUniform2f(glGetUniformLocation(GMultiProg.GetId(), "offset"), x0, y0);
	glUniform2f(glGetUniformLocation(GMultiProg.GetId(), "scale"), x1 - x0, y1 - y0);
	glUniform1i(glGetUniformLocation(GMultiProg.GetId(), "tex0"), 0);
	glUniform1i(glGetUniformLocation(GMultiProg.GetId(), "tex1"), 1);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);	check();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_1->GetId());	check();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_2->GetId());	check();
	glActiveTexture(GL_TEXTURE0);

	GLuint loc = glGetAttribLocation(GMultiProg.GetId(), "vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);	check();
	glEnableVertexAttribArray(loc);	check();
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 ); check();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (render_target)
	{
		//glFinish();	check();
		//glFlush(); check();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport ( 0, 0, GScreenWidth, GScreenHeight );
	}
}

bool GfxTexture::CreateRGBA(int width, int height, const void* data)
{
	Width = width;
	Height = height;
	glGenTextures(1, &Id);
	check();
	glBindTexture(GL_TEXTURE_2D, Id);
	check();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	check();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);
	check();
	glBindTexture(GL_TEXTURE_2D, 0);
	IsRGBA = true;
	return true;
}

bool GfxTexture::CreateGreyScale(int width, int height, const void* data)
{
	Width = width;
	Height = height;
	glGenTextures(1, &Id);
	check();
	glBindTexture(GL_TEXTURE_2D, Id);
	check();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, Width, Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
	check();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);
	check();
	glBindTexture(GL_TEXTURE_2D, 0);
	IsRGBA = false;
	return true;
}

bool GfxTexture::GenerateFrameBuffer()
{
	//Create a frame buffer that points to this texture
	glGenFramebuffers(1, &FramebufferId);
	check();
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId);
	check();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Id, 0);
	check();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	check();
	return true;
}

void GfxTexture::SetPixels(const void* data)
{
	glBindTexture(GL_TEXTURE_2D, Id);
	check();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, IsRGBA ? GL_RGBA : GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
	check();
	glBindTexture(GL_TEXTURE_2D, 0);
	check();
}

void SaveFrameBuffer(const char* fname)
{
	void* image = malloc(GScreenWidth*GScreenHeight*4);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	check();
	glReadPixels(0,0,GScreenWidth,GScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, image);

	unsigned error = lodepng::encode(fname, (const unsigned char*)image, GScreenWidth, GScreenHeight, LCT_RGBA);
	if(error) 
		printf("error: %d\n",error);

	free(image);

}

void GfxTexture::Save(const char* fname)
{
	void* image = malloc(Width*Height*4);
	glBindFramebuffer(GL_FRAMEBUFFER,FramebufferId);
	check();
	glReadPixels(0,0,Width,Height,IsRGBA ? GL_RGBA : GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
	check();
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	unsigned error = lodepng::encode(fname, (const unsigned char*)image, Width, Height, IsRGBA ? LCT_RGBA : LCT_GREY);
	if(error) 
		printf("error: %d\n",error);

	free(image);
}

void* GfxTexture::ret_img() {
	void* image = malloc(Width * Height * 4);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId);
	check();
	glReadPixels(0, 0, Width, Height, IsRGBA ? GL_RGBA : GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
	check();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return image;
}

Image::Image() {}

Image::~Image() {
	free(this->img_ptr);
}

void Image::load_img(GfxTexture* tex) {
	this->img_ptr = tex->ret_img();
}

void Image::reload_img(void* new_img_ptr) {
	this->img_ptr = new_img_ptr;
}

void* Image::get_img_ptr() {
	return img_ptr;
}