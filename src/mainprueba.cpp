#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include <Magick++.h>
#include <iostream> 
using namespace std; 
using namespace Magick; 
#include <Imagen.h>
#include <vector>

void uso()
{
  fprintf(stderr,"How to use: ./executable original_image mask_imagek out windows \n");
  fprintf(stderr,"Example: \n");
  fprintf(stderr,"./executable war.png war_mask.png output.png 1 \n");
  exit(1);
};



//returns a vector with all of positions where you will find the hole
vector<int> getPosNegros(Imagen uR, Imagen uG, Imagen uB){
    int cols = uR.cols();
    int fils = uR.fils();

    vector<int> aux;
    aux.clear();
    for(int i=0;i<cols*fils;i++)
    {
        if(uR.datos[i] == 0 && uG.datos[i] == 0 && uB.datos[i] ==0)
        {
            aux.push_back(i);
        }
    }
    
    return aux;
}

//Check whether a number belongs to a vector
//We use it to check if a pixel is in the vector called pos_negros
//I mean if that pixel is a hole or not

bool checkNumVector(vector<int> pos_negros, int num){
             vector<int>::iterator it;

  for ( it=pos_negros.begin() ; it < pos_negros.end(); it++ )
  {

      if(*it == num)
          return true;
  }


    return false;

}

//Take a neighbor's pixel
vector<double> takeNeighbours(Imagen oR, vector<int> pos_negros, int pos,int ventana)
{
    int cols=oR.cols();
    int fils=oR.fils();
    int k=0;
    int l=0;
    vector<double> vecinos;
    vecinos.clear();

    k=(pos-fils*ventana)-ventana;//We position the top of the window

    //we go through the window and while the pixel isn't black in the mask
    //we keep using the same pixel value
    for(int i=0;i<(ventana*ventana+1)*(ventana*ventana+1);i++)
    {
        if (checkNumVector(pos_negros, k)==false)//check if the pixel is black or not
        {
            vecinos.push_back(oR.datos[k]);
        }else{
            vecinos.push_back(-1);//set -1 the black pixel of the mask //marcamos -1 el pixel negro de la mask

        }
        k++;
        l++;



        //to go through the square window
        if(ventana*ventana < l)
        {
             k=(k+fils)-(ventana*ventana+1);
             l=0;
        }
        
    }
    
    //To check if the neighbors were saved.
    //Comprobación de que ha guardado bien los vecinos
      /*   vector<int>::iterator it;

     cout << "vecinos : "<<endl;
  for ( it=vecinos.begin() ; it < vecinos.end(); it++ )
    cout << *it << endl;*/

    return vecinos;
}

//Makes calculating average between windows
double calcDif(vector<double> neigh1, vector<double> neigh2)
{

    double num=0;
    for(int i=0;i<neigh1.size();i++)
    {
        if(neigh1[i] != -1 && neigh2[i] != 1){
                  num+=(neigh1[i]-neigh2[i])*(neigh1[i]-neigh2[i]);
        }

    }

    return num;

}

//calculates the smallest vector, minimal difference
int minVector(int *vec, int dim)
{
    int min=0;
    for(int i=1; i<dim-1;i++)
    {
        if(*vec < *(vec+i))
        {
        ;
        }
        else{
            min=i;
            *vec=*(vec+i);
        }
    }

    return min;
}


void paintPixel(double* oR, int pos, int candidato){

    *(oR+pos)=*(oR+candidato);//set candidate pixel in the hole //pone pixel candidato en el agujero


}

void checkCandidate(Imagen &oR, Imagen &oG, Imagen &oB, vector<int> pos_negros, int ventana)
{
    int pos=pos_negros[0];
    int aux=pos;
    int fils = oR.fils();
    int cols = oR.cols();

    vector<double> v1;
    vector<double> v2;
    vector<double> v3;
    v1.clear();
    v2.clear();
    v3.clear();

    double dif=0;
    int candidato=0;
    int *candidatos = (int*)malloc(sizeof(int)*fils*cols);


// until the positions where there are holes listed is not empty
     // repeat the algorithm with each empty pixel.

    while(!pos_negros.empty())
    {
        pos=pos_negros[0];
        aux=pos;

    for(int i=0;i<fils*cols;i++){
        *(candidatos+i)=1000000;
    }

    //fist windows, its always fixed
    v1=takeNeighbours(oR,pos_negros,pos,ventana);



    //looking for neighbors from down
   for(int i=0;i<ventana*ventana+1;i++)//
    {
        pos=pos+fils;
        v2=takeNeighbours(oR,pos_negros,pos,ventana);
        dif = calcDif(v1,v2);
        *(candidatos+pos) = dif;
       // cout << "can: " << candidatos[pos] << endl;
    }

    //looking for neighbors from top
    pos=aux;
    for(int i=0;i<ventana*ventana+1;i++)//
    {
        pos=pos-fils;
        v2=takeNeighbours(oR,pos_negros,pos,ventana);
        dif = calcDif(v1,v2);
        *(candidatos+pos) = dif;
       // cout << "can: " << candidatos[pos] << endl;
    }

    //looking for neighbors from right
    pos=aux;
    for(int i=0;i<ventana*ventana+1;i++)//
    {
        pos++;
        v2=takeNeighbours(oR,pos_negros,pos,ventana);
        dif = calcDif(v1,v2);
        *(candidatos+pos) = dif;
       // cout << "can: " << candidatos[pos] << endl;
    }
   
    //looking for neighbors from left
    pos=aux;
    for(int i=0;i<ventana*ventana+1;i++)//
    {
        pos--;
        v2=takeNeighbours(oR,pos_negros,pos,ventana);
        dif = calcDif(v1,v2);
        *(candidatos+pos) = dif;
       // cout << "can: " << candidatos[pos] << endl;
    }

    //looking for the pixel smallest
    //the smallest pixel is the one that is the most similar 
    candidato=minVector(candidatos,fils*cols);

    //painting the pixel in the 3 RGB images
     paintPixel(oR.datos, aux, candidato);
     paintPixel(oG.datos, aux, candidato);
     paintPixel(oB.datos, aux, candidato);

     pos_negros.erase(pos_negros.begin());//remove the pixel that has been painted
    }





    free(candidatos);


}


main(int argc,char **argv)
{
  //Check how many arguments there and show how to execute the software
  if(argc<5)
    uso();


  argv++;
  char * imagen_original=*argv++;
  char * imagen_mask = *argv++;
  char * salida_char=*argv++;
  int ventana=atoi(*argv++);
 



 
  //Reading mask image
  Imagen uR=lee(imagen_mask,0);
  Imagen uG=lee(imagen_mask,1);
  Imagen uB=lee(imagen_mask,2);

//Reading original image with hole
  Imagen oR=lee(imagen_original,0);
  Imagen oG=lee(imagen_original,1);
  Imagen oB=lee(imagen_original,2);

  

  int nfil=uR.fils();
  int ncol=uR.cols();

  vector<int> pos_negros;

  //Return a vector with the values where are holes.
  pos_negros = getPosNegros(uR, uG, uB);


  cout << "hay " << pos_negros.size() << " black pixels in the mask(holes)" << endl;


//We call the function which it will check what are the best candidates and will change it
checkCandidate(oR, oG, oB, pos_negros, ventana);



//writing output
char salida_completa[256];
  strcpy(salida_completa,salida_char);
  escribe(salida_completa,oR,oG,oB);



  return 0;
  
}
