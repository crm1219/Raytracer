#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <vector>
using namespace std;

#define PI 3.14159
#define MAX_RECURS_LEVEL 7

// Pixel map structs
typedef struct Color 
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} Color;

typedef struct ppm_metadata 
{
    uint16_t width;
    uint16_t height;
    uint16_t max_val;
} ppm_metadata;

// Scene object structs
typedef struct Object 
{
    float c_diff[3] = {0, 0, 0};
    float c_spec[3] = {0, 0, 0};
    float reflection = 0;
    Color **texture_map = NULL;
    ppm_metadata texture_md = {0, 0, 0};
    virtual float intersect(float *ray_orig, float *ray_dest) = 0;
    virtual void get_normal(float *intersection_point, float *int_normal) {};
    virtual Color get_texture_color(float *intersection_point) {return {0, 0, 0};};
    virtual ~Object() {};
} Object;

typedef struct Sphere : Object 
{
    float position[3] = {0, 0, 0};
    float radius = 0;
    float intersect(float *ray_orig, float *ray_dest) override;
    void get_normal(float *intersection_point, float *int_normal) override;
    Color get_texture_color(float *intersection_point) override;
    ~Sphere() {};
} Sphere;

typedef struct Plane : Object 
{
    float position[3] = {0, 0, 0};
    float normal[3] = {0, 0, 0};
    float intersect(float *ray_orig, float *ray_dest) override;
    void get_normal(float *intersection_point, float *int_normal) override;
    ~Plane() {};
    Color get_texture_color(float *intersection_point) override;
} Plane;

typedef struct Triangle : Object 
{
    float corner1[3] = {0, 0, 0};
    float corner2[3] = {0, 0, 0};
    float corner3[3] = {0, 0, 0};
    float normal[3] = {0, 0, 0};
    float intersect(float *ray_orig, float *ray_dest) override;
    void get_normal(float *intersection_point, float *int_normal) override;
    Color get_texture_color(float *intersection_point) override;
    ~Triangle() {};
} Triangle;

typedef struct Light 
{
    float position[3] = {0, 0, 0};
    float color[3] = {0, 0, 0};
    float radial_a0 = 0;
    float radial_a1 = 0;
    float radial_a2 = 0;
    float theta = 0;
    float angular_a0 = 0;
    float direction[3] = {0, 0, 0};
} Light;

typedef struct Scene
{
    bool read = true;
    std::vector<Object *> scene_objects = {};
    std::vector<Light *> scene_lights = {};
} Scene;

// Read/write functions
Scene *scene_read(char *filename, float *camera_width, float *camera_height);
Color **ppm_read(char *filename, ppm_metadata *metadata);
void ppm_write(char *filename, Color **pix_map, ppm_metadata *metadata);

// Vector calculation functions
void v3_from_points(float *dst, float *a, float *b);
void v3_add(float *dst, float *a, float *b);
void v3_subtract(float *dst, float *a, float *b);
void v3_scale(float *dst, float s);
float v3_dot_product(float *a, float *b);
void v3_cross_product(float *dst, float *a, float *b);
float v3_length(float *a);
float v3_angle(float *a, float *b);
void v3_reflect(float *dst, float *v, float *n);
void v3_normalize(float *dst, float *a);

// Raycast function
Color raycast(int img_width, int img_height, int img_x, int img_y, float camera_width, float camera_height, Scene *scene);

Color shade(Object *object_hit, float *intersect_point, float *ray_direction, int level, Scene *scene);
