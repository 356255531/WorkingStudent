#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "camera.h"
#include "graphics.h"
#include "tracking_toolbox.h"
#include "types.h"

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
	// GfxTexture y_tex, u_tex, v_tex;
	GfxTexture y_tex_1, y_tex_2;
	y_tex_1.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	y_tex_2.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);

	GfxTexture sum_tex, diff_tex, sum_prod_tex, time_difference;
	sum_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	sum_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	sum_tex.GenerateFrameBuffer();
	diff_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	diff_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	diff_tex.GenerateFrameBuffer();
	sum_prod_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	sum_prod_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	sum_prod_tex.GenerateFrameBuffer();
	diff_prod_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	diff_prod_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	diff_prod_tex.GenerateFrameBuffer();
	// GfxTexture y_read_tex;
	// y_read_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	// y_read_tex.GenerateFrameBuffer();
	// u_tex.CreateGreyScale(MAIN_TEXTURE_WIDTH >> 1, MAIN_TEXTURE_HEIGHT >> 1);

	// v_tex.CreateGreyScale(MAIN_TEXTURE_WIDTH >> 1, MAIN_TEXTURE_HEIGHT >> 1);

	// Create the pipline textures
	GfxTexture 	sobel_tex;
	//             blurred_sobel_tex,
	//             window_blurred_sobel_tex,
	//             harris_response_tex,
	//             decision_tex,
	//             output_tex;

	sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	sobel_tex.GenerateFrameBuffer();
	// blurred_sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	// blurred_sobel_tex.GenerateFrameBuffer();
	// window_blurred_sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	// window_blurred_sobel_tex.GenerateFrameBuffer();
	// harris_response_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	// harris_response_tex.GenerateFrameBuffer();
	// decision_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	// decision_tex.GenerateFrameBuffer();
	// output_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
	// output_tex.GenerateFrameBuffer();
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

	// boost::shared_ptr<Image> origin_img(new Image());
	// boost::shared_ptr<Image> new_origin_img(new Image());
	// boost::shared_ptr<Image> feature_map(new Image());
	// boost::shared_ptr<Image> new_feature_map(new Image());
	int count = 0;
	GfxTexture* curr_tex_ptr = &y_tex_1;
	GfxTexture* prev_tex_ptr = &y_tex_1;
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
			curr_tex_ptr->SetPixels(data);
			// u_tex.SetPixels(data + upos);
			// v_tex.SetPixels(data + vpos);
			cam->EndReadFrame();
		}

		//begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
		BeginFrame();

		// DrawSobelRect(&y_tex, -1, -1, 1, 1, &sobel_tex);
		// DrawBlurRect(&sobel_tex, -1, -1, 1, 1, &blurred_sobel_tex);
		// DrawWindowBlurredSoeblRect(&blurred_sobel_tex, -1, -1, 1, 1, &window_blurred_sobel_tex);
		// DrawHarrisRect(&blurred_sobel_tex, -1, -1, 1, 1, &harris_response_tex);
		// DrawNonMaxSupRect(&harris_response_tex, -1, -1, 1, 1, &decision_tex, 0.1);
		// DrawTextureRect(&decision_tex, -1, -1, 1, 1, NULL);

		if (5 == count) {
			DrawTexturePlusRect(prev_tex_ptr, curr_tex_ptr, -1, -1, 1, 1, &sum_tex);
			DrawTextureDiffRect(prev_tex_ptr, curr_tex_ptr, -1, -1, 1, 1, &diff_tex);
			DrawSobelRect(&sum_tex, -1, -1, 1, 1, &sobel_tex);
			DrawTextureMultiRect(&sum_tex, &sobel_tex, curr_tex_ptr, -1, -1, 1, 1, &sum_prod_tex);
			DrawTextureMultiRect(&diff_tex, &sobel_tex, curr_tex_ptr, -1, -1, 1, 1, &diff_prod_tex);
			prev_tex_ptr = curr_tex_ptr;
			curr_tex_ptr = curr_tex_ptr == &y_tex_1? &y_tex_2 : &y_tex_1;
			count = 0;
		}
		count++;
		EndFrame();

		// if (0 == i % 10) {
		// 	DrawTextureRect(&y_tex,-1,-1,1,1,&y_read_tex);
		// 	origin_img->reload_img(new_origin_img->get_img_ptr());
		// 	feature_map->reload_img(new_feature_map->get_img_ptr());
		// 	new_origin_img->load_img(&y_read_tex);
		// 	new_feature_map->load_img(&decision_tex);
		// 	boost::tuple<int, int> move_vector = compute_move_vector(
		// 																origin_img, 
		// 																feature_map, 
		// 																new_origin_img, 
		// 																new_feature_map,
		// 																MAIN_TEXTURE_HEIGHT,
		// 																MAIN_TEXTURE_WIDTH
		// 															);
		// }

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
