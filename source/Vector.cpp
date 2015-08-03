///////////////////////////////////////////
#include "Vector.h"        
///////////////////////////////////////////

#define SIN_COS_PRECISION 65536

float _SIN[SIN_COS_PRECISION];
float _COS[SIN_COS_PRECISION];

///////////////////////////////////////////
matrix MRX=   {1,0,0,0, 
               0,1,0,0, 
               0,0,1,0, 
               0,0,0,1 };

matrix MRY=   {1,0,0,0, 
               0,1,0,0, 
               0,0,1,0, 
               0,0,0,1 };

matrix MRZ=   {1,0,0,0, 
               0,1,0,0, 
               0,0,1,0, 
               0,0,0,1 };

matrix MRXYZ= {1,0,0,0, 
               0,1,0,0, 
               0,0,1,0, 
               0,0,0,1 };

vector _TMP_VEC;

///////////////////////////////////////////

float MIN (float a,float b){if(a<b)return a;return b;}
float MAX (float a,float b){if(a>b)return a;return b;}
float sqr (float a){return (a*a);}
float fsin(float a){return _SIN[(int)(a*SIN_COS_PRECISION)&(SIN_COS_PRECISION-1) ];}
float fcos(float a){return _COS[(int)(a*SIN_COS_PRECISION)&(SIN_COS_PRECISION-1) ];}

///////////////////////////////////////////
 vector::vector () {}
ivector::ivector () {}
ivector2d::ivector2d () {}

vector::vector (float a, float b,float c) 
{
 x = a;
 y = b;
 z = c; 
}

ivector::ivector (int a, int b,int c) 
{
  x = a;
  y = b;
  z = c;
}

ivector2d::ivector2d (int a, int b) 
{
  x = a;
  y = b;
}

///////////////////////////////////////////

void INIT_MATRIX_VECTOR_CALCULATION(void)
{
  for(int a=0;a<SIN_COS_PRECISION;a++) _SIN [a]=sin(a*PI*2/SIN_COS_PRECISION);
  for(    a=0;a<SIN_COS_PRECISION;a++) _COS [a]=cos(a*PI*2/SIN_COS_PRECISION);
}

vector  inline   VECTOR (float a,float b,float c){ vector _TMP_VEC;_TMP_VEC.x=a;_TMP_VEC.y=b;_TMP_VEC.z=c;return _TMP_VEC; }
vector  inline   VEC    (ivector a){ vector _TMP_VEC;_TMP_VEC.x=a.x;_TMP_VEC.y=a.y;_TMP_VEC.z=a.z;return _TMP_VEC; }
ivector IVECTOR (float a,float b,float c){ ivector _TMP_VEC;_TMP_VEC.x=a;_TMP_VEC.y=b;_TMP_VEC.z=c;return _TMP_VEC; }
ivector IVEC (vector a){ ivector _TMP_VEC;_TMP_VEC.x=a.x;_TMP_VEC.y=a.y;_TMP_VEC.z=a.z;return _TMP_VEC; }
void    operator != (ivector &a,ivector &b){ a.x=-b.x;a.y=-b.y;a.z=-b.z; }
void    operator += (ivector &a,ivector &b){ a.x+=b.x;a.y+=b.y;a.z+=b.z; }
void    operator -= (ivector &a,ivector &b){ a.x-=b.x;a.y-=b.y;a.z-=b.z; }
void    operator /= (ivector &a,float b){ a.x/=b  ;a.y/=b  ;a.z/=b  ; }
void    operator *= (ivector &a,float b){ a.x*=b  ;a.y*=b  ;a.z*=b  ; }
float   operator  * (ivector &a,ivector &b){ return (a.x*b.x+a.y*b.y+a.z*b.z); }
ivector operator  * (ivector &a,float i){ ivector _TMP_VEC;_TMP_VEC.x=a.x*i;_TMP_VEC.y=a.y*i;_TMP_VEC.z=a.z*i;return _TMP_VEC; }
ivector operator  / (ivector &a,float i){ ivector _TMP_VEC;_TMP_VEC.x=a.x/i;_TMP_VEC.y=a.y/i;_TMP_VEC.z=a.z/i;return _TMP_VEC; }
ivector operator  + (ivector &a,ivector &b){ ivector _TMP_VEC;_TMP_VEC.x=a.x+b.x;_TMP_VEC.y=a.y+b.y;_TMP_VEC.z=a.z+b.z;return _TMP_VEC; }
ivector operator  - (ivector &a,ivector &b){ ivector _TMP_VEC;_TMP_VEC.x=a.x-b.x;_TMP_VEC.y=a.y-b.y;_TMP_VEC.z=a.z-b.z;return _TMP_VEC; }
ivector operator  ^ (ivector &a,ivector &b){ ivector _TMP_VEC;_TMP_VEC.x=a.y*b.z-b.y*a.z;_TMP_VEC.y=-a.x*b.z+b.x*a.z;_TMP_VEC.z=-a.y*b.x+b.y*a.x;return _TMP_VEC;};
ivector operator  ! (ivector &a){ ivector _TMP_VEC;float len=1/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);a.x*=len;a.y*=len;a.z*=len;return _TMP_VEC=a;};
ivector operator  * (float i, ivector &a){ ivector _TMP_VEC;_TMP_VEC.x=a.x*i;_TMP_VEC.y=a.y*i;_TMP_VEC.z=a.z*i;return _TMP_VEC; }
ivector operator  / (float i, ivector &a){ ivector _TMP_VEC;_TMP_VEC.x=a.x/i;_TMP_VEC.y=a.y/i;_TMP_VEC.z=a.z/i;return _TMP_VEC; }
void    operator != (vector &a,vector &b){ a.x=-b.x;a.y=-b.y;a.z=-b.z; }
void    operator += (vector &a,vector &b){ a.x+=b.x;a.y+=b.y;a.z+=b.z; }
void    operator -= (vector &a,vector &b){ a.x-=b.x;a.y-=b.y;a.z-=b.z; }
void    operator /= (vector &a,float   b){ a.x/=b  ;a.y/=b  ;a.z/=b  ; }
void    operator *= (vector &a,float   b){ a.x*=b  ;a.y*=b  ;a.z*=b  ; }
void    operator *= (vector &a,matrix &b){ VECTORMUL(_TMP_VEC.x,_TMP_VEC.y,_TMP_VEC.z,b.a,a.x,a.y,a.z);a=_TMP_VEC; }
float   operator  * (vector &a,vector &b){ return (a.x*b.x+a.y*b.y+a.z*b.z); }
vector  operator  ! (vector  &a){ vector _TMP_VEC;float len=1/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);a.x*=len;a.y*=len;a.z*=len;return _TMP_VEC=a;};
vector  operator  * (float i,vector  a){ a.x*=i;a.y*=i;a.z*=i;return a; }
vector  operator  / (float i,vector  a){ a.x/=i;a.y/=i;a.z/=i;return a; }
vector  operator  * (vector &a,float i){ vector _TMP_VEC;_TMP_VEC.x=a.x*i;_TMP_VEC.y=a.y*i;_TMP_VEC.z=a.z*i;return _TMP_VEC; }
vector  operator  / (vector &a,float i){ vector _TMP_VEC;_TMP_VEC.x=a.x/i;_TMP_VEC.y=a.y/i;_TMP_VEC.z=a.z/i;return _TMP_VEC; }
vector  operator  + (vector &a,vector &b) { vector _TMP_VEC;_TMP_VEC.x=a.x+b.x;_TMP_VEC.y=a.y+b.y;_TMP_VEC.z=a.z+b.z;return _TMP_VEC; }
vector  operator  - (vector &a,vector &b) { vector _TMP_VEC;_TMP_VEC.x=a.x-b.x;_TMP_VEC.y=a.y-b.y;_TMP_VEC.z=a.z-b.z;return _TMP_VEC; }
vector  operator  ^ (vector &a,vector &b) { vector _TMP_VEC;_TMP_VEC.x=a.y*b.z-b.y*a.z;_TMP_VEC.y=-a.x*b.z+b.x*a.z;_TMP_VEC.z=-a.y*b.x+b.y*a.x;return _TMP_VEC;};
matrix  operator  * (matrix &a, matrix &b){ matrix r;MATRIXMUL(r.a,a.a,b.a);return r; }
matrix  operator  + (matrix &a, matrix &b){ matrix r;MATRIXADD(r.a,a.a,b.a);return r; }
matrix  operator  - (matrix &a, matrix &b){ matrix r;MATRIXSUB(r.a,a.a,b.a);return r; }
void    operator *= (matrix &a, matrix &b){ matrix r;MATRIXMUL(r.a,a.a,b.a);a=r; }
void    operator += (matrix &a, matrix &b){ matrix r;MATRIXADD(r.a,a.a,b.a);a=r; }
void    operator -= (matrix &a, matrix &b){ matrix r;MATRIXSUB(r.a,a.a,b.a);a=r; }
void    operator += (matrix &x, vector &y){    ADD_V(x,y); }
void    operator -= (matrix &x, vector &y){    SUB_V(x,y); }
void    operator << (matrix &c,ivector &b){    ROT_V(c,b); }
void    operator << (matrix &c, vector &b){    ROT_V(c,b); }
void    operator >> (matrix &c,ivector &b){ INVROT_V(c,b); }
void    operator >> (matrix &c, vector &b){ INVROT_V(c,b); }
void    operator *= (matrix &b,ivector &c){ MATRIXSCALE(b,c); }
void    operator *= (matrix &b, vector &c){ MATRIXSCALE(b,c); }
void    operator /= (matrix &b,ivector &c){ ivector d=IVECTOR(1/c.x,1/c.y,1/c.z);MATRIXSCALE(b,d); }
void    operator /= (matrix &b, vector &c){  vector d= VECTOR(1/c.x,1/c.y,1/c.z);MATRIXSCALE(b,d); }
void    operator << (vector &c,ivector &b){    ROT_VV(c,b); }
void    operator << (vector &c, vector &b){    ROT_VV(c,b); }
void    operator >> (vector &c,ivector &b){ INVROT_VV(c,b); }
void    operator >> (vector &c, vector &b){ INVROT_VV(c,b); }
void    operator *= (matrix &b,float &c){ for(int t=0;t<15;t++)b.a[t]*=c;}
void    operator /= (matrix &b,float &c){ for(int t=0;t<15;t++)b.a[t]/=c;}


//////////////////////////
// Invert a matrix. (Matrix MUST be orhtonormal!)
//   in - Input matrix
//   out - Output matrix
//////////////////////////
void matrixInvert(float in[16], float out[16])
{
  // Transpose rotation
  out[ 0] = in[ 0];  out[ 1] = in[ 4];  out[ 2] = in[ 8];
  out[ 4] = in[ 1];  out[ 5] = in[ 5];  out[ 6] = in[ 9];
  out[ 8] = in[ 2];  out[ 9] = in[ 6];  out[10] = in[10];
  
  // Clear shearing terms
  out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;

  // Translation is minus the dot of tranlation and rotations
  out[12] = -(in[12]*in[ 0]) - (in[13]*in[ 1]) - (in[14]*in[ 2]);
  out[13] = -(in[12]*in[ 4]) - (in[13]*in[ 5]) - (in[14]*in[ 6]);
  out[14] = -(in[12]*in[ 8]) - (in[13]*in[ 9]) - (in[14]*in[10]);
}	 

//void     operator |=(ivector &a,ivector &b){ float len=1/sqrt(b.x*b.x+b.y*b.y+b.z*b.z);a.x=b.x*len;a.y=b.y*len;a.z=b.z*len; }
//void     operator |=(vector &a,vector &b){ float len=1/sqrt(b.x*b.x+b.y*b.y+b.z*b.z);a.x=b.x*len;a.y=b.y*len;a.z=b.z*len; }
//vector   operator !(vector  &a){ vector _TMP_VEC;float len=1/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);_TMP_VEC.x*=len;_TMP_VEC.y*=len;_TMP_VEC.z*=len;return _TMP_VEC;};
//vector   operator !(vector &a            ){ vector _TMP_VEC;_TMP_VEC.x=-a.x;_TMP_VEC.y=-a.y;_TMP_VEC.z=-a.z;return _TMP_VEC; }
/*       
void   inline TRANSPOSE(matrix &m){ 
              XCHGf(m.a[ 1],m.a[ 4]);
              XCHGf(m.a[ 2],m.a[ 8]);
              XCHGf(m.a[ 3],m.a[12]);
              XCHGf(m.a[ 6],m.a[ 9]);
              XCHGf(m.a[ 7],m.a[13]);
              XCHGf(m.a[11],m.a[14]);
              }
*/


