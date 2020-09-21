#include "switch_controller.h"

#pragma once

void pushButton(Button button, int wait_time, int loop_count = 1);
void pushDpad(Dpad button, int wait_time, int pushing_time = 100,
              int loop_count = 1);
void tiltJoystick(int lx, int ly, int rx, int ry, int tilting_time);
void tiltLeftJoystick(int lx, int ly, int tilting_time);
void tiltRightJoystick(int rx, int ry, int tilting_time);
void tiltJoystickAsMashButton(int lx, int ly, int rx, int ry, Button button,
                              int tilting_time);
void tiltLeftJoystickAsMashButton(int lx, int ly, Button button,
                                  int tilting_time);
void tiltRightJoystickAsMashButton(int rx, int ry, Button button,
                                   int tilting_time);
void flash(int count = 1);