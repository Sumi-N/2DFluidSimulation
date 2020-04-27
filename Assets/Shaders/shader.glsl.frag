#version 430 core

out vec4 color;

in vec2 texcoord;

layout(binding = 0) uniform sampler2D texture;
layout(binding = 1) uniform sampler2D velocitytexture;


vec2 advect(vec2 coords, float timestep, float rdx, sampler2D u) 
{
  // follow the velocity field "back in time"
    vec2 pos = coords - timestep * rdx * vec2(texture2D(u, coords));
  // interpolate and write to the output fragment
    //vec4 xNew = f4texRECTbilerp(x, pos);
    return pos;
}

// void jacobi(vec2 coords,   : WPOS,   // grid coordinates

//    out
//    half4 xNew : COLOR,  // result

//    uniform
//    half alpha,
//             uniform
//    half rBeta,      // reciprocal beta

//    uniform samplerRECT x,   // x vector (Ax = b)

//    uniform samplerRECT b)   // b vector (Ax = b)
// {
//   // left, right, bottom, and top x samples

//   half4 xL = h4texRECT(x, coords - half2(1, 0));
//   half4 xR = h4texRECT(x, coords + half2(1, 0));
//   half4 xB = h4texRECT(x, coords - half2(0, 1));
//   half4 xT = h4texRECT(x, coords + half2(0, 1));

//   // b sample, from center

//    half4 bC = h4texRECT(b, coords);

//   // evaluate Jacobi iteration
//   xNew = (xL + xR + xB + xT + alpha * bC) * rBeta;
// }

void main()
{
    //color = vec4(1.0, 0, 0, 1.0);
    color = texture2D(texture, texcoord);
}