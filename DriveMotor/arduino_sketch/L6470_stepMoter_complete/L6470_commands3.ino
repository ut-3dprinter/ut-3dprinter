/*L6470 3台目コントロール　コマンド
 引数-----------------------
 dia   1:正転 0:逆転,
 spd  (20bit)(0.015*spd[step/s])
 pos  (22bit)
 n_step (22bit)
 act   1:絶対座標をマーク  0:絶対座標リセット
 mssec ミリ秒
 val 各レジスタに書き込む値
 ---------------------------
 L6470_run3(dia,spd);//指定方向に連続回転 [n]
 L6470_stepclock3(dia);//指定方向にstepピンのクロックで回転[n]
 L6470_move3(dia,n_step);//指定方向に指定数ステップする[B]
 L6470_goto3(pos);//指定座標に最短でいける回転方向で移動[B]
 L6470_gotodia3(dia,pos);//回転方向を指定して指定座標に移動[B]
 L6470_gountil3(act,dia,spd);//指定した回転方向に指定した速度で回転し、スイッチのONで急停止と座標処理[B]
 L6470_relesesw3(act,dia);//スイッチがOFFに戻るまで最低速度で回転し、停止と座標処理[B]
 L6470_gohome3();//座標原点に移動[B]
 L6470_gomark3();//マーク座標に移動[B]
 L6470_resetpos3();//絶対座標リセット[n]
 L6470_resetdevice3();//L6470リセット[n]
 L6470_softstop3();//回転停止、保持トルクあり[B]
 L6470_hardstop3();//回転急停止、保持トルクあり[n]
 L6470_softhiz3();//回転停止、保持トルクなし[B]
 L6470_hardhiz3();//回転急停止、保持トルクなし[n]
 L6470_getstatus3();//statusレジスタの値を返す[n]（L6470_getparam_status();と同じ）
 
 L6470_busydelay3(msec);　//busyフラグがHIGHになってから、指定ミリ秒待つ。

 [n]:busy信号なし、もしくは瞬間的。　
 [B]:busy信号あり。(オープンドレインのトランジスタがONになること)
 
 レジスタ書き込みコマンド
 L6470_setparam_abspos3(val); //[R, WS]現在座標default 0x000000 (22bit)
 L6470_setparam_elpos3(val); //[R, WS]コイル励磁の電気的位置default 0x000 (2+7bit)
 L6470_setparam_mark3(val); //[R, WR]マーク座標default 0x000000 (22bit)
 //ありませんL6470_spped3 //[R] 現在速度read onry  (20bit)
 L6470_setparam_acc3(val); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
 L6470_setparam_dec3(val); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
 L6470_setparam_maxspeed3(val); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
 L6470_setparam_minspeed3(val); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val+[step/s])
 L6470_setparam_fsspd3(val); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
 L6470_setparam_kvalhold3(val); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6470_setparam_kvalrun3(val); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6470_setparam_kvalacc3(val); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6470_setparam_kvaldec3(val); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
 L6470_setparam_intspd3(val); //[R, WH]逆起電力補償切替点速度default 0x0408 (14bit) (0.238*val[step/s])
 L6470_setparam_stslp3(val); //[R, WH]逆起電力補償低速時勾配default 0x19 (8bit) (0.000015*val[% s/step])
 L6470_setparam_fnslpacc3(val); //[R, WH]逆起電力補償高速時加速勾配default 0x39 (8bit) (0.000015*val[% s/step])
 L6470_setparam_fnslpdec3(val); //[R, WH]逆起電力補償高速時減速勾配default 0x29 (8bit) (0.000015*val[% s/step])
 L6470_setparam_ktherm3(val); //[R, WR]不明default 0x0 (4bit) (0.03125*val+1)
 //ありませんL6470_adcout3 //[R] read onry (5bit) ADCによる逆起電力補償の大きさかな？
 L6470_setparam_ocdth3(val); //[R, WR]過電流しきい値default 0x8 (4bit) (375*val+375[mA])
 L6470_setparam_stallth3(val); //[R, WR]失速電流しきい値？default 0x40 (7bit) (31.25*val+31.25[mA])
 L6470_setparam_stepmood3(val); //[R, WH]ステップモードdefault 0x07 (1+3+1+3bit)
 L6470_setparam_alareen3(val); //[R, WS]有効アラームdefault 0xff (1+1+1+1+1+1+1+1bit)
 L6470_setparam_config3(val); //[R, WH]各種設定default 0x2e88 (3+3+2+1+1+1+1+1+3bit)
 //L6470_status //[R]状態read onry (16bit)

 [R]:読み取り専用
 [WR]:いつでも書き換え可
 [WH]:書き込みは出力がハイインピーダンスの時のみ可
 [WS]:書き換えはモータが停止している時のみ可
 
 
 レジスタ読み込みコマンド(返り値　long型)
 L6470_getparam_abspos3();
 L6470_getparam_elpos3();
 L6470_getparam_mark3();
 L6470_getparam_speed3();
 L6470_getparam_acc3();
 L6470_getparam_dec3();
 L6470_getparam_maxspeed3();
 L6470_getparam_minspeed3();
 L6470_getparam_fsspd3();
 L6470_getparam_kvalhold3();
 L6470_getparam_kvalrun3();
 L6470_getparam_kvalacc3();
 L6470_getparam_kvaldec3();
 L6470_getparam_intspd3();
 L6470_getparam_stslp3();
 L6470_getparam_fnslpacc3();
 L6470_getparam_fnslpdec3();
 L6470_getparam_ktherm3();
 L6470_getparam_adcout3();
 L6470_getparam_ocdth3();
 L6470_getparam_stallth3();
 L6470_getparam_stepmood3();
 L6470_getparam_alareen3();
 L6470_getparam_config3();
 L6470_getparam_status3();
 */
//２台目用
void L6470_setparam_abspos3(long val){L6470_transfer3(0x01,3,val);}
void L6470_setparam_elpos3(long val){L6470_transfer3(0x02,2,val);}
void L6470_setparam_mark3(long val){L6470_transfer3(0x03,3,val);}
void L6470_setparam_acc3(long val){L6470_transfer3(0x05,2,val);}
void L6470_setparam_dec3(long val){L6470_transfer3(0x06,2,val);}
void L6470_setparam_maxspeed3(long val){L6470_transfer3(0x07,2,val);}
void L6470_setparam_minspeed3(long val){L6470_transfer3(0x08,2,val);}
void L6470_setparam_fsspd3(long val){L6470_transfer3(0x15,2,val);}
void L6470_setparam_kvalhold3(long val){L6470_transfer3(0x09,1,val);}
void L6470_setparam_kvalrun3(long val){L6470_transfer3(0x0a,1,val);}
void L6470_setparam_kvalacc3(long val){L6470_transfer3(0x0b,1,val);}
void L6470_setparam_kvaldec3(long val){L6470_transfer3(0x0c,1,val);}
void L6470_setparam_intspd3(long val){L6470_transfer3(0x0d,2,val);}
void L6470_setparam_stslp3(long val){L6470_transfer3(0x0e,1,val);}
void L6470_setparam_fnslpacc3(long val){L6470_transfer3(0x0f,1,val);}
void L6470_setparam_fnslpdec3(long val){L6470_transfer3(0x10,1,val);}
void L6470_setparam_ktherm3(long val){L6470_transfer3(0x11,1,val);}
void L6470_setparam_ocdth3(long val){L6470_transfer3(0x13,1,val);}
void L6470_setparam_stallth3(long val){L6470_transfer3(0x14,1,val);}
void L6470_setparam_stepmood3(long val){L6470_transfer3(0x16,1,val);}
void L6470_setparam_alareen3(long val){L6470_transfer3(0x17,1,val);}
void L6470_setparam_config3(long val){L6470_transfer3(0x18,2,val);}

//２台目用
long L6470_getparam_abspos3(){return L6470_getparam3(0x01,3);}
long L6470_getparam_elpos3(){return L6470_getparam3(0x02,2);}
long L6470_getparam_mark3(){return L6470_getparam3(0x03,3);}
long L6470_getparam_speed3(){return L6470_getparam3(0x04,3);}
long L6470_getparam_acc3(){return L6470_getparam3(0x05,2);}
long L6470_getparam_dec3(){return L6470_getparam3(0x06,2);}
long L6470_getparam_maxspeed3(){return L6470_getparam3(0x07,2);}
long L6470_getparam_minspeed3(){return L6470_getparam3(0x08,2);}
long L6470_getparam_fsspd3(){return L6470_getparam3(0x15,2);}
long L6470_getparam_kvalhold3(){return L6470_getparam3(0x09,1);}
long L6470_getparam_kvalrun3(){return L6470_getparam3(0x0a,1);}
long L6470_getparam_kvalacc3(){return L6470_getparam3(0x0b,1);}
long L6470_getparam_kvaldec3(){return L6470_getparam3(0x0c,1);}
long L6470_getparam_intspd3(){return L6470_getparam3(0x0d,2);}
long L6470_getparam_stslp3(){return L6470_getparam3(0x0e,1);}
long L6470_getparam_fnslpacc3(){return L6470_getparam3(0x0f,1);}
long L6470_getparam_fnslpdec3(){return L6470_getparam3(0x10,1);}
long L6470_getparam_ktherm3(){return L6470_getparam3(0x11,1);}
long L6470_getparam_adcout3(){return L6470_getparam3(0x12,1);}
long L6470_getparam_ocdth3(){return L6470_getparam3(0x13,1);}
long L6470_getparam_stallth3(){return L6470_getparam3(0x14,1);}
long L6470_getparam_stepmood3(){return L6470_getparam3(0x16,1);}
long L6470_getparam_alareen3(){return L6470_getparam3(0x17,1);}
long L6470_getparam_config3(){return L6470_getparam3(0x18,2);}
long L6470_getparam_status3(){return L6470_getparam3(0x19,2);}

//3台目用
void L6470_run3(int dia,long spd){
  if(dia==1)
    L6470_transfer3(0x51,3,spd);
  else
    L6470_transfer3(0x50,3,spd);
}
void L6470_stepclock3(int dia){
  if(dia==1)
    L6470_transfer3(0x59,0,0);    
  else
    L6470_transfer3(0x58,0,0);
}
void L6470_move3(int dia,long n_step){
  if(dia==1)
    L6470_transfer3(0x41,3,n_step);
  else
    L6470_transfer3(0x40,3,n_step);
}
void L6470_goto3(long pos){
  L6470_transfer3(0x60,3,pos);
}
void L6470_gotodia3(int dia,int pos){
  if(dia==1)    
    L6470_transfer3(0x69,3,pos);
  else    
    L6470_transfer3(0x68,3,pos);
}
void L6470_gountil3(int act,int dia,long spd){
  if(act==1)
    if(dia==1)
      L6470_transfer3(0x8b,3,spd);
    else
      L6470_transfer3(0x8a,3,spd);
  else
    if(dia==1)
      L6470_transfer3(0x83,3,spd);
    else
      L6470_transfer3(0x82,3,spd);
}  
void L6470_relesesw3(int act,int dia){
  if(act==1)
    if(dia==1)
      L6470_transfer3(0x9b,0,0);
    else
      L6470_transfer3(0x9a,0,0);
  else
    if(dia==1)
      L6470_transfer3(0x93,0,0);
    else
      L6470_transfer3(0x92,0,0);
}
void L6470_gohome3(){
  L6470_transfer3(0x70,0,0);
}
void L6470_gomark3(){
  L6470_transfer3(0x78,0,0);
}
void L6470_resetpos3(){
  L6470_transfer3(0xd8,0,0);
}
void L6470_resetdevice3(){
  L6470_send3(0x00);//nop命令
  L6470_send3(0x00);
  L6470_send3(0x00);
  L6470_send3(0x00);
  L6470_send3(0xc0);
}
void L6470_softstop3(){
  L6470_transfer3(0xb0,0,0);
}
void L6470_hardstop3(){
  L6470_transfer3(0xb8,0,0);
}
void L6470_softhiz3(){
  L6470_transfer3(0xa0,0,0);
}
void L6470_hardhiz3(){
  L6470_transfer3(0xa8,0,0);
}

long L6470_getstatus3(){
  long val=0;
  L6470_send3(0xd0);
  for(int i=0;i<=1;i++){
    val = val << 8;
    digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
    val = val | SPI.transfer(0x00); // アドレスもしくはデータ送信。
    SPI.transfer(0x00); // アドレスもしくはデータ送信。
    digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル 
  }
  return val;
}
void L6470_transfer3(int add,int bytes,long val){//2台目に送信
  int data[3];
//while(!busy_flag2()){} //BESYが解除されるまで待機
  while(!digitalRead(PIN_BUSY3)){}//BESYピン出力が解除されるまで待機
  L6470_send3(add);
  for(int i=0;i<=bytes-1;i++){
    data[i] = val & 0xff;  
    val = val >> 8;
  }
  if(bytes==3){
    L6470_send3(data[2]);
  }
  if(bytes>=2){
    L6470_send3(data[1]);
  }
  if(bytes>=1){
    L6470_send3(data[0]);
  }  
}
void L6470_send3(unsigned char add_or_val){//3台目に送信

  digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
  SPI.transfer(add_or_val); // アドレスもしくはデータ送信。
  SPI.transfer(0); // 0送信。
  SPI.transfer(0);
  digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル。
}

void L6470_busydelay3(long time){
//while(!busy_flag2()){}
  while(!digitalRead(PIN_BUSY3)){}//BESYピン出力が解除されるまで待機
  delay(time);
}

long L6470_getparam3(int add,int bytes){//3台目に送信
  long val=0;
  int send_add = add | 0x20;
  L6470_send3(send_add);
  for(int i=0;i<=bytes-1;i++){
    val = val << 8;
    digitalWrite(PIN_SPI_SS, LOW); // ~SSイネーブル。
    val = val | SPI.transfer(0x00); // アドレスもしくはデータ送信。
    SPI.transfer(0x00);
    SPI.transfer(0x00); // アドレスもしくはデータ送信。
    digitalWrite(PIN_SPI_SS, HIGH); // ~SSディスエーブル 
  }
  return val;
}

int busy_flag3(){
int sta=L6470_getparam_status3();
sta = sta >> 1;
sta = sta & 0x0001;
return sta;
}

