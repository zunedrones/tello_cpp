# Sobre
Biblioteca para enviar comandos ao tello, transmitir vídeo e informações do drone. Foi utilizado como base o repositório disponível em: https://github.com/carlospzlz/ctello

# Configurações do OpenCV
- Compilação via CMake
- Backend: Wayland
- Bibliotecas:
```
sudo apt-get install build-essential cmake git pkg-config \
                     libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev \
                     libv4l-dev libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
                     gfortran openexr libatlas-base-dev python3-dev python3-numpy \
                     libtbb2 libtbb-dev libdc1394-22-dev libopenexr-dev \
                     libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev \
                     wayland-protocols

```
- Compilação:
```
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D INSTALL_C_EXAMPLES=OFF \
      -D INSTALL_PYTHON_EXAMPLES=OFF \
      -D ENABLE_FAST_MATH=ON \
      -D BUILD_opencv_java=OFF \
      -D BUILD_ZLIB=ON \
      -D BUILD_TIFF=ON \
      -D WITH_GTK=ON \
      -D WITH_FFMPEG=ON \
      -D WITH_1394=ON \
      -D OPENCV_GENERATE_PKGCONFIG=ON \
      -D OPENCV_PC_FILE_NAME=opencv4.pc \
      -D OPENCV_ENABLE_NONFREE=ON \
      -D WITH_GSTREAMER=ON \
      -D WITH_V4L=ON \
      -D WITH_WAYLAND=ON \
      -D WITH_OPENGL=ON \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
```

# Como utilizar
## Via CMake Tools no VS Code:
1. Ctrl+Shift+p: Quick Start
2. Ctrl+Shift+p: Selecionar Kit GCC ou Clang
3. Ctrl+Shift+p: Configure
4. Compilar

## Via terminal:
- ~/tello_cpp$
1. mkdir build
2. cd build
3. cmake ..
4. make -j
5. sudo make install
