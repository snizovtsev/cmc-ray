varying vec2 pixel;

void main(void)
{
    gl_Position = gl_Vertex;
    pixel = vec2(gl_Vertex);
}
