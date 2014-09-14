#include <stdio.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define X_SEARCH_RANGE 10 //調べる範囲[mm]
#define Y_SEARCH_RANGE 10
#define Z_UPPER_RANGE 0
#define Z_UNDER_RANGE 400
#define R1 50 //上の板の半径[mm]
#define R2 10 //下の板
#define L1 80 //一本目のアーム[mm]
#define L2 140 //二本目
#define THETA_UNDER_LIMIT 0 //動かすθの下限[°]
#define THETA_UPPER_LIMIT 120 //　　　　　上限

double theta_under_limit = M_PI * THETA_UNDER_LIMIT * 1.0 / 180;
double theta_upper_limit = M_PI * THETA_UPPER_LIMIT * 1.0 / 180;

int kbhit(void);
int getch(void);

int main()
{
  int xs,ys,zs;
  double x[3],y[3];
  double theta1,theta2,theta[3],pre_theta[3];
  double disc;
  int i,j;
  double cos1 = cos(M_PI*2/3);
  double sin1 = sin(M_PI*2/3);
  double cos2 = cos(M_PI*4/3);
  double sin2 = sin(M_PI*4/3);
  char ForEdge[Y_SEARCH_RANGE*2][X_SEARCH_RANGE*2];
  double min_theta = 100;

  pre_theta[0] = 100;
  pre_theta[1] = 100;
  pre_theta[2] = 100;

  FILE *data;
  data = fopen("data.txt","w");
  if(data == NULL) {perror("fopen:");return 1;}


  for(zs=-Z_UNDER_RANGE;zs<-Z_UPPER_RANGE;zs++){
    for(xs=-X_SEARCH_RANGE;xs<X_SEARCH_RANGE;xs++){
      for(ys=-Y_SEARCH_RANGE;ys<Y_SEARCH_RANGE;ys++){
	x[0] = xs;
	x[1] = xs*cos1 - ys*sin1;
	x[2] = xs*cos2 - ys*sin2;
	y[0] = ys;
	y[1] = xs*sin1 + ys*cos1;
	y[2] = xs*sin2 + ys*cos2;
	
	for(i=0;i<3;i++){
	  double kou1 = 2*(x[i]+R1-R2)*L1;
	  double kou2 = 2*zs*L1;
	  double kou3 = (x[i]+R1-R2)*(x[i]+R1-R2) + y[i]*y[i] + zs*zs + L1*L1 - L2*L2;
	  disc = kou1*kou1 + kou2*kou2 - kou3*kou3;
	  if(disc < 0) {
	    ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 0;
	    break;
	  }
	  theta1 = 2 * atan( ( kou1 + sqrt(disc) ) / ( kou3 - kou2) );
	  theta2 = 2 * atan( ( kou1 - sqrt(disc) ) / ( kou3 - kou2) );
	  if(theta1 >= theta2 ){theta[i] = theta1;}
	  else{theta[i] = theta2;}
	  if( (theta_under_limit > theta[i]) || (theta_upper_limit < theta[i]) ){
	    ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 0;
	    //printf("b%d %d %d\n",xs,ys,zs);
	    break;
	  }
	  if(i==2){
	    //printf("c%d %d %d\n",xs,ys,zs);
	    ForEdge[ys+Y_SEARCH_RANGE][xs+X_SEARCH_RANGE] = 1;
	    if( (ys != -Y_SEARCH_RANGE) &&
		(ForEdge[ys+Y_SEARCH_RANGE-1][xs+X_SEARCH_RANGE] == 1) ){
	      fprintf(stdout,"%lf % lf %lf  %d %d %d\n",theta[0],theta[1],theta[2],xs,ys,zs);
	    }   
	  }
	}	
      }
    }
  }
  fclose(data);
  return 0;
}



