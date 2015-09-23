// ロータリーエンコーダーの配線に合わせる
int inA  = 2;
int inB  = 4;

int A = HIGH;
int B = HIGH;
int preA;
int preB;
int count = 0;
char veloc  = 0;
int data[10] = {0,0,0,0,0};
int loc = 0;

int period = 33;

void setup()  
{  
  // ピンの設定
  // INPUTモードにする
  pinMode(inA, INPUT);
  pinMode(inB, INPUT);
  
  // プルアップを有効にする
  digitalWrite(inA, HIGH);
  digitalWrite(inB, HIGH);

  Serial.begin(9600);
  Serial.write(0);
}

void loop()
{
  observeState();
  delay(3);
}

//ピン状態の変化を監視
void observeState() {
  A = digitalRead(inA);
  B = digitalRead(inB);

  //前のピン状態から変化していれば、エンコーダ状態を更新する(updateState(A,B))
  if(A != preA || B != preB)
    updateState(A, B);

  //observeState()が呼ばれた回数をカウント
  count = (count+1)%period;
  //(period)回呼ばれたらupdateData()を実行
  if(!count) updateData();
}

//エンコーダ状態の更新
//(period)回呼ばれたらupdateData()呼び出し
void updateState(int a, int b) {
  veloc++;
  preA = a;
  preB = b;
}

//データの更新、およびシリアル通信
void updateData() {
  data[loc] = veloc;
  char finalVeloc = 0;
  for(int i=0; i<5; i++) {
    finalVeloc += data[i];
  }
  finalVeloc /= 5;
  
  //if(finalVeloc != veloc)
  Serial.write(finalVeloc);
  
  veloc = 0;
  loc = (loc+1)%5;
}
