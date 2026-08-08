cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nwatch)

target_sources(app PRIVATE
    src/main.c
    src/gfx/gfx.c
    src/event/event.c
    src/rtc/rtc.c
    src/timer/timer.c
    src/input/input.c
    src/ui/ui.c
    src/drivers/spi.c
    src/obj_manager/obj_manager.c
)

target_include_directories(app PRIVATE
    src/gfx
    src/event
    src/rtc
    src/timer
    src/input
    src/ui
    src/drivers
    src/obj_manager
)
