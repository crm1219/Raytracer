#include "raycast_lib.h"

int main(int argc, char *argv[])
{
    // Check for proper number of args
    if (argc != 5) 
    {
        printf("Usage: raycast width height input.scene output.ppm\n");
        return 1;
    }

    // Declare/initialize starting variables
    float img_width = atoi(argv[1]);
    float img_height = atoi(argv[2]);
    float camera_width = 0;
    float camera_height = 0;
    ppm_metadata metadata = {static_cast<uint16_t>(img_width), static_cast<uint16_t>(img_height), 255};
    
    // Read data from file
    Scene *scene = scene_read(argv[3], &camera_width, &camera_height);

    // Check if there is an error in reading from the file
    if (!scene->read)
    {
        fprintf(stderr, "Error reading from file.\n");
        return 1;
    }

    // Allocate pixel map
    Color **map = (Color **)malloc(img_height * sizeof(Color *));
    for (int i = 0; i < img_height; i++)
    {
        map[i] = (Color *)malloc(img_width * sizeof(Color));
    }

    // Loop through output image pixels and assign a color to each
    for (float img_y = 0; img_y < img_height; img_y++)
    {
        for (float img_x = 0; img_x < img_width; img_x++)
        {
            // for yss in range (0, ss)
                // for xss in range (0, ss)
                    // Rdx = ... + xss * xssdelta
                    // Rdy = ... + yss * yssdelta
                    // Add a bit of random twitch to xssdelta/yssdelta - or create a sufficiently large table w/ different values for supersampling
                    // Adaptive sampling -> only do this if there's a boundary b/n pixels (either different objects or shadows)
            // Average together color for each pixel
            Color pixel = raycast(img_width, img_height, img_x, img_y, camera_width, camera_height, scene);
            map[static_cast<int>(img_y)][static_cast<int>(img_x)] = pixel;
        }
    }

    // Write result to output file
    ppm_write(argv[4], map, &metadata);

    // Delete allocated objects
    for (auto object: scene->scene_objects)
    {
       delete object;
    }

    for (auto light: scene->scene_lights)
    {
       delete light;
    }

    return 0;
}