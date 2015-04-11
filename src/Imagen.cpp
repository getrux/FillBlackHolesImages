#include <stdio.h>
#include <math.h>
#include <Imagen.h>

Imagen::Imagen()
{
  dim[0]=0;
  dim[1]=0;
  datos=new double[1];
}

Imagen::Imagen(Imagen & im2)
{
  dim[0]=im2.fils();
  dim[1]=im2.cols();
  int largo=dim[0]*dim[1];
  datos=new double[largo];
  for(int i=0; i< largo; i++)
    datos[i]=im2.datos[i];
}

Imagen::Imagen(int fil, int col)
{
  dim[0]=fil;
  dim[1]=col;
  datos=new double[fil*col];
}

Imagen::Imagen(int fil, int col, float val)
{
  dim[0]=fil;
  dim[1]=col;
  datos=new double[fil*col];
  for(int i=0; i<fil*col;i++)
    datos[i]=val;
}

Imagen::~Imagen()
{
  delete[] datos;
}

void Imagen::operator*=(float dt)
{
  int largo=dim[0]*dim[1];
  for(int i=0;i<largo;i++)
    datos[i]*=dt;
}

void Imagen::operator+=(float dt)
{
  int largo=dim[0]*dim[1];
  for(int i=0;i<largo;i++)
    datos[i]+=dt;
}

void Imagen::operator+=(Imagen & im2)
{
  int fil2=im2.fils();
  int col2=im2.cols();
  if(dim[0]!=fil2 || dim[1]!=col2)
    {
      fprintf(stderr,"Diferentes dimensiones al sumar imagenes \n");
      return;
    }

  int largo=dim[0]*dim[1];
  for(int i=0; i<largo; i++)
    datos[i]+=im2.datos[i];

  return;
}

void Imagen::operator-=(Imagen & im2)
{
  int fil2=im2.fils();
  int col2=im2.cols();
  if(dim[0]!=fil2 || dim[1]!=col2)
    {
      fprintf(stderr,"Diferentes dimensiones al restar imagenes \n");
      return;
    }

  int largo=dim[0]*dim[1];
  for(int i=0; i<largo; i++)
    datos[i]-=im2.datos[i];

  return;
}


void Imagen::operator*=(Imagen  & im2)
{
  int fil2=im2.fils();
  int col2=im2.cols();
  if(dim[0]!=fil2 || dim[1]!=col2)
    {
      fprintf(stderr,"Diferentes dimensiones al multiplicar imagenes \n");
      return;
    }

  int largo=dim[0]*dim[1];
  for(int i=0; i<largo; i++)
    this->datos[i] *= (&im2)->datos[i];

  return;
}

void Imagen::operator/=(Imagen  & im2)
{
  // si el divisor es 0, pone 0 en el cociente...
  int fil2=im2.fils();
  int col2=im2.cols();
  if(dim[0]!=fil2 || dim[1]!=col2)
    {
      fprintf(stderr,"Diferentes dimensiones al multiplicar imagenes \n");
      return;
    }

  int largo=dim[0]*dim[1];
  double div;
  for(int i=0; i<largo; i++)
    {
      div=im2.datos[i];
      if(fabs(div)>1e-10)
	datos[i]/=div;
      else
	datos[i]=0.0;
    }
  return;
}

void Imagen::operator=(Imagen  & im2)
{
  // este operador no duplica la memoria //
  int fil2=im2.fils();
  int col2=im2.cols();
  if(dim[0]!=fil2 || dim[1]!=col2)
    {
      //      fprintf(stderr,"Diferentes dimensiones al copiar imagenes \n");
      dim[0]=fil2;
      dim[1]=col2;
      delete[] datos;
      datos=new double[fil2*col2];
    }

  int largo=dim[0]*dim[1];
  for(int i=0; i<largo; i++)
    datos[i]=im2.datos[i];

  return;
}


float Imagen::minval()
{
  float m=1e20;
  int largo=dim[0]*dim[1];
  for(int i=0; i<largo; i++)
    m=min(m,datos[i]);

  return m;
}


float Imagen::maxval()
{
  float m=-1e20;
  int largo=dim[0]*dim[1];
  for(int i=0; i<largo; i++)
    m=max(m,datos[i]);

  return m;
}
