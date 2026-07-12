#include "raycast_lib.h"

/// Intersect Functions

// Project 4: get the ACTUAL intersection
    // multiply destination ray by t and add to origin

// Function for a ray intersecting with a plane
float Plane::intersect(float *ray_orig, float *ray_dest)
{
    // Assertions to check that things that should be normalized are, in fact, normalized
    assert(v3_length(normal) > 0.99999 && v3_length(normal) < 1.00001);
    assert(v3_length(ray_dest) > 0.99999 && v3_length(ray_dest) < 1.00001);

    // Calculate dot product between ray vector and plane normal
    float dot_product = v3_dot_product(normal, ray_dest);

    // Check if there's no intersection (i.e. ray is perpendicular to plane normal)
    if (dot_product == 0)
    {
        return INFINITY;
    }

    // Otherwise, calculate and return distance to the plane
    else 
    {
        float difference[3] = {0, 0, 0};
        v3_subtract(difference, ray_orig, position);
        float distance = -1 * (v3_dot_product(normal, difference) / dot_product);
        return distance;
    }
}

// Function for a ray intersecting with a sphere
float Sphere::intersect(float *ray_orig, float *ray_dest)
{
    // Assertion to check that things that should be normalized are, in fact, normalized
    assert(v3_length(ray_dest) > 0.99999 && v3_length(ray_dest) < 1.00001);
    
    // Calculate t_close (distance along ray to point closest to the center of the sphere)
    float output[3] = {0, 0, 0}; 
    v3_subtract(output, position, ray_orig);
    float t_close = v3_dot_product(ray_dest, output);

    // Calculate x_close (point on the ray closest to the center of the sphere)
    output[0] = ray_dest[0];
    output[1] = ray_dest[1];
    output[2] = ray_dest[2];
    v3_scale(output, t_close);
    float x_close[3] = {0, 0, 0};
    v3_add(x_close, ray_orig, output);

    // Calculate distance from closest point on ray to the center of the sphere
    v3_subtract(output, x_close, position);
    float distance = v3_length(output);

    // Case 1 - distance exceeds radius; ray does not intersect with sphere
    if (distance > radius)
    {
        return INFINITY;
    }

    // Case 2 - distance equals radius; ray intersects with circumference of sphere
    else if (distance == radius)
    {
        return t_close;
    }

    // Case 3 - distance is less than radius; ray intersects with interior of sphere
    else
    {
        // Calculate distance from ray origin to first hit point on the sphere's surface
        float a = sqrt(radius * radius - distance * distance);
        return (t_close - a);
    }
}

// TODO: Function for a ray intersecting with a triangle
float Triangle::intersect(float *ray_orig, float *ray_dest)
{
    // Assertion to check that things that should be normalized are, in fact, normalized
    assert(v3_length(ray_dest) > 0.99999 && v3_length(ray_dest) < 1.00001);

    // Calculate dot product between ray vector and triangle normal
    float dot_product = v3_dot_product(normal, ray_dest);

    // Check if there's no intersection (i.e. ray is perpendicular to plane normal)
    if (dot_product == 0)
    {
        return INFINITY;
    }

    // Otherwise, calculate distance to the plane defined by the triangle
    float difference[3] = {0, 0, 0};
    v3_subtract(difference, ray_orig, corner1);
    float distance = -1 * (v3_dot_product(normal, difference) / dot_product);

    // Check if the triangle is behind the camera (i.e. distance to the plane is negative)
    if (distance < 0)
    {
        return INFINITY;
    }

    // Calculate point at which the ray intersects this plane
    float ray_to_intersect[3] = {ray_dest[0], ray_dest[1], ray_dest[2]};
    v3_scale(ray_to_intersect, distance);
    float intersect_point[3] = {0, 0, 0};
    v3_add(intersect_point, ray_to_intersect, ray_orig);

    // Calculate whether the intersection point is inside the triangle
    float leg1[3] = {0, 0, 0};
    float leg2[3] = {0, 0, 0};
    float leg3[3] = {0, 0, 0};

    float corner_to_intersect1[3] = {0, 0, 0};
    float corner_to_intersect2[3] = {0, 0, 0};
    float corner_to_intersect3[3] = {0, 0, 0};

    float crossprod1[3] = {0, 0, 0};
    float crossprod2[3] = {0, 0, 0};
    float crossprod3[3] = {0, 0, 0};

    v3_subtract(leg1, corner3, corner1);
    v3_subtract(leg2, corner2, corner3);
    v3_subtract(leg3, corner1, corner2);

    v3_subtract(corner_to_intersect1, intersect_point, corner1);
    v3_subtract(corner_to_intersect2, intersect_point, corner3);
    v3_subtract(corner_to_intersect3, intersect_point, corner2);

    v3_cross_product(crossprod1, leg1, corner_to_intersect1);
    v3_cross_product(crossprod2, leg2, corner_to_intersect2);
    v3_cross_product(crossprod3, leg3, corner_to_intersect3);

    if (v3_dot_product(normal, crossprod1) > 0 && v3_dot_product(normal, crossprod2) > 0 && v3_dot_product(normal, crossprod3) > 0)
    {
        return distance;
    }
    return INFINITY;
}

// Normal Functions

// Function for getting the normal of a plane
void Plane::get_normal(float *intersection_point, float *int_normal)
{
    int_normal[0] = normal[0];
    int_normal[1] = normal[1];
    int_normal[2] = normal[2];
}

// Function for getting the normal of a sphere
void Sphere::get_normal(float *intersection_point, float *int_normal)
{
    float pre_norm_normal[3] = {0, 0, 0};
    v3_subtract(pre_norm_normal, intersection_point, position);
    v3_normalize(int_normal, pre_norm_normal);
}

// Function for getting the normal of a triangle
void Triangle::get_normal(float *intersection_point, float *int_normal)
{
    int_normal[0] = normal[0];
    int_normal[1] = normal[1];
    int_normal[2] = normal[2];
}

// Texture Functions

// Function for mapping a texture to a plane
Color Plane::get_texture_color(float *intersection_point)
{
    return {0, 0, 0};
}

Color Sphere::get_texture_color(float *intersection_point)
{
    float d[3] = {0, 0, 0};
    float d_norm[3] = {0, 0, 0};
    v3_from_points(d, position, intersection_point);
    v3_normalize(d_norm, d);
    float phi = atan2(d_norm[2], d_norm[0]);
    float theta = acos(d_norm[1]);
    int u = ((phi / (2 * PI)) + 0.5) * texture_md.width;
    int v = (theta / PI) * texture_md.height;
    return texture_map[v][u];
}

Color Triangle::get_texture_color(float *intersection_point)
{return {0, 0, 0};}

/// File Read/Write Functions

// Function to read from a scene file
Scene *scene_read(char *filename, float *camera_width, float *camera_height)
{
    Scene *scene = new Scene();
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) 
    {
        scene->read = false;
        fprintf(stderr, "Unable to open file.\n");
        return scene;
    }
    // Check that file is the correct type
    char filetype[100];
    fgets(filetype, 12, input_file);
    if (strcmp(filetype, "img410scene") != 0) 
    {
        scene->read = false;
        fprintf(stderr, "Incorrect file type.\n");
        fclose(input_file);
        return scene;
    }

    // Read objects from file
    char object_type[100];
    while (fscanf(input_file, " %s ", object_type) == 1 && (strcmp(object_type, "end") != 0))
    {
        char property_type[100];
        // read camera properties
        if (strcmp(object_type, "camera") == 0)
        {
            float width, height;
            while(fscanf(input_file, " %s ", property_type) == 1 && (strcmp(property_type, ";") != 0))
            {
                if (strcmp(property_type, "width:") == 0)
                {
                    fscanf(input_file, " %f", &width);
                    *camera_width = width;
                }
                else if (strcmp(property_type, "height:") == 0)
                {
                    fscanf(input_file, " %f", &height);
                    *camera_height = height;
                }
            }
        }

        // Read plane information
        else if (strcmp(object_type, "plane") == 0)
        {
            Plane *newPlane = new Plane();
            while(fscanf(input_file, " %s ", property_type) == 1 && (strcmp(property_type, ";") != 0))
            {
                float prop1, prop2, prop3;
                if (strcmp(property_type, "c_diff:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newPlane->c_diff[0] = prop1;
                    newPlane->c_diff[1] = prop2;
                    newPlane->c_diff[2] = prop3;
                }
                else if (strcmp(property_type, "c_spec:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newPlane->c_spec[0] = prop1;
                    newPlane->c_spec[1] = prop2;
                    newPlane->c_spec[2] = prop3;
                }
                else if (strcmp(property_type, "reflection:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newPlane->reflection = prop1;
                }
                else if (strcmp(property_type, "position:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newPlane->position[0] = prop1;
                    newPlane->position[1] = prop2;
                    newPlane->position[2] = prop3;
                }
                else if (strcmp(property_type, "normal:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    float normal[3] = {prop1, prop2, prop3};
                    v3_normalize(newPlane->normal, normal);
                }
            }
            scene->scene_objects.push_back(newPlane);
        }

        // Read sphere information
        else if (strcmp(object_type, "sphere") == 0)
        {
            Sphere *newSphere = new Sphere();
            while(fscanf(input_file, " %s ", property_type) == 1 && (strcmp(property_type, ";") != 0))
            {
                float prop1, prop2, prop3;
                if (strcmp(property_type, "c_diff:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newSphere->c_diff[0] = prop1;
                    newSphere->c_diff[1] = prop2;
                    newSphere->c_diff[2] = prop3;
                }
                else if (strcmp(property_type, "c_spec:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newSphere->c_spec[0] = prop1;
                    newSphere->c_spec[1] = prop2;
                    newSphere->c_spec[2] = prop3;
                }
                else if (strcmp(property_type, "reflection:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newSphere->reflection = prop1;
                }
                else if (strcmp(property_type, "texture:") == 0)
                {
                    char filename[64];
                    fscanf(input_file, " %s", filename);
                    // Extract pixel map and metadata from ppm texture file
                    ppm_metadata metadata;
                    int length = strlen(filename) - 2;
                    char new_filename[64];
                    for (int i = 0; i < length; i++)
                    {
                        new_filename[i] = filename[i + 1];
                    }
                    new_filename[length] = '\0';
                    Color **pixmap = ppm_read(new_filename, &metadata);
                    // Assign pixel map and metadata to sphere attributes
                    newSphere->texture_map = pixmap;
                    newSphere->texture_md = metadata;
                }
                else if (strcmp(property_type, "position:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newSphere->position[0] = prop1;
                    newSphere->position[1] = prop2;
                    newSphere->position[2] = prop3;
                }
                else if (strcmp(property_type, "radius:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newSphere->radius = prop1;
                }
            }
            scene->scene_objects.push_back(newSphere);
        }
        // Read triangle information
        else if (strcmp(object_type, "triangle") == 0)
        {
            Triangle *newTriangle = new Triangle();
            while(fscanf(input_file, " %s ", property_type) == 1 && (strcmp(property_type, ";") != 0))
            {
                float prop1, prop2, prop3;
                if (strcmp(property_type, "c_diff:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newTriangle->c_diff[0] = prop1;
                    newTriangle->c_diff[1] = prop2;
                    newTriangle->c_diff[2] = prop3;
                }
                else if (strcmp(property_type, "c_spec:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newTriangle->c_spec[0] = prop1;
                    newTriangle->c_spec[1] = prop2;
                    newTriangle->c_spec[2] = prop3;
                }
                else if (strcmp(property_type, "reflection:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newTriangle->reflection = prop1;
                }
                else if (strcmp(property_type, "corner1:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newTriangle->corner1[0] = prop1;
                    newTriangle->corner1[1] = prop2;
                    newTriangle->corner1[2] = prop3;
                }
                else if (strcmp(property_type, "corner2:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newTriangle->corner2[0] = prop1;
                    newTriangle->corner2[1] = prop2;
                    newTriangle->corner2[2] = prop3;
                }
                else if (strcmp(property_type, "corner3:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newTriangle->corner3[0] = prop1;
                    newTriangle->corner3[1] = prop2;
                    newTriangle->corner3[2] = prop3;
                }
            }

            // Calculate the normal of the triangle
            float leg1[3] = {0, 0, 0};
            float leg2[3] = {0, 0, 0};
            float normal[3] = {0, 0, 0};
            v3_from_points(leg1, newTriangle->corner1, newTriangle->corner2);
            v3_from_points(leg2, newTriangle->corner1, newTriangle->corner3);
            v3_cross_product(normal, leg1, leg2);
            v3_normalize(normal, normal);
            
            // Flip the normal so it points in the correct direction (since vertices are ordered clockwise)
            v3_scale(normal, -1);

            newTriangle->normal[0] = normal[0];
            newTriangle->normal[1] = normal[1];
            newTriangle->normal[2] = normal[2];
            
            scene->scene_objects.push_back(newTriangle);
        }
        // Read triangle information
        else if (strcmp(object_type, "light") == 0)
        {
            Light *newLight = new Light();
            while(fscanf(input_file, " %s ", property_type) == 1 && (strcmp(property_type, ";") != 0))
            {
                float prop1, prop2, prop3;
                if (strcmp(property_type, "position:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newLight->position[0] = prop1;
                    newLight->position[1] = prop2;
                    newLight->position[2] = prop3;
                }
                else if (strcmp(property_type, "color:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newLight->color[0] = prop1;
                    newLight->color[1] = prop2;
                    newLight->color[2] = prop3;
                }
                else if (strcmp(property_type, "radial_a0:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newLight->radial_a0 = prop1;
                }
                else if (strcmp(property_type, "radial_a1:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newLight->radial_a1 = prop1;
                }
                else if (strcmp(property_type, "radial_a2:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newLight->radial_a2 = prop1;
                }
                else if (strcmp(property_type, "theta:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    // Convert degrees to radians
                    prop1 = prop1 * PI / 180;
                    newLight->theta = prop1;
                }
                else if (strcmp(property_type, "angular_a0:") == 0)
                {
                    fscanf(input_file, " %f", &prop1);
                    newLight->angular_a0 = prop1;
                }
                else if (strcmp(property_type, "direction:") == 0)
                {
                    fscanf(input_file, " %f %f %f", &prop1, &prop2, &prop3);
                    newLight->direction[0] = prop1;
                    newLight->direction[1] = prop2;
                    newLight->direction[2] = prop3;
                }
            }
            scene->scene_lights.push_back(newLight);
        }
    }
    return scene;
}

// Function for reading data from a ppm file
Color **ppm_read(char *filename, ppm_metadata *metadata)
{
    // Check that file opened properly
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) 
    {
        fprintf(stderr, "Unable to open file.");
        return NULL;
    }
    // Check that file is the correct type
    char filetype[100];
    fgets(filetype, 3, input_file);
    if (strcmp(filetype, "P3") != 0) 
    {
        fprintf(stderr, "Incorrect file type.");
        fclose(input_file);
        return NULL;
    }
    // Move through file comments
    while (fscanf(input_file, "\n#%s", filetype) == 1)
    {
        fgets(filetype, 100, input_file);
    };

    // Read width, height, and max val
    uint16_t width;
    uint16_t height;
    uint16_t max_val;
    fscanf(input_file, "%hd %hd %hd\n", &width, &height, &max_val);
    metadata->width = width;
    metadata->height = height;
    metadata->max_val = max_val;

    // Read pixel data
    uint16_t pix;
    Color **map = (Color **)malloc(height * sizeof(Color *));
    for (int i = 0; i < height; i++)
    {
        map[i] = (Color *)malloc(width * sizeof(Color));
    }

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++)
        {
            fscanf(input_file, "%hd ", &pix);
            map[i][j].red = pix;
            fscanf(input_file, "%hd ", &pix);
            map[i][j].green = pix;
            fscanf(input_file, "%hd ", &pix);
            map[i][j].blue = pix;
        }
    }
    fclose(input_file);
    return map;
}   

// Function to write to a ppm file
void ppm_write(char *filename, Color **pixmap, ppm_metadata *metadata)
{
    // Open file and print title/metadata
    FILE *output_file = fopen(filename, "w");
    fprintf(output_file, "P3\n%hd %hd\n%hd\n", metadata->width, metadata->height, metadata->max_val);
    
    // Write new pixel map to file
    for (int i = 0; i < metadata->height; i++)
    {
        for (int j = 0; j < metadata->width; j++)
        {
            fprintf(output_file, "%hd\n%hd\n%hd\n", pixmap[i][j].red, pixmap[i][j].green, pixmap[i][j].blue);
        }
    }
    fclose(output_file);

    // Free allocated memory for output pixel map
    for (int i = 0; i < metadata->height; i++)
    {
        free(pixmap[i]);
    }
    free(pixmap);
}

/// Vector Calculations Functions

// Function to construct a vector from points
void v3_from_points(float *dst, float *a, float *b)
{
    for (int i = 0; i < 3; i++)
    {
        dst[i] = b[i] - a[i];
    }
}

// Function to add two vectors
void v3_add(float *dst, float *a, float *b)
{
    for (int i = 0; i < 3; i++)
    {
        dst[i] = a[i] + b[i];
    }
}

// Function to subtract two vectors
void v3_subtract(float *dst, float *a, float *b)
{
    for (int i = 0; i < 3; i++)
    {
        dst[i] = a[i] - b[i];
    }
}

// Function to scale a vector
void v3_scale(float *dst, float s)
{
    for (int i = 0; i < 3; i++)
    {
        dst[i] *= s;
    }
}

// Function to calculate the dot product of two vectors
float v3_dot_product(float *a, float *b)
{
    float dot_product = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    return dot_product;
}

// Calculate the cross product of two vectors
void v3_cross_product(float *dst, float *a, float *b)
{
    dst[0] = a[1] * b[2] - a[2] * b[1];
    dst[1] = a[2] * b[0] - a[0] * b[2];
    dst[2] = a[0] * b[1] - a[1] * b[0];
}

// Function to calculate the length of a vector
float v3_length(float *a)
{
    float length = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    return length;
}

// Calculate the angle between two vectors
float v3_angle(float *a, float *b)
{
    return acos(v3_dot_product(a, b) / (v3_length(a) * v3_length(b)));
}

// Function to normalize a vector
void v3_normalize(float *dst, float *a)
{
    float length = v3_length(a);
    for (int i = 0; i < 3; i++)
    {
        dst[i] = a[i] / length;
    }
}

// Reflect a vector over an axis
void v3_reflect(float *dst, float *v, float *n)
{
    float multiplier = 2 * v3_dot_product(n, v);
    float copy[3] = {n[0], n[1], n[2]};
    v3_scale(copy, multiplier);
    v3_subtract(dst, v, copy);
}

/// Raycasting Function

// Function to determine the color of a single pixel
Color raycast(int img_width, int img_height, int img_x, int img_y, float camera_width, float camera_height, Scene *scene)
{
    // Calculate and normalize origin and destination rays
    float ray_orig[3] = {0, 0, 0};
    float ray_dest[3] = {static_cast<float>(-0.5 * camera_width + (img_x * camera_width) / img_width + 1 / (img_width * 2)), static_cast<float>(0.5 * camera_height - (img_y * camera_height) / img_height + 1 / (img_height * 2)), -1};
    float norm_rd[3] = {0, 0, 0};
    v3_normalize(norm_rd, ray_dest);
            
    // Declare variables for minimum t value and intersected object
    float min_t = INFINITY;
    Object *intersect = NULL;

    // Loop through scene objects
    for (auto object: scene->scene_objects)
    {
        // Cast a ray towards each object
        float new_t = object->intersect(ray_orig, norm_rd);
                
        // If there's an intersection and it's closer than any previously intersected objects, set new values for new t and intersected object
        if (new_t < min_t && new_t >= 0)
        {
            min_t = new_t;
            intersect = object;
        }
    }

    // Create an array to store the outputted color
    Color pixel_color = {0, 0, 0};
    
    // Check if the ray actually intersected with an object
    if (intersect != NULL)
    {
        // Calculate location of intersection
        float ray_to_intersect[3] = {norm_rd[0], norm_rd[1], norm_rd[2]};
        v3_scale(ray_to_intersect, min_t);
        float intersect_point[3] = {0, 0, 0};
        v3_add(intersect_point, ray_to_intersect, ray_orig);

        // Shade pixel using recursive function
        pixel_color = shade(intersect, intersect_point, norm_rd, 0, scene);
    }

    return pixel_color;
}

Color shade(Object *object_hit, float *intersect_point, float *ray_direction, int level, Scene *scene)
{
    // Declare variable to store output color and initialize to black
    Color output_color = {0, 0, 0};

    // if level exceeds maximum recursion level, return black
    if (level > MAX_RECURS_LEVEL)
    {
        return output_color;
    }

    // Declare variable to store the sum of diffuse and specular light contributions to the pixel
    float light_ds[3] = {0, 0, 0};

    // Calculate the view vector
    float view_vector[3] = {ray_direction[0], ray_direction[1], ray_direction[2]};
    v3_scale(view_vector, -1);

    // Determine the normal at the intersection
    float object_normal[3] = {0, 0, 0};
    object_hit->get_normal(intersect_point, object_normal);

    // Loop through lights present in the scene
    for (Light *light: scene->scene_lights)
    {
        // Construct and normalize a ray from the intersection towards the light
        float ld[3] = {0, 0, 0};
        float ld_norm[3] = {0, 0, 0};
        bool shadow = false;
        v3_from_points(ld, intersect_point, light->position);
        v3_normalize(ld_norm, ld);

        // Calculate the distance from the intersection to the light
        float light_distance = v3_length(ld);

        // Loop through scene objects
        for (auto object: scene->scene_objects)
        {
            // Ensure object currently being checked is not the intersected object
            if (object != object_hit)
            {
                // If the ray cast toward the light from the intersection intersects with another object, the light is in shadow and should be disregarded
                float t = object->intersect(intersect_point, ld_norm);
                if (t >= 0 && t < light_distance)
                {
                    shadow = true;
                    break;
                }
            }
        }
        // If the light is not in shadow, illuminate the scene
        if (!shadow)
        {
            // Calculate the dot product of the light vector and object normal
            float diffuse_dp = v3_dot_product(ld_norm, object_normal);
            // If it is not positive, the light can be disregarded as it does not illuminate the object
            if (diffuse_dp > 0)
            {
                float object_color[3] = {0, 0, 0};
                if (object_hit->texture_map == NULL)
                {
                    object_color[0] = object_hit->c_diff[0]; 
                    object_color[1] = object_hit->c_diff[1]; 
                    object_color[2] = object_hit->c_diff[2]; 
                }
                else
                {
                    Color texture_color = object_hit->get_texture_color(intersect_point);
                    object_color[0] = static_cast<float>(texture_color.red) / 255;
                    object_color[1] = static_cast<float>(texture_color.green) / 255;
                    object_color[2] = static_cast<float>(texture_color.blue) / 255;
                }
                // Calculate diffuse contribution using diffuse dot product
                float I_diff[3] = {object_color[0] * light->color[0] * diffuse_dp, object_color[1] * light->color[1] * diffuse_dp, object_color[2] * light->color[2] * diffuse_dp};

                // Calculate specular contribution using view vector and reflection vector
                float reflection[3] = {0, 0, 0};
                v3_reflect(reflection, ld_norm, object_normal);
                float I_spec[3] = {object_hit->c_spec[0] * light->color[0] * static_cast<float>(pow(v3_dot_product(view_vector, reflection), 20)), object_hit->c_spec[1] * light->color[1] * static_cast<float>(pow(v3_dot_product(view_vector, reflection), 20)), object_hit->c_spec[2] * light->color[2] * static_cast<float>(pow(v3_dot_product(view_vector, reflection), 20))};

                // Calculate radial attenuation
                float f_rad = 1 / (light->radial_a0 + light->radial_a1 * light_distance + light->radial_a2 * light_distance * light_distance);

                // Determine angular attenuation
                float f_ang;
                float v_orig[3] = {ld_norm[0], ld_norm[1], ld_norm[2]};
                v3_scale(v_orig, -1);
                float spotlight_dp = v3_dot_product(v_orig, light->direction);
                // If the angular exponent is zero, the light is a point light and no angular attenuation occurs
                if (light->angular_a0 == 0)
                {
                    f_ang = 1;
                }
                // If the intersection point is outside of the cone of a spot light, the light does not illuminate it
                else if (cos(light->theta) < spotlight_dp)
                {
                    f_ang = 0;
                }
                // Otherwise, calculate angular attenuation
                else
                {
                    f_ang = pow(spotlight_dp, light->angular_a0);
                }

                // Calculate the total contribution the light makes to each color of the pixel and add these contributions to the output color
                light_ds[0] += f_rad * f_ang * (I_diff[0] + I_spec[0]);
                light_ds[1] += f_rad * f_ang * (I_diff[1] + I_spec[1]);
                light_ds[2] += f_rad * f_ang * (I_diff[2] + I_spec[2]);
            }
        }
    }
  
    // Multiply by 255 to determine the correct colors
    v3_scale(light_ds, 255);

    Color light_reflected = {0, 0, 0};

    // Only perform reflection-related code if current object hit is reflective
    if (object_hit->reflection != 0)
    {
        // Compute the reflection of the ray over the normal at the point where it intersects the object
        float reflected_ray[3] = {0, 0, 0};
        v3_reflect(reflected_ray, ray_direction, object_normal);
        v3_normalize(reflected_ray, reflected_ray);

        // Declare variables for minimum t value and intersected object
        float min_t = INFINITY;
        Object *new_object = NULL;

        // Loop through scene objects
        for (auto object: scene->scene_objects)
        {
            if (object != object_hit)
            {
                // Cast a ray towards each object
                float new_t = object->intersect(intersect_point, reflected_ray);
                        
                // If there's an intersection and it's closer than any previously intersected objects, set new values for new t and intersected object
                if (new_t < min_t && new_t >= 0)
                {
                    min_t = new_t;
                    new_object = object;
                }
            }
        }

        // If the reflected ray hits another object, recursively call the shade function on the object
        if (new_object != NULL)
        {
            // Calculate new location of intersection on object hit by the reflected ray
            float ray_to_intersect[3] = {reflected_ray[0], reflected_ray[1], reflected_ray[2]};
            v3_scale(ray_to_intersect, min_t);
            float new_intersect[3] = {0, 0, 0};
            v3_add(new_intersect, ray_to_intersect, intersect_point);

                // Recursively call the shade function
                light_reflected = shade(new_object, new_intersect, reflected_ray, level + 1, scene);
        }
    }

    // Determine coefficients for reflected and diffuse/specular light
    float k1 = object_hit->reflection;
    float k0 = 1.0 - k1;

    // Calculate ultimate color of light
    Color return_color = {static_cast<uint16_t>(k0 * light_ds[0] + k1 * light_reflected.red), static_cast<uint16_t>(k0 * light_ds[1] + k1 * light_reflected.green), static_cast<uint16_t>(k0 * light_ds[2] + k1 * light_reflected.blue)};

    // Ensure no color exceeds 255
    if (return_color.red > 255)
    {
        return_color.red = 255;
    }
    if (return_color.green > 255)
    {
        return_color.green = 255;
    }
    if (return_color.blue > 255)
    {
        return_color.blue = 255;
    }

    return return_color;
}