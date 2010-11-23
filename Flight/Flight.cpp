//surface.dat MUST ALWAYS BE SQUARE!
#include "Flight.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>  //read and write to string

#include "../../XFiles/SetUp.h"  //loads of declarations and initialisations
#include "../../XFiles/Controls.h"
#include "../../XFiles/TrueType.h"
#include "../../XFiles/DrawLightPos.h"
#include "../../XFiles/DrawLightCol.h"
#include "../../XFiles/DrawMatCol.h"

void EventLoop()
{
 CCam Camera1;
 bool quitit=false;
 SDL_Event event;
 SDL_MouseMotionEvent event2; //Not yet used
 SDL_keysym  *whatkey;

 bool verbose=false;

 if(hidemousecursor)SDL_ShowCursor(0);
      if(LIGHTS){
	      speed=20.0;
	      angspeed=0.05;}
      else
      {
	      speed=5.0;
	      angspeed=0.005;}



//  Go to while(!quitit) to see what happens once data set up

// Go to Render Scene for graphics bit

 //   if_stream opens for input
 //   of_stream opens for output
 //   f_stream opens for both
 //
    if(staked){
	    Camera1.SetPos(0.0,0.0,0.0);
	    Camera1.SetView(1000.0,0.0,0.0);
    }

    string filename ="surface.dat";
    //ASSUMES SQUARE
    ifstream file_in;
    file_in.open(filename.c_str());
    
    file_in >> n_nodes  >> ntri >> iside;
    ihalfside=iside/2;
    cout <<  n_nodes << "  number of nodes" << endl;
    cout <<  ntri<< "  number of triangles" << endl;
    cout << "read next line\n";


    cout << "start read loop\n";

    int namedepth;
    glGetIntegerv(GL_MAX_NAME_STACK_DEPTH,&namedepth);
    cout << "Max depth of Name stack ="<< namedepth << endl;
    //Note this is nothing to do with the size of NameBuffer!
    //Stack depth is 3 for example if car-number wheel number bolt number

    Triangles=(Triangle*)calloc(ntri, sizeof(Triangle));
    NodeV=(D3Dvec*)calloc(n_nodes, sizeof(D3Dvec));
    Normals=(D3Dvec*)calloc(ntri, sizeof(D3Dvec));
    edges1=(D3Dvec*)calloc(ntri, sizeof(D3Dvec));
    edges2=(D3Dvec*)calloc(ntri, sizeof(D3Dvec));

    xshift=(double**)calloc(iside,sizeof(double*));
    yshift=(double**)calloc(iside,sizeof(double*));
    index1=(int**)calloc(iside,sizeof(int*));
    index2=(int**)calloc(iside,sizeof(int*));

    for(int i=0; i< iside; i++){
      xshift[i]=(double*)calloc(iside,sizeof(double));
      yshift[i]=(double*)calloc(iside,sizeof(double));
      index1[i]=(int*)calloc(iside,sizeof(int));
      index2[i]=(int*)calloc(iside,sizeof(int));
    }

    for(int i=0; i< iside; i++){
    for(int j=0; j< iside; j++){
	    index1[i][j]=0;
	    index2[i][j]=0;}}

    int id,l,m,n;
    double x,y,z;

    
    for(int i=0; i<ntri;i++){
	int j,k,l;
        file_in >>l;
        file_in >>m;
        file_in >>n;
        // same result of file_in >> l >>m >>n;



        Triangles[i]=Triangle();   //initialise
        Triangles[i].SetTri(l,m,n);
	if(i%2==0){
	j=i%(iside*2);
	l=j/2;
	k=i/iside/2;
	index1[l][k]=i;
	index2[j/2][k]=i+1;
	}
    }


    //Not using this yet, but set neighbours for each Triangle
    //with periodic boundary conditions
    bool EVEN;
    for(int j=0; j < iside; j++){
     if(j%2==0)EVEN=true;
     if(j%2==1)EVEN=false;
      for(int i=0; i < iside; i++){
		      if(EVEN){
			      if(i > 0 && i < iside-1){
			      if(j > 0 && j < iside-1){
		      Triangles[index1[i][j]].SetNeighb(
			      index2[i][j-1] ,
			      index1[i-1][j] ,
			      index2[i][j]  );
		      Triangles[index2[i][j]].SetNeighb(
			      index1[i][j+1] ,
			      index2[i+1][j] ,
			      index1[i][j]  ); 
	                 }}
		      if(i==0){
			 Triangles[index1[i][j]].SetN2(index1[i+iside-1][j]);
		      }
		      if(i==iside-1){
			 Triangles[index2[i][j]].SetN2(index2[i+1-iside][j]);
		      }
		      if(j==0){
			 Triangles[index1[i][j]].SetN1(index2[i][j+iside-1]);
		      }
		      if(j==iside-1){
			 Triangles[index2[i][j]].SetN1(index1[i][j+1-iside]);
		      }
		      EVEN=false;
		      }
		      else
		      {
			      if(i > 0 && i < iside-1){
			      if(j > 0 && j < iside-1){
		      Triangles[index1[i][j]].SetNeighb(
			      index1[i+1][j] ,
			      index2[i][j-1] ,
			      index2[i][j]  );
		      Triangles[index2[i][j]].SetNeighb(
			      index2[i-1][j] ,
			      index1[i][j+1] ,
			      index1[i][j]  ); 
	                  }}
		      if(i==0){ 
			 Triangles[index2[i][j]].SetN1(index2[i+iside-1][j]);
		      }
		      if(i==iside-1){
			 Triangles[index1[i][j]].SetN1(index1[i+1-iside][j]);
		      }
		      if(j==0){
			 Triangles[index1[i][j]].SetN2(index2[i][j+iside-1]);
		      }
		      if(j==iside-1){
			 Triangles[index2[i][j]].SetN2(index1[i][j+1-iside]);
		      }
	              EVEN=true;
		      }

      }
    } 
    for(int i=0; i < n_nodes; i++){
          NodeV[i]=D3Dvec();
          file_in >> x >> y >> z;
	  //z=z/100.0;
          NodeV[i].SetVec(x,y,z);
     }
    xside=NodeV[1].GetX()-NodeV[0].GetX();

    xlength=xside*iside;
    scalefac=TotalSide/xlength;
    double H;
    double minheight=1e12, maxheight=-1e12;
    cout << "Scale factor =" << scalefac
	    << "  xlength=" << xlength
	    << "  xside= " << xside 
	    << "  iside= " << iside <<endl;
    for(int i=0; i<n_nodes; i++){
       x=NodeV[i].GetX(); y=NodeV[i].GetY(); z=NodeV[i].GetZ();
       NodeV[i].SetVec(x*scalefac,y*scalefac, z*scalefac);
       H=NodeV[i].GetZ();
       if( H > maxheight )maxheight=H;
       if( H < minheight )minheight=H;
    }
    xlength=xlength*scalefac;
    cout << xside<<endl;
    xside=xside*scalefac;
    cout << xside<<  "  "  << scalefac << endl;
    cout << "maximim height is " << maxheight << endl;
    cout << "minimim height is " << minheight << endl;

    X1=-xlength/2.0,X2=xlength/2.0,Y1=-xlength/2.0,Y2=xlength/2.0
	    ,Z1=minheight, Z2=maxheight;

    CollideTree=new OTree;
    ONode* ReturnNode;
    int Depth;
    //incude possibility of point in all eight octants (with overlaps)
    CollideTri=(intList**)calloc(8*(MD+1),sizeof(intList*));

    for(int i=0; i< ntri; i++){
	    if(verbose){
	    cout << "************************************************\n";
            cout << "            ADDING TRIANGLE TO TREE  << " << i  << endl;
	    cout << "************************************************\n";}
	 Depth=0;
         ReturnNode=CollideTree->AddToTree(X1,X2,Y1,Y2,Z1,Z2,i,Depth);
    }

    ix=(int)(Camera1.CamPos.GetX()/xside);
    iz=(int)(Camera1.CamPos.GetZ()/xside);

    RealCamPos=Camera1.CamPos;

    ixold=ix; izold=iz;

    SetShiftX(ix);
    SetShiftY(iz);

    //Normals for lighting
    double xvals[3],yvals[3],zvals[3];
    D3Dvec cross;

    for(int i=0; i<ntri; i++){
		 // DONT Transform to OpenGL coordinates

                 int j=0;
                 xvals[j]=NodeV[ Triangles[i].Get1()].GetX();
                 yvals[j]=NodeV[ Triangles[i].Get1()].GetY();
                 zvals[j]=NodeV[ Triangles[i].Get1()].GetZ();
		 j=1;
                 xvals[j]=NodeV[ Triangles[i].Get2()].GetX();
                 yvals[j]=NodeV[ Triangles[i].Get2()].GetY();
                 zvals[j]=NodeV[ Triangles[i].Get2()].GetZ();
		 j=2;
                 xvals[j]=NodeV[ Triangles[i].Get3()].GetX();
                 yvals[j]=NodeV[ Triangles[i].Get3()].GetY();
                 zvals[j]=NodeV[ Triangles[i].Get3()].GetZ();


		 if(LIGHTS){

	      D3Dvec edge1, edge2,cross,normal;
	      edge1.SetX(xvals[1]-xvals[0]);
	      edge1.SetY(yvals[1]-yvals[0]);
	      edge1.SetZ(zvals[1]-zvals[0]);
	      edge2.SetX(xvals[2]-xvals[0]);
	      edge2.SetY(yvals[2]-yvals[0]);
	      edge2.SetZ(zvals[2]-zvals[0]);

	      cross=edge1*edge2;
	      Normalise(cross);

	      Normals[i]=D3Dvec();  //call constructor
              Normals[i]=cross;
	      edges1[i]=edge1;
	      edges2[i]=edge2;
		 }
    }

    istart=0;
    istop=ntri;
    

 while(!quitit){
        
       while(SDL_PollEvent(&event)){

          switch(event.type){
               case SDL_QUIT:
                 quitit=true;
                 break;
                 case SDL_MOUSEBUTTONDOWN:
		    if(!mousedown)
			    firstmousedown=true;
		    else
		            firstmousedown=false;
                    mousedown=true;
                 break;
                 case SDL_MOUSEBUTTONUP:
                    mousedown=false;   //mousedown remains true untill mouse up
                 break;

                 case SDL_MOUSEMOTION:
                  if(mousedown){
                         if(MouseOn)
				 if(tethered || staked)
					Camera1.MouseView();
					else
					Camera1.MouseLookAt();

		  }  //MouseView: change view direction
                  else{
                         if(MouseOn)
				 if(tethered || staked)
					Camera1.MouseLookAt();
					else
					Camera1.MouseView();
		  }  //MouseLookat: change point from where camera "looks at" scene
                 break;  


               case SDL_KEYDOWN:
                   whatkey=&event.key.keysym;
                   HandleKeyPress(whatkey);
                   break;
               case SDL_KEYUP:
                   whatkey=&event.key.keysym;
                   HandleKeyRelease(whatkey);
               default:
                 break;
                     } // end of case
                } //end of inner loop
       /************************************************************/
              CheckMove(Camera1);
		    if(a_Pressed){
			    speed=speed+0.5;
			    if(speed > 60)speed=60;
		    }
		    if(z_Pressed){
			    speed=speed-0.5;
			    if(speed  < 0)speed=0;
		    }

              RenderScene(Camera1); 
	      //(does picking in DrawLightPos etc)
            } //end of outer loop

}
void RenderScene(CCam & Camera1)
{
   glClearColor(RedM[0][0],GreenM[0][0],BlueM[0][0] ,AlphaM[0][0]);
	float fogColor[4]= 
	{RedM[0][0],GreenM[0][0],BlueM[0][0],AlphaM[0][0]};
	glFogfv(GL_FOG_COLOR, fogColor);
   //background colour initialised

       glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
       glLoadIdentity();

       int ix,iz;

       //
       //camera  pos      view     up vector
       if(exwhyzed){
       gluLookAt(
        Camera1.CamPos.GetX(),   Camera1.CamPos.GetY(),  Camera1.CamPos.GetZ(),
        Camera1.CamView.GetX(), Camera1.CamView.GetY(), Camera1.CamView.GetZ(),
        Camera1.jay.GetX(),   Camera1.jay.GetY(),   Camera1.jay.GetZ());   
        }
       if(boom){
        gluLookAt(
        Camera1.CamPos.GetX(),   Camera1.CamPos.GetY(),  Camera1.CamPos.GetZ(),
        Camera1.CamView.GetX(), Camera1.CamView.GetY(), Camera1.CamView.GetZ(),
        Camera1.jprime.GetX(),   Camera1.jprime.GetY(),   Camera1.jprime.GetZ());   
        }


      glLineWidth(3.0);


      if(LIGHTS)
	      glEnable(GL_LIGHTING);
      else
	      glDisable(GL_LIGHTING);
     if(LIGHTS){

     glEnable(GL_BLEND);

     for(int il=0; il<ilights; il++){

     CosZen=cos(Zen[il]/convert);
     SinZen=sin(Zen[il]/convert);
     SinAz=sin(Az[il]/convert);
     CosAz=cos(Az[il]/convert);

     //check initial for Pesky openGL coordinates too
     gLightPosition[0]=Rho[il]*SinZen*CosAz;
     gLightPosition[2]=-Rho[il]*SinZen*SinAz;
     gLightPosition[1]=Rho[il]*CosZen;
     if(Rho[il] >  L_Inf)
	     gLightPosition[3]=0.0; // at infinity if =0 
     else   
             gLightPosition[3]=1.0;   // xyzw, pos= x/w,y/w,z/w

     float ambient[4]={RedL[il][0], GreenL[il][0], BlueL[il][0], AlphaL[il][0]}; 
     float diffuse[4]={RedL[il][1], GreenL[il][1], BlueL[il][1], AlphaL[il][1]}; 
     float specular[4]={RedL[il][2], GreenL[il][2], BlueL[il][2], AlphaL[il][2]}; 
     glLightfv(MYLIGHTS[il],GL_AMBIENT,ambient);
     glLightfv(MYLIGHTS[il],GL_DIFFUSE,diffuse);
     glLightfv(MYLIGHTS[il],GL_SPECULAR,specular);
     glLightfv(MYLIGHTS[il],GL_POSITION,gLightPosition);
      }
      }


      if(!helpscreen ){
	      bool drawscene=true;
	      DrawScene(drawscene);
              if(!MouseOn){
	      if(picking){
                drawscene=false;
	        DrawScene(drawscene);
	      }}
      }// endif for !helpscreen

      if(helpscreen){

      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glEnable(GL_BLEND);
	      //left top right bottom
	      //Matrix is diagonal
	      //  2/(right-left), 2/(top-bottom), -2/(far-near)
	      //  call in OrthMode is to glOrtho with near=0, far=1
	      //  Args are  left, top, right, bottom

      OrthoMode(-1, 1, 1, -1);
      glColor3ub(255,255,255);
      glBindTexture(GL_TEXTURE_2D,gl_Texture[0]);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glBegin(GL_QUADS);
             // 0,0 is screen centre
             // -1,-1 is screen bottom left
	      glTexCoord2f(0.0, 1.0); //top left
	      glVertex2f(-1.0, 1.0); 

	      glTexCoord2f(0.0, 0.0); // Bottom Left
	      glVertex2f(-1.0, -1.0); // Bottom Left

	      glTexCoord2f(1.0, 0.0); // Bottom  right
	      glVertex2f(1.0, -1.0); // Bottom right

	      glTexCoord2f(1.0, 1.0); // Top right
	      glVertex2f(1.0, 1.0); // Top right
      glEnd();

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);
      PerspectiveMode();

       }  //endif for helpscreen

      if(lightpos){
	      picking=false; //disable 3D picking
              glDisable(GL_DEPTH_TEST);
              glDisable(GL_LIGHTING);
                  bool drawbox;
	          drawbox=true;
	          DrawLightPos(drawbox);  //draw the box
              glEnable(GL_DEPTH_TEST);
              glEnable(GL_LIGHTING);

	      drawbox=false;
	      DrawLightPos(drawbox);  //use pickmatrix instead
	      picking=true;
	       }
      if(lightcol){
	      picking=false; //disable 3D picking
              glDisable(GL_DEPTH_TEST);
              glDisable(GL_LIGHTING);
                  bool drawbox;
	          drawbox=true;
	          DrawLightCol(drawbox);  //draw
              glEnable(GL_DEPTH_TEST);
              glEnable(GL_LIGHTING);

	      drawbox=false;   //pickmatrix
	      DrawLightCol(drawbox);
	      picking=true;
	       }
      if(matcol){
	      picking=false; //disable 3D picking
              glDisable(GL_DEPTH_TEST);
              glDisable(GL_LIGHTING);
                  bool drawbox;
	          drawbox=true;
	          DrawMatCol(drawbox);  //draw
              glEnable(GL_DEPTH_TEST);
              glEnable(GL_LIGHTING);

	      drawbox=false;   //pickmatrix
	      DrawMatCol(drawbox);
	      picking=true;
	       }

	      SDL_GL_SwapBuffers();
}

void Init(){
   InitialiseGL(WIDTH, HEIGHT);

   //  IMPORTANT  //
   // The width in pixels MUST be a whole number times 4! 
   // Otherwise the swapped image is garbage
   // don't know if it is gimp output (seems likely
   // as size=header file (54 bytes)+bytesperpix*width*height
   // is not right in ls -l unless width is a multiple of 4
   //
   // If not a multiple of 4, Load in gimp, then go 
   //  Image->Canvas-Size, adjust, then resize.
   //
   //  Also, just use a paintbrush in the Gimp for white edges

   CreateTexture(gl_Texture,"../../bitmaps/Back1.bmp",0);
   CreateTexture(gl_Texture,"../../bitmaps/LightPos.bmp",1);
   CreateTexture(gl_Texture,"../../bitmaps/buttonmask.bmp",2);
   CreateTexture(gl_Texture,"../../bitmaps/button.bmp",3);
   CreateTexture(gl_Texture,"../../bitmaps/WhiteBack.bmp",4);
   CreateTexture(gl_Texture,"../../bitmaps/DownArrow.bmp",5);
   CreateTexture(gl_Texture,"../../bitmaps/UpArrow.bmp",6);
   CreateTexture(gl_Texture,"../../bitmaps/LightCol.bmp",7);
   CreateTexture(gl_Texture,"../../bitmaps/MatCol.bmp",8);

   //Material 0 is background
   //
   for(int mp=0; mp<Mprops; mp++){
    
   RedM[0][mp]=1.0;  //diffuse
   GreenM[0][mp]=1.0;
   BlueM[0][mp]=1.0;
   AlphaM[0][mp]=1.0;
   ShineM[0][mp]=127.0;

   RedMslide[0][mp]=RedM[0][mp]*(Bottom_TopM-Bottom_BottomM);
   GreenMslide[0][mp]=GreenM[0][mp]*(Bottom_TopM-Bottom_BottomM);
   BlueMslide[0][mp]=BlueM[0][mp]*(Bottom_TopM-Bottom_BottomM);
   AlphaMslide[0][mp]=AlphaM[0][mp]*(Bottom_TopM-Bottom_BottomM);
   ShineMslide[0][mp]=ShineM[0][mp]*(Bottom_TopM-Bottom_BottomM)/127.0;
   Bottom1M[0][mp]=Bottom_BottomM+RedMslide[0][mp];
   Bottom2M[0][mp]=Bottom_BottomM+GreenMslide[0][mp];
   Bottom3M[0][mp]=Bottom_BottomM+BlueMslide[0][mp];
   Bottom4M[0][mp]=Bottom_BottomM+AlphaMslide[0][mp];
   Bottom5M[0][mp]=Bottom_BottomM+ShineMslide[0][mp];
   }
   //other materials initialsied later

   glClearColor(RedM[0][0],GreenM[0][0],BlueM[0][0] ,AlphaM[0][0]);
   //background colour initialised

	  MYLIGHTS[0]=GL_LIGHT0;
	  MYLIGHTS[1]=GL_LIGHT1;
	  MYLIGHTS[2]=GL_LIGHT2;
	  MYLIGHTS[3]=GL_LIGHT3;
	  MYLIGHTS[4]=GL_LIGHT4;
	  MYLIGHTS[5]=GL_LIGHT5;
	  MYLIGHTS[6]=GL_LIGHT6;
	  MYLIGHTS[7]=GL_LIGHT7;

     //To specifiy a light we need to name them as follows
     // from GL_LIGHT0, GL_LIGHT1,GL_LIGHT2,... GL_LIGHTN.

     /* Second Argument can be
    GL_AMBIENT, GL_DIFFUSE,GL_SPECULAR,GL_POSITION,
    GL_SPOT_CUTTOFF, GL_SPOT_DIRECTION, GL_SPOT_EXPONENT,
    GL_CONSTANT_ATTENIATION, GL_LINEAR_ATTENUATION,
    GL_QUADRATIC_ATTENUATION   */
    //
    //
     for(int il=0;il<ilights;il++){
     RedL[il][0]=0.0;  
     GreenL[il][0]=0.0;  
     BlueL[il][0]=0.0;  
     AlphaL[il][0]=1.0;  
	     // light from all over
     RedL[il][1]=1.0;  
     GreenL[il][1]=1.0;  
     BlueL[il][1]=1.0;  
     AlphaL[il][1]=1.0;  
     // light component reflecting diffusely
     RedL[il][2]=1.0;  
     GreenL[il][2]=1.0;  
     BlueL[il][2]=1.0;  
     AlphaL[il][2]=1.0;  
     //light reflecting specularly
     //
     for(int lp=0;lp<Lprops;lp++){
        RedLslide[il][lp]=RedL[il][lp]*(Bottom_TopC-Bottom_BottomC);
	Bottom1C[il][lp]=Bottom_BottomC+RedLslide[il][lp];
     	GreenLslide[il][lp]=GreenL[il][lp]*(Bottom_TopC-Bottom_BottomC);
	Bottom2C[il][lp]=Bottom_BottomC+GreenLslide[il][lp];
        BlueLslide[il][lp]=BlueL[il][lp]*(Bottom_TopC-Bottom_BottomC);
	Bottom3C[il][lp]=Bottom_BottomC+BlueLslide[il][lp];
        AlphaLslide[il][lp]=AlphaL[il][lp]*(Bottom_TopC-Bottom_BottomC);
	Bottom4C[il][lp]=Bottom_BottomC+AlphaLslide[il][lp];
     }
     lprop=0;

     float ambient[4]={RedL[il][0], GreenL[il][0], BlueL[il][0], AlphaL[il][0]}; 
     float diffuse[4]={RedL[il][1], GreenL[il][1], BlueL[il][1], AlphaL[il][1]}; 
     float specular[4]={RedL[il][2], GreenL[il][2], BlueL[il][2], AlphaL[il][2]}; 


     glLightfv(MYLIGHTS[il],GL_AMBIENT, ambient);
     glLightfv(MYLIGHTS[il],GL_DIFFUSE, diffuse);
     glLightfv(MYLIGHTS[il],GL_SPECULAR, specular);

     Zen[il]=45.0;
     Az[il]=245.0;
     Rho[il]=6000.0;

     CosZen=cos(Zen[il]/convert);
     SinZen=sin(Zen[il]/convert);
     SinAz=sin(Az[il]/convert);
     CosAz=cos(Az[il]/convert);

     zenangle[il]=Zen[il]/180.*(Left_Right-Left_Left);
     azimangle[il]=Az[il]/360.*(Left_Right-Left_Left);
     rhovalue[il]=Rho[il]/L_Dist_Scale*(Left_Right-Left_Left);

     Left1[il]=Left_Left+zenangle[il];
     Left2[il]=Left_Left+azimangle[il];      //initialise
     Left3[il]=Left_Left+rhovalue[il];

     //pesky OpenGL coordinates
     //(check later gLightPosition, this is hust initaialisation)
     gLightPosition[0]=Rho[il]*SinZen*CosAz;  //Pesky OpenGL coordinates
     gLightPosition[2]=-Rho[il]*SinZen*SinAz;
     gLightPosition[1]=Rho[il]*CosZen;
     gLightPosition[3]=1.0;   

     glLightfv(MYLIGHTS[il],GL_POSITION,gLightPosition);

     
     glEnable( MYLIGHTS[il] );


     }
     ilite=0;  //ilite is the current light being altered by slidebar


     double RedI[imats],GreenI[imats],BlueI[imats],AlphaI[imats],ShineI[imats];

     //hand pick initial colours
     // black for im=1;
     RedI[1]=0.0; GreenI[1]=0.0; BlueI[1]=0.0; AlphaI[1]=0.0; ShineI[1]=100.0;
     // Red1 for im=2;
     RedI[2]=0.7; GreenI[2]=0.0; BlueI[2]=0.0; AlphaI[2]=0.0; ShineI[2]=100.0;
     // Red2 for im=3;
     RedI[3]=1.0; GreenI[3]=0.0; BlueI[3]=0.0; AlphaI[3]=0.0; ShineI[3]=100.0;
     // Green1 for im=4;
     RedI[4]=0.0; GreenI[4]=0.7; BlueI[4]=0.0; AlphaI[4]=0.0; ShineI[4]=100.0;
     // Green2 for im=5;
     RedI[5]=0.0; GreenI[5]=1.0; BlueI[5]=0.0; AlphaI[5]=0.0; ShineI[5]=100.0;
     // Blue1 for im=6;
     RedI[6]=0.0; GreenI[6]=0.0; BlueI[6]=0.7; AlphaI[6]=0.0; ShineI[6]=100.0;
     // Blue2 for im=7;
     RedI[7]=0.0; GreenI[7]=1.0; BlueI[7]=1.0; AlphaI[7]=0.0; ShineI[7]=100.0;
     // Yellow1 for im=8;
     RedI[8]=0.7; GreenI[8]=0.7; BlueI[8]=0.0; AlphaI[8]=0.0; ShineI[8]=100.0;
     // Yellow2 for im=9;
     RedI[9]=1.0; GreenI[9]=1.0; BlueI[9]=0.0; AlphaI[9]=0.0; ShineI[9]=100.0;



     //recall material 0 is background, if we make imats bigger than 10
     //we make everything grey
     for(int im=1; im < imats; im++){
	     for(int mp=0; mp < Mprops; mp++){
     if(im<=10){
       RedM[im][mp]=RedI[im];
       GreenM[im][mp]=GreenI[im];
       BlueM[im][mp]=BlueI[im];
       AlphaM[im][mp]=AlphaI[im];
       ShineM[im][mp]=ShineI[im];

       RedMslide[im][mp]=RedM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom1M[im][mp]=Bottom_BottomM+RedMslide[im][mp];
       GreenMslide[im][mp]=GreenM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom2M[im][mp]=Bottom_BottomM+GreenMslide[im][mp];
       BlueMslide[im][mp]=BlueM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom3M[im][mp]=Bottom_BottomM+BlueMslide[im][mp];
       AlphaMslide[im][mp]=AlphaM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom4M[im][mp]=Bottom_BottomM+AlphaMslide[im][mp];
       ShineMslide[im][mp]=ShineM[im][mp]*(Bottom_TopM-Bottom_BottomM)/127.0;
       Bottom5M[im][mp]=Bottom_BottomM+ShineMslide[im][mp];
     }
     else
     {
       RedM[im][mp]=0.5;
       GreenM[im][mp]=0.5;
       BlueM[im][mp]=0.5;
       AlphaM[im][mp]=0.5;
       ShineM[im][mp]=127.0;
       RedMslide[im][mp]=RedM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom1M[im][mp]=Bottom_BottomM+RedMslide[im][mp];
       GreenMslide[im][mp]=GreenM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom2M[im][mp]=Bottom_BottomM+GreenMslide[im][mp];
       BlueMslide[im][mp]=BlueM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom3M[im][mp]=Bottom_BottomM+BlueMslide[im][mp];
       AlphaMslide[im][mp]=AlphaM[im][mp]*(Bottom_TopM-Bottom_BottomM);
       Bottom4M[im][mp]=Bottom_BottomM+AlphaMslide[im][mp];
       ShineMslide[im][mp]=ShineM[im][mp]*(Bottom_TopM-Bottom_BottomM)/127.0;
       Bottom5M[im][mp]=Bottom_BottomM+ShineMslide[im][mp];
     }
	     }
     }
     imat=0;
     mprop=0;


//    OK, and now for FOG!

     if(foggy){
	float fogColor[4]= 
	{RedM[0][0],GreenM[0][0],BlueM[0][0],AlphaM[0][0]};
	glFogfv(GL_FOG_COLOR, fogColor);

        float g_FogDensity=2.0e-4;
	glFogi(GL_FOG_MODE, GL_EXP2);	
	glFogf(GL_FOG_DENSITY, g_FogDensity);
	glHint(GL_FOG_HINT, GL_DONT_CARE);				
        glFogf(GL_FOG_START, 10000.0);	
	glFogf(GL_FOG_END, 10000.1);	
	glEnable(GL_FOG);	
     }

     glEnable( GL_LIGHTING);

 //    buildFont();

}

void SetShiftX(int ix){

    cout << "ix is " << ix <<  endl;
    for(int i=0;i< iside; i++){
    for(int j=0;j< iside; j++){
	    xshift[i][j]=0.0;
    }}
    //initialise

    if(ix > 0){
	    for(int i=0; i < ix ; i++){
		    cout << "x shifting column " << iside-1-i << endl;
		    for(int j=0; j<iside ; j++){
                         xshift[i][j]=xlength;
		    }
	    }
	    xshiftedge=ix-1; // (counter ++)
	    cout << xshiftedge << " shift x  b "  << endl;
    }
    if(ix <0)
    {
	    for(int i=0; i< -ix; i++){
		    cout << "x shifting column " << i << endl;
		    for(int j=0; j< iside; j++){
                       xshift[iside-1-i][j]=-xlength;
		    }
	    } 
	    xshiftedge=iside-1+(1+ix); //ix negative!  (counter ++)
	    cout << xshiftedge << " shift x  b "  << endl;
    } 
    if(ix==0){
	    startoffx=true;
	    xshiftedge=-10;}
    }    
void SetShiftY(int iz){

    cout << "iz is  " << iz <<  endl;
    for(int i=0;i< iside; i++){
    for(int j=0;j< iside; j++){
	    yshift[i][j]=0.0;
    }}
    //initialise

    if( iz >  0){
	    for(int i=0; i < iz ; i++){
		    cout << "y shifting row " << iside-1-i << endl;
		    for(int j=0; j<iside ; j++){
                         yshift[j][iside-1-i]=-xlength;
		    }
	    }
	    yshiftedge=iside-1-(iz-1);   //(counter ++)
	    cout << yshiftedge << " shift y  a " << iz << endl;
    }
    if(iz<0)
    {
	    for(int i=0; i<  -iz; i++){
		    cout << "y shifting row " << i << endl;
		    for(int j=0; j< iside; j++){
                       yshift[j][i]=xlength;
		    }
	    } 
	    yshiftedge=-(iz-1);  //iz negative (and counter ++)
	    cout << yshiftedge << " shift y  b "  << endl;
    } 
    if(iz==0){
	    startoffy=true;
	    yshiftedge=-10; }

    }    
void DrawScene(bool drawscene)
{
      int viewport[4];
      SDL_GetMouseState(&mousex, &mousey);
      UINT NameBuffer[ntri];
      glSelectBuffer(ntri,NameBuffer);

    if(!drawscene){

      (void)glRenderMode(GL_SELECT);
      glInitNames();  //Ignored unless in GL_SELECT 

      glGetIntegerv(GL_VIEWPORT, viewport);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix( 
	 (double) mousex, viewport[3]-(double) mousey, 5.0, 5.0, viewport);
      //In draw lightPos etc, we were selecting objects 2D on the 2D screen
      //so OrthoMode was used. here we pick 3D objects.
      gluPerspective(45.0f,(GLfloat)WIDTH/GLfloat(HEIGHT), 100.0f, 150000.0f);
      glMatrixMode(GL_MODELVIEW);

    }


      int i, i1, i2, i3;
      float ex, why, zed;
      double xvals[3],yvals[3],zvals[3];
      for(int k=0; k< iside; k++){
      for(int l=0; l< iside; l++){
	      for(int m=0; m<2; m++){

	      if(m==0)i=index1[l][k]; if(m==1)i=index2[l][k];
              i1=Triangles[i].Get1(); i2=Triangles[i].Get2(); i3=Triangles[i].Get3();

	      //DONT transform to open GL coords
                 int j=0;
                 xvals[j]=NodeV[i1].GetX();
                 yvals[j]=NodeV[i1].GetY();
                 zvals[j]=NodeV[i1].GetZ();
		 xvals[j]=xvals[j]+xshift[l][k];
		 yvals[j]=yvals[j]+yshift[l][k];
		 j=1;
                 xvals[j]=NodeV[i2].GetX();
                 yvals[j]=NodeV[i2].GetY();
                 zvals[j]=NodeV[i2].GetZ();
		 xvals[j]=xvals[j]+xshift[l][k];
		 yvals[j]=yvals[j]+yshift[l][k];
		 j=2;
                 xvals[j]=NodeV[i3].GetX();
                 yvals[j]=NodeV[i3].GetY();
                 zvals[j]=NodeV[i3].GetZ();
		 xvals[j]=xvals[j]+xshift[l][k];
		 yvals[j]=yvals[j]+yshift[l][k];

	//	 if(l==0){
	//	 cout << xvals[j] << " " << yvals[j]  
	//   << "  ilk=" << i << "  " << l << "  " << k << endl;
	//	 }

                 if(drawscene){
		 if(LIGHTS){
		 ex=Normals[i].GetX(); why=Normals[i].GetY(); zed=Normals[i].GetZ();
	         glNormal3f( ex, zed,-why);
	      //pesky openGL coordinates 
              }}

       if(drawscene){
      int matl=2;
      if(i==istart+iplot)matl=6;
      if(LIGHTS){
      float mat_amb[4]={RedM[matl][0], GreenM[matl][0],BlueM[matl][0],
		                AlphaM[matl][0] };
      float mat_diff[4]={RedM[matl][1], GreenM[matl][1],BlueM[matl][1],
		                AlphaM[matl][1] };
      float mat_spec[4]={RedM[matl][2], GreenM[matl][2],BlueM[matl][2],
		                AlphaM[matl][2] };

      float shine[]={ShineM[2][0]};
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
        }
        else
	{
	glColor3ub((int)(255*RedM[matl][1]),(int)(255*GreenM[matl][1])
			,(int)(255*BlueM[matl][1]));
	}
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      }  //endif drawscene 
       if(!drawscene && PickTri)glPushName(i);

          if(drawscene){
	      glBegin(GL_TRIANGLES);
                 glVertex3f( xvals[0], zvals[0], -yvals[0] );
                 glVertex3f( xvals[1], zvals[1], -yvals[1] );
          	 glVertex3f( xvals[2], zvals[2], -yvals[2] );
             glEnd();
	  }
	  else{
	      if(PickTri){
		     glPushName(i);
		     glBegin(GL_TRIANGLES);
                         glVertex3f( xvals[0], zvals[0], -yvals[0] );
                         glVertex3f( xvals[1], zvals[1], -yvals[1] );
                         glVertex3f( xvals[2], zvals[2], -yvals[2] );
                     glEnd();
		     glPopName();
	      }
	      else
	      {
		     glPushName(i1);
		     glBegin(GL_POINTS);
                         glVertex3f( xvals[0], zvals[0], -yvals[0] );
                     glEnd();
		     glPopName();
		     glPushName(i2);
		     glBegin(GL_POINTS);
                         glVertex3f( xvals[1], zvals[1], -yvals[1] );
                     glEnd();
		     glPopName();
		     glPushName(i3);
		     glBegin(GL_POINTS);
                         glVertex3f( xvals[2], zvals[2], -yvals[2] );
                     glEnd();
		     glPopName();
	      }


	  }

       if(!drawscene &&  PickTri)glPopName();
	       

	     if(drawlines){
	     if(LIGHTS){
      float mat_amb[4]={RedM[1][0], GreenM[1][0],BlueM[1][0],
		                AlphaM[1][0] };
      float mat_diff[4]={RedM[1][1], GreenM[1][1],BlueM[1][1],
		                AlphaM[1][1] };
      float mat_spec[4]={RedM[1][2], GreenM[1][2],BlueM[1][2],
		                AlphaM[1][2] };
      float shine[]={ShineM[1][0]};
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	     }
	     else
	     {
	glColor3ub((int)(255*RedM[1][1]),(int)(255*GreenM[1][1]),(int)(255*BlueM[1][1]));
	}  //endif for LIGHTS
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	      glBegin(GL_TRIANGLES);
          	glVertex3f( xvals[0], zvals[0], -yvals[0] );
          	glVertex3f( xvals[1], zvals[1], -yvals[1] );
          	glVertex3f( xvals[2], zvals[2], -yvals[2] );
             glEnd(); } //endif for drawlines

              } // end k l m loops
          } 
     }	    



      if(!drawscene){
         PerspectiveMode(); //needed after glPickMatrix
         glPopMatrix();
         glFlush();

         int hits=glRenderMode(GL_RENDER);
	 //cout << " number of hits =" << hits << endl;
	 ProcessTriangleHits(hits, NameBuffer );
       }
      
}
void ProcessTriangleHits(int hits, UINT  Buffer[]){
	 UINT* ptr_toBuffer;
	 UINT i,j;
	 UINT names;

	 //for nodes, get lots of hits for same node
	 //as it in lots of triangles
	 //for triangles, get lots of hits if we hover
	 //over node

	 ptr_toBuffer=Buffer;
	 if(hits>0){
		 names=*ptr_toBuffer;
		 if(names>0){
		 cout << "************************************\n";
		 cout << "hits=" <<hits << endl;
		 cout << "************************************\n";
		 }
	 }
	 for(int i=0; i< hits; i++){
		 names=*ptr_toBuffer;
		  if(names>0){
		  cout <<" number of names  for hit  " << i+1
			 << " is "   << names << endl;
		  }
		  float z1,z2;

		  ptr_toBuffer++;    //
		  z1=(float)*ptr_toBuffer/0x7fffffff; 
		  ptr_toBuffer++;    
                  z2=(float)*ptr_toBuffer/0x7fffffff; 
		  if(names>0)
		  cout << " depths between " <<  z1 << "  and " << z2 << endl;
		  ptr_toBuffer++;
		  if(names>0){cout << "i=" << i << "  They are " << endl;
		  for(j=0; j< names; j++){
			  cout <<   "j= " << j << "  " << *ptr_toBuffer << "  ";
		          ptr_toBuffer++;  //child objects of name
		  }  
		  cout << endl;}

	//
	}  // end loop for hits
        SDL_GetMouseState(&mousex, &mousey);

}
