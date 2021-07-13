/**
 * @file snowball_bowling.ino
 * @brief ミニゲーム「雪玉ボウル」でストライクを出し続ける
 * @author 5ebec
 * @date 2021-07-14
 */
#include <switch_controller_util.h>

void scrollCaption() {
  pushButton(Button::B, 600);
  pushButton(Button::A, 600);
}

void blackout() { flash(35); }

/* main */
/**
 * @brief 一投目
 */
void setup() {
  /* コントローラー接続 */
  pushButton(Button::ZL, 100, 10);
  pushButton(Button::A, 1000);

  /* 会話 */
  // ボンドに話しかけることができる位置から開始
  pushButton(Button::A, 500);
  // 「若いの! 雪玉ボウル やりたくなっただか? いつでも歓迎だ!」
  scrollCaption();
  // 「うれしいねぇ！ でも タダじゃないだよ！ 参加料に20ルピー貰うが いいだか?」
  scrollCaption();
  // 「どうも ありがとうだ！！ だば 準備するから ちょっと待つだよ...」 (暗転)
  scrollCaption();
  blackout();
  // 「それじゃあ この雪玉さ 使っていいから 頑張って ピンを倒すだよ！」
  scrollCaption();
  // 「この遊び 雪玉は2回まで転がせるだ！」 (カメラ移動)
  scrollCaption();
  flash(5);
  // 「もし一回で ピンを全部倒せたら ストライク！ という プロ級のプレイだ！」
  scrollCaption();
  // 「んで2回目で ピンを全部倒せたら スペア！ という まあまあなプレイだ！」
  scrollCaption();
  // 「それじゃ 頑張るだ！！」 (ボンドが移動)
  scrollCaption();
  flash(10);

  /* 投げ */
  // もつ
  pushButton(Button::A, 1500);
  // 移動
  tiltLeftJoystick(100, 10, 1000);
  // 方向調整
  tiltLeftJoystick(-50, -100, 100);
  // 投げる (カメラ移動)
  pushButton(Button::R, 100);
  flash(60);
  // 「おおおっと！ 記録は 10本！！ ストライクってヤツだ！ 輝かしい記録だ！！」
  // (暗転)
  scrollCaption();
  blackout();
}

/**
 * @brief 二投目以降
 */
void loop() {
  /* 会話 */
  // 「若いの！すげぇだな！特別な道具を分けてやるだよ！」
  scrollCaption();
  // 「と思っただが… 荷物がいっぱいみたいだな？ 代わりに これを受け取るだ！」
  scrollCaption();
  // *金ルピー*
  scrollCaption();
  // 「さて もう1回 遊んでいくだか？」
  scrollCaption();
  // 「うれしいねぇ！ でも タダじゃないだよ！ 参加料に20ルピー貰うが いいだか?」
  // (暗転)
  scrollCaption();
  blackout();
  // 「それじゃ 頑張るだ！！」
  scrollCaption();

  /* 投げ */
  // 持てるところまで移動
  tiltLeftJoystick(-100, 0, 200);
  // もつ
  pushButton(Button::A, 1300);
  // 移動
  tiltLeftJoystick(-100, 0, 1500);
  // 方向調整
  tiltLeftJoystick(50, 100, 100);
  // 投げる (カメラ移動)
  pushButton(Button::R, 100);
  flash(70);
  // 「おおおっと！ 記録は 10本！！ ストライクってヤツだ！ 輝かしい記録だ！！」
  // (暗転)
  scrollCaption();
  blackout();
}
