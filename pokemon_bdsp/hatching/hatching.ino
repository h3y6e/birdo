/**
 * @file hatching.ino
 * @brief 自動孵化装置
 * (日本語ROM)
 * @author 5ebec
 * @date 2021-11-19
 * @details
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
 * @details
 * 必要歩数は ⌊EGG_CYCLE / 2⌋ * 255
 * 参考: https://wiki.ポケモン.com/wiki/タマゴのサイクル数
 */
const int EGG_CYCLE = 35;

/**
 * @brief 孵化に使用するボックスの数(1~6)
 */
const int BOX = 6;

/**
 * @brief 初期位置(廃人ロード最南)に移動する
 */
void moveToInit() {
  // メニューを開く
  pushButton(X, 500);
  // 「タウンマップ」を押す
  tiltLeftJoystick(-100, -100, 1000);
  // マップ画面が開くまで待機
  pushButton(A, 1100);
  // ズイタウン を選択する
  pushButton(A, 700);
  // 「はい」を押し、ポケモンセンター前に移動するまで待機
  pushButton(A, 7000);
  // 初期位置に移動
  tiltLeftJoystick(-100, 0, 600);
  rideBike();
  tiltLeftJoystick(0, 100, 5300);
}

/**
 * @brief 自転車に乗る
 */
void rideBike() {
  pushButton(PLUS, 500);
  pushDpad(UP, 400);
}

/* タマゴ受け取り系 */

/**
 * @brief タマゴを受け取る
 *
 * @details
 * タマゴ出現時:　A/Bボタン14回で終了
 *   おお！　あんたか
 *   預かっていた　ポケモンを　世話して　おったら……　なんと！
 *   ポケモンが　タマゴを　持っておったんじゃ！
 *   どこから　持ってきたか　わからんが　あんたの　ポケモンが　持っていた
 *   タマゴなんじゃ
 *   やっぱり　欲しいじゃろ？
 *   [はい]
 *   へいほぅは　預かり屋　じいさんから　
 *   タマゴを　もらった！
 *   「タマゴを ボックスへ　送信しました！」
 *   大事に　育てなさいよ！
 *
 * タマゴ未出現時:　A/Bボタン4回で終了
 *   おお　あんたか！　よく来たな
 *   [ポケモン]と　[ポケモン]は　元気じゃぞ！
 *   2匹の　仲は とっても 良いようじゃ or まずまずの　ようじゃ or
 * それほど　良くないがなぁ
 *
 */
void getEgg() {
  // 話し掛ける
  pushButton(A, 400, 4);
  pushButton(B, 400, 2);
  pushButton(A, 400, 4);
  flash(7);
  pushButton(A, 600, 2);
  pushButton(A, 400, 2);
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
    tiltLeftJoystick(0, 100, 5085);
    tiltLeftJoystick(-100, 0, 400);
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
  // ⌈EGG_CYCLE / 2⌉ * 254歩
  roundTrip((EGG_CYCLE + 1) / 2);
  tiltLeftJoystick(0, -100, 10400);
  for (int i = 0; i < 5; i++) {
    pushButton(A, 200, 2);
    flash(76);
    pushButton(A, 5000);
  }
}

/* ボックス操作系 */

/**
 * @brief ボックスを開く
 */
void openBox() {
  // メニューを開く
  pushButton(X, 450);
  // 「ポケモン」にカーソルを合わせて開く
  pushDpad(RIGHT, 100);
  pushButton(A, 1200);
  // ボックスを開く
  pushButton(R, 1600);
}

/**
 * @brief ボックスを閉じる
 */
void closeBox() {
  // メニューに戻る
  pushButton(B, 1100);
  pushButton(B, 900);
  // 「タウンマップ」にカーソルを合わせて閉じる
  pushDpad(LEFT, 100);
  pushButton(B, 500);
}

/**
 * @brief 手持ちのポケモンをボックスに預ける
 *
 * @param column 何列目にポケモンを預けるか(0~5)
 */
void sendToBox(int column) {
  /* 手持ちの孵化したポケモンを範囲選択してボックスの指定列に移す */
  // 「はんい」モードにする
  pushButton(Y, 100, 2);
  // ポケモンの2匹目にカーソルを当てる
  pushDpad(LEFT, 200);
  pushDpad(DOWN, 100);
  // 手持ちのポケモン5匹を範囲選択する
  pushButton(A, 100);
  pushDpad(DOWN, 50, 100, 3);
  pushDpad(DOWN, 150);
  // 選択したポケモンを持ち上げる
  pushButton(A, 150);
  // 指定列にポケモンを移動させる
  pushDpad(UP, 100);
  if (column < 3) {
    pushDpad(RIGHT, 50, 100, column + 1);
  } else {
    pushDpad(LEFT, 50, 100, 6 - column);
  }
  pushButton(A, 150);
}

/**
 * @brief ボックスのポケモンを手持ちに戻す
 *
 * @param column 何列目のポケモンを戻すか
 */
void returnFromBox(int column) {
  // 隣列にカーソルを移動させる
  pushDpad(RIGHT, 150);
  // ポケモン5匹を範囲選択する
  pushButton(A, 100);
  pushDpad(DOWN, 50, 100, 3);
  pushDpad(DOWN, 150);
  // 選択したポケモンを持ち上げる
  pushButton(A, 150);
  // 手持ちにポケモンを移動する
  pushDpad(DOWN, 100);
  if (column < 3) {
    pushDpad(LEFT, 50, 100, column + 1);
  } else {
    pushDpad(RIGHT, 50, 100, 6 - column);
  }
  pushButton(A, 500);
}

/**
 * @brief ボックスを入れ替える
 *
 * @param box 何個目と入れ替えるか
 */
void swapBox(int box) {
  // ボックス一覧
  pushDpad(UP, 150);
  pushDpad(UP, 100);
  pushButton(A, 400);
  // 1番目と2番目を入れ替え
  pushButton(Y, 100);
  pushDpad(RIGHT, 50);
  pushButton(Y, 100);
  if (box > 2) {
    // 1番目とbox番目を入れ替え
    pushDpad(LEFT, 50);
    pushButton(Y, 150);
    if (box < 4) {
      pushDpad(RIGHT, 50, 100, box - 1);
    } else {
      pushDpad(LEFT, 50, 100, 7 - box);
    }
    pushButton(Y, 100);
  }
  pushButton(B, 400);
}

/* main */

void setup() {
  // コントローラ接続
  pushButton(L, 100, 5);
  // 初期位置
  moveToInit();

  for (int box = 1; box < BOX; box++) {
    // タマゴを受け取る
    getEggs(30);

    // ボックスを開き、手持ちの5匹を隣のボックスの5列目に預ける
    openBox();
    if (box == 1) {
      pushButton(L, 700);
    } else {
      pushButton(R, 700);
    }
    sendToBox(5);

    // 元のボックスに戻り0列目を手持ちに入れる
    if (box == 1) {
      pushButton(R, 700);
    } else {
      pushButton(L, 700);
    }
    pushDpad(RIGHT, 200);
    returnFromBox(0);
    closeBox();

    for (int column = 0; column < 6; column++) {
      // タマゴを孵す
      hatch();

      // ボックス操作
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

      // 初期位置に移動
      tiltLeftJoystick(0, 100, 10400);
    }
  }
}

void loop() {}
