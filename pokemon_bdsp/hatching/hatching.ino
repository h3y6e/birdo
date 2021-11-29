/**
 * @file hatching.ino
 * @brief 自動孵化装置
 * (日本語ROM)
 * @author 5ebec
 * @date 2021-11-19
 * @detail
 * 初期条件:
 * 0. まるいおまもり を手に入れておく
 *    ゲーム内言語が日本語(文字モード: 漢字)
 * 1. 「設定」で　話の速さ: 速い, 手持ち/ボックス: 自動で送る, おまかせレポート:
 * しない　になっている
 * 2.
 * メニューで「タウンマップ」が左上隅、「ポケモン」がその右隣にある
 * 3. じてんしゃが＋ボタン呼び出しの上側に登録されている
 * 3. ズイタウンにいる
 * 4. 孵化を早める特性(ほのおのからだ等)を持つポケモンが手持ちにいる
 * 5.
 * ボックス1(ボックス一覧で左上隅のボックス)から指定したボックスまでが空で、かつボックス18の6列目が空いている
 * 6. じてんしゃに乗っており、ギアが4速である
 *
 */
#include <switch_controller_util.h>

/**
 * @brief タマゴのサイクル数
 * (5, 10, 15, 20, 25, 30, 35, 40)
 *
 * @detail
 * 必要歩数は ⌊EGG_CYCLE / 2⌋ * 255
 * 参考: https://wiki.ポケモン.com/wiki/タマゴのサイクル数
 */
const int EGG_CYCLE = 35;

/**
 * @brief 孵化に使用するボックスの数
 */
const int BOX = 3;

/**
 * @brief 初期位置(廃人ロード最南)に移動する
 */
void moveToInit() {
  // メニューを開く
  pushButton(Button::X, 500);
  // 「タウンマップ」を押す
  tiltLeftJoystick(-100, -100, 1000);
  // マップ画面が開くまで待機
  pushButton(Button::A, 1100);
  // ズイタウン を選択する
  pushButton(Button::A, 700);
  // 「はい」を押し、ポケモンセンター前に移動するまで待機
  pushButton(Button::A, 7000);
  // 初期位置に移動
  tiltLeftJoystick(-100, 0, 580);
  rideBike();
  tiltLeftJoystick(0, 100, 5300);
}

/**
 * @brief 自転車に乗る
 */
void rideBike() {
  pushButton(Button::PLUS, 500);
  pushDpad(Dpad::UP, 400);
}

/* タマゴ受け取り系 */

/**
 * @brief タマゴを受け取る
 */
void getEgg() {
  // 話し掛ける
  //
  // おお！　あんたか
  // 預かっていた　ポケモンを　世話して　おったら……　なんと！
  // ポケモンが　タマゴを　持っておったんじゃ！
  // どこから　持ってきたか　わからんが　あんたの　ポケモンが　持っていた
  // タマゴなんじゃ
  // やっぱり　欲しいじゃろ？
  // [はい]
  pushButton(Button::A, 400, 8);
  // へいほぅは　預かり屋　じいさんから　
  // タマゴを　もらった！
  // 「タマゴを ボックスへ　送信しました！」
  // 大事に　育てなさいよ！
  pushButton(Button::B, 400, 2);
  flash(6);
  pushButton(Button::A, 600, 2);
  pushButton(Button::A, 400, 2);

  // タマゴ未所持:
  // おお　あんたか！　よく来たな
  // [ポケモン]と [ポケモン]は 元気じゃぞ！
  // 2匹の　仲は まずまずの or それほど　よくない or とっても　よい ようじゃ
}

/**
 * @brief タマゴをn個回収する
 *
 * @param n タマゴの個数
 */
void getEggs(int n) {
  for (int i = 0; i < n; i++) {
    // 預かり屋じいさんの所まで移動
    tiltLeftJoystick(0, -100, 10400);
    tiltLeftJoystick(0, 100, 5080);
    tiltLeftJoystick(-100, 0, 380);
    // タマゴを貰う
    getEgg();
    // 初期位置に戻る
    tiltLeftJoystick(100, 0, 900);
    tiltLeftJoystick(0, 100, 5400);
  }
}

/**
 * @brief 初期位置から自転車で廃人ロードを往復する (1往復254歩)
 *
 * @param times 往復回数
 *
 */
void roundTrip(int times) {
  for (int i = 0; i < times; i++) {
    tiltLeftJoystick(0, -100, 10400);  // 127歩
    tiltLeftJoystick(0, 100, 10400);   // 127歩
  }
}

/**
 * @brief 孵化
 */
void hatch() {
  roundTrip((EGG_CYCLE + 1) / 2);   // ⌊EGG_CYCLE / 2⌋ * 254歩
  tiltLeftJoystick(0, -100, 2000);  // 約20歩
  for (int i = 0; i < 5; i++) {
    pushButton(Button::A, 200, 2);
    flash(76);
    pushButton(Button::A, 5000);
  }
  // 初期位置に移動
  tiltLeftJoystick(0, 100, 10400);
}

/* ボックス操作系 */

/**
 * @brief ボックスを開く
 */
void openBox() {
  // メニューを開く
  pushButton(Button::X, 450);
  // 「ポケモン」にカーソルを合わせて開く
  pushDpad(Dpad::RIGHT, 100);
  pushButton(Button::A, 1200);
  // ボックスを開く
  pushButton(Button::R, 1600);
}

/**
 * @brief ボックスを閉じる
 */
void closeBox() {
  // メニューに戻る
  pushButton(Button::B, 1100);
  pushButton(Button::B, 900);
  // 「タウンマップ」にカーソルを合わせて閉じる
  pushDpad(Dpad::LEFT, 100);
  pushButton(Button::B, 500);
}

/**
 * @brief 手持ちのポケモンをボックスに預ける
 *
 * @param column 何列目にポケモンを預けるか(0~5)
 */
void sendToBox(int column) {
  /* 手持ちの孵化したポケモンを範囲選択してボックスの指定列に移す */
  // 「はんい」モードにする
  pushButton(Button::Y, 100, 2);
  // ポケモンの1匹目にカーソルを当てる
  pushDpad(Dpad::LEFT, 150);
  pushDpad(Dpad::DOWN, 100);
  // 手持ちのポケモン5匹を範囲選択する
  pushButton(Button::A, 100);
  pushDpad(Dpad::DOWN, 50, 100, 3);
  pushDpad(Dpad::DOWN, 150);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 150);
  // 指定列にポケモンを移動させる
  pushDpad(Dpad::UP, 100);
  if (column < 3) {
    pushDpad(Dpad::RIGHT, 50, 100, column + 1);
  } else {
    pushDpad(Dpad::LEFT, 50, 100, 6 - column);
  }
  pushButton(Button::A, 150);
}

/**
 * @brief ボックスのポケモンを手持ちに戻す
 *
 * @param column 何列目のポケモンを戻すか
 */
void returnFromBox(int column) {
  // 隣列にカーソルを移動させる
  pushDpad(Dpad::RIGHT, 150);
  // ポケモン5匹を範囲選択する
  pushButton(Button::A, 100);
  pushDpad(Dpad::DOWN, 50, 100, 3);
  pushDpad(Dpad::DOWN, 150);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 150);
  // 手持ちにポケモンを移動する
  pushDpad(Dpad::DOWN, 100);
  if (column < 3) {
    pushDpad(Dpad::LEFT, 50, 100, column + 1);
  } else {
    pushDpad(Dpad::RIGHT, 50, 100, 6 - column);
  }
  pushButton(Button::A, 500);
}

/**
 * @brief ボックスを入れ替える
 *
 * @param box 何個目のボックスをボックス1と入れ替えるか
 */
void swapBox(int box) {
  // ボックス一覧
  pushDpad(Dpad::UP, 150);
  pushDpad(Dpad::UP, 100);
  pushButton(Button::A, 300);
  pushButton(Button::Y, 150);
  pushDpad(Dpad::RIGHT, 50, 100, box - 1);
  pushButton(Y, 100);
  pushButton(B, 150);
}

/* main */

void setup() {
  // コントローラ接続
  pushButton(Button::L, 100, 5);
  // 初期位置
  moveToInit();

  for (int box = 1; box < BOX; box++) {
    // タマゴを受け取る
    getEggs(30);

    // ボックスを開き、手持ちの5匹を左のボックスの5列目に預ける
    openBox();
    pushButton(Button::L, 700);
    sendToBox(5);

    // 元のボックスに戻り0列目を手持ちに入れる
    pushButton(Button::R, 700);
    pushDpad(Dpad::RIGHT, 200);
    returnFromBox(0);
    closeBox();

    for (int column = 0; column < 6; column++) {
      // タマゴを孵す
      hatch();

      openBox();
      if (column < 5) {
        // 手持ちを入れ替える
        sendToBox(column);
        returnFromBox(column + 1);
      } else {
        // ボックスを入れ替える
        if (BOX > 1) swapBox(box + 1);
      }
      closeBox();
    }
  }
}

void loop() {}
