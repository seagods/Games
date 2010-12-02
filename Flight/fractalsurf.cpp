#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "/usr/local/include/SDL/SDL.h"
#include "/home/user1/daddio/libs/SOURCES/ranlib/src/ranlib.h"

using namespace std;

const int N=6;  //eg N =3, line divided into 2, then 2 again, then 2 again, so 8 segments
int main(){

	//we are using the  ranlib library
	//
	int iseg=(int)pow(2,N);   //Number of segments is 2**N
        int iseg2=iseg/2;
        int Num=iseg+1;   //number of nodes on baseline is number of segments+1
	int Num2=iseg;  //iseg shall be successively halved, keep copy

	double Array[Num][Num];
	int isquare[4];
        int trinodes[ Num2*Num2*2][3];
	bool EVEN;
        double reciproot2=1.0/sqrt(2.0);

	int istop;
	int i1,j1,i2,j2,i3,j3,i4,j4,i1max,j1max;


	long iseed1, iseed2;

	unsigned int sdlticks0, sdlticks1, sdlticks2;

	sdlticks0=SDL_GetTicks();

	cout << "enter an integer\n";
	int igarbage;
	cin >> igarbage;

	sdlticks1=SDL_GetTicks();

	cout << "enter another integer\n";
	cin >> igarbage;
	sdlticks2=SDL_GetTicks();

	iseed1=sdlticks1-sdlticks0;
	iseed2=sdlticks2-sdlticks0;
	cout << "seeds set to " << iseed1 << "  and " << iseed2 << endl;
	
	setall(iseed1, iseed2);

	long iran;
	float fran;

	for(int i=0; i<Num;i++)
		for(int j=0; j<Num; j++)
			Array[i][j]=0.0;

        double distx, disty, shiftx, shifty;
        //spacing
	distx=0.125; disty=0.125;

	shiftx=distx*(double)(Num2/2);
	shifty=disty*(double)(Num2/2);

	double maxheight=-1e12, minheight=1e12, meanheight=0.0;

	float av, sd;  //mean and standard deviation for normal dist.
	//	iran=ignlgi();  // random integers 1 to 2147483562
		//fran=ranf();  // random integers 1 to 1.0

	cout <<  "enter variance\n" << endl;
	double var;
	cin >> var;
	float sd0=sqrt(var);
        float rootfac=1.0/sqrt(2.0);

        float sdrootfac=sd*rootfac;
	double D, H;
	cout << "enter Housedorf dimension   2 <D<3\n";
	cin >> D;
	H=3.0-D;
	float sd1;
        sd1=sd0/pow(2.0,H)*sqrt(1.0-pow(2.0, 2*H-2));  //eqn 1.0 Peitgen and Saupe
        sd=sd1;
	av=0.0;

        float levelfactor=pow(2.0,H);

	// Note that we have periodic boundary conditions
	// at the edges

        for(int k=0; k< N; k++){
 //       two sweeps
 //       iseg and iseg2 halved at end of second sweep
 //       first sweep - diagonals
          istop=(int)pow(2.0,k);
          for(int i=0; i < istop; i++){
             for(int j=0; j < istop; j++){

                isquare[0]=iseg*i; isquare[1]=iseg*(i+1);
                isquare[2]=iseg*j; isquare[3]=iseg*(j+1);
 
                i1=isquare[0]; j1=isquare[2];
                i2=isquare[1]; j2=isquare[2];
		i3=isquare[0]; j3=isquare[3];
		i4=isquare[1]; j4=isquare[3];

                Array[i1+iseg2][j1+iseg2]=(Array[i1][j1]
		   +Array[i2][j2]+Array[i3][j3]+Array[i4][j4])/4.0
		+gennor(av, sd);

                               }  //end i loop
	          } //end j loop - continue k loop

//      second sweep
		for(int i=0; i<istop; i++){
	           for(int j=0; j<istop; j++){
                   isquare[0]=iseg*i;  isquare[1]=iseg*(i+1);
                   isquare[2]=iseg*j;  isquare[3]=iseg*(j+1);

                   i1=isquare[0];  j1=isquare[2];
                   i2=isquare[1];  j2=isquare[2];
                   i3=isquare[0];  j3=isquare[3];
                   i4=isquare[1];  j4=isquare[3];
      if(j1==0){
              Array[i1+iseg2][j1]=
     (Array[i1][j1]+Array[i2][j2]+Array[i1+iseg2][j1+iseg2]
     +Array[i1+iseg2][Num2-iseg2] )/4.0
		+gennor(av, sdrootfac );
      }
      if(j3 < Num2) {
              Array[i1+iseg2][j3]=
     (Array[i3][j3]+Array[i4][j4]+Array[i1+iseg2][j3+iseg2]
     +Array[i1+iseg2][j3-iseg2] )/4.0;
		+gennor(av, sdrootfac);
      }
      else {
              Array[i1+iseg2][j3]=Array[i1+iseg2][0];
      }
      if(i1==0){
              Array[i1][j1+iseg2]=
     (Array[i1][j1]+Array[i3][j3]+Array[i1+iseg2][j1+iseg2]
     +Array[Num2-iseg2][j1+iseg2] )/4.0
		+gennor(av, sdrootfac);
      }
      if(i2 < Num2){
              Array[i2][j1+iseg2]=
     (Array[i2][j2]+Array[i4][j4]+Array[i2-iseg2][j1+iseg2]
     +Array[i2+iseg2][j1+iseg2] )/4.0;
		+gennor(av, sdrootfac);
      }
      else
      {
              Array[i2][j1+iseg2]=Array[0][j1+iseg2];
      }
                           } //end j loop
                   }  //end i loop
             iseg=iseg/2;
             iseg2=iseg/2;
             sd=sd/levelfactor;
             sdrootfac=sd*rootfac;


	  } //end k loop


	for(int i=0; i< Num; i++){
		Array[i][Num2]=Array[i][0];
		Array[Num2][i]=Array[0][i];
	} // now have periodic boundary condition

	for(int i=0; i<Num;i++){
	for(int j=0; j<Num;j++){
		meanheight=meanheight+Array[i][j];
		if(Array[i][j]>maxheight)
				maxheight=Array[i][j];
		if(Array[i][j]<minheight)
				minheight=Array[i][j];
	}
	}

       meanheight=meanheight/( (double)Num)/((double)Num);
       ofstream file_out;

       file_out.open("surface.dat", ios::out);

       file_out << Num*Num << "  " << Num2*Num2*2 << "  " << Num-1 <<endl;
       // number of nodes, number of triangles

       int it=0;
       for(int i=0; i<Num2;i++){
         if(i%2==0)
           EVEN=true;
         else
           EVEN=false;

	 for(int j=0;j <Num2; j++){
            if(EVEN){
              trinodes[it][0]=Num*i+j;
              trinodes[it][1]=Num*i+j+1;
              trinodes[it][2]=Num*(i+1)+j;
	      file_out 
		      << trinodes[it][0] << "  "
		      << trinodes[it][1] << "  "
		      << trinodes[it][2] << "  "
		      << endl;
              it++;
              trinodes[it][2]=Num*i+j+1;
              trinodes[it][0]=Num*(i+1)+j+1;
              trinodes[it][1]=Num*(i+1)+j;
	      file_out 
		      << trinodes[it][0] << "  "
		      << trinodes[it][1] << "  "
		      << trinodes[it][2] << "  "
		      << endl;
              it++;
              EVEN=false;
	    }
            else
	    {
              trinodes[it][2]=Num*i+j;
              trinodes[it][0]=Num*i+j+1;
              trinodes[it][1]=Num*(i+1)+j+1;
	      file_out 
		      << trinodes[it][0] << "  "
		      << trinodes[it][1] << "  "
		      << trinodes[it][2] << "  "
		      << endl;
              it++;
              trinodes[it][1]=Num*i+j;
              trinodes[it][2]=Num*(i+1)+j+1;
              trinodes[it][0]=Num*(i+1)+j;
	      file_out 
		      << trinodes[it][0] << "  "
		      << trinodes[it][1] << "  "
		      << trinodes[it][2] << "  "
		      << endl;
              it++;
              EVEN=true;
	    }
          } //end j loop
       } // end i loop
       int inode=0;
       double vnodes[Num*Num][3];
       for(int i=0; i < Num; i++){
          for(int j=0; j < Num; j++){
           vnodes[inode][0]=j*distx-shiftx;
           vnodes[inode][1]=i*disty-shifty;
           vnodes[inode][2]=Array[i][j]-meanheight;
	      file_out 
		      << vnodes[inode][0] << "  "
		      << vnodes[inode][1] << "  "
		      << vnodes[inode][2] << "  "
		      << endl;
	   inode++;
	  }
       }


       maxheight=maxheight-meanheight;
       minheight=minheight-meanheight;
       cout << "mean height was " << meanheight << " now shifted to zero\n";
       cout << "min and max heights  are now"
	       << minheight << "  and  " << maxheight << endl; 

}

