
# raspistill/raspivid/raspiyuv/visnav_raspivid

SET(COMPILE_DEFINITIONS -Werror)

include_directories(${PROJECT_SOURCE_DIR}/host_applications/linux/libs/bcm_host/include)
include_directories(${PROJECT_SOURCE_DIR}/host_applications/linux/apps/raspicam/)
include_directories(${PROJECT_SOURCE_DIR}/host_applications/linux/libs/sm)
include_directories(/opt/vc/include)
link_directories(/opt/vc/lib)

# Add openCV libraries
find_package(OpenCV REQUIRED)

set (GL_SCENE_SOURCES
   gl_scenes/models.c
   gl_scenes/mirror.c
   gl_scenes/yuv.c
   gl_scenes/sobel.c
   gl_scenes/square.c
   gl_scenes/teapot.c
   gl_scenes/vcsm_square.c)

set (COMMON_SOURCES
   RaspiCamControl.c
   RaspiCLI.c
   RaspiPreview.c)

# Set my files (to be compiled in c++):
set (CXX_FILES
   	VisNav_lib.cpp
    )

SET_SOURCE_FILES_PROPERTIES(VisNav_lib.cpp PROPERTIES LANGUAGE CXX) 

#G#add_executable(raspistill ${COMMON_SOURCES} RaspiStill.c  RaspiTex.c RaspiTexUtil.c tga.c ${GL_SCENE_SOURCES})
#G#add_executable(raspiyuv   ${COMMON_SOURCES} RaspiStillYUV.c)
#G#add_executable(raspivid   ${COMMON_SOURCES} RaspiVid.c)
#G#add_executable(raspividyuv  ${COMMON_SOURCES} RaspiVidYUV.c)
add_executable(visnav_raspivid   ${COMMON_SOURCES} ${CXX_FILES} VisNavRaspiVid.c)
add_executable(visnav_raspividyuv   ${COMMON_SOURCES} ${CXX_FILES} VisNavRaspiVidYUV.c)
#add_executable(visnav_raspivid   ${COMMON_SOURCES} VisNavRaspiVid.c)

set (MMAL_LIBS mmal_core mmal_util mmal_vc_client)

#G#target_link_libraries(raspistill ${MMAL_LIBS} vcos bcm_host brcmGLESv2 brcmEGL m)
#G#target_link_libraries(raspiyuv   ${MMAL_LIBS} vcos bcm_host)
#G#target_link_libraries(raspivid   ${MMAL_LIBS} vcos bcm_host)
#G#target_link_libraries(raspividyuv   ${MMAL_LIBS} vcos bcm_host)
target_link_libraries(visnav_raspivid   ${MMAL_LIBS} pthread vcos bcm_host ${OpenCV_LIBS} )
target_link_libraries(visnav_raspividyuv   ${MMAL_LIBS} pthread vcos bcm_host ${OpenCV_LIBS} )
#G#install(TARGETS raspistill raspiyuv raspivid visnav_raspivid raspividyuv RUNTIME DESTINATION bin)
