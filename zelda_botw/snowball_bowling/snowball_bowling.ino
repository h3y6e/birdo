/**
 * @file snowball_bowling.ino
 * @brief ミニゲーム「雪玉ボウル」でストライクを出し続ける
 * @author 5ebec
 * @date 2021-07-14
 */
#include <switch_controller_util.h>

/**
 * @brief 暗転時間 (*200ms)
 */
const int BLACKOUT_COUNT = 35;

/**
 * @brief ムービー時間 (*200ms)
 *
 * @detail
 * 雪玉の移動に合わせて視点移動が行われる時間。
 * ピンがある程度停止した後に次のフェーズへと移行するが、
 * 時間は不安定要素であるため、安定して稼働させる場合は長めに設定すると良い。
 * サイクル速度を上げたい場合は60、安定させたい場合は80
 */
const int MOVIE_COUNT = 70;

/**
 * @brief 台詞を進める
 *
 * @param count 台詞送りの回数
 */
void scrollCaption(int count) {
  for (int i = 0; i < count; i++) {
    pushButton(Button::B, 600);
    pushButton(Button::A, 600);
  }
}

/* main */
/**
 * @brief 一投目
 */
void setup() {
  /* コントローラー接続 */
  pushButton(Button::ZL, 100, 12);
  pushButton(Button::A, 1000);

  /* 会話 */
  // ボンドに話しかけることができる位置から開始
  pushButton(Button::A, 600);
  // 「若いの! 雪玉ボウル やりたくなっただか? いつでも歓迎だ!」
  // 「うれしいねぇ！ でも タダじゃないだよ！ 参加料に20ルピー貰うが いいだか?」
  // 「どうも ありがとうだ！！ だば 準備するから ちょっと待つだよ...」
  // (暗転)
  scrollCaption(3);
  flash(BLACKOUT_COUNT);
  // 「それじゃあ この雪玉さ 使っていいから 頑張って ピンを倒すだよ！」
  // 「この遊び 雪玉は2回まで転がせるだ！」
  // (カメラ移動)
  scrollCaption(2);
  flash(6);
  // 「もし一回で ピンを全部倒せたら ストライク！ という プロ級のプレイだ！」
  // 「んで2回目で ピンを全部倒せたら スペア！ という まあまあなプレイだ！」
  // 「それじゃ 頑張るだ！！」 (ボンドが移動)
  scrollCaption(3);
  flash(11);

  /* 投げ */
  // もつ
  pushButton(Button::A, 1300);
  // 移動
  tiltLeftJoystick(100, 5, 1000);
  // 方向調整
  tiltLeftJoystick(-45, -100, 100);
  // 投げる
  pushButton(Button::R, 100);
  flash(MOVIE_COUNT);
  // 「おおおっと！ 記録は 10本！！ ストライクってヤツだ！ 輝かしい記録だ！！」
  // (暗転)
  scrollCaption(1);
  flash(BLACKOUT_COUNT);
}

/**
 * @brief 二投目以降
 */
void loop() {
  /* 会話 */
  // 「若いの！すげぇだな！特別な道具を分けてやるだよ！」
  // 「と思っただが… 荷物がいっぱいみたいだな？ 代わりに これを受け取るだ！」
  // *金ルピー*
  // 「さて もう1回 遊んでいくだか？」
  // 「うれしいねぇ！ でも タダじゃないだよ！ 参加料に20ルピー貰うが いいだか?」
  // (暗転)
  scrollCaption(5);
  flash(BLACKOUT_COUNT);
  // 「それじゃ 頑張るだ！！」
  scrollCaption(1);

  /* 投げ */
  // 持てるところまで移動
  tiltLeftJoystick(-100, 0, 200);
  // もつ
  pushButton(Button::A, 1300);
  // 移動
  tiltLeftJoystick(-100, 0, 1500);
  // 方向調整
  tiltLeftJoystick(45, 100, 100);
  // 投げる
  pushButton(Button::R, 100);
  flash(MOVIE_COUNT);
  // 「おおおっと！ 記録は 10本！！ ストライクってヤツだ！ 輝かしい記録だ！！」
  // (暗転)
  scrollCaption(1);
  flash(BLACKOUT_COUNT);
}
