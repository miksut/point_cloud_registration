# Computer Graphics: Local Point Cloud Registration

This repository hosts an implementation of the [Iterative Closest Point (ICP)](https://ieeexplore.ieee.org/document/4767965/keywords#keywords) algorithm for the local registration of two roughly aligned point clouds. The project has been completed as part of a lecture offered by the [Visualization and MultiMedia Lab](https://www.ifi.uzh.ch/en/vmml/teaching.html) in the Department of Informatics at the University of Zurich, Switzerland.

---
## Project Description
Given a reference point cloud of the [armadillo model](https://datarepository.wolframcloud.com/resources/Stanford-Armadillo/) and a roughly aligned partial point cloud of the same model, the ICP algorithm's goal is to accurately register the partial point cloud to the reference point cloud. Concretely, the alignment transformation is computed in an iterative way. After each iteration, the two models are more closely aligned. The application shows the progress of this alignment process via a slider in the GUI. 

In each iteration, the ICP algorithm performs the following tasks:
- Discovering the corresponding points by performing nearest neighbor queries
- Computing the optial alignment transformation (rotation and translation) based on the current correspondences
- Calculating the current registration error

The application itself builds upon a framework provided by the Visualization and MultiMedia Lab. The following image shows an exemplary rendering.

<img
    src="/res/point_clouds.png"
    height="250"
    align="left">

---

## Installation and Setup
This program has the following dependencies:

 1. OpenGL
 2. GLEW
 3. GLFW
 4. FreeImage
 5. Assimp

### Dependencies
OpenGL should already exist on the platform. The Assimp source code is included in this repository and will be built the first time when building this project.

**on Mac:**

Install the remaining three dependencies using homebrew:
```
brew install glew
brew install glfw3
brew install freeimage
```

**on Windows:**
All dependencies are included in this repository. When running CMake, choose "Visual Studio xx WIN64" or choose "Visual Studio xx" as the target platform. In the second case, make sure to definitely choose x64 in the next field.

**on Linux:**

Install the remaining three dependencies using apt-get:
```
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libfreeimage3 libfreeimage-dev
```

### Build
After installing the dependencies, use CMake to build the project. Make sure that the build folder is inside the project folder:

```
[project root]
	|-- build
	|-- CMake
	|-- doc
	|-- external
	|-- res
	|-- shader
	|-- src
	
```

In case of encountering an error when compiling `kdtree_index.h` in the FLANN library, add the following line:
```
#include <cmath>
```
In case of encountering an error when compiling `serialization.h` in the FLANN library, add the following lines (around line 92 where there are similar codes):
```
#ifdef _MSC_VER
BASIC_TYPE_SERIALIZER(unsigned __int64);
#endif
```
---