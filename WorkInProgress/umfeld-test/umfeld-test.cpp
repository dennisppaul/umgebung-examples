#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
}

void setup() {
    umfeld_image = loadImage("../umfeld.png");
    rectMode(CENTER);
}

void draw() {
    background(0.85f);

    pushMatrix();
    stroke(1.0f);
    fill(0.5f, 0.85f, 1.0f);
    translate(width * 0.33, height * 0.5f, 0);
    rotateX(mouseY * 0.03);
    rotateY(mouseX * 0.07);
    box(width * 0.25f);
    popMatrix();

    pushMatrix();
    stroke(1.0f);
    fill(1.0f, 0.25f, 0.35f);
    translate(width * 0.66, height * 0.5f, 0);
    rotateX(mouseY * 0.05f);
    rotateY(mouseX * 0.05f);
    sphereDetail(mouseX / 40);
    sphere(width * 0.125f);
    popMatrix();

    fill(1.0f);
    image(umfeld_image, mouseX, mouseY);
    noFill();
    stroke(0.0f);
    strokeWeight(10);
    circle(mouseX, mouseY, 512.0f);
    strokeWeight(1);
}