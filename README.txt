
This project depends on CMake to generate build files. Use this command to generate files to build Daguerreo for 32-bit using OpenGL 2.1:
    cmake -DUSE_OPENGL21=ON -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 <src>
        -> where <src> is the Daguerreo src directory.
