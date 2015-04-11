#ifndef IMAGEN_H
#define IMAGEN_H


class Imagen
{
 public:
  double * datos; 
  int dim[2];
  Imagen();
  Imagen(Imagen & im2);
  Imagen(int fil, int col);
  Imagen(int fil, int col, float val);
  ~Imagen();
  void  operator*=(float dt);
  void  operator+=(float val);
  void  operator+=(Imagen & im2);
  void  operator-=(Imagen & im2);
  void  operator*=(Imagen & im2);
  void  operator/=(Imagen & im2);
  void  operator=(Imagen & im2);
  int fils(){return dim[0];}
  int cols(){return dim[1];} 
  int area(){return dim[0]*dim[1];}
  inline double & operator()(int i, int j)
    {
      return(datos[i*dim[1]+j]);
    };
  float maxval();
  float minval();


  void recorta(float M,float m);
  Imagen & desplaza(int dim, int paso);

};


Imagen & lee(char * archima, int campo);
int compara_dims(Imagen & im1, Imagen & im2);
void escribe(char * archima, Imagen & imR, Imagen & imG, Imagen & imB);
void escribe(char * archima, Imagen & imR);

inline double max(double a, double b)
{
  if(a<=b)
    return(b);
  else
    return(a);
};


inline double min(double a, double b)
{
  if(a>=b)
    return(b);
  else
    return(a);
};

inline int signo(double a)
{ 
  if(a!=0.0)
    if(a>0)
      return(1);
    else
      return(-1);
  else 
    return(0);
} ;

#endif

