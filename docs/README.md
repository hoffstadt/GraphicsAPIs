## Dear Py3D

[![build status](https://github.com/hoffstadt/DearPy3D/workflows/Windows%20Builds/badge.svg?branch=master)](https://github.com/hoffstadt/DearPy3D/actions?workflow=Windows%20Builds)

This repo is the working location of the eventual [Dear PyGui](https://github.com/hoffstadt/DearPyGui) 3D Engine.

This project is still early and parts of it are being ported over from earlier prototypes.


## Building
### Windows
**Requirements**
- [_git_](https://git-scm.com/)
- [_Visual Studio 2019_ (windows)](https://visualstudio.microsoft.com/vs/) with the following workflows:
  * Python development
  * Desktop development with C++
  * Game development with C++
- [Vulkan SDK](https://vulkan.lunarg.com/)

Instructions:
1. From within a local directory, enter the following bash commands:
```
git clone --recursive https://github.com/hoffstadt/DearPy3D
```
4. In Visual Studio, use _Open a local folder_ to open _DearPy3D_ folder. This will use CMakeLists.txt as the project file.
5. Run _Build All_.

## Gallery
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_3.png)
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_1.png)
![](https://github.com/hoffstadt/DearPyGui/blob/assets/3d_demo_2.png)
