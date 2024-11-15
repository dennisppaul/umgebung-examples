# Install Dear ImGui

1. Clone docking branch of ImGui\
   (It is recommended to put imgui next to umgebung)
    ```
    - [your-app]
    - umgebung/
    - imgui/
    ```
    ```shell
    git clone --single-branch --branch docking https://github.com/ocornut/imgui.git
    ```
3. Adjust `IMGUI_PATH` in CMakeLists.txt