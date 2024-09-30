### Lab class exercises for INF585 Computer Animation
## Download
### Clone
```
git clone --recurse-submodules https://github.com/RStevensonR/INF585-CSC51085EP.git
```
or
```
git clone https://github.com/RStevensonR/INF585-CSC51085EP.git
cd INF585-CSC51085EP/
git submodule init
git submodule update
```
Note: Do not use the "Code -> Download ZIP" option from the GitHub GUI as you won't get the CGP library as submodule with the code. If you use it, you have to download the library manually (source link at the end).
## Compilation

Scenes are in a directory `INF585-CSC51085EP/0*_*/`

Each scene is provided with its own CMakeLists.txt and Makefile

Compilation via CMake
```
cd INF585-CSC51085EP/01_introduction/
cmake -B build/
cd build/
make -j8 # or any -jN, with N=$(nproc)
./01_introduction
```
Compilation via Makefile
```
cd INF585-CSC51085EP/01_introduction/
make -j8 # or any -jN, with N=$(nproc)
./00_introduction
```
More details are provided in the dedicated [Installation setup and Compilation page](https://graphicscomputing.fr/cgp/compilation/content/01_compilation/index.html) if needed (Windows, Linux, Mac).
Requirement

The code requires the GLFW library in Linux and Mac.
Linux (/Ubuntu)
```
sudo apt-get install libglfw3-dev
```
MacOS:

- You may use [brew](https://brew.sh/) to install [glfw](https://formulae.brew.sh/formula/glfw). And then add the following two lines on your .zshrc to add glfw library to the path
```
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
```
- Alternatively, a pre-compiled glfw can be used without installation. To activate it, open the CMakeLists.txt file and set the option MACOS_GLFW_PRECOMPILED to ON. You may need to explicitely allow the mac to use the library if security triggers an alert.

Windows:
- The simplest way is the use of [Visual Studio](https://visualstudio.microsoft.com/es/downloads/) with the `scripts/windows_cmake_visual.bat` generating a visual studio project.

## IDE

The full edition/compilation/debug toolchain should work with the following IDE:

- [Visual Studio Code/vscode](https://code.visualstudio.com/) on Linux and MacOS: You may open a project with the dedicated configuration file vscode.code-workspace.
- [CLion](https://www.jetbrains.com/clion/) on any plateform. Note that as student you are elligible for a free licence to this IDE. A project can be loaded directly from the CMakeLists.txt.
- [QtCreator](https://www.qt.io/product/development-tools) on Linux and MacOS. Install it via your package manager (apt/brew). A project can be loaded directly from the CMakeLists.txt (you may have to indicate to build in RelWithDebInfo instead of the default Debug).
- [Visual Studio](https://visualstudio.microsoft.com/es/downloads/) on Windows only. Free for individuals.

## Scripts

You may use the provided scripts (in directory scripts/) and configuration files to ease some of the compilation process

- `vscode.code-workspace`: workspace configuration file for Visual Code Studio (vscode).
- `scripts/linux_compile_run_cmake.py`: Run CMake, compile and run the code on Linux and MacOS. linux_clean.py remove temporary files.
- `scripts/windows_cmake_visual.bat`: On Windows, call CMake and generate a Visual Studio project. CMake must be accessible on the command line path. scripts/windows_clean.bat remove the temporary files and the visual studio project.
- `scripts/linux_compile_emscripten.py`: Generate a webpage runing the code of the scene. The script can run on Linux and MacOS if emscripten is installed.

## Links

- [Source code CGP Library](https://github.com/drohmer/cgp)
- [CGP Library](https://graphicscomputing.fr/cgp/documentation/01_general/index.html) 
- [C++ Basic exercise (FR) *Working on the EN version](https://imagecomputing.net/course/2023_2024/inf443/lab/content/02/b_cpp_bases/index.html)

