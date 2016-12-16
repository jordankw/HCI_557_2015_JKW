#version 410 core                                                 
uniform sampler2D texture1;
in vec2 pass_TexCoord;
in vec4 pass_Color;
out vec4 color;   

void main(void)                                                   
{                
	vec4 tex_color = texture(texture1, pass_TexCoord) * 0.65 + (0.5 * pass_Color); 
	tex_color.w = 1;
	//vec4 tex_color = texture(texture1, pass_TexCoord) * pass_Color;                                              
    color = tex_color;
	//color = vec4(0,0,0,1);
}                           