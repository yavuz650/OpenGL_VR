# OpenGL Playground

Inspired by the [ILLIXR](https://github.com/ILLIXR) project

## Docker Container- Recommended
This simple application has a bunch of dependencies, so a docker image
is built for it. 

 1. Expose your xhost to Docker `xhost +local:root`
 2. Pull the image, create a container, and start it

    `sudo docker run --privileged --name ubuntuGL -e "DISPLAY=${DISPLAY}" --hostname ubuntuGL -it -v /tmp/.X11-unix:/tmp/.X11-unix ystozlu/ubuntu_gl /bin/bash`

    If you allowed Docker to access your discrete GPU, i.e. by installing NVIDIA Container Toolkit (or AMD equivalent of it), you can use your discrete GPU as your OpenGL renderer. Use the following command to do that instead,

    `sudo docker run --privileged --name ubuntuGL -e "DISPLAY=${DISPLAY}" --hostname ubuntuGL -it -v /tmp/.X11-unix:/tmp/.X11-unix --gpus all ystozlu/ubuntu_gl /bin/bash`

 3. Once in the container, run the application `cd /home/OpenGL_VR && ./main`

Two windows should appear. Use WASD to move and mouse buttons to look around.
