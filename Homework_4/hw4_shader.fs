#version 410 core                                                 

uniform sampler2D texture2;

uniform sampler2D tex;

in vec2 pass_TexCoord;                                                          
in vec3 pass_Color;                                               
out vec4 color;   
//uniform int texture_blend;
                                       
void main(void)                                                   
{                     

	vec4 tex_color = texture(texture2, pass_TexCoord);
                                       
    color = tex_color;                   
} 