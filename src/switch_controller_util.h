#include "switch_controller.h"

#pragma once

void pushButton(Button button, int wait_msec, int loop_count = 1);
void pushDpad(Dpad button, int pushing_time_msec, int wait_msec = 100,
              int loop_count = 1);
void tiltJoystick(int lx, int ly, int rx, int ry, int tilting_time_msec);
void tiltLeftJoystick(int lx, int ly, int tilting_time_msec);
void tiltRightJoystick(int rx, int ry, int tilting_time_msec);
void tiltJoystickAsMashButton(int lx, int ly, int rx, int ry, Button button,
                              int tilting_time_msec);
void tiltLeftJoystickAsMashButton(int lx, int ly, Button button,
                                  int tilting_time_msec);
void tiltRightJoystickAsMashButton(int rx, int ry, Button button,
                                   int tilting_time_msec);
void flash(int count = 1);