cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nwatch)

target_sources(app PRIVATE
    src/main.c
    src/ssd1306/ssd1306.c
    src/gfx/gfx.c
    src/event/event.c
    src/rtc/rtc.c
    src/timer/timer.c
    src/input/input.c
)

target_include_directories(app PRIVATE
    src/ssd1306
    src/gfx
    src/event
    src/rtc
    src/timer
    src/input
)
