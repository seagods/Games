//surface.dat MUST ALWAYS BE SQUARE!
#include "Flight.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>		//read and write to string
#include "../../XFiles/SetUp.h"  //loads of declarations and initialisations
#include "../../XFiles/Controls.h"
#include "../../XFiles/TrueType.h"  //Following includes need Truetype
#include "../../XFiles/DrawLightPos.h"
#include "../../XFiles/DrawLightCol.h"
#include "../../XFiles/DrawMatCol.h"
#include "../../XFiles/DrawTriHits.h"
#include "../../XFiles/DrawNodeHits.h"

void EventLoop()
{
 CCam Camera1;
 bool quitit=false;
 SDL_Event event;
 SDL_MouseMotionEvent event2; //Not yet used
 SDL_keysym  *whatkey;

 bool verbose=false;
 ipicked=-10000;

 if(hidemousecursor)SDL_ShowCursor(0);
      if(LIGHTS){
	      speed=20.0;
              speed_diff=1.0;
	      angspeed=0.05;}
      else
      {
	      speed=5.0;
              speed_diff=.2;
	      angspeed=0.005;}

      if( !drawlines){ 
          speed=speed/2.; angspeed=angspeed/2.; speed_diff=speed_diff/2.0;}
      if(flight){
          speed=speed/2.; speed_diff=speed_diff/2; angspeed=angspeed/2.;}

     int pointsize=45;  //need fonts 
     /* ******************************************************************** */
     /* We only use font10 in this case  --- Monochrome is rasterised bitmap  

     font1=new OGLFT::Monochrome("/usr/share/fonts/times/Timeg.ttf",pointsize);
     if(font1==0 || !font1->isValid()){
        cout << "Font1 Failed" << endl;
      //   exit(1);
     }
     else{
        cout << "Font 1 is fine!" << endl;  }

     font2=new OGLFT::Monochrome("/usr/share/fonts/times/Timegi.ttf",pointsize);
     if(font2==0 || !font2->isValid()){
        cout << "Font2 Failed" << endl;
     //   exit(1);
     }
     font3=new OGLFT::Monochrome("/usr/share/fonts/times/Timegbd.ttf",pointsize);
     if(font3==0 || !font3->isValid()){
        cout << "Font3 Failed" << endl;
      //  exit(1);
     }
     font4=new OGLFT::Monochrome("/usr/share/fonts/times/Timegibd.ttf",pointsize);
     if(font4==0 || !font4->isValid()){
        cout << "Font4 Failed" << endl;
    //    exit(1);
     }
     font5=new OGLFT::Monochrome("/usr/share/fonts/Fonts-Linux/slgrrg__.ttf",pointsize);
     if(font5==0 || !font5->isValid()){
        cout << "Font5 Failed" << endl;
   //     exit(1);
     }
     font6=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmex10.ttf",pointsize);
     if(font6==0 || !font6->isValid()){
        cout << "Font6 Failed" << endl;
    //    exit(1);
     }
     font7=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmmi10.ttf",pointsize);
     if(font7==0 || !font7->isValid()){
        cout << "Font7 Failed" << endl;
    //    exit(1);
     }
     font8=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmr10.ttf",pointsize);
     if(font8==0 || !font8->isValid()){
        cout << "Font8 Failed" << endl;
    //    exit(1);
     }
     font9=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmsy10.ttf",pointsize);
     if(font9==0 || !font9->isValid()){
        cout << "Font9 Failed" << endl;
     //   exit(1);
     }
     */
     pointsize=12;
     font10=new OGLFT::Monochrome("/usr/share/fonts/times/Timeg.ttf",pointsize);

     if(font10==0 || !font10->isValid()){
        cout << "Font10 Failed" << endl;
        exit(1);
     }
    /*
     pointsize=6;
     font11=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmsy10.ttf",pointsize);
     if(font11==0 || !font11->isValid()){
        cout << "Font11 Failed" << endl;
    //    exit(1);
     }
     pointsize=60;
     font12=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmsy10.ttf",pointsize);
     if(font12==0 || !font12->isValid()){
        cout << "Font12 Failed" << endl;
   //     exit(1);
     }
     pointsize=75;
     font13=new OGLFT::Monochrome("/usr/share/fonts/texcm/cmsy10.ttf",pointsize);
     if(font13==0 || !font13->isValid()){
        cout << "Font13 Failed" << endl;
   //     exit(1);
     }
      */
     //Paint it  black
     font10->setForegroundColor(0.0, 0.0, 0.0);
     glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

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
    if(picking){
           TriBuffer=(UINT*)calloc(ntri,sizeof(UINT)); 
           //Don't have a separate node buffer because it gets
          // confused and crashes if we swap between the two
          //just have tribuffer since triangles > nodes
     }
                                
    ihalfside=iside/2;
    cout <<  n_nodes << "  number of nodes" << endl;
    cout <<  ntri<< "  number of triangles" << endl;

    int namedepth;
    glGetIntegerv(GL_MAX_NAME_STACK_DEPTH,&namedepth);
    cout << "Max depth of Name stack ="<< namedepth << endl;
    //Stack depth is 3 for example if car-number wheel number bolt number

    Triangles=(Triangle*)calloc(ntri, sizeof(Triangle));
    NodeV=(D3Dvec*)calloc(n_nodes, sizeof(D3Dvec));
    NodeNormals=(D3Dvec*)calloc(n_nodes, sizeof(D3Dvec));
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


    //Set neighbours for each Triangle
    //with periodic boundary conditions
    bool EVEN;
    for(int j=0; j < iside; j++){
     if(j%2==0)EVEN=true;
     if(j%2==1)EVEN=false;
      for(int i=0; i < iside; i++){
		      if(EVEN){
			      if(i > 0 && i < iside-1){
			      if(j > 0 && j < iside-1){
                          //    cout << "Tri=" << index1[i][j] << endl;
                          //    cout << "Tri=" << index2[i][j] << endl;
		      Triangles[index1[i][j]].SetNeighb(
			      index2[i][j-1] ,
			      index1[i-1][j] ,
			      index2[i][j]  );
		      Triangles[index2[i][j]].SetNeighb(
			      index1[i][j+1] ,
			      index2[i+1][j] ,
			      index1[i][j]  ); 
	                 }}
                      if(periodic_xz){
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
		      }}
		      EVEN=false;
		      }
		      else
		      {
			      if(i > 0 && i < iside-1){
			      if(j > 0 && j < iside-1){
                        //      cout << "Tri=" << index1[i][j] << endl;
                        //      cout << "Tri=" << index2[i][j] << endl;
		      Triangles[index1[i][j]].SetNeighb(
			      index1[i+1][j] ,
			      index2[i][j-1] ,
			      index2[i][j]  );
		      Triangles[index2[i][j]].SetNeighb(
			      index2[i-1][j] ,
			      index1[i][j+1] ,
			      index1[i][j]  ); 
	                  }}
                      if(periodic_xz){
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
		      }}
	              EVEN=true;
		      }

      }  //end for loop in i
    }  //end for loop in j

    //NodeTris[i] is a linked list of triangles attached to Node[i]
    NodeTris=(intList**)calloc(n_nodes,sizeof(intList*));
    for(int i=0; i< n_nodes; i++){
        NodeTris[i]=new intList;
    }

    for(int i=0; i< ntri; i++){
      int l;
      for(int it=0; it<3; it++){
      if(it==0)l=Triangles[i].Get1();
      if(it==1)l=Triangles[i].Get2();
      if(it==2)l=Triangles[i].Get3();
      ListNode* Node1=new ListNode;
      Node1->SetNode(i);
      NodeTris[l]->AddToTop(Node1);

      if(periodic_xz){
      // match bottom of square to top
      if(l>0 && l < iside){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[l+iside*(iside+1)]->AddToTop(Node1);
      }
      // match top of square bottom
      if(l> iside*(iside+1) && l < (iside+1)*(iside+1)-1){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[l-iside*(iside+1)]->AddToTop(Node1);
      }
      // match left of square to right
      if(l%(iside+1)==0){
         if(l>iside && l < iside*(iside+1)){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[l+iside]->AddToTop(Node1);
         }
      }
      // match right of square to left
      if(l%(iside+1)==iside){
         if(l>iside && l < iside*(iside+1)){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[l-iside]->AddToTop(Node1);
         }
      }

      //four corners
      if(l==0){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[iside]->AddToTop(Node1);
         ListNode* Node2=new ListNode;
         Node2->SetNode(i);
         NodeTris[iside*(iside+1)]->AddToTop(Node2);
         ListNode* Node3=new ListNode;
         Node3->SetNode(i);
         NodeTris[(iside+1)*(iside+1)-1]->AddToTop(Node3);
      }
      if(l==iside){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[0]->AddToTop(Node1);
         ListNode* Node2=new ListNode;
         Node2->SetNode(i);
         NodeTris[iside*(iside+1)]->AddToTop(Node2);
         ListNode* Node3=new ListNode;
         Node3->SetNode(i);
         NodeTris[(iside+1)*(iside+1)-1]->AddToTop(Node3);
      }
      if(l==iside*(iside+1)){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[0]->AddToTop(Node1);
         ListNode* Node2=new ListNode;
         Node2->SetNode(i);
         NodeTris[iside]->AddToTop(Node2);
         ListNode* Node3=new ListNode;
         Node3->SetNode(i);
         NodeTris[(iside+1)*(iside+1)-1]->AddToTop(Node3);
      }
      if(l==(iside+1)*(iside+1)-1){
         ListNode* Node1=new ListNode;
         Node1->SetNode(i);
         NodeTris[0]->AddToTop(Node1);
         ListNode* Node2=new ListNode;
         Node2->SetNode(i);
         NodeTris[iside*(iside+1)]->AddToTop(Node2);
         ListNode* Node3=new ListNode;
         Node3->SetNode(i);
         NodeTris[iside]->AddToTop(Node3);
      }
      }//endif periodic_xz
      } //end it loop

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
    for(int i=0; i< n_nodes;i++){
       NodeNormals[i]=D3Dvec();
    }



    ListNode* Head;
    ListNode* Tail;
    for(int i=0; i< n_nodes;i++){
    Head=NodeTris[i]->GetHead();
    Tail=NodeTris[i]->GetTail();
       int it;
       while(Head->GetNextPtr() != Tail){
         Head=Head->GetNextPtr();
         it=*(Head->GetDatumPtr());
         NodeNormals[i]=NodeNormals[i]+Normals[it];
       }
    }
    for(int i=0; i< n_nodes;i++){
       Normalise(NodeNormals[i]);
    }

   

    istart=0;
    ishow=0;

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
			    speed=speed+speed_diff;
			    if(speed > 60)speed=60;
		    }
		    if(z_Pressed){
			    speed=speed-speed_diff;
			    if(speed  < 0)speed=0;
		    }
              if(Gouraud){
                glShadeModel(GL_SMOOTH);}
                else{
                glShadeModel(GL_FLAT);}

              RenderScene(Camera1); 
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
       if(exwhyzed || aeroplane){
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
              if(t_Pressed){
                 if(PickTri){PickTri=false;} else {PickTri=true;}
                 t_Pressed=false;
              }
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
              bool temp_pick;
              temp_pick=picking;
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
	      picking=temp_pick;
	       }
      if(lightcol){
              bool temp_pick;
              temp_pick=picking;
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
	      picking=temp_pick;
	       }
      if(matcol){
              bool temp_pick;
              temp_pick=picking;
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
	      picking=temp_pick;
	       }
       if(!MouseOn){
         //if MouseOn is true, using mouse controls and see no cursor
       if(!matcol && !lightpos && !lightcol){
       if(picking){
          if(ipicked >=0){
             if(PickTri){
               bool drawbox=true;
               picking=false;
               glDisable(GL_DEPTH_TEST);
               glDisable(GL_LIGHTING);
               DrawTriHits(drawbox); 
               glEnable(GL_DEPTH_TEST);
               glEnable(GL_LIGHTING);
               drawbox=false;
               DrawTriHits(drawbox); 
               picking=true;
             }
             else
             {
               bool drawbox=true;
               picking=false;
               glDisable(GL_DEPTH_TEST);
               glDisable(GL_LIGHTING);
               DrawNodeHits(drawbox); 
               glEnable(GL_DEPTH_TEST);
               glEnable(GL_LIGHTING);
               drawbox=false;
               DrawNodeHits(drawbox); 
               picking=true;
             } //endif else for PickTri
           } //endif for ipicked >= 
         }  //endif for picking
        }  //endif for !matcol etc
       }  //endif for MouseOn
   SDL_GL_SwapBuffers();

}

#include "../../XFiles/Init.cpp"

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

      glInitNames();  //Ignored unless in GL_SELECT 
      if(picking){
           glSelectBuffer(ntri,TriBuffer);
       }

    if(!drawscene){
      (void)glRenderMode(GL_SELECT);

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
      for(int k=0; k< iside; k++){         // k=square cell column number
      for(int l=0; l< iside; l++){             // l=square cell row number
	      for(int m=0; m<2; m++){            // two triangles per square cell

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

                 if(drawscene){
		 if(LIGHTS){
		 ex=Normals[i].GetX(); why=Normals[i].GetY(); zed=Normals[i].GetZ();
	         glNormal3f( ex, zed,-why);
	         //pesky openGL coordinates 
                 }}
 
      if(drawscene){
      int matl=2;
      if(i < 10)matl=6;
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

          if(drawscene){
	      glBegin(GL_TRIANGLES);
                 if(Gouraud){
                 ex=NodeNormals[i1].GetX();  // over-rule ex why zed of triangle normal
                 why=NodeNormals[i1].GetY();  
                 zed=NodeNormals[i1].GetZ();  
	         glNormal3f( ex, zed,-why);
                 glVertex3f( xvals[0], zvals[0], -yvals[0] );
                 ex=NodeNormals[i2].GetX();  
                 why=NodeNormals[i2].GetY();  
                 zed=NodeNormals[i2].GetZ();  
	         glNormal3f( ex, zed,-why);
                 glVertex3f( xvals[1], zvals[1], -yvals[1] );
                 ex=NodeNormals[i3].GetX();  
                 why=NodeNormals[i3].GetY();  
                 zed=NodeNormals[i3].GetZ();  
	         glNormal3f( ex, zed,-why);
          	 glVertex3f( xvals[2], zvals[2], -yvals[2] );
                 }
                 else{
	         glNormal3f( ex, zed,-why);
                 glVertex3f( xvals[0], zvals[0], -yvals[0] );
                 glVertex3f( xvals[1], zvals[1], -yvals[1] );
          	 glVertex3f( xvals[2], zvals[2], -yvals[2] );
                }  //end if else for Gouraud
              glEnd();
             } //endif drawscene
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
	  }

	     if(drawlines && drawscene){
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
          	glVertex3f( xvals[0], zvals[0]+5, -yvals[0] );
          	glVertex3f( xvals[1], zvals[1]+5, -yvals[1] );
          	glVertex3f( xvals[2], zvals[2]+5, -yvals[2] );
             glEnd(); 
       } //endif for drawlines && drawscene


              } // end m loop 
          //l= row number for square cells
          // k=column number
            if(!drawscene && picking && !PickTri){
                 double xv,yv,zv;
                 //we are picking nodes! NODEPICK
                 
                 int idraw1,idraw2,idraw3,idraw4,icell;
                 //pick bottom left of each cell
                 icell=k+iside*l;
                 idraw1=icell+l;  // bottom left
                 xv=NodeV[idraw1].GetX();
                 yv=NodeV[idraw1].GetY();
                 zv=NodeV[idraw1].GetZ();
		 xv=xv+xshift[l][k];
		 yv=yv+yshift[l][k];
		 glPushName(idraw1);
		 glBegin(GL_POINTS);
                     glVertex3f( xv, zv, -yv );
                 glEnd();
           
		 glPopName();
              //   cout << "icell=" << icell << " row=" << l << " col=" << k << endl;
               //   cout << "bottom left=" << idraw1 << endl;
                 // if last column do bottom right
                 if(k%iside==iside-1){
                 idraw2=idraw1+1;
                 xv=NodeV[idraw2].GetX();
                 yv=NodeV[idraw2].GetY();
                 zv=NodeV[idraw2].GetZ();
		 xv=xv+xshift[l][k];
		 yv=yv+yshift[l][k];
		 glPushName(idraw2);
		 glBegin(GL_POINTS);
                     glVertex3f( xv, zv, -yv );
                 glEnd();
		 glPopName();
             //    cout << "bottom right=" << idraw2 << endl;
                 }
                 // if last row do top left
                 if(l%iside==iside-1){
                 idraw3=icell+iside+l+1;
                 xv=NodeV[idraw3].GetX();
                 yv=NodeV[idraw3].GetY();
                 zv=NodeV[idraw3].GetZ();
		 xv=xv+xshift[l][k];
		 yv=yv+yshift[l][k];
		 glPushName(idraw3);
		 glBegin(GL_POINTS);
                     glVertex3f( xv, zv, -yv );
                 glEnd();
		 glPopName();
            //     cout << "top left=" << idraw3 << endl;
                 }
                // if last row last col do top right
                 if(l%iside==iside-1 && k%iside==iside-1){
                 idraw4=idraw3+1;
                 xv=NodeV[idraw4].GetX();
                 yv=NodeV[idraw4].GetY();
                 zv=NodeV[idraw4].GetZ();
		 xv=xv+xshift[l][k];
		 yv=yv+yshift[l][k];
		 glPushName(idraw4);
		 glBegin(GL_POINTS);
                     glVertex3f( xv, zv, -yv );
                 glEnd();
		 glPopName();
               //  cout << "top right=" << idraw4 << endl;
                 }
               
               }   

          } //end l loop
     } //end k loop 	    
      
      if(!drawscene){
         PerspectiveMode(); //needed after glPickMatrix
         glPopMatrix();
         glFlush();
         int hits=glRenderMode(GL_RENDER);
	 if(picking && PickTri)ipicked=ProcessTriangleHits(hits, TriBuffer );
	 if(picking && !PickTri){
                 ipicked=ProcessNodeHits(hits, TriBuffer );}
       }
}
int ProcessTriangleHits(int hits, UINT Buffer[]){
	 UINT* ptr_toBuffer;
	 UINT i,j;
	 UINT names;

         int ireturn=-19999;
         double zmin=1000000;

	 //for nodes, get lots of hits for same node
	 //as it in lots of triangles
	 //for triangles, get lots of hits if we hover
	 //over node

	 ptr_toBuffer=Buffer;
	 if(hits>0){
		 names=*ptr_toBuffer;
	 /*
	 if(names>0){
		 cout << "************************************\n";
		 cout << "hits=" <<hits << endl;
		 cout << "************************************\n";
		 }
          */
	 }

	 for(int i=0; i< hits; i++){
		 names=*ptr_toBuffer;
	
          /*
                  if(names>0){
		  cout <<" number of names  for hit  " << i+1
			 << " is "   << names << endl;
		  }
          */                  
		  float z1,z2;

		  ptr_toBuffer++;    //
		  z1=(float)*ptr_toBuffer/0x7fffffff; 
		  ptr_toBuffer++;    
                  z2=(float)*ptr_toBuffer/0x7fffffff; 
                  
          /*
		  if(names>0)
		  cout << " depths between " <<  z1 << "  and " << z2 << endl;
          */
		  ptr_toBuffer++;
		  if(names>0){
                  //cout << "i=" << i << "  They are " << endl;
		  for(j=0; j< names; j++){
			//  cout <<   "j= " << j << "  " << *ptr_toBuffer << "  ";
                          if(z1<zmin){zmin=z1; ireturn=*ptr_toBuffer;}
                          if(z2<zmin){zmin=z2; ireturn=*ptr_toBuffer;}
		          ptr_toBuffer++;  //child objects of name

		  }  
		//  cout << endl;
                }

	}  // end loop for hits
        SDL_GetMouseState(&mousex, &mousey);
        return ireturn;
}
int ProcessNodeHits(int hits, UINT Buffer[]){
	 UINT* ptr_toBuffer;
	 UINT i,j;
	 UINT names;

         int ireturn=-19999;
         double zmin=1000000;

	 //for nodes, get lots of hits for same node
	 //as it in lots of triangles
	 //for triangles, get lots of hits if we hover
	 //over node

	 ptr_toBuffer=Buffer;
	 if(hits>0){
		 names=*ptr_toBuffer;
	 /*
	 if(names>0){
		 cout << "************************************\n";
		 cout << "hits=" <<hits << endl;
		 cout << "************************************\n";
		 }
          */
	 }

	 for(int i=0; i< hits; i++){
		 names=*ptr_toBuffer;
	
          /*
                  if(names>0){
		  cout <<" number of names  for hit  " << i+1
			 << " is "   << names << endl;
		  }
          */                  
		  float z1,z2;

		  ptr_toBuffer++;    //
		  z1=(float)*ptr_toBuffer/0x7fffffff; 
		  ptr_toBuffer++;    
                  z2=(float)*ptr_toBuffer/0x7fffffff; 
                  
          /*
		  if(names>0)
		  cout << " depths between " <<  z1 << "  and " << z2 << endl;
          */
		  ptr_toBuffer++;
		  if(names>0){
                  //cout << "i=" << i << "  They are " << endl;
		  for(j=0; j< names; j++){
			//  cout <<   "j= " << j << "  " << *ptr_toBuffer << "  ";
                          if(z1<zmin){zmin=z1; ireturn=*ptr_toBuffer;}
                          if(z2<zmin){zmin=z2; ireturn=*ptr_toBuffer;}
		          ptr_toBuffer++;  //child objects of name

		  }  
		//  cout << endl;
                }

	}  // end loop for hits
        SDL_GetMouseState(&mousex, &mousey);
        return ireturn;
}
