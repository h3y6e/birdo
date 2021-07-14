/**
 * @file hatching.ino
 * @brief 5番道路の預かり屋からタマゴを回収→孵化→ボックスに預ける を繰り返す
 * (英語ROM)
 * @author 5ebec
 * @date 2020-09-18
 * @detail
 * 初期条件:
 * 0. 丸いお守りを手にいれておく
 *    ロトム自転車を最大まで強化しておく
 *    ゲーム内言語が英語
 *    (日本語は不可．その他言語でも使用できる可能性はあるが未検証)
 * 1. 5番道路にいる
 * 3. 孵化を早める特性(ほのおのからだ等)を持つポケモン1体のみが手持ちにいる
 * 4. メニューで"Map"が左上，"Pokémon"がその右隣にある
 * 5.
 * 現在のボックスから指定したボックスまでが空で，かつ左のボックスの少なくとも6列目が空いている
 * 6. 自転車に乗っている
 * 7. "OPTIONS"で Text Speed: Fast, Send to Boxes: Automatic,
 *    Give Nicknames: Don't give, Autosave: Off にしておく
 */
#include <switch_controller_util.h>

/**
 * @brief タマゴのサイクル数
 * (5, 10, 15, 20, 25, 30, 35, 40)
 *
 * @detail
 * 参考: https://wiki.ポケモン.com/wiki/タマゴのサイクル数
 */
const int EGG_CYCLE = 35;

/**
 * @brief 孵化に使用できるボックスの数
 */
const int BOX = 3;

/**
 * @brief 空飛ぶタクシーで初期位置 (5番道路 預かり屋の前) に移動する
 */
void moveToInit() {
  // メニューを開く
  pushButton(Button::X, 400);
  // "TOWN MAP"を押す
  // マップ画面が開くまで待機
  pushButton(Button::A, 2400);
  // ジュンヤのキャンプ前 から 預かり屋の前 へ移動する
  tiltLeftJoystick(20, -20, 200);
  // 預かり屋の前を選択し，「はい」を押す
  pushButton(Button::A, 700);
  // 初期位置に移動するまで待機
  pushButton(Button::A, 2200);
}

/* タマゴ受け取り系 */

/**
 * @brief 初期位置から移動してタマゴを受け取る
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
  // OR *The Egg has been sent to a Box!*
  // "Take good care of it, and raise it well."
  pushButton(Button::B, 200, 18);
  // 初期位置に戻る
  tiltLeftJoystick(100, 50, 500);
  pushButton(Button::PLUS, 500);
}

/**
 * @brief タマゴをn個回収する
 *
 * @param n タマゴの個数
 */
void getEggs(int n) {
  for (int i = 0; i < n; i++) {
    // 1個目は出現しているのでスキップ
    if (i != 0) {
      rideToRight(14.0);
    }
    //初期位置に戻る
    moveToInit();
    // タマゴを貰う
    getEgg();
  }
}

/* タマゴサイクル消費系 */

/**
 * @brief 右に向かって自転車で移動する
 *
 * @param running_time 走行時間 (秒)
 *
 * @detail
 * 初期位置から橋の右端まで行くのには約14.5秒かかる
 * ターボは開放後約6.5秒後に再び使えるようになる
 *
 */
void rideToRight(float running_time) {
  if (running_time < 3.5) {
    tiltLeftJoystick(100, 0, running_time * 1000);
  } else {
    // ターボが溜まるまで右に進む
    tiltLeftJoystick(100, 0, 3500);
    // ターボを開放し右に進む
    tiltLeftJoystickAsMashButton(100, 0, Button::R,
                                 (running_time - 3.5) * 1000);
  }
}

/**
 * @brief 自転車で橋を往復する (片道11秒)
 *
 * @param running_time 走行時間 (秒)
 */
void roundTrip(float running_time) {
  int i = -1;
  while (running_time > 0) {
    if (running_time >= 3.3) {
      tiltLeftJoystick(i * 100, 0, 3300);
      running_time -= 3.3;
    } else {
      tiltLeftJoystick(i * 100, 0, running_time * 1000);
      break;
    }
    if (running_time >= 7.7) {
      tiltLeftJoystickAsMashButton(i * 100, 0, Button::R, 7700);
      running_time -= 7.7;
    } else {
      tiltLeftJoystickAsMashButton(i * 100, 0, Button::R, running_time * 1000);
      break;
    }
    i *= -1;
  }
}

/* ボックス操作系 */

/**
 * @brief ボックスを開く
 */
void openBox() {
  // メニューを開く
  pushButton(Button::X, 400);
  // "POKÉMON"にカーソルを合わせて開く
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
  // "TOWN MAP"にカーソルを合わせて閉じる
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
  // "Multiselect"モードにする
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

  // "Multiselect"モードにする
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
 * @brief 右のボックスに移動する
 */
void moveToRightBox() {
  // ボックスを開く
  openBox();
  // 右のボックスに移動
  pushButton(Button::R, 100);
  // ボックスを閉じる
  closeBox();
}

/**
 * @brief 左のボックスに移動する
 */
void moveToLeftBox() {
  // ボックスを開く
  openBox();
  // 右のボックスに移動
  pushButton(Button::L, 100);
  // ボックスを閉じる
  closeBox();
}

/* main */

/**
 * @brief 接続時に一度だけ実行される関数
 */
void setup() {
  /* 初期設定 */
  // Bボタン連打でフィールド画面に戻る
  pushButton(Button::B, 500, 4);
  // メニューを開く
  pushButton(Button::X, 400);
  // "TOWN MAP"にカーソルをあわせる
  pushDpad(Dpad::LEFT_UP, 100, 800);
  // メニューを閉じる
  pushButton(Button::B, 500);
  // 初期位置に移動
  moveToInit();
  // 最初のタマゴを出現させる
  rideToRight(14.0);

  // タマゴを30個受け取る
  getEggs(30);

  /* ボックスを開き，手持ちの5体を左のボックスの5列目に預ける */
  // ボックスを開く
  openBox();
  // 左のボックスへ移動する
  pushButton(Button::L, 100);
  // "Multiselect"モードにする
  pushButton(Button::Y, 100, 2);
  // 預けるポケモンの1匹目にカーソルを当てる
  pushDpad(Dpad::LEFT, 100);
  pushDpad(Dpad::DOWN, 100);
  // 預けるポケモン5匹を範囲選択する
  pushButton(Button::A, 100);
  pushDpad(Dpad::UP, 100, 100, 2);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 100);
  // 5列目にポケモンを移動させる
  pushDpad(Dpad::UP, 100);
  pushDpad(Dpad::LEFT, 100);
  pushButton(Button::A, 100);

  /* 元のボックスに戻り0列目を手持ちに入れる */
  // 右のボックスへ移動する
  pushButton(Button::R, 100);
  // 0列目にカーソルを移動させる
  pushDpad(Dpad::RIGHT, 100, 100, 2);
  // ポケモン5匹を範囲選択する
  pushButton(Button::A, 100);
  pushDpad(Dpad::UP, 100);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 100);
  // 手持ちにポケモンを移動する
  pushDpad(Dpad::LEFT, 100);
  pushDpad(Dpad::DOWN, 100);
  pushButton(Button::A, 100);
  // ボックスを閉じる
  closeBox();

  for (int column = 0; column < 6; column++) {
    /* タマゴを孵化させる */
    // 橋の右端まで行く
    rideToRight(14.5);
    //サイクル数に応じて往復する
    float count = (float)((EGG_CYCLE + 1) / 2) / 2 - 1;
    roundTrip(11 * count);
    for (int i = 0; i < 5; i++) {
      // *Oh?*
      pushButton(Button::A, 200);
      // 孵化ムービー
      flash(70);
      pushButton(Button::B, 200);
      // 1歩進む
      tiltLeftJoystick(0, -100, 200);
    }

    if (column < 5) {
      /* ボックスを開き，k列目に手持ちの5体を預ける */
      // ボックスを開く
      openBox();
      // "Multiselect"モードにする
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

      // 隣列にカーソルを移動させる
      pushDpad(Dpad::RIGHT, 100);
      // ポケモン5匹を範囲選択する
      pushButton(Button::A, 100);
      pushDpad(Dpad::UP, 100);
      // 選択したポケモンを持ち上げる
      pushButton(Button::A, 100);
      // 手持ちにポケモンを移動する
      if (column < 2) {
        pushDpad(Dpad::LEFT, 100, 100, column + 2);
      } else {
        pushDpad(Dpad::RIGHT, 100, 100, 5 - column);
      }
      pushDpad(Dpad::DOWN, 100);
      pushButton(Button::A, 100);
    } else {
      /* column= 5の場合，終了する */
    }
    // ボックスを閉じる
    closeBox();
  }
  // moveToNextBox();
}

/**
 * @brief `setup()`実行後にループ実行される関数
 */
void loop() {}
