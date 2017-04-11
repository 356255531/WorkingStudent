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

#define check() assert(glGetError() == 0)

char tmpbuff[MAIN_TEXTURE_WIDTH * MAIN_TEXTURE_HEIGHT * 4];

//entry point
int main(int argc, const char **argv)
{
	//init graphics and the camera
	InitGraphics();
	CCamera* cam = StartCamera(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT, 30, false);

	// Create the yuv textures
	GfxTexture y_tex, u_tex, v_tex;
	y_tex.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	// u_tex.CreateGreyScale(MAIN_TEXTURE_WIDTH >> 1, MAIN_TEXTURE_HEIGHT >> 1);
	// v_tex.CreateGreyScale(MAIN_TEXTURE_WIDTH >> 1, MAIN_TEXTURE_HEIGHT >> 1);

	// Create the pipline textures
	GfxTexture 	sobel_tex,
	            blurred_sobel_tex,
	            window_blurred_sobel_tex,
	            harris_response_tex,
	            output_tex;

	sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	sobel_tex.GenerateFrameBuffer();
	blurred_sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	blurred_sobel_tex.GenerateFrameBuffer();
	window_blurred_sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	window_blurred_sobel_tex.GenerateFrameBuffer();
	harris_response_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	harris_response_tex.GenerateFrameBuffer();
	output_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	output_tex.GenerateFrameBuffer();
	check();

	printf("Running frame loop\n");

	//read start time
	long int start_time;
	long int time_difference;
	struct timespec gettime_now;
	clock_gettime(CLOCK_REALTIME, &gettime_now);
	start_time = gettime_now.tv_nsec ;
	double total_time_s = 0;

	initscr();      /* initialize the curses library */
	keypad(stdscr, TRUE);  /* enable keyboard mapping */
	nonl();          // tell curses not to do NL->CR/NL on output
	cbreak();       /* take input chars one at a time, no wait for \n */
	clear();
	nodelay(stdscr, TRUE);

	for (int i = 0; i < 3000; i++)
	{
		//spin until we have a camera frame
		const void* frame_data; int frame_sz;
		while (!cam->BeginReadFrame(frame_data, frame_sz)) {};

		//lock the chosen frame buffer, and copy it directly into the corresponding open gl texture
		{
			const uint8_t* data = (const uint8_t*)frame_data;
			// int ypitch = MAIN_TEXTURE_WIDTH;
			// int ysize = ypitch * MAIN_TEXTURE_HEIGHT;
			// int uvpitch = MAIN_TEXTURE_WIDTH / 2;
			// int uvsize = uvpitch * MAIN_TEXTURE_HEIGHT / 2;
			//int upos = ysize+16*uvpitch;
			//int vpos = upos+uvsize+4*uvpitch;
			// int upos = ysize;
			// int vpos = upos + uvsize;
			//printf("Frame data len: 0x%x, ypitch: 0x%x ysize: 0x%x, uvpitch: 0x%x, uvsize: 0x%x, u at 0x%x, v at 0x%x, total 0x%x\n", frame_sz, ypitch, ysize, uvpitch, uvsize, upos, vpos, vpos+uvsize);
			y_tex.SetPixels(data);
			// u_tex.SetPixels(data + upos);
			// v_tex.SetPixels(data + vpos);
			cam->EndReadFrame();
		}

		//begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
		BeginFrame();

		DrawSobelRect(&y_tex, -1, -1, 1, 1, &sobel_tex);
		DrawBlurredSobelRect(&sobel_tex, -1, -1, 1, 1, &blurred_sobel_tex);
		DrawWindowBlurredSoeblRect(&blurred_sobel_tex, -1, -1, 1, 1, &window_blurred_sobel_tex);
		DrawHarrisRect(&window_blurred_sobel_tex, -1, -1, 1, 1, &harris_response_tex, 0.06);
		DrawSelectionRect(&harris_response_tex, -1, -1, 1, 1, &output_tex);
		DrawTextureRect(&output_tex, -1, -1, 1, 1, NULL);

		EndFrame();

		//read current time
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		time_difference = gettime_now.tv_nsec - start_time;
		if (time_difference < 0) time_difference += 1000000000;
		total_time_s += double(time_difference) / 1000000000.0;
		start_time = gettime_now.tv_nsec;

		//print frame rate
		float fr = float(double(i + 1) / total_time_s);
		if ((i % 30) == 0)
		{
			mvprintw(0, 0, "Framerate: %g\n", fr);
			move(0, 0);
			refresh();
		}

	}

	StopCamera();

	endwin();
}
