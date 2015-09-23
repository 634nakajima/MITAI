// ロータリーエンコーダーの配線に合わせる
int inA  = 2;
int inB  = 4;

// ロータリーエンコーダーの状態を記憶する
// 割り込み中に変化する変数はvolatileはをつけて宣言する
volatile char m_nOldRot = 0;

// 今回はLCDに回転回数を表示させる
volatile char m_nValue  = 0;
volatile char m_Value  = 0;

int data[10] = {0,0,0,0,0,0,0,0,0,0};
int loc = 0;
int state = 0;
int prevstate = 0;

void setup()  
{  
  // ピンの設定
  // INPUTモードにします。
  pinMode(inA, INPUT);
  pinMode(inB, INPUT);
  
  // プルアップを有効にします
  digitalWrite(inA, HIGH);
  digitalWrite(inB, HIGH);
  
  // 外部割り込みを設定します
  // D2ピンが 変化 した時にrotRotEnd()を呼び出します
  attachInterrupt(0, rotRotEnc, CHANGE);
  Serial.begin(9600);
}

void loop()
{
  m_nValue = 0;
  for (int i=0;i<10;i++) {
    m_nValue += data[i];
  }
  Serial.write(m_nValue);
  loc = (loc+1)%10;
  data[loc] = 0;
  delay(150);

}

// 外部割り込みから呼び出される変数
void rotRotEnc(void)
{
  if(!digitalRead(inA)){  // ロータリーエンコーダー回転開始
    if(digitalRead(inB)){
      //右回転かな?
      m_nOldRot = 'R';
    } else {
      //左回転かな?
      m_nOldRot = 'L';
    }
  } else {  // ロータリーエンコーダー回転停止
    if(digitalRead(inB)){
      if(m_nOldRot == 'L'){ 
        // 左回転の時の処理
        //m_nValue++;
        data[loc] += 1; 
      }
    } else {
      if(m_nOldRot == 'R'){
        //右回転の時の処理
        //m_nValue++;
        data[loc] += 1; 
      }
    }
    // ここでロータリーエンコーダーの状態をクリア
    m_nOldRot = 0;
  }
}
