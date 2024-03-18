cmake_minimum_required(VERSION 3.10...3.27)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

#set project name
project(pinctrl)

#add executables
#set(PINCTRL_SRCS
#    pinctrl.c
#    gpiolib.cpp
#    util.c
#    gpiochip_bcm2835.c
#    gpiochip_bcm2712.c
#    gpiochip_rp1.c
#)
# add_executable(pinctrl ${PINCTRL_SRCS})

set(PINCTRL2_SRCS
    pinctrl2.cpp
    gpiolib.cpp
    util.cpp
    gpiochip_bcm2835.cpp
    gpiochip_bcm2712.cpp
    gpiochip_rp1.cpp
)
add_executable(pinctrl2 ${PINCTRL2_SRCS})
