# OpenGL Playground

Inspired by the [ILLIXR](https://github.com/ILLIXR) project

## Docker Container- Recommended
This simple application has a bunch of dependencies, so a docker image
is built for it. 

 1. Expose your xhost to Docker `xhost +local:root`
 2. Pull the image, create a container, and start it

    `sudo docker run --privileged --name ubuntuGL -e "DISPLAY=${DISPLAY}" --hostname ubuntuGL -it -v /tmp/.X11-unix:/tmp/.X11-unix --gpus all ystozlu/ubuntu_gl /bin/bash`

 3. Run the application `cd /home/OpenGL_VR && ./main`

Two windows should appear. Use WASD to move and mouse buttons to look around.