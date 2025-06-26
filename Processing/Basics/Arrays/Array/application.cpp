/**
 * Array. 
 * 
 * An array is a list of data. Each piece of data in an array 
 * is identified by an index number representing its position in 
 * the array. Arrays are zero based, which means that the first 
 * element in the array is [0], the second element is [1], and so on. 
 * In this example, an array named "coswave" is created and
 * filled with the cosine values. This data is displayed three 
 * separate ways on the screen.  
 */

#include "Umfeld.h" // @umfeld(include)

using namespace umfeld;

float* coswave; // @umfeld(arrays are declared slightly differntly in Processing `float[] coswave`)

void settings() {
    size(640, 360);
}

void setup() {
    coswave = new float[width];
    for (int i = 0; i < width; i++) {
        float amount = map(i, 0, width, 0, PI);
        coswave[i]   = abs(cos(amount));
    }
    background(1.0); // @umfeld(color_range)
    noLoop();
}

void draw() {
println("draw() called");
    int y1 = 0;
    int y2 = height / 3;
    for (int i = 0; i < width; i++) {
        stroke(coswave[i]); // @umfeld(color_range)
        line(i, y1, i, y2);
    }

    y1 = y2;
    y2 = y1 + y1;
    for (int i = 0; i < width; i++) {
        stroke(coswave[i] / 4); // @umfeld(color_range)
        line(i, y1, i, y2);
    }

    y1 = y2;
    y2 = height;
    for (int i = 0; i < width; i++) {
        stroke(1.0 - coswave[i]); // @umfeld(color_range)
        line(i, y1, i, y2);
    }
}
