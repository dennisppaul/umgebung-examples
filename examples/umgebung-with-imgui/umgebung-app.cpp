#include "Umgebung.h"
#include "UmgebungImGui.h"

using namespace umgebung;

class UmgebungApp : public PApplet {

    PShape mShape;
    int    mWidth  = 1024;
    int    mHeight = 768;

    int   mVerticalPosition   = 0;
    int   mHorizontalPosition = 0;
    bool  mShowDemoWindow     = false;
    float mColor[3]           = {1.0, 0.5f, 0.0};

    void arguments(std::vector<std::string> args) override {
        for (std::string& s: args) {
            println("> ", s);
            if (begins_with(s, "--width")) {
                mWidth = get_int_from_argument(s);
            }
            if (begins_with(s, "--height")) {
                mHeight = get_int_from_argument(s);
            }
        }
    }

    void settings() override {
        imgui_font(sketchPath() + "../RobotoMono-Regular.ttf", 12); // set custom font + size
        size(mWidth, mHeight);
        monitor               = 0;
        fullscreen            = false;
        borderless            = false;
        antialiasing          = 8;
        resizable             = false;
        always_on_top         = false;
        enable_retina_support = true;
        headless              = false;
        no_audio              = true;
    }

    void setup() override {
        println("width : ", width);
        println("height: ", height);
    }

    void draw() override {
        background(1);
        ImGui::Begin("Umgebung");
        ImGuiColorEditFlags flags = 0;
        flags |= ImGuiColorEditFlags_NoOptions;
        flags |= ImGuiColorEditFlags_NoSidePreview;
        ImGui::Text("You can drag this window outside the frame.");
        ImGui::ColorPicker3("Color", mColor, flags);
        ImGui::SliderInt("X", &mHorizontalPosition, -50, 50);
        ImGui::SliderInt("Y", &mVerticalPosition, -50, 50);
        ImGui::Checkbox("Show ImGui Demo Window", &mShowDemoWindow);
        ImGui::End();
        if (mShowDemoWindow) {
            ImGui::ShowDemoWindow();
        }

        translate(width / 2, height / 2);
        constexpr int s = 100;
        auto& [r, g, b] = mColor;
        fill(1 - r, 1 - g, 1 - b);
        ellipse(-mHorizontalPosition, -mVerticalPosition, 100, 100);
        fill(r, g, b);
        ellipse(mHorizontalPosition, mVerticalPosition, 100, 100);

        noFill();
        line(0, -s, 0, s);
        line(-s, 0, s, 0);
    }

    void keyPressed() override {
        if (key == 'q') {
            exit();
        }
        println((char) key, " pressed");
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}