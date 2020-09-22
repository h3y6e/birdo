/**
 * @file hatching_en.ino
 * @brief 5番道路の預かり屋からタマゴを回収→孵化→ボックスに預ける を繰り返す
 * @author 5ebec
 * @date 2020-09-18
 * @detail
 * 初期条件:
 * 0. 丸いお守りを手にいれておく
 *    ロトム自転車を最大まで強化しておく
 *    ゲーム内言語が英語
 *    (日本語は不可．その他言語でも使用できる可能性はあるが未検証)
 * 1. 5番道路にいる
 * 3. 特性「ほのおのからだ」のポケモン1体のみが手持ちにいる
 * 4. メニューで"Map"が左上，"Pokémon"がその右隣にある
 * 5. ボックスが空
 * 6. 自転車に乗っている
 */
#include <switch_controller_util.h>

/**
 * @brief 空飛ぶタクシーで初期位置 (5番道路 預かり屋の前) に移動する
 */
void moveToInit() {
  // メニューを開く
  pushButton(Button::X, 400);
  // 「タウンマップ」を押す
  // マップ画面が開くまで待機
  pushButton(Button::A, 2500);

  // ジュンヤのキャンプ前 から 預かり屋の前 へ移動する
  // マップ画面で矢印ボタンを ↑,↓ と入力すると安定して預かり屋の前に移動出来る
  pushDpad(Dpad::UP, 200);
  pushDpad(Dpad::DOWN, 100);
  // 預かり屋の前を選択し，「はい」を押す
  pushButton(Button::A, 600);
  // 初期位置に移動するまで待機
  pushButton(Button::A, 2200);
}

/**
 * @brief 初期位置から預かり屋さんに移動しタマゴを受け取る
 */
void getEgg() {
  // 初期位置から預かり屋さんのところまで移動する
  pushButton(Button::PLUS, 500);
  tiltLeftJoystick(-100, -60, 600);

  // 預かり屋さんからタマゴを貰う
  // `A` -> "We found your Pokémon holding an Egg. Would you like to take it?"
  // `A` -> "Yes"
  pushButton(Button::A, 900, 2);
  // その後はB連打
  // *You received an Egg from the Nursery worker!*
  // *The Egg has been added to your party.*
  // "Take good care of it, and raise it well."
  pushButton(Button::B, 200, 20);
  // 初期位置に戻る
  tiltLeftJoystick(100, 50, 500);
  pushButton(Button::PLUS, 500);
}

/**
 * @brief 右に向かって自転車で移動する
 *
 * @param running_time_msec 走行時間 (ミリ秒)
 *
 * @detail
 * 初期位置から橋の右端まで行くのには14.5秒かかる
 * ターボは開放後6.5秒後に再び使えるようになる
 *
 */
void rideToRight(int running_time_msec) {
  if (running_time_msec < 3300) {
    tiltLeftJoystick(100, 0, running_time_msec);
  } else {
    // ターボが溜まるまで右に進む
    tiltLeftJoystick(100, 0, 3300);
    // ターボを開放し右に進む
    tiltLeftJoystickAsMashButton(100, 0, Button::R, running_time_msec - 3300);
  }
}

/**
 * @brief タマゴを5個回収する
 */
void getFiveEggs() {
  for (int i = 0; i < 5; i++) {
    // 1個目はもう出現しているのでスキップ
    if (i != 0) {
      rideToRight(14000);
    }
    //初期位置に戻る
    moveToInit();
    // タマゴを貰う
    getEgg();
    // 何個目のタマゴかを伝える
    flash(i + 1);
  }
}

/**
 * @brief 自転車で橋を往復する (片道7秒間)
 *
 * @param count 回数 (1往復で2カウントとする)
 */
void roundTrip(int count) {
  for (int i = 0; i < count; i++) {
    tiltLeftJoystick((2 * (i % 2) - 1) * 100, 0, 3300);
    tiltLeftJoystickAsMashButton((2 * (i % 2) - 1) * 100, 0, Button::R, 3700);
  }
}

/**
 * @brief 手持ちのタマゴを全て孵化させる
 */
void hatchFiveEggs() {
  // 1匹目を孵化
  rideToRight(14000);  // 要検証
  roundTrip(5);
  pushButton(Button::B, 500, 30);
  flash();
  moveToInit();
  // 残り4匹を孵化
  for (int i = 0; i < 4; i++) {
    rideToRight(15000);  // 要検証
    pushButton(Button::B, 500, 30);
    moveToInit();
    flash(i + 2);
  }
}

/**
 * @brief ボックスを開く
 */
void openBox() {
  // メニューを開く
  pushButton(Button::X, 400);
  // 「ポケモン」にカーソルを合わせて開く
  pushDpad(Dpad::RIGHT, 100);
  pushButton(Button::A, 1200);
  // ボックスを開く
  pushButton(Button::R, 1400);
}

/**
 * @brief ボックスを閉じる
 */
void closeBox() {
  // メニューに戻る
  pushButton(Button::B, 1200, 2);
  // 「マップ」にカーソルを合わせて閉じる
  pushDpad(Dpad::LEFT, 100);
  pushButton(Button::B, 500);
}

/**
 * @brief 孵化した手持ちのポケモンをボックスに預ける
 *
 * @param column 何列目にポケモンを預けるか
 */
void sendToBox(int column) {
  // ボックスを開く
  openBox();

  /* 手持ちの孵化したポケモンを範囲選択してボックスの指定列に移す */
  // 「はんい」モードにする
  pushButton(Button::Y, 100, 2);
  // 孵化したポケモンの1匹目にカーソルを当てる
  pushDpad(Dpad::LEFT, 100);
  pushDpad(Dpad::DOWN, 100);
  // 孵化したポケモン5匹を範囲選択する
  pushButton(Button::A, 100);
  pushDpad(Dpad::UP, 100, 100, 2);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 100);
  // 指定列にポケモンを移動させる
  pushDpad(Dpad::UP, 100);
  if (column < 3) {
    pushDpad(Dpad::RIGHT, 100, 100, column + 1);
  } else {
    pushDpad(Dpad::LEFT, 100, 100, 6 - column);
  }
  pushButton(Button::A, 100);

  // ボックスを閉じる
  closeBox();
}

/**
 * @brief デバッグ用関数: ボックスのポケモンを手持ちに戻す
 *
 * @param column 何列目のポケモンを戻すか
 */
void returnFromBox(int column) {
  // ボックスを開く
  openBox();

  // 「はんい」モードにする
  pushButton(Button::Y, 100, 2);
  // 指定列にカーソルを移動させる
  if (column < 4) {
    pushDpad(Dpad::RIGHT, 100, 100, column);
  } else {
    pushDpad(Dpad::LEFT, 100, 100, 7 - column);
  }
  // ポケモン5匹を範囲選択する
  pushButton(Button::A, 100);
  pushDpad(Dpad::UP, 100);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 100);
  // 手持ちにポケモンを移動する
  if (column < 3) {
    pushDpad(Dpad::LEFT, 100, 100, column + 1);
  } else {
    pushDpad(Dpad::RIGHT, 100, 100, 6 - column);
  }
  pushDpad(Dpad::DOWN, 100);
  pushButton(Button::A, 100);

  // ボックスを閉じる
  closeBox();
}

/**
 * @brief ボックスを次のボックスに移動させる
 */
void moveToNextBox() {
  // ボックスを開く
  openBox();
  // ボックスを移動
  pushButton(Button::R, 100);
  // pushButton(Button::L, 100);  // デバッグ用
  // ボックスを閉じる
  closeBox();
}

/**
 * @brief 接続時に一度だけ実行される関数
 */
void setup() {
  /* 初期設定 */
  // Bボタン連打でフィールド画面に戻る
  pushButton(Button::B, 500, 4);
  // メニューを開く
  pushButton(Button::X, 400);
  // 「タウンマップ」にカーソルをあわせる
  pushDpad(Dpad::LEFT_UP, 100, 800);
  // メニューを閉じる
  pushButton(Button::B, 500);
  // 初期位置に移動
  moveToInit();

  /* 最初のタマゴを出現させる */
  rideToRight(10000);
}

/**
 * @brief `setup()`実行後にループ実行される関数
 */
void loop() {
  for (int column = 0; column < 6; column++) {
    // タマゴを貰う
    getFiveEggs();
    // タマゴを孵化させる
    hatchFiveEggs();
    // ボックスに預ける
    sendToBox(column);
  }
  moveToNextBox();
}
