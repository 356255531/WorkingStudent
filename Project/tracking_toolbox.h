#ifndef TRACKING_TOOLBOX_H
#define TRACKING_TOOLBOX_H
#include <stdio.h>
#include <stdint.h>
#include <boost/tuple/tuple.hpp>
template <typename T>
boost::tuple<int, int> 
compute_move_vector(T origin_img, T feature_map, T new_origin_img, T new_feature_map, int length, int width) {
    for (int i = 0; i < length; ++i) {
        printf("%d\n", *((uint8_t*)origin_img->get_img_ptr() + i));
    }
    return boost::make_tuple(1, 1);
}
#endif