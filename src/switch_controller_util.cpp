/**
 * @file switch_controller_util.cpp
 * @brief Switchコントローラーを操作するための関数群
 * @author 5ebec
 * @date 2020-09-18
 */
#include "switch_controller_util.h"

// ボタンを押してから離すまでの時間など (ミリ秒)
const uint16_t DEFAULT_DELAY = 100;

/**
 * @brief Switchコントローラーのボタンを押す
 *
 * @param button 押すボタン
 * @param wait_time 待ち時間 (ミリ秒)
 * @param loop_count ボタンを押す回数 デフォルト値は1
 */
void pushButton(Button button, int wait_time, int loop_count) {
  for (int i = 0; i < loop_count; i++) {
    SwitchController().pressButton(button);
    delay(DEFAULT_DELAY);
    SwitchController().releaseButton(button);
    delay(wait_time);
  }
}

/**
 * @brief Switchコントローラーの十字ボタンを押す
 *
 * @param button 押す十字ボタン
 * @param wait_time 待ち時間 (ミリ秒)
 * @param pushing_time ボタンを押す時間の長さ (ミリ秒) デフォルト値は100
 * @param loop_count 十字ボタンを押す回数 デフォルト値は1
 */
void pushDpad(Dpad button, int wait_time, int pushing_time, int loop_count) {
  for (int i = 0; i < loop_count; i++) {
    SwitchController().pressDpad(button);
    delay(pushing_time);
    SwitchController().releaseDpad();
    delay(wait_time);
  }
}

/**
 * @brief Switchコントローラーのジョイスティックを傾ける
 *
 * @param lx LスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ly LスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param rx RスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ry RスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param tilting_time スティックを傾ける時間
 */
void tiltJoystick(int lx, int ly, int rx, int ry, int tilting_time) {
  SwitchController().setStickTiltRatio(lx, ly, rx, ry);
  delay(tilting_time);
  SwitchController().setStickTiltRatio(0, 0, 0, 0);
  delay(DEFAULT_DELAY);
}

/**
 * @brief SwitchコントローラーのLスティックを傾ける
 *
 * @param lx LスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ly LスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param tilting_time スティックを傾ける時間
 */
void tiltLeftJoystick(int lx, int ly, int tilting_time) {
  SwitchController().setStickTiltRatio(lx, ly, 0, 0);
  delay(tilting_time);
  SwitchController().setStickTiltRatio(0, 0, 0, 0);
  delay(DEFAULT_DELAY);
}

/**
 * @brief SwitchコントローラーのRスティックを傾ける
 *
 * @param rx RスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ry RスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param tilting_time スティックを傾ける時間
 */
void tiltRightJoystick(int rx, int ry, int tilting_time) {
  SwitchController().setStickTiltRatio(0, 0, rx, ry);
  delay(tilting_time);
  SwitchController().setStickTiltRatio(0, 0, 0, 0);
  delay(DEFAULT_DELAY);
}

/**
 * @brief Switchコントローラーのボタンを連打しながらジョイスティックを傾ける
 *
 * @param lx LスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ly LスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param rx RスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ry RスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param button 同時に押すボタン
 * @param tilting_time スティックを傾ける時間 (ミリ秒)
 */
void tiltJoystickAsMashButton(int lx, int ly, int rx, int ry, Button button,
                              int tilting_time) {
  SwitchController().setStickTiltRatio(lx, ly, rx, ry);
  for (int i = 0; i < tilting_time / DEFAULT_DELAY; i++) {
    SwitchController().pressButton(button);
    delay(DEFAULT_DELAY / 2);
    SwitchController().releaseButton(button);
    delay(DEFAULT_DELAY / 2);
  }
  SwitchController().setStickTiltRatio(0, 0, 0, 0);
  delay(DEFAULT_DELAY);
}

/**
 * @brief Switchコントローラーのボタンを連打しながらLスティックを傾ける
 *
 * @param lx LスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ly LスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param button 同時に押すボタン
 * @param tilting_time スティックを傾ける時間 (ミリ秒)
 */
void tiltLeftJoystickAsMashButton(int lx, int ly, Button button,
                                  int tilting_time) {
  SwitchController().setStickTiltRatio(lx, ly, 0, 0);
  for (int i = 0; i < tilting_time / DEFAULT_DELAY; i++) {
    SwitchController().pressButton(button);
    delay(DEFAULT_DELAY / 2);
    SwitchController().releaseButton(button);
    delay(DEFAULT_DELAY / 2);
  }
  SwitchController().setStickTiltRatio(0, 0, 0, 0);
  delay(DEFAULT_DELAY);
}

/**
 * @brief Switchコントローラーのボタンを連打しながらRスティックを傾ける
 *
 * @param rx RスティックのX方向変位 [％] -100 ~ 100 の範囲で設定
 * @param ry RスティックのY方向変位 [％] -100 ~ 100 の範囲で設定
 * @param button 同時に押すボタン
 * @param tilting_time スティックを傾ける時間 (ミリ秒)
 */
void tiltRightJoystickAsMashButton(int rx, int ry, Button button,
                                   int tilting_time) {
  SwitchController().setStickTiltRatio(0, 0, rx, ry);
  for (int i = 0; i < tilting_time / DEFAULT_DELAY; i++) {
    SwitchController().pressButton(button);
    delay(DEFAULT_DELAY / 2);
    SwitchController().releaseButton(button);
    delay(DEFAULT_DELAY / 2);
  }
  SwitchController().setStickTiltRatio(0, 0, 0, 0);
  delay(DEFAULT_DELAY);
}

/**
 * @brief ArduinoのRXLEDをLチカ (200*countミリ秒)
 *
 * @param count 点滅回数 デフォルト値は1
 */
void flash(int count) {
  for (int i = 0; i < count; i++) {
    RXLED1;
    delay(100);
    RXLED0;
    delay(100);
  }
}