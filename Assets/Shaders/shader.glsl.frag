#version 430 core

out vec4 color;

in vec2 texcoord;

const float TIME_STAMP = 0.1f;

layout(binding = 0) uniform sampler2D graphictexture;
layout(binding = 1) uniform sampler2D velocitytexture;
layout(binding = 2) uniform sampler2D pressuertexture;
 




// rdx is one grid scale
// The parameter u is the velocity field texture.
// x is the field that is to be advected. This could be the velocity.

vec4 advect(vec2 coords, float timestep, float rdx, sampler2D u, sampler2D x) 
{
  // follow the velocity field "back in time"
    vec2 pos = coords - timestep * rdx * texture2D(u, coords).rg;
  // interpolate and write to the output fragment
    vec4 color = texture2D(x, pos);
    return color;
}

vec4 jacobi(vec2 coords, float alphe, float rBeta, sampler2D x, sampler2D b){

     // left, right, bottom, and top x samples

    vec4 xL = texture2D(x, coords - vec2(1, 0));
    vec4 xR = texture2D(x, coords + vec2(1, 0));
    vec4 xB = texture2D(x, coords - vec2(0, 1));
    vec4 xT = texture2D(x, coords + vec2(0, 1));

    // b sample, from center

    vec4 bC = texture2D(b, coords);

    vec4 color =  (xL + xR + xB + xT + alphe * bC) * rBeta;

    return color;
}

float divergence(vec2 coords, float halfrdx, sampler2D w)  // vector field
{
  vec4 wL = texture2D(w, coords - vec2(1, 0));
  vec4 wR = texture2D(w, coords + vec2(1, 0));
  vec4 wB = texture2D(w, coords - vec2(0, 1));
  vec4 wT = texture2D(w, coords + vec2(0, 1));

  float div = halfrdx * ((wR.x - wL.x) + (wT.y - wB.y));
  return div;
}

vec4 gradient(vec2 coords, float halfrdx, sampler2D p, sampler2D w)
{
  float pL = texture2D(p, coords - vec2(1, 0)).r;
  float pR = texture2D(p, coords + vec2(1, 0)).r;
  float pB = texture2D(p, coords - vec2(0, 1)).r;
  float pT = texture2D(p, coords + vec2(0, 1)).r;

  vec4 uNew = texture2D(w, coords);
  uNew.xy -= halfrdx * vec2(pR - pL, pT - pB);

  return uNew;
}

// The x parameter represents the texture (velocity or pressure field) from which we read interior values.
// The offset parameter contains the correct offset to the interior cells adjacent to the current boundary.
// The coords parameter contains the position in texture coordinates of the fragment being processed.
// For velocity boundaries, scale is set to -1, and for pressure it is set to 1.
vec4 boundary(vec2 coords, vec2 offset, float scale, sampler2D x)
{
  vec4 bv = scale * texture2D(x, coords + offset);
  return bv;
}

void main()
{
    color = texture2D(graphictexture, texcoord);
    // vec2 velocity = texture2D(velocitytexture, texcoord).xy;
    // if(velocity.x > 0.5 || velocity.y > 0.5){
    //   color = texture2D(velocitytexture, texcoord);
    // }else{
    //   color = vec4(0, 0, 0, 1.0);
    // }

    //float resultcolor = abs(texture2D(graphictexture, texcoord).x);
    //color = vec4(resultcolor, 0, 0, 1.0);

    //vec2 velocity = abs(texture2D(graphictexture, texcoord).xy);
    //color = vec4(velocity, 0, 1.0);


    // // Get current texture
    // vec4 u = texture2D(graphictexture, texcoord);

    // // Apply the first 3 operators in Equation 12.

    // u = advect(texcoord, TIME_STAMP, 1, graphictexture, velocitytexture);

    // u = diffuse(u);
    // u = addForces(u);
    // // Now apply the projection operator to the result.
    // p = computePressure(u);
    // u = subtractPressureGradient(u, p);
}