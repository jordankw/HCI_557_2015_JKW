#version 410 core                                                 


uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D tex;

in vec2 pass_TexCoord;                                                          
in vec3 pass_Color;                                               
out vec4 color;   
//uniform int texture_blend;
                                       
void main(void)                                                   
{                     
	//vec4 pass4 = vec4(pass_Color, 1.0);
	
	//vec4 t1 = texture(texture1, pass_TexCoord);
	//vec4 t2 = texture(texture2, pass_TexCoord);
	//vec4 t3 = texture(texture3, pass_TexCoord);

	//vec4 tex_color = texture(texture2, pass_TexCoord);
	vec4 tex_color = texture(texture1, pass_TexCoord) * 0.33f + texture(texture2, pass_TexCoord) * 0.33f + texture(texture3, pass_TexCoord) * 0.44f;
	//vec4 tex_color = texture(texture1, pass_TexCoord) / texture(texture2, pass_TexCoord) / texture(texture3, pass_TexCoord);
	//color = mix(t1, t2, t3);                                          
    color = tex_color;                   
} 