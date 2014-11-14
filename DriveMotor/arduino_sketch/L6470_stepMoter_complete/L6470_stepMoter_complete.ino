/*デイジーチェン接続 
Arduino11番ピン（MOSI）- 6番ピン(SDI) L6470(一台目) 3番ピン(SDO) - (SDI)L6470(二台目)(SDO) - 12番ピン（MISO) Arduino
SCK 共通
SS - CS共通
*/
#include <SPI.h>
//#include <MsTimer2.h>
//#include <stdio.h>



// ピン定義。

#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9
#define PIN_BUSY2 8
#define PIN_BUSY3 7

void setup()
{
  
  delay(1000);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  pinMode(PIN_BUSY2, INPUT);
  pinMode(PIN_BUSY3, INPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(9600);
  digitalWrite(PIN_SPI_SS, HIGH);
  
     L6470_resetdevice(); //1台目のL6470リセット
     L6470_resetdevice2(); //2台目のL6470リセット
     L6470_resetdevice3();
     Serial.print("resetdevise end\n");
     L6470_setup();  //1台目のL6470を設定 
     L6470_setup2();  //2台目のL6470を設定 
     L6470_setup3();
     Serial.print("setup end\n");
     L6470_getstatus(); //1台目のフラグ解放
     L6470_getstatus2();//2台目のフラグ解放
     L6470_getstatus3();
     Serial.print("getstatus end\n");
     
// MsTimer2::set(25, fulash);//シリアルモニター用のタイマー割り込み
// MsTimer2::start();
  Serial.flush();
  
delay(2000);
 
}

void loop(){
    //PCからint値3つを取得
    unsigned int t[3];
    unsigned char low,high;
    int i;
  if(Serial.available() > 5 ){
    for(i=0;i<3;i++){
      low  = Serial.read();
      //Serial.print(low);
      high = Serial.read();
      //Serial.print(high);
      t[i] = (unsigned int)low + (unsigned int)( high << 8 );
    }
    
    L6470_goto(  (long)t[0] );
    L6470_goto2( (long)t[1] );
    L6470_goto3( (long)t[2] );
    
    
    /*  
    Serial.print(t[0]);
    Serial.print("kugir
    i\n"); 
    Serial.print(t[1]);
    Serial.print("kugiri2\n");
    Serial.print(t[2]);
    Serial.print("kugiri3\n");
    */
    
    Serial.flush();
    Serial.print("ack");
  }
}

void L6470_setup(){
//L6470_setparam_acc(0x30); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
//L6470_setparam_dec(0x30); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_maxspeed(0x20); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
//L6470_setparam_minspeed(0x1200); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
//L6470_setparam_fsspd(0x027); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
L6470_setparam_kvalhold(0xFF); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalrun(0xFF); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalacc(0xFF); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvaldec(0xFF); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_ocdth(0xF);
L6470_setparam_stallth(0x7F);

//L6470_setparam_stepmood(0x02); //ステップモードdefault 0x07 (1+3+1+3bit)
}

void L6470_setup2(){
//L6470_setparam_acc2(0x30); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
//L6470_setparam_dec2(0x30); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_maxspeed2(0x2a); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
//L6470_setparam_minspeed2(0x1200); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
//L6470_setparam_fsspd2(0x027); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
L6470_setparam_kvalhold2(0xFF); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalrun2(0xFF); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalacc2(0xFF); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvaldec2(0xFF); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_ocdth2(0xF);
L6470_setparam_stallth2(0x7F);

//L6470_setparam_stepmood2(0x02); //ステップモードdefault 0x07 (1+3+1+3bit)
}

void L6470_setup3(){
//L6470_setparam_acc3(0x30); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
//L6470_setparam_dec3(0x30); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_maxspeed3(0x2a); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
//L6470_setparam_minspeed3(0x1200); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
//L6470_setparam_fsspd3(0x027); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
L6470_setparam_kvalhold3(0xFF); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalrun3(0xFF); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalacc3(0xFF); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvaldec3(0xFF); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_ocdth3(0xF);
L6470_setparam_stallth3(0x7F);

//L6470_setparam_stepmood2(0x02); //ステップモードdefault 0x07 (1+3+1+3bit)
}

void fulash(){
  

long a=L6470_getparam_abspos();
long b=L6470_getparam_speed();
long c=L6470_getparam_abspos2();
long d=L6470_getparam_speed2();
char str[15];
snprintf(str,sizeof(str),"1pos=0x%6.6X ",a);
Serial.print(str);
snprintf(str,sizeof(str),"1spd=0x%5.5X ",b);
Serial.print(str);
snprintf(str,sizeof(str),"2pos=0x%6.6X ",c);
Serial.print(str);
snprintf(str,sizeof(str),"2spd=0x%5.5X ",d);
Serial.println(str);
  
 /* Serial.print("0x");
  Serial.print( L6470_getparam_abspos(),HEX);
  Serial.print(" 0x");
  Serial.print( L6470_getparam_speed(),HEX);
  Serial.print(" 0x");
  Serial.print( L6470_getparam_abspos2(),HEX);
  Serial.print(" 0x");
  Serial.println( L6470_getparam_speed2(),HEX);
  */
}


