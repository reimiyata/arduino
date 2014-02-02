////ライブラリ////

//メロディ用のライブラリをインクルード（Arduinoのサンプルに入っている"pitches.h"ファイルを、スケッチと同じフォルダにコピーして使います）
#include "pitches.h"
//サーボのライブラリをインクルード
#include <Servo.h>
//出力用サーボのインスタンスの宣言
Servo myservo;

////ピンと変数の設定////

//圧力センサー
int pressPin = A0;

//光センサー（今回は使わない）
int lightPin = A1;

//温度センサー（今回は使わない）
int tempPin = A2;

//サーボモーター
#define SERVO 10

//LEDピン
int ledPinX = 13;
int ledPinY = 12;
int ledPinZ = 9;

////ブザー関係////

//Noteの数を入れる変数
int notenum;

//ファンファーレのメロディ定義（自分でつくると楽しいよ。※このメロディは友人のスタッフが作ってくれました。）
int melody[] = {
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_GS4, NOTE_AS4, NOTE_C5, 0, NOTE_AS4, NOTE_C5
  };
double noteDurations[] = {
  8.0, 8.0, 8.0, 2.6, 2.6, 2.6 ,8.0 ,8.0, 8.0, 2.0
  };

//noteDurations配列の要素数を取得して、notenumに入れる
int notenum = sizeof(noteDurations) / sizeof(noteDurations[0]);

//セットアップ
void setup() {
  //シリアル
  Serial.begin(9600); 

  //圧力センサー
  pinMode(pressPin, INPUT);

  //光センサー（今回使わない）
  pinMode(lightPin, INPUT);

  //温度センサー（今回使わない）
  pinMode(tempPin, INPUT);

  //サーボモーターのピン
  myservo.attach(SERVO);
  myservo.write(0);
  //LED
  pinMode(ledPinX, OUTPUT);  
  pinMode(ledPinY, OUTPUT);  
  pinMode(ledPinZ, OUTPUT);  
}

void loop() {
  // アナログインプットからの値を読み取る
  int pressValue;
  int lightValue;//（今回は使わない）
  int tempValue;//（今回は使わない）
  int val = 0;
  pressValue = analogRead(pressPin);
  lightValue = analogRead(lightPin);//（今回は使わない）
  tempValue = analogRead(tempPin);//（今回は使わない）
  val = map(pressValue, 0, 1023, 0, 250);  //map関数でpressValueを調整

  Serial.println(val);  //デバッグ用

  //一定の値を超えると、LEDが光って、ブザーが鳴り、サーボモーターが動く
  if(val>50 && val<=100){
    digitalWrite(ledPinX, HIGH);
    digitalWrite(ledPinY, LOW);
    digitalWrite(ledPinZ, LOW);
    delay(10);
  } else if(val>100 && val<=150){
    digitalWrite(ledPinX, HIGH);
    digitalWrite(ledPinY, HIGH);  
    digitalWrite(ledPinZ, LOW);  
    delay(10);
  }else if(val>150 && val<=200){
    digitalWrite(ledPinX, HIGH);
    digitalWrite(ledPinY, HIGH);  
    digitalWrite(ledPinZ, HIGH);
    delay(10);
  }else if(val>200){
    digitalWrite(ledPinX, HIGH);
    digitalWrite(ledPinY, HIGH);  
    digitalWrite(ledPinZ, HIGH);
    delay(10);

    //メロディーを流す
    for (int thisNote = 0; thisNote < notenum; thisNote++) {
      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      double noteDuration = 1000.0/noteDurations[thisNote];
      tone(8, melody[thisNote],noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(notenum);
    }

    //サーボモーターをゆっくり動かす  
    for(val = 0;val < 50;val++){
        myservo.write(val);
        delay(50);
    }
    delay(300);
    myservo.write(0);

  }else{
    digitalWrite(ledPinX, LOW);
    digitalWrite(ledPinY, LOW);  
    digitalWrite(ledPinZ, LOW);
    delay(10);
  }
}
