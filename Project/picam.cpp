#include <stdio.h>
#include <unistd.h>
#include "camera.h"
#include "graphics.h"
#include <time.h>
#include <curses.h>

#define MAIN_TEXTURE_WIDTH 768
#define MAIN_TEXTURE_HEIGHT 512

#define TEXTURE_GRID_COLS 4
#define TEXTURE_GRID_ROWS 4

//entry point
int main(int argc, const char **argv)
{
	//init graphics and the camera
	InitGraphics();
	CCamera* cam = StartCamera(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT, 15, false);

	//create 2 textures, one for data and another for result
	GfxTexture ytexture, greysobeltexture;

	ytexture.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);

	greysobeltexture.CreateRGBA(MAIN_TEXTURE_WIDTH / 2, MAIN_TEXTURE_HEIGHT / 2);
	greysobeltexture.GenerateFrameBuffer();

	printf("Running frame loop\n");

	for (int i = 0; i < 3000; i++)
	{
		//spin until we have a camera frame
		const void* frame_data; int frame_sz;
		while (!cam->BeginReadFrame(frame_data, frame_sz)) {};

		//lock the chosen frame buffer, and copy it directly into the corresponding open gl texture
		{
			const uint8_t* data = (const uint8_t*)frame_data;
			ytexture.SetPixels(data);
			cam->EndReadFrame();
		}

		BeginFrame();

		DrawSobelRect(&ytexture, -1.f, -1.f, 1.f, 1.f, &greysobeltexture);
		DrawTextureRect(&greysobeltexture, -1.f, -1.f, 1.f, 1.f, NULL);

		EndFrame();
	}

	StopCamera();

	endwin();
}
