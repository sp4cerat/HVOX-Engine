#define  PI 3.14159265358979

#define  NORMAL_RAW(n1,n2,n3,a1,a2,a3,b1,b2,b3){\
         n1 = a2*b3-a3*b2;\
		 n2 = a3*b1-a1*b3;\
         n3 = a1*b2-a2*b1; }

//MATRIX:
//
// a[0] .. a[3]
//           .
// a[12] . a[15]

#define  MINICLIP(hit, r,s,t,  s1,s2,s3,   b1,b2,b3, w1,w2,w3,   a1,a2,a3, u1,u2,u3, v1,v2,v3){{\
   	     int lmin1=b1;if(w1<=b1)lmin1=w1;\
		 int lmin2=b2;if(w2<=b2)lmin2=w2;\
		 int lmin3=b3;if(w3<=b3)lmin3=w3;\
		 int lmax1=b1;if(w1>=b1)lmax1=w1;\
		 int lmax2=b2;if(w2>=b2)lmax2=w2;\
		 int lmax3=b3;if(w3>=b3)lmax3=w3;\
		 int dmin1=a1;if(u1<=a1)dmin1=u1;if(v1<=dmin1)dmin1=v1;\
		 int dmin2=a2;if(u2<=a2)dmin2=u2;if(v2<=dmin2)dmin2=v2;\
		 int dmin3=a3;if(u3<=a3)dmin3=u3;if(v3<=dmin3)dmin3=v3;\
		 int dmax1=a1;if(u1>=a1)dmax1=u1;if(v1>=dmax1)dmax1=v1;\
		 int dmax2=a2;if(u2>=a2)dmax2=u2;if(v2>=dmax2)dmax2=v2;\
		 int dmax3=a3;if(u3>=a3)dmax3=u3;if(v3>=dmax3)dmax3=v3;\
		 bool calculate=true;\
		 if(lmax1<dmin1)calculate=false;\
		 if(lmax2<dmin2)calculate=false;\
		 if(lmax3<dmin3)calculate=false;\
		 if(lmin1>dmax1)calculate=false;\
		 if(lmin2>dmax2)calculate=false;\
		 if(lmin3>dmax3)calculate=false;\
  	 if(calculate){\
		 float  c1=b1-a1,c2=b2-a2,c3=b3-a3;\
         float  d1=u1-a1,d2=u2-a2,d3=u3-a3;\
         float  e1=v1-a1,e2=v2-a2,e3=v3-a3;\
         float  f1=w1-b1,f2=w2-b2,f3=w3-b3;\
		 float  n1,n2,n3,zw;\
		 NORMAL_RAW(n1,n2,n3,d1,d2,d3,e1,e2,e3);\
		 zw=(n1*f1+n2*f2+n3*f3) ;if(zw==0)zw=1.0f;\
		 t=-(n1*c1+n2*c2+n3*c3) / zw;\
		 if((t>=0)&&(t<=1)){\
		  s1=t*f1+b1;\
		  s2=t*f2+b2;\
		  s3=t*f3+b3;\
		  float q1=s1-a1;\
		  float q2=s2-a2;\
		  float q3=s3-a3;\
             zw=(d1*e2-d2*e1);if(zw!=0.0f)s=(d1*q2-d2*q1)/zw;else\
            {zw=(d1*e3-d3*e1);if(zw!=0.0f)s=(d1*q2-d2*q1)/zw;else\
            {zw=(d3*e2-d2*e3);s=(d3*q2-d2*q3);if(zw!=0.0f)s/=zw;}}\
		 if((s>=0)&&(s<=1)){\
		  if(abs(d1)<abs(d2)){XCHGf(d1,d2);XCHGf(e1,e2);XCHGf(q1,q2);}\
		  if(abs(d1)<abs(d3)){XCHGf(d1,d3);XCHGf(e1,e3);XCHGf(q1,q3);}\
          if(d1==0)d1=1.0f;\
		  r=(q1-s*e1)/d1;\
		 if((r>=0)&&(r<=1)&&((r+s)<=1))hit=true; \
		 if((r<0)||(r>1)||((r+s)>1)) \
		       r=s=t=2.0f;\
		 }else r=s=t=3.0f;\
		 }else r=s=t=4.0f;\
	  }else    r=s=t=5.0f;\
}}	   //20mul	3div   infinity

//		               s=(d1*q2-d2*q1)/(zw=(d1*e2-d2*e1));\
		  if((zw)==0){ s=(d1*q3-d3*q1)/(zw=(d1*e3-d3*e1));\
		  if((zw)==0){ s=(d3*q2-d2*q3)/    (d3*e2-d2*e3);}}\
//
#define  MATRIXCLIP(t,s,r, s1,s2,s3, b1,b2,b3, w1,w2,w3, a1,a2,a3, u1,u2,u3, v1,v2,v3){{\
		 float  c1=b1-a1,c2=b2-a2,c3=b3-a3;\
         float  d1=u1-a1,d2=u2-a2,d3=u3-a3;\
         float  e1=v1-a1,e2=v2-a2,e3=v3-a3;\
         float  f1=w1-b1,f2=w2-b2,f3=w3-b3;\
		 if(e1>d1){XCHGf(d1,e1);XCHGf(d2,e2);XCHGf(d3,e3);}\
		 \
		 float  c3d1=c3*d1-c1*d3,\
		        e2d1=e2*d1-e1*d2,\
			    e3d1=e3*d1-e1*d3,\
			    f3d1=f3*d1-f1*d3;\
		 t= (c3d1*e2d1-(c2*d1-c1*d2)*e3d1)/\
		    (f3d1*e2d1-(f2*d1-f1*d2)*e3d1);\
		 s= (c3d1-t*f3d1)/e3d1;\
		 r= (c1-s*e1-t*f1)/d1;t=-t;\
					\
		 s1=b1+f1*t;\
		 s2=b2+f2*t;\
		 s3=b3+f3*t;\
		 }}	 //22mul 3div
	                        //*NORMAL_LENGTH
#define  NORMALf(nx,ny,nz,dx1,dy1,dz1,dx2,dy2,dz2)\
         {float mx= (float)dy1*(float)dz2-(float)dy2*(float)dz1;\
          float my=-(float)dx1*(float)dz2+(float)dx2*(float)dz1;\
          float mz=-(float)dy1*(float)dx2+(float)dy2*(float)dx1;\
          float nl=set_light/sqrt(mx*mx+my*my+mz*mz+1);\
          nx=(mx*nl)+207-128;\
          ny=(my*nl)+232-128;\
          nz= mz*nl;\
         };
#define  NORMAL(nx,ny,nz,dx1,dy1,dz1,dx2,dy2,dz2){\
          int x1=dx1,y1=dy1,z1=dz1,x2=dx2,y2=dy2,z2=dz2;\
          if( abs(x1)+abs(y1)+abs(z1)>(1<<15) ) {x1>>=7;y1>>=7;z1>>=7;\
          if( abs(x1)+abs(y1)+abs(z1)>(1<<15) ) {x1>>=7;y1>>=7;z1>>=7;}}\
          if( abs(x2)+abs(y2)+abs(z2)>(1<<15) ) {x2>>=7;y2>>=7;z2>>=7;\
          if( abs(x2)+abs(y2)+abs(z2)>(1<<15) ) {x2>>=7;y2>>=7;z2>>=7;}}\
          nx= y1*z2-y2*z1;\
		  ny=-x1*z2+x2*z1;\
		  nz=-y1*x2+y2*x1;\
          int nl=(abs(nx)+abs(ny)+abs(nz))/set_light;nl++;\
          nx/=nl;ny/=nl;nz/=nl; nx+=140; ny+=140;}

#define  NORMALP(n,d1,d2){NORMAL(n.x,n.y,n.z,d1.x,d1.y,d1.z,d2.x,d2.y,d2.z);};
#define  NORMALPT(n,d1,d2){d1.tz<<=8;d2.tz<<=8;NORMAL(n.tx,n.ty,n.tz,d1.tx,d1.ty,d1.tz,d2.tx,d2.ty,d2.tz);d1.tz>>=8;d2.tz>>=8;n.tz>>=8;};
// rot y,x,z
#define  XCHG(x,y){int z=x;x=y;y=z;}
#define  XCHGf(x,y){float z=x;x=y;y=z;}

#define ROT(tx,ty,tz,x,y,z,SIN1,COS1,SIN2,COS2,SIN3,COS3){\
       {float   x_zw,y_zw,z_zw;\
		x_zw = + z   *SIN1 +    x*COS1 ;\
        z_zw = - x   *SIN1 +    z*COS1 ;\
        y_zw = + z_zw*SIN2 +    y*COS2 ;\
        tz   = - y   *SIN2 + z_zw*COS2 ;\
        tx   = + y_zw*SIN3 + x_zw*COS3 ;\
        ty   = - x_zw*SIN3 + y_zw*COS3 ;}}
//#define ROTP(q,SIN1,COS1,SIN2,COS2,SIN3,COS3){ROT(q.p.x, q.p.y, q.p.z, q.p.x, q.p.y, q.p.z, SIN1,COS1,SIN2,COS2,SIN3,COS3);}
//#define ROTPWORLD(p){ROT(p.x,p.y,p.z,p.x,p.y,p.z,SiN1,CoS1,SiN2,CoS2,SiN3,CoS3);}
//#define ADDPWORLD(p){p.x+=movx;p.y+=movy;p.z+=movz;}
//#define P2WORLD(p){MULPT(p,256);}
#define ROTPWORLD(q){float abx,aby,abz;ROT(abx,aby,abz,q.p.x,q.p.y,q.p.z,SiN1,CoS1,SiN2,CoS2,SiN3,CoS3);q.t.x=256*abx;q.t.y=256*aby;q.t.z=256*abz;}

#define INVROT(tx,ty,tz,x,y,z,SIN1,COS1,SIN2,COS2,SIN3,COS3){\
       {float   x_zw,y_zw,z_zw;\
        x_zw = - y   *SIN3 + x   *COS3 ;\
        y_zw = + x   *SIN3 + y   *COS3 ;\
        z_zw = + y_zw*SIN2 + z   *COS2 ;\
        ty   = - z   *SIN2 + y_zw*COS2 ;\
        tx   = - z_zw*SIN1 + x_zw*COS1 ;\
        tz   = + x_zw*SIN1 + z_zw*COS1 ;}}

/*float y_zw =   z   *SIN1 +    y*COS1 ;\
  float z_zw =   y   *SIN1 -    z*COS1 ;\
  float x_zw =   z_zw*SIN2 +    x*COS2 ;\
        tz   =   x   *SIN2 - z_zw*COS2 ;\
        tx   =   y_zw*SIN3 + x_zw*COS3 ;\
        ty   = - x_zw*SIN3 + y_zw*COS3 ;}
*/
#define hi(a) (((a)>>8)&255)
#define lo(a) ((a)&255)

#define  MATRIXMUL(c,b,a){\
\
          c[ 0]=a[ 0]*b[ 0]+a[ 1]*b[ 4]+a[ 2]*b[ 8]+a[ 3]*b[12];\
          c[ 1]=a[ 0]*b[ 1]+a[ 1]*b[ 5]+a[ 2]*b[ 9]+a[ 3]*b[13];\
          c[ 2]=a[ 0]*b[ 2]+a[ 1]*b[ 6]+a[ 2]*b[10]+a[ 3]*b[14];\
          c[ 3]=a[ 0]*b[ 3]+a[ 1]*b[ 7]+a[ 2]*b[11]+a[ 3]*b[15];\
\
          c[ 4]=a[ 4]*b[ 0]+a[ 5]*b[ 4]+a[ 6]*b[ 8]+a[ 7]*b[12];\
          c[ 5]=a[ 4]*b[ 1]+a[ 5]*b[ 5]+a[ 6]*b[ 9]+a[ 7]*b[13];\
          c[ 6]=a[ 4]*b[ 2]+a[ 5]*b[ 6]+a[ 6]*b[10]+a[ 7]*b[14];\
          c[ 7]=a[ 4]*b[ 3]+a[ 5]*b[ 7]+a[ 6]*b[11]+a[ 7]*b[15];\
\
          c[ 8]=a[ 8]*b[ 0]+a[ 9]*b[ 4]+a[10]*b[ 8]+a[11]*b[12];\
          c[ 9]=a[ 8]*b[ 1]+a[ 9]*b[ 5]+a[10]*b[ 9]+a[11]*b[13];\
          c[10]=a[ 8]*b[ 2]+a[ 9]*b[ 6]+a[10]*b[10]+a[11]*b[14];\
          c[11]=a[ 8]*b[ 3]+a[ 9]*b[ 7]+a[10]*b[11]+a[11]*b[15];\
\
          c[12]=a[12]*b[ 0]+a[13]*b[ 4]+a[14]*b[ 8]+a[15]*b[12];\
          c[13]=a[12]*b[ 1]+a[13]*b[ 5]+a[14]*b[ 9]+a[15]*b[13];\
          c[14]=a[12]*b[ 2]+a[13]*b[ 6]+a[14]*b[10]+a[15]*b[14];\
          c[15]=a[12]*b[ 3]+a[13]*b[ 7]+a[14]*b[11]+a[15]*b[15];\
\
         }

//row
//#define  MATRIXMUL(c,b,a){\
\
          c[ 0]=a[ 0]*b[ 0]+a[ 1]*b[ 4]+a[ 2]*b[ 8];\
          c[ 1]=a[ 0]*b[ 1]+a[ 1]*b[ 5]+a[ 2]*b[ 9];\
          c[ 2]=a[ 0]*b[ 2]+a[ 1]*b[ 6]+a[ 2]*b[10];\
\
          c[ 4]=a[ 4]*b[ 0]+a[ 5]*b[ 4]+a[ 6]*b[ 8];\
          c[ 5]=a[ 4]*b[ 1]+a[ 5]*b[ 5]+a[ 6]*b[ 9];\
          c[ 6]=a[ 4]*b[ 2]+a[ 5]*b[ 6]+a[ 6]*b[10];\
\
          c[ 8]=a[ 8]*b[ 0]+a[ 9]*b[ 4]+a[10]*b[ 8];\
          c[ 9]=a[ 8]*b[ 1]+a[ 9]*b[ 5]+a[10]*b[ 9];\
          c[10]=a[ 8]*b[ 2]+a[ 9]*b[ 6]+a[10]*b[10];\
\
          c[12]=a[12]*b[ 0]+a[13]*b[ 4]+a[14]*b[ 8]+b[12];\
          c[13]=a[12]*b[ 1]+a[13]*b[ 5]+a[14]*b[ 9]+b[13];\
          c[14]=a[12]*b[ 2]+a[13]*b[ 6]+a[14]*b[10]+b[14];\
\
         }

// 36* 27+   column
//deefine  MATRIXMUL(c,a,b){\
\
          c[ 0]=a[ 0]*b[ 0]+a[ 1]*b[ 4]+a[ 2]*b[ 8];\
          c[ 1]=a[ 0]*b[ 1]+a[ 1]*b[ 5]+a[ 2]*b[ 9];\
          c[ 2]=a[ 0]*b[ 2]+a[ 1]*b[ 6]+a[ 2]*b[10];\
          c[ 3]=a[ 0]*b[ 3]+a[ 1]*b[ 7]+a[ 2]*b[11]+a[ 3];\
\
          c[ 4]=a[ 4]*b[ 0]+a[ 5]*b[ 4]+a[ 6]*b[ 8];\
          c[ 5]=a[ 4]*b[ 1]+a[ 5]*b[ 5]+a[ 6]*b[ 9];\
          c[ 6]=a[ 4]*b[ 2]+a[ 5]*b[ 6]+a[ 6]*b[10];\
          c[ 7]=a[ 4]*b[ 3]+a[ 5]*b[ 7]+a[ 6]*b[11]+a[ 7];\
\
          c[ 8]=a[ 8]*b[ 0]+a[ 9]*b[ 4]+a[10]*b[ 8];\
          c[ 9]=a[ 8]*b[ 1]+a[ 9]*b[ 5]+a[10]*b[ 9];\
          c[10]=a[ 8]*b[ 2]+a[ 9]*b[ 6]+a[10]*b[10];\
          c[11]=a[ 8]*b[ 3]+a[ 9]*b[ 7]+a[10]*b[11]+a[11];\
\
         }

//column
//#define MATRIXSCALE(b,c){\
 b.a[ 0]*=c.x;b.a[ 1]*=c.y;b.a[ 2]*=c.z;\
 b.a[ 4]*=c.x;b.a[ 5]*=c.y;b.a[ 6]*=c.z;\
 b.a[ 8]*=c.x;b.a[ 9]*=c.y;b.a[10]*=c.z;\
}

// row
#define MATRIXSCALE(b,c){\
 b.a[ 0]*=c.x;b.a[ 1]*=c.x;b.a[ 2]*=c.x;b.a[ 3]*=c.x;\
 b.a[ 4]*=c.y;b.a[ 5]*=c.y;b.a[ 6]*=c.y;b.a[ 7]*=c.y;\
 b.a[ 8]*=c.z;b.a[ 9]*=c.z;b.a[10]*=c.z;b.a[11]*=c.z;\
}

#define  MATRIXADD(a,b){ for (int i=0;i<15;i++)a[i]+=b[i]; }
#define  MATRIXSUB(a,b){ for (int i=0;i<15;i++)a[i]-=b[i]; }
//column
//#define  VECTORMUL(x1,y1,z1,a,x2,y2,z2){\
          x1=a[0]*x2+a[1]*y2+a[2 ]*z2+a[3 ];\
          y1=a[4]*x2+a[5]*y2+a[6 ]*z2+a[7 ];\
          z1=a[8]*x2+a[9]*y2+a[10]*z2+a[11];\
         }
//row
#define  VECTORMUL(x1,y1,z1,a,x2,y2,z2){\
          x1=a[0]*x2+a[4]*y2+a[8 ]*z2+a[12];\
          y1=a[1]*x2+a[5]*y2+a[9 ]*z2+a[13];\
          z1=a[2]*x2+a[6]*y2+a[10]*z2+a[14];\
         }

#define LEN(a) sqrt(a.x*a.x+a.y*a.y+a.z*a.z)

///////////////////////////////////////////

#define ROT_X(a,x){ MRX[10]=MRX[5 ]= _COS[ -x &(SIN_COS_PRECISION-1)];\
                    MRX[9 ]=MRX[6 ]=-_SIN[ -x &(SIN_COS_PRECISION-1)];MRX[9 ]=-MRX[9 ];\
                    matrix r;MATRIXMUL(r.a,a.a,MRX.a);a=r;}

#define ROT_Y(a,y){ MRY[0 ]=MRY[10]= _COS[ -y &(SIN_COS_PRECISION-1)];\
                    MRY[2 ]=MRY[8 ]=-_SIN[ -y &(SIN_COS_PRECISION-1)];MRY[2 ]=-MRY[2 ];\
                    matrix r;MATRIXMUL(r.a,a.a,MRY.a);a=r;}

#define ROT_Z(a,z){ MRZ[0 ]=MRZ[5 ]= _COS[ -z &(SIN_COS_PRECISION-1)];\
                    MRZ[4 ]=MRZ[1 ]=-_SIN[ -z &(SIN_COS_PRECISION-1)];MRZ[4 ]=-MRZ[4 ];\
                    matrix r;MATRIXMUL(r.a,a.a,MRZ.a);a=r;}

//column

//#define ADD_V(c,b){\
                 c.a[ 3] += c.a[ 0]*b.x + c.a[ 1]*b.y + c.a[ 2]*b.z ;\
                 c.a[ 7] += c.a[ 4]*b.x + c.a[ 5]*b.y + c.a[ 6]*b.z ;\
                 c.a[11] += c.a[ 8]*b.x + c.a[ 9]*b.y + c.a[10]*b.z ;\
                 c.a[15] += c.a[12]*b.x + c.a[13]*b.y + c.a[14]*b.z ;\
                 }
//#define SUB_V(c,b){\
                 c.a[ 3] -= c.a[ 0]*b.x + c.a[ 1]*b.y + c.a[ 2]*b.z ;\
                 c.a[ 7] -= c.a[ 4]*b.x + c.a[ 5]*b.y + c.a[ 6]*b.z ;\
                 c.a[11] -= c.a[ 8]*b.x + c.a[ 9]*b.y + c.a[10]*b.z ;\
                 c.a[15] -= c.a[12]*b.x + c.a[13]*b.y + c.a[14]*b.z ;\
                 }

//row
#define ADD_V(c,b){\
                 c.a[12] += c.a[ 0]*b.x + c.a[ 4]*b.y + c.a[ 8]*b.z ;\
                 c.a[13] += c.a[ 1]*b.x + c.a[ 5]*b.y + c.a[ 9]*b.z ;\
                 c.a[14] += c.a[ 2]*b.x + c.a[ 6]*b.y + c.a[10]*b.z ;\
                 c.a[15] += c.a[ 3]*b.x + c.a[ 7]*b.y + c.a[11]*b.z ;\
                 }
#define SUB_V(c,b){\
                 c.a[12] -= c.a[ 0]*b.x + c.a[ 4]*b.y + c.a[ 8]*b.z ;\
                 c.a[13] -= c.a[ 1]*b.x + c.a[ 5]*b.y + c.a[ 9]*b.z ;\
                 c.a[14] -= c.a[ 2]*b.x + c.a[ 6]*b.y + c.a[10]*b.z ;\
                 c.a[15] -= c.a[ 3]*b.x + c.a[ 7]*b.y + c.a[11]*b.z ;\
                 }

#define INVROT_VV(v,c){              float sx= _SIN[ (int)( c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cx= _COS[ (int)( c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sy= _SIN[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cy= _COS[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sz= _SIN[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cz= _COS[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     ROT(v.x,v.y,v.z,v.x,v.y,v.z,sx,cx,sy,cy,sz,cz);\
                                     }

#define ROT_VV(v,c){                 float sx= _SIN[ (int)( c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cx= _COS[ (int)( c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sy= _SIN[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cy= _COS[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sz= _SIN[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cz= _COS[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     INVROT(v.x,v.y,v.z,v.x,v.y,v.z,sx,cx,sy,cy,sz,cz);\
                                     }

#define INVROT_V(m,c){               float sx=-_SIN[ (int)(-c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cx= _COS[ (int)(-c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sy=-_SIN[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cy= _COS[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sz=-_SIN[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cz= _COS[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     MRXYZ.a[0 ]=cy*cz;\
                                     MRXYZ.a[1 ]=cy*sz;\
                                     MRXYZ.a[2 ]=-sy;\
                                     MRXYZ.a[4 ]=sx*sy*cz-cx*sz;\
                                     MRXYZ.a[5 ]=sx*sy*sz+cx*cz;\
                                     MRXYZ.a[6 ]=sx*cy;\
                                     MRXYZ.a[8 ]=cx*sy*cz+sx*sz;\
                                     MRXYZ.a[9 ]=cx*sy*sz-sx*cz;\
                                     MRXYZ.a[10]=cx*cy;\
                                     matrix r;MATRIXMUL(r.a,m.a,MRXYZ.a);m=r;\
									 }
        
#define ROT_V(m,c){                  float sx=-_SIN[ (int)(-c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cx= _COS[ (int)(-c.y*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sy=-_SIN[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cy= _COS[ (int)( c.x*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     float sz=-_SIN[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)],cz= _COS[ (int)( c.z*SIN_COS_PRECISION) & (SIN_COS_PRECISION-1)];\
                                     MRXYZ.a[0 ]=cy*cz;\
                                     MRXYZ.a[4 ]=cy*sz;\
                                     MRXYZ.a[8 ]=-sy;\
                                     MRXYZ.a[1 ]=sx*sy*cz-cx*sz;\
                                     MRXYZ.a[5 ]=sx*sy*sz+cx*cz;\
                                     MRXYZ.a[9 ]=sx*cy;\
                                     MRXYZ.a[2 ]=cx*sy*cz+sx*sz;\
                                     MRXYZ.a[6 ]=cx*sy*sz-sx*cz;\
                                     MRXYZ.a[10]=cx*cy;\
                                     matrix r;MATRIXMUL(r.a,m.a,MRXYZ.a);m=r;\
									 }
