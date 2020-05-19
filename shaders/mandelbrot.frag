#version 410
in vec4 fPos;

uniform vec2 centerPos;
uniform float zoom;

uniform mat4 proj;
uniform float width;
uniform float height;

uniform int colorPermute;
uniform bool invertColor;

uniform bool julia;
uniform bool z2;
uniform bool z3;
uniform bool z4;
uniform bool z5;
uniform bool maxDistColor;
float currentDistance_sqrd;
uniform int maxit;
float threshold = 4.0f;

out vec4 FragColor;

vec4 tc;



// color function that is stole from someone on github
//https://github.com/gendestry/Mandelbrot/
vec4 map_to_color(float t) {
    

    float r = 10.0 * (1.0 - t) * t * t * t;
    float g = 17.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 12.0 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;


    if(invertColor){
        r=1.0-r;
        g = 1.0-g;
        b = 1.0-b;
    }
    if(colorPermute ==0)
        return vec4(r, g, b, 1.0f);
    else if(colorPermute ==1)
        return vec4(b, r, g, 1.0f);
    else if(colorPermute==2)
        return vec4(g,b,r,1.0f);
    else if(colorPermute ==3)
        return vec4(g, r, b, 1.0f);
    else if(colorPermute==4)
        return vec4(b, g, r, 1.0f);
    else if(colorPermute==5)
        return vec4(r, b, g, 1.0f);
    else if(colorPermute==6)
        return vec4(r,b,g,1.0f);

}	


void main()
{


    // combination of matrix multiplication with proj and scaling with width and height -> constant aspect ratio
    vec4 Pos = proj*fPos;

    // cx and cy are the pixel coordinates, they are used as input to mandelbrot formula z_n+1 = z_n^2 + c;
    // x component represents the real part, y component the imaginary part
    double cx = width/height*Pos.x*zoom + centerPos.x;
    double cy = height/width*Pos.y*zoom + centerPos.y;          // c is translated according to the WASD input


    // mandelbrot iteration, x is the real part of z, y is the imaginary part of z
    double x=0;
    double y=0;

    double x_new;   //temporary variable 
 
    uint iteration = 0;
    double maxDist = 0.0f;
    double meanDist = 0.0f;
    while (iteration < maxit)
    {
        // calculate new x and y
        
        // julia
        if(julia){
            if(iteration == 0){
            x=cx;
            y=cy;
            }
            x_new = x*x - y*y - 0.6;
            y = x*y + y*x + 0.6;
            x = x_new;
        };


        if(z5){
        // z^5
            double x1 = x*x - y*y;
            double y1 = 2*x*y;
            double x2 = x*x*x - 3*y*y*x;
            double y2 = -y*y*y + 3*x*x*y;
            x = x1*x2 - y1*y2 + cx;
            y = x1*y2 + y1*x2 + cy;
        };

        if(z4){
        // z^4
            double x_prime = x*x - y*y;
            double y_prime = x*y + y*x;
            x = x_prime*x_prime - y_prime*y_prime +cx;
            y = x_prime*y_prime + y_prime*x_prime + cy;
        };

        if(z3){
        // z^3
        x_new = x*x*x - 3*y*y*x + cx;
        y = -y*y*y + 3*x*x*y + cy;
        x = x_new;
        };

        // mandelbrot
        if(z2){
            x_new = x*x - y*y + cx;
            y = x*y + y*x + cy;
            x = x_new;
        };


        double norm_sqrd = x*x + y*y;

        maxDist = max(sqrt(norm_sqrd), maxDist);
        meanDist += norm_sqrd;
        // if point in complex plane is a threshold distance away from the origin, terminate the loop
        if (norm_sqrd > threshold)
            break;

        iteration++;
    }

    float Dist = float(maxDist);

    if(!maxDistColor)
        FragColor = map_to_color((iteration/float(maxit)));
    else
        
        tc = map_to_color((iteration/float(maxit)));

        currentDistance_sqrd = pow(float(cx),2) + pow(float(cy),2);

        if(tc.x + tc.y + tc.z >0.001f || currentDistance_sqrd >4.0f)
            FragColor = tc;
        else
            FragColor = map_to_color(Dist/10.0f);
}