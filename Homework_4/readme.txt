Jordan Williams
HCI 557
Homework 4


I used the same project for both parts of the assignment. In hw4_main.cpp I load either hw_shader.fs or hw_shader_part2.fs. hw_shader.fs displays one texture on the plane, and hw_shader.fs displays two. I tried 2 methods of blending for part 2. One adds the color values from each image, and the other divides texture 1 values by texture 2 values, then divides again by the texture 3 value. I used the same texture coordinates for each texture as well.

The most difficulty I had was with loading textures to the GPU. I thought that malloc() in the image load function would return pointers to different locations in memory when loading multiple textures, when it actually kept returning the same memory address. Because of this my options were to copy the data referenced by the pointer after each texture was loaded, or to point the function loading data to the gpu directly to the address returned by Load(). This way my loop would point to the same location each time, but load different textures.
