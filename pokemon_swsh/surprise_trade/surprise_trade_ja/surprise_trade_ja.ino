/**
 * @file surprise_trade_ja.ino
 * @brief マジカル交換を行う
 * @author 5ebec
 * @date 2020-09-22
 * @detail
 * 初期条件:
 * 0. ゲーム内言語が日本語
 * (英語は不可．その他言語でも使用できる可能性はあるが未検証)
 * 1. 交換したいポケモンたちを1つのボックスに入れる
 * (30匹未満でも問題無いが，整頓されていたほうが早い)
 * 2. インターネット接続したい場合は予めしておく
 */
#include <switch_controller_util.h>

/**
 * @brief ボックスのm行n列目のポケモンを選ぶ
 *
 * @param m 何行目か
 * @param n 何列目か
 */
void selectPokemon(int m, int n) {
  if (m != 4) {
    pushDpad(Dpad::DOWN, 100, 100, m);
  } else {
    pushDpad(Dpad::UP, 100, 100, 3);
  }
  if (n < 4) {
    pushDpad(Dpad::RIGHT, 100, 100, n);
  } else {
    pushDpad(Dpad::LEFT, 100, 100, 7 - n);
  }
  // 状態bで台詞を送るために少し遅らせる
  pushButton(Button::A, 400, 2);
}

/**
 * @brief 接続時に一度だけ実行される関数
 */
void setup() {
  // Bボタン連打でフィールド画面に戻る
  pushButton(Button::B, 400, 4);

  // 1ボックス内のポケモンに処理を施す 約1分間で1サイクル
  for (int row = 0; row < 5; row++) {
    for (int column = 0; column < 6; column++) {
      /**
       * 状態:
       * a. 交換開始前/交換完了
       * b. 交換未完了/cから遷移
       * c. 処理中 (Yを押しても反応しない期間)
       * */
      /* cから遷移したbはここでaに遷移する */
      // a,b. YY通信を開く
      // c. 「交換完了!」 -> 交換ムービー
      pushButton(Button::Y, 700);
      // a,b. マジカル交換を選ぶ / c. 交換ムービー
      pushDpad(Dpad::DOWN, 100);
      pushButton(Button::A, 1500);
      // a. 交換するポケモンを選ぶ
      // b. *通信できる人を 探している途中です!*
      //    *すでに通信をしているときに 別の通信をはじめることは*
      // c. 交換ムービー
      selectPokemon(row, column);
      // a. (インターネット接続の場合) *通信しています しばらくお待ちください*
      // b. 待機 / c. 交換ムービー
      flash(17);
      // a. *レポートを書いてマジカル交換を始めてもよろしいですか?*
      // b. *別の通信をはじめることは できません*
      // c. 交換ムービー
      pushButton(Button::A, 500);
      // a. *通信できる人を探します!*
      //    *通信する項目を選ぶと探すのをキャンセルできます*
      // b. *今の通信を キャンセルしてもよろしいですか?* フィールド画面に戻る
      // c. 交換ムービー
      pushButton(Button::B, 500, 2);
      // a. 「探しています」 / b. 待機 / c. 交換ムービー終了
      flash(110);
      // a,b. 待機
      // c. 送られてきたポケモンが図鑑に登録されていなかった場合，図鑑に登録する
      pushButton(Button::B, 800, 2);
      /* ここで分岐する(a -> b/c), cはbへ遷移する */
      // a. 交換ムービー(21~22秒間)
      // b. YY通信を開き，フィールド画面に戻る
      // c. 待機 2回目のYで交換ムービーに入ればaへ遷移
      pushButton(Button::Y, 2000, 2);
      flash(100);
      // a. 送られてきたポケモンが図鑑に登録されていなかった場合，図鑑に登録する
      // b. 待機 交換完了していればcへ遷移
      // c. 待機
      pushButton(Button::B, 800, 2);
    }
  }
}

/**
 * @brief `setup()`実行後にループ実行される関数
 */
void loop() {}
