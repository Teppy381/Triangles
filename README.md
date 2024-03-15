
<!-- PROJECT LOGO -->
<br />
<div align="center">
  <!-- <a href="https://github.com/Teppy381/Triangles">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a> -->

  <h1 align="center">Triangles</h1>
</div>


## About The Project

This project is a task on course "Uses and applications of C++ language" by K.I.Vladimirov. The task was to check which triangles intersect in 3D space.

What it does:
1) Takes from console set of points and interprets them as triangles
2) Opens window with 3D view of these triangles, colored red if they intersect with others, and blue otherwise
3) You can move and rotate camera to get a better look

### Built With

* ![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=for-the-badge)
* ![CMake Badge](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=fff&style=for-the-badge)
* ![Vulkan Badge](https://img.shields.io/badge/Vulkan-AC162C?logo=vulkan&logoColor=fff&style=for-the-badge)


<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

You will need to install [Vulkan SDK](https://vulkan.lunarg.com/)

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/Teppy381/Triangles.git
   cd Triangles
   ```
2. Build a project
   ```sh
   cmake -B build
   ```
   At this point it might take a while to download dependencies (glfw, glm, gTest)

   ```sh
   cmake --build build
   ```

### Unit and E2E tests
*
   ```sh
   ctest --test-dir build
   ```


<!-- USAGE EXAMPLES -->
## Usage

* Launch from console
   ```sh
   build/visualization/vulkan_triangles.exe
   ```

* For a showcase you can use files used in tests to not type triangles manually
   ```sh
   build/visualization/vulkan_triangles.exe < "tests/E2E_tests/flower.txt"
   ```

* Controls:\
    Move Left = `A`\
    Move Right = `D`\
    Move Forward = `W`\
    Move Backward = `S`\
    Move Up = `SPACE`\
    Move Down = `LEFT_ALT`

    (Free rotating camera works only in freeCam mode)\
    Look Left = `LEFT`\
    Look Right = `RIGHT`\
    Look Up = `UP`\
    Look Down = `DOWN`

    Accelerate = `LEFT_SHIFT`\
    Go Home = `BACKSPACE`

    Switch Camera Mode = `1`, `2`
