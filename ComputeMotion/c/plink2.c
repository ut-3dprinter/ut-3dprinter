#include <stdio.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include "kbhit.h"

#define X_SEARCH_RANGE 100 //調べる範囲[mm]
#define Y_SEARCH_RANGE 100
#define Z_SEARCH_RANGE 200
#define R1 100 //上半径[mm]
#define R2 10 //下
#define L1 120 //アーム[mm]

#define Z_UNDER_LIMIT 0
#define Z_UPPER_LIMIT 200
#define MANUAL_FRAG 0
#define MABIKI 5 //3D表示のドットを MABIKI %　に間引き（表示が重いから）


int main()
{
  int xs,ys,zs;
  double x[3],y[3],z[3];
  int i;
  double cos1 = cos(M_PI*2/3);
  double sin1 = sin(M_PI*2/3);
  double cos2 = cos(M_PI*4/3);
  double sin2 = sin(M_PI*4/3);
  char ForEdge[Y_SEARCH_RANGE*2][X_SEARCH_RANGE*2];
  int manual_frag = MANUAL_FRAG;
  int under_limit_frag = 0;
  int z_under_limit;

  srand(10); //間引き用の乱数

  FILE *data; //3D表示用のファイル
  data = fopen("data.txt","w");
  if(data == NULL) {perror("fopen:");return 1;}

  FILE *gp; //gnuplotへのパイプ
  gp = popen("gnuplot -persist","w");
  if(gp == NULL) {perror("popen:");return 1;}
  fprintf(gp,"set size square\n",-Y_SEARCH_RANGE,Y_SEARCH_RANGE);
  fprintf(gp,"set xrange [%d:%d]\n",-X_SEARCH_RANGE,X_SEARCH_RANGE);
  fprintf(gp,"set yrange [%d:%d]\n",-Y_SEARCH_RANGE,Y_SEARCH_RANGE);
  fprintf(gp,"set zrange [%d:%d]\n",-Z_SEARCH_RANGE,0);


  for(zs=0;zs<Z_SEARCH_RANGE;zs++){
    for(xs=-X_SEARCH_RANGE;xs<X_SEARCH_RANGE;xs++){
      for(ys=-Y_SEARCH_RANGE;ys<Y_SEARCH_RANGE;ys++){
	//点(xs,ys,zs)について条件を満たすthetaが存在するかの計算
	x[0] = xs;
	x[1] = xs*cos1 - ys*sin1;
	x[2] = xs*cos2 - ys*sin2;
	y[0] = ys;
	y[1] = xs*sin1 + ys*cos1;
	y[2] = xs*sin2 + ys*cos2;
	
	for(i=0;i<3;i++){
	  double in_root = L1*L1-(R1-x[i])*(R1-x[i])-y[i]*y[i];
	  if(in_root <0){
	    ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 0;
	    break;
	  }
	  z[i] = zs + sqrt(in_root);
	  if( (z[i] > Z_UPPER_LIMIT) || (z[i] < Z_UNDER_LIMIT) ){
	    ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 0;
	    break;
	  }
	  if(i==2){
	    ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 1;
	    if(!under_limit_frag){
	      under_limit_frag = 1;
	      z_under_limit = zs;
	    }
	  }
	}
      }
    }
    
    if(under_limit_frag){
      //z = zs　でスライスしてedgeだけ取り出したグラフの描写
      fprintf(gp,"set xrange [%d:%d]\n",-X_SEARCH_RANGE,X_SEARCH_RANGE);
      fprintf(gp,"set yrange [%d:%d]\n",-Y_SEARCH_RANGE,Y_SEARCH_RANGE);
      fprintf(gp,"set zrange [%d:%d]\n",z_under_limit,Z_SEARCH_RANGE);
      fprintf(gp,"set ticslevel 0\n");
      fprintf(gp,"set view 60,30,1.0,1.5\n");
      fprintf(gp,"splot '-' title \"z = %d\"\n",zs);
      for(xs=-X_SEARCH_RANGE;xs<X_SEARCH_RANGE;xs++){
	for(ys=-Y_SEARCH_RANGE;ys<Y_SEARCH_RANGE;ys++){
	  if(ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] == 1){
	    if( (xs == -X_SEARCH_RANGE)  ||
		(xs == X_SEARCH_RANGE-1) ||
		(ys == -Y_SEARCH_RANGE)  ||
		(ys == Y_SEARCH_RANGE-1) 
		){
	      fprintf(gp,"%d %d %d\n",xs,ys,zs);
	      if( (rand()%100) < MABIKI ) fprintf(data,"%d %d %d\n",xs,ys,zs);
	    }else if( (ForEdge[ys+Y_SEARCH_RANGE-1][xs+X_SEARCH_RANGE-1] == 0 ) ||
		      (ForEdge[ys+Y_SEARCH_RANGE-1][xs+X_SEARCH_RANGE+1] == 0 ) ||
		      (ForEdge[ys+Y_SEARCH_RANGE+1][xs+X_SEARCH_RANGE+1] == 0 ) ||
		      (ForEdge[ys+Y_SEARCH_RANGE+1][xs+X_SEARCH_RANGE-1] == 0 ) 
		      ){
	      //	     ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 2;
	      fprintf(gp,"%d %d %d\n",xs,ys,zs);
	      if( (rand()%100) < MABIKI ) fprintf(data,"%d %d %d\n",xs,ys,zs);
	    }
	  }
	}
      }
      if(manual_frag){
	while(1){
	  if(kbhit()){
	    char a;
	    if((a = getch())=='b') zs-=2;
	    else if( a == 'e') manual_frag = 0;
	    break;
	  }
	}
      }else{
	if(kbhit()){
	  getch();
	  manual_frag = 1;
	}
	sleep(0.5);
      }
      fprintf(gp,"e\n");
      //    fprintf(gp,"set size square\n");
      fprintf(data,"\n",xs,ys,zs);    
      }

  }
  fclose(data);
  //  fprintf(gp,"set pm3d map\n");

  //3Dデータの描写
  fprintf(gp,"set zrange [%d:%d]\n",z_under_limit,Z_SEARCH_RANGE);
  fprintf(gp,"set view 60,30,1.0,1.5\n");
  fprintf(gp,"set ticslevel 0\n");
  fprintf(gp,"splot 'data.txt'\n");
  pclose(gp);
  return 0;
}
