#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <string.h>  
#include <termios.h>  
#include <time.h>  
#include <sys/wait.h>  
  
/* <asm/termbits.h> で定義されているボーレートの設定．これは 
 <termios.h>からインクルードされる． */  
#define BAUDRATE B9600  
//Arduinoのソフトウェアシリアルでも使えるように9600bpsにする  
  
/* 適切なシリアルポートを指すように，この定義を変更 
 * 我が家の環境ではArduinoは/dev/ttyACM0になってました*/
#define MODEMDEVICE "/dev/ttyACM0"  
  
#define BUFFSIZE 256  
#define COULDNOTFORK -1  
  
#define FALSE 0  
#define TRUE 1  
  
volatile int STOP = FALSE;  
static int fd = -1;  
  
/* Functions */  
void serial_init(int fd);  
void recv_process();  
void send_process(pid_t result_pid);  
  

// シリアルポートの初期化  
void serial_init(int fd) {  
    struct termios tio;  
    memset(&tio, 0, sizeof(tio));  
    tio.c_cflag = CS8 | CLOCAL | CREAD;  
    /* 
     BAUDRATE: ボーレートの設定．cfsetispeed と cfsetospeed も使用できる． 
     CS8     : 8n1 (8 ビット，ノンパリティ，ストップビット 1) 
     CLOCAL  : ローカル接続，モデム制御なし 
     CREAD   : 受信文字(receiving characters)を有効にする． 
     */  
  
    tio.c_cc[VTIME] = 0; /* キャラクタ間タイマは未使用 */  
  
    /* 
     ICANON  : カノニカル入力(行単位入力）を有効にする 
     */  
    tio.c_lflag = ICANON;  
  
    /* 
     IGNPAR  : パリティエラーのデータは無視する 
     ICRNL   : CR を NL に対応させる(これを行わないと，他のコンピュータで 
     CR を入力しても，入力が終りにならない) 
     それ以外の設定では，デバイスは raw モードである(他の入力処理は行わない) 
     */  
    tio.c_iflag = IGNPAR | ICRNL;  
  
    // ボーレートの設定  
    cfsetispeed(&tio, BAUDRATE);  
    cfsetospeed(&tio, BAUDRATE);  
    // デバイスに設定を行う  
    tcsetattr(fd, TCSANOW, &tio);  
}  

  
int main(int argc,char **argv) {  
    pid_t result_pid;  
    int fd_port,fd_data,read_count;
    char *filename,chain_num;
    struct termios oldtio, newtio;  

    short buf[3];

    /* 
     読み書きのためにモデムデバイスをオープンする．ノイズによって CTRL-C 
     がたまたま発生しても接続が切れないように，tty 制御はしない． 
     */  
  
    if(argc != 2){
      printf("usage: %s filename\n",argv[0]);
      return(-1);
    }
    filename = argv[1];

    fd_port = open(MODEMDEVICE, O_RDWR | O_NOCTTY);  
    if (fd_port < 0) {  
        perror("can't open usb_port\n");  
        return (-1);  
    } 

    fd_data = open( filename, O_RDWR );  
    if (fd_data < 0) {  
      perror("can't open file\n");  
        return (-1);  
    } 
  
    tcgetattr(fd, &oldtio); /* 現在のシリアルポートの設定を待避させる*/  
    memset(&newtio, 0, sizeof(newtio));/* 新しいポートの設定の構造体をクリアする */  

    //シリアルポート準備  
    serial_init(fd);  

    int i;
    char low[3],high[3];
    char rcv[10];
    unsigned char send_data[6];
    //データ送信
    while(1){
      if(read(fd_data,buf,sizeof(short)*3) == 0){
	perror("read");
	exit(1);
      }
      for(i=0;i<3;i++){
	send_data[2*i] = (char)buf[i];          //low_byte
	send_data[2*i+1] = (char)( buf[i] >> 8 );   //high_byte
      }
      write(fd_port,send_data,6);
      //printf("%u %u %u\n",send_data[0],send_data[1], send_data[2]);
      read(fd_port,rcv,4);
      usleep(10000);

    }
    return 0;  
}  
