///////////////////////////////////////////
#ifndef _VECTOR_
#define _VECTOR_
///////////////////////////////////////////
#define SCALE_FACTOR 6
#define START_SUBDIV 0
#define SHADE_FLAT 1
///////////////////////////////////////////
#include <math.h>
///////////////////////////////////////////
#include "math_defs.h"
///////////////////////////////////////////
typedef struct {unsigned char r,g,b,a;}  color;
typedef struct {float a[16];          }  matrix;
///////////////////////////////////////////
const matrix permutation={ 1,0,0,0,  
                           0,1,0,0, 
                           0,0,0,1, 
                           0,0,1,0 };

const matrix ID={1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1 };
///////////////////////////////////////////
class vector {
  public:
    float x,y,z;
    vector (void);
    vector (float,float,float);
};
class ivector {
  public:
    int x,y,z;
    ivector (void);
    ivector (int,int,int);
};
class ivector2d {
  public:
    int x,y;
    ivector2d (void);
    ivector2d (int,int);
};
///////////////////////////////////////////
void INIT_MATRIX_VECTOR_CALCULATION(void);
///////////////////////////////////////////
float MIN(float a,float b);
float MAX(float a,float b);
float sqr(float a);
float fsin(float a);
float fcos(float a);
///////////////////////////////////////////
//ivector IVECTOR (float a,float b,float c);
//ivector IVEC    (vector a);
vector  inline   VECTOR (float a,float b,float c);
vector  inline   VEC    (ivector a);
void    operator !=(ivector &a,ivector &b);
void    operator +=(ivector &a,ivector &b);
void    operator -=(ivector &a,ivector &b);
void    operator /=(ivector &a,float b);
void    operator *=(ivector &a,float b);
float   operator  *(ivector &a,ivector &b);
ivector operator  *(ivector &a,float i);
ivector operator  /(ivector &a,float i);
ivector operator  +(ivector &a,ivector &b);;
ivector operator  -(ivector &a,ivector &b);
ivector operator  ^(ivector &a,ivector &b);
ivector operator  !(ivector &a);
ivector operator  *(float i, ivector &a);
ivector operator  /(float i, ivector &a);
void    operator !=(vector &a,vector &b);
void    operator +=(vector &a,vector &b);
void    operator -=(vector &a,vector &b);
void    operator /=(vector &a,float   b);
void    operator *=(vector &a,float   b);
void    operator *=(vector &a,matrix &b);
float   operator  *(vector &a,vector &b);
vector  operator  !(vector  &a);
vector  operator  *(float i,vector  a);
vector  operator  /(float i,vector  a);
vector  operator  *(vector &a,  float i );
vector  operator  /(vector &a,  float i );
vector  operator  +(vector &a, vector &b);
vector  operator  -(vector &a, vector &b);
vector  operator  ^ (vector &a, vector &b); // crossproduct
/////////////////////////////////////////// 
matrix  operator  * (matrix &a, matrix &b);
matrix  operator  + (matrix &a, matrix &b);
matrix  operator  - (matrix &a, matrix &b);
void    operator *= (matrix &a, matrix &b);
void    operator += (matrix &a, matrix &b);
void    operator -= (matrix &a, matrix &b);
void    operator += (matrix &x, vector &y);
void    operator -= (matrix &x, vector &y);
//void    operator << (matrix &c,ivector &b);
void    operator << (matrix &c, vector &b);
//void    operator >> (matrix &c,ivector &b);
void    operator >> (matrix &c, vector &b);
//void    operator *= (matrix &b,ivector &c);
void    operator *= (matrix &b, vector &c);
//void    operator /= (matrix &b,ivector &c);
void    operator /= (matrix &b, vector &c);
//void    operator << (vector &c,ivector &b);
void    operator << (vector &c, vector &b);
//void    operator >> (vector &c,ivector &b);
void    operator >> (vector &c, vector &b);
void    operator *= (matrix &b,float &c);
void    operator /= (matrix &b,float &c);
///////////////////////////////////////////
void    matrixInvert(float in[16], float out[16]);
///////////////////////////////////////////
#endif
///////////////////////////////////////////
