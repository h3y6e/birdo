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
 * メニューで「タウンマップ」が左上隅、「ポケモン」がその右隣にあり、タウンマップにカーソルが当たっている
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
 * イーブイ: 4590 steps = [35/2] * 255
 * @detail
 * 参考: https://wiki.ポケモン.com/wiki/タマゴのサイクル数
 */
const int EGG_CYCLE = 35;

/**
 * @brief 孵化に使用できるボックスの数
 */
const int BOX = 3;

/**
 * @brief 初期位置(ズイタウン ポケモンセンター横)に移動する
 *
 * @param in_town ズイタウンにいる場合はtrue、210番道路にいる場合はfalse
 * デフォルト値はtrue
 */
void moveToInit(bool in_town = true) {
  // メニューを開く
  pushButton(Button::X, 500);
  // 「タウンマップ」を押す
  // マップ画面が開くまで待機
  pushButton(Button::A, 1100);
  if (!in_town) pushDpad(Dpad::DOWN, 100, 100, 2);
  // ズイタウン を選択する
  pushButton(Button::A, 700);
  // 「はい」を押し、ポケモンセンター前に移動するまで待機
  pushButton(Button::A, 7000);
  // 初期位置に徒歩で移動
  tiltLeftJoystick(-100, 0, 580);  // 4歩
}

/**
 * @brief 自転車に乗る
 *
 */
void rideBike() {
  pushButton(Button::PLUS, 500);
  pushDpad(Dpad::UP, 400);
}

/* タマゴ受け取り系 */

/**
 * @brief 初期位置から移動してタマゴを受け取る
 */
void getEgg() {
  // 預かり屋じいさんの所まで徒歩で移動
  tiltLeftJoystick(0, -100, 1250);
  tiltLeftJoystick(-100, 0, 850);
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

  tiltLeftJoystick(100, 0, 840);
}

/**
 * @brief タマゴをn個回収する
 *
 * @param n タマゴの個数
 */
void getEggs(int n) {
  for (int i = 0; i < n; i++) {
    // 歩数を貯める
    rideBike();
    tiltLeftJoystick(0, -100, 5500);
    tiltLeftJoystick(0, 100, 10400);
    tiltLeftJoystick(0, -100, 5500);
    //初期位置に戻る
    moveToInit();
    // タマゴを貰う
    getEgg();
  }
}

/**
 * @brief 初期位置から自転車で廃人ロードを往復する (1往復254歩)
 *
 * @param times 往復回数
 *
 */
void roundTrip(int times) {
  if (times > 0) {
    tiltLeftJoystick(0, -100, 6000);  // 69歩
    for (int i = 0; i < times - 1; i++) {
      tiltLeftJoystick(0, 100, 10400);   // 127歩
      tiltLeftJoystick(0, -100, 10400);  // 127歩
    }
    tiltLeftJoystick(0, 100, 10400);  // 127歩
    tiltLeftJoystick(0, -100, 5500);  // 58歩
  }
}

/**
 * @brief 孵化
 */
void hatch() {
  rideBike();
  roundTrip((EGG_CYCLE + 1) / 2);
  for (int i = 0; i < 5; i++) {
    pushButton(Button::A, 200, 2);
    flash(76);
    pushButton(Button::A, 5000);
  }
  // 210番道路に行く
  tiltLeftJoystick(0, -100, 10400);
  // 初期位置に移動
  moveToInit(false);
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
  pushDpad(Dpad::DOWN, 100, 100, 4);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 150);
  // 指定列にポケモンを移動させる
  pushDpad(Dpad::UP, 100);
  if (column < 3) {
    pushDpad(Dpad::RIGHT, 100, 100, column + 1);
  } else {
    pushDpad(Dpad::LEFT, 100, 100, 6 - column);
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
  pushDpad(Dpad::DOWN, 100, 100, 4);
  // 選択したポケモンを持ち上げる
  pushButton(Button::A, 150);
  // 手持ちにポケモンを移動する
  pushDpad(Dpad::DOWN, 100);
  if (column < 3) {
    pushDpad(Dpad::LEFT, 100, 100, column + 1);
  } else {
    pushDpad(Dpad::RIGHT, 100, 100, 6 - column);
  }
  pushButton(Button::A, 500);
}

/* main */

void setup() {
  // コントローラ接続
  pushButton(Button::L, 100, 5);
  // 初期位置
  moveToInit();

  // タマゴを受け取る
  getEggs(30);

  // ボックスを開き、手持ちの5匹を左のボックスの5列目に預ける
  openBox();
  pushButton(Button::L, 500);
  sendToBox(5);

  // 元のボックスに戻り0列目を手持ちに入れる
  pushButton(Button::R, 700);
  pushDpad(Dpad::RIGHT, 200);
  returnFromBox(0);
  closeBox();
  moveToInit();

  for (int column = 0; column < 6; column++) {
    // タマゴを孵す
    hatch();

    // 手持ちを入れ替える
    openBox();
    sendToBox(column);
    if (column != 5) returnFromBox(column + 1);
    closeBox();
  }
}

void loop() {}
