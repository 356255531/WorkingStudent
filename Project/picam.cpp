#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "camera.h"
#include "graphics.h"
#include "tracking_toolbox.h"
#include "types.h"

#define MAIN_TEXTURE_WIDTH 768
#define MAIN_TEXTURE_HEIGHT 512

// #define TEXTURE_GRID_COLS 4
// #define TEXTURE_GRID_ROWS 4

#define check() assert(glGetError() == 0)

char tmpbuff[MAIN_TEXTURE_WIDTH * MAIN_TEXTURE_HEIGHT * 4];

// Functionality can be changed by defining different Macro

#define HARRIS_DETECTOR
//#define KLT_GPU
//#define KLT_GPU_PATCH

int main(int argc, const char **argv)
{
    //init graphics and the camera
    InitGraphics();
    CCamera* cam = StartCamera(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT, 30, false);

#ifdef HARRIS_DETECTOR
    int counter = 0;
    // Create the yuv textures
    GfxTexture y_tex;
    y_tex.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);

    // Create the pipline textures
    GfxTexture  sobel_tex,
                blurred_sobel_tex,
                window_blurred_sobel_tex,
                harris_response_tex,
                decision_tex,
                output_tex;

    sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    sobel_tex.GenerateFrameBuffer();
    blurred_sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    blurred_sobel_tex.GenerateFrameBuffer();
    window_blurred_sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    window_blurred_sobel_tex.GenerateFrameBuffer();
    harris_response_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    harris_response_tex.GenerateFrameBuffer();
    decision_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    decision_tex.GenerateFrameBuffer();
    //output_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    //output_tex.GenerateFrameBuffer();
    

#elif defined KLT_GPU
    GfxTexture y_tex_1, y_tex_2;
    y_tex_1.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    y_tex_2.CreateGreyScale(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);

    GfxTexture sum_diff_tex, sobel_tex, grad_tex, multi_tex;
    sum_diff_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
    sum_diff_tex.GenerateFrameBuffer();
    sobel_tex.CreateRGBA(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);
    sobel_tex.GenerateFrameBuffer();
    grad_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
    grad_tex.GenerateFrameBuffer();
    multi_tex.CreateRGBA(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
    multi_tex.GenerateFrameBuffer();
#endif

    check();
    printf("Running frame loop\n");

    //read start time
    long int start_time;
    long int time_difference;
    struct timespec gettime_now;
    clock_gettime(CLOCK_REALTIME, &gettime_now);
    start_time = gettime_now.tv_nsec ;
    double total_time_s = 0;

    // initscr();      /* initialize the curses library */
    // keypad(stdscr, TRUE);  /* enable keyboard mapping */
    // nonl();          // tell curses not to do NL->CR/NL on output
    // cbreak();       /* take input chars one at a time, no wait for \n */
    // clear();
    // nodelay(stdscr, TRUE);

#ifdef KLT_GPU
    int counter = 0;
    GfxTexture* curr_tex_ptr = &y_tex_1;
    GfxTexture* prev_tex_ptr = &y_tex_1;
#endif

    for (int i = 0; i < 3000; i++)
    {
        //spin until we have a camera frame
        const void* frame_data; int frame_sz;
        while (!cam->BeginReadFrame(frame_data, frame_sz)) {};

        //lock the chosen frame buffer, and copy it directly into the corresponding open gl texture
        {
            const uint8_t* data = (const uint8_t*)frame_data;
#ifdef HARRIS_DETECTOR
            y_tex.SetPixels(data);
#elif defined KLT_GPU
            curr_tex_ptr->SetPixels(data);
#endif
            cam->EndReadFrame();
        }

        //begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
        BeginFrame();

#ifdef HARRIS_DETECTOR
        DrawSobelRect(&y_tex, -1, -1, 1, 1, &sobel_tex);
        DrawBlurRect(&sobel_tex, -1, -1, 1, 1, &blurred_sobel_tex);
        DrawHarrisRect(&blurred_sobel_tex, -1, -1, 1, 1, &harris_response_tex);
        DrawNonMaxSupRect(&harris_response_tex, -1, -1, 1, 1, &decision_tex, 0.1);
        DrawTextureRect(&decision_tex, -1, -1, 1, 1, NULL);
/* output the coordinate
        void* data_decision = decision_tex.ret_img();
        unsigned char* decision = (unsigned char*)data_decision; // Change the img to 32-bit rgba array
        counter = 0;
        for (int j = 0; j < MAIN_TEXTURE_HEIGHT * MAIN_TEXTURE_WIDTH; ++j) {
            if (decision[4*j] == 255) {printf("%d %d\n", j / MAIN_TEXTURE_WIDTH, j % MAIN_TEXTURE_WIDTH);counter++;} // Output the first 50 corners
            
            if (counter == 20) break;
        }
        printf("end visul");
        free(data_decision);
*/
#elif defined KLT_GPU
        
            // Calculate the graident and multi value
            DrawTexturePlusDiffRect(prev_tex_ptr, curr_tex_ptr, -1, -1, 1, 1, &sum_diff_tex);      
            // void* data_sum_diff =sum_diff_tex.ret_img();
            // unsigned char* sum_diff = (unsigned char*)data_sum_diff;
            // for (int j = 0; j < MAIN_TEXTURE_HEIGHT * MAIN_TEXTURE_WIDTH; ++j) {
            //  if (sum_diff[4 * j + 1] > 128) printf("%u %u\n", sum_diff[4 * j], sum_diff[4 * j + 1]);
            // }
            // free(data_sum_diff);
            // DrawTextureRect(&sum_diff_tex, -1, -1, 1, 1, NULL);
            DrawSimpleSobelRect(&sum_diff_tex, -1, -1, 1, 1, &sobel_tex);
            // void* data_sobel_tex = sobel_tex.ret_img();
            // unsigned char* sobel = (unsigned char*)data_sobel_tex;
            // for (int j = 0; j < MAIN_TEXTURE_HEIGHT * MAIN_TEXTURE_WIDTH; ++j) {
            //  if (sobel[4 * j] < 100) printf("%u %u\n", sobel[4 * j], sobel[4 * j + 1]);
            // }
            // free(data_sobel_tex);
            // DrawTextureRect(&sobel_tex, -1, -1, 1, 1, NULL);
            DrawGradientRect(&sobel_tex, -1, -1, 1, 1, &grad_tex);
            // void* data_grad_tex = grad_tex.ret_img();
            // unsigned char* grad = (unsigned char*)data_grad_tex;
            // for (int j = 0; j < MAIN_TEXTURE_HEIGHT * MAIN_TEXTURE_WIDTH; ++j) {
            //  if (grad[4 * j + 1] != 128) printf("%u %u\n", grad[4 * j], grad[4 * j + 1]);
            // }
            // free(data_grad_tex);
            // DrawTextureRect(&grad_tex, -1, -1, 1, 1, NULL);
            DrawTextureMultiRect(&sum_diff_tex, &sobel_tex, -1, -1, 1, 1, &multi_tex);
            // DrawTextureRect(&multi_tex, -1, -1, 1, 1, NULL);

        if (10 == count) {
            void* data_grad = grad_tex.ret_img();
            void* data_multi = multi_tex.ret_img();
            unsigned char* grad = (unsigned char*)data_grad;
            unsigned char* multi = (unsigned char*)data_multi;

            long x_grad_squre = 0, y_grad_squre = 0, x_y_grad_squre = 0;
            long diff_x_grad = 0, diff_y_grad = 0;
            for (int j = 0; j < MAIN_TEXTURE_HEIGHT * MAIN_TEXTURE_WIDTH; ++j) {
                x_grad_squre += (grad[j * 4] - 128) * 256;
                y_grad_squre += (grad[j * 4 + 1] - 128) * 256;
                x_y_grad_squre += (grad[j * 4 + 2] - 128) * 256;
                diff_x_grad += (multi[j * 4] - 128) * 64;
                diff_y_grad += (multi[j * 4 + 1] - 128) * 64;
            }
            // printf("%d %d %d %d %d\n", x_grad_squre, y_grad_squre, x_y_grad_squre, diff_x_grad, diff_y_grad);
            printf(
                        "%u %u\n", 
                        (y_grad_squre * diff_x_grad - x_y_grad_squre * diff_y_grad) * 2 / (1.0 * (x_grad_squre  * y_grad_squre - x_y_grad_squre * x_y_grad_squre)),
                        (x_grad_squre * diff_y_grad - x_y_grad_squre * diff_x_grad) * 2 / (1.0 * (x_grad_squre  * y_grad_squre - x_y_grad_squre * x_y_grad_squre))
                    );

            prev_tex_ptr = curr_tex_ptr;
            curr_tex_ptr = curr_tex_ptr == &y_tex_1? &y_tex_2 : &y_tex_1;
            free(data_grad);
            free(data_multi);
            count = 0;
        }
        count++;
#endif

        EndFrame();

         // read current time
         clock_gettime(CLOCK_REALTIME, &gettime_now);
         time_difference = gettime_now.tv_nsec - start_time;
         if (time_difference < 0) time_difference += 1000000000;
         total_time_s += double(time_difference) / 1000000000.0;
         start_time = gettime_now.tv_nsec;

         //print frame rate
         float fr = float(double(i + 1) / total_time_s);
         // if ((i % 30) == 0)
         // {
         //     mvprintw(0, 0, "Framerate: %g\n", fr);
         //     move(0, 0);
         //     refresh();
         // }

    }

    StopCamera();

    endwin();
}
