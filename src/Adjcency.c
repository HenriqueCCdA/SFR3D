#include<Adjcency.h>
/*********************************************************************
 * VIZ : calcula os elementos vinzinhos                              *
 * ------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 * ------------------------------------------------------------------* 
 * m     -> memoria principal                                        *
 * el    -> conectividade nodal                                      *
 * nelcon-> indefinido                                               *
 * nViz  -> indefinido                                               *
 * nen   -> numero de nos por elemento                               *
 * nnode -> numero de nos da malha                                   *
 * numel -> numero de elmentos                                       *
 * maxNo -> numero maximo nos por elemento                           * 
 * maxViz-> numero maximo nos de vizinhos                            * 
 * ------------------------------------------------------------------*
 * Paramanetros de saida:                                            *
 * ------------------------------------------------------------------*
 * nelcon-> vizinhas dos elementos                                   *
 * nViz  -> numero de vizinhos por elemento                          *
 * ------------------------------------------------------------------*
 * *******************************************************************/
void viz(Memoria *m ,long *el    ,long *nelcon
        ,short *nViz,short *nen  ,long nnode
        ,long numel ,short maxNo ,short maxViz){
  
  long *nodcon,nEdge,i;
  
  Myalloc(long,m,nodcon      ,nnode ,"nodcon",_AD_);
  
  adj2d(el,nodcon,nelcon,nen,numel,nnode,maxNo,maxViz,&nEdge);
  
  for(i=0;i<numel;i++)
    nViz[i] = nen[i];
  
  Mydealloc(m,nodcon,"nodcon",_AD_);


}
/*********************************************************************/

/*********************************************************************
 * adj2D : calcula os elementos vinzinhos                            *
 * ------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 * ------------------------------------------------------------------* 
 * el    -> conectividade nodal                                      *
 * nodcon-> indefinido                                               *
 * nelcon-> indefinido                                               *
 * nen   -> numero de nos por elemento                               *
 * numel -> numero de elmentos                                       *
 * nnode -> numero de nos da malha                                   *
 * maxNo -> numero maximo nos por elemento                           * 
 * maxViz-> numero maximo nos de vizinhos                            * 
 * nEdge -> indefinifo                                               * 
 * ------------------------------------------------------------------*
 * Paramanetros de saida:                                            *
 * ------------------------------------------------------------------*
 * nelcon-> vizinhas dos elementos                                   *
 * nEdge -> numero de arestas                                        * 
 * ------------------------------------------------------------------*
 * *******************************************************************/
void adj2d(long *el  ,long *nodcon,long *nelcon,short *nen
          ,long numel, long nnode ,short maxNo , short maxViz
          ,long *nEdge){
 
 
  long  i,nel1,nel2,no1,no2,no21,no22;
  int   j,k;
  short is1,is2; 
  bool imiss;
  short int isnod3[][2]= {{0,1},{1,2},{2,0}};  
  short int isnod4[][2]= {{0,1},{1,2},{2,3},{3,0}};  
  
  for(nel1=0;nel1<numel;nel1++)
    for(j=0;j<maxViz;j++) 
      VET(nel1,j,nelcon,maxViz) = -1;
 
  for(i = 0;i < nnode;i++)
    nodcon[i] = -1; 
  
  *nEdge = 0;
  do{
    imiss = false;
    for(nel1=0;nel1 < numel;nel1++){ 
      is1 = nen[nel1];
      for(j=0;j<is1;j++){
        if(VET(nel1,j,nelcon,maxViz) == -1){
/*... triangulo*/
          if(is1 == 3) {
            no1 = VET(nel1,isnod3[j][0],el,maxNo)-1; 
            no2 = VET(nel1,isnod3[j][1],el,maxNo)-1; 
          }
/*...................................................................*/

/*... quadrilatero*/
          else if(is1 == 4){
            no1 = VET(nel1,isnod4[j][0],el,maxNo)-1; 
            no2 = VET(nel1,isnod4[j][1],el,maxNo)-1; 
          }
/*...................................................................*/
          if( nodcon[no1] == -1 || nodcon[no2] == -1){
            nodcon[no1] = nodcon[no2] = nel1;
            imiss       = true;
          }
        }
      }
    }
    
/*...*/ 
    for(nel1=0;nel1 < numel;nel1++){ 
      is1 = nen[nel1];
      for(j=0;j<is1;j++){
        if(VET(nel1,j,nelcon,maxViz) == -1){
/*... triangulo*/
          if(is1 == 3){
           no1  = VET(nel1,isnod3[j][0],el,maxNo)-1; 
           no2  = VET(nel1,isnod3[j][1],el,maxNo)-1;
          }
/*...................................................................*/ 

/*... quadrilateros*/
          else if(is1 == 4){
           no1  = VET(nel1,isnod4[j][0],el,maxNo)-1; 
           no2  = VET(nel1,isnod4[j][1],el,maxNo)-1;
          }
/*...................................................................*/ 
          nel2 = nodcon[no1];
          if(nel2 > -1){
            if(nel2 == nodcon[no2] && nel2 != nel1){
              is2 = nen[nel2];
/*... triangulos*/
              for(k=0;k<is2;k++){
/*... triangulo*/
                if(is2 == 3){
                  no21  = VET(nel2,isnod3[k][0],el,maxNo)-1; 
                  no22  = VET(nel2,isnod3[k][1],el,maxNo)-1;
                }
/*...................................................................*/

/*... tquadrilateros*/
                else if(is2 == 4){
                  no21  = VET(nel2,isnod4[k][0],el,maxNo)-1; 
                  no22  = VET(nel2,isnod4[k][1],el,maxNo)-1;
                }
/*...................................................................*/
                if( (no21 == no2) && (no22 == no1)){
                      VET(nel1,j,nelcon,maxViz) = nel2;
                      VET(nel2,k,nelcon,maxViz) = nel1;
                      nodcon[no1] = nodcon[no2] = -1;
                      imiss       = true; 
                      (*nEdge)++;
                 } 
                 
              }
/*...................................................................*/
            }
          } 
        }
      }
    }
/*...................................................................*/

    for(nel1=0;nel1 < numel;nel1++){ 
      is1 = nen[nel1];
      for(j=0;j<is1;j++){
        if(VET(nel1,j,nelcon,maxViz) == -1){
/*... triangulo*/
          if( is1 == 3){
            no1  = VET(nel1,isnod3[j][0],el,maxNo)-1; 
            no2  = VET(nel1,isnod3[j][1],el,maxNo)-1;
          }
/*...................................................................*/

/*... quadrilateros*/
          if( is1 == 4){
            no1  = VET(nel1,isnod4[j][0],el,maxNo)-1; 
            no2  = VET(nel1,isnod4[j][1],el,maxNo)-1;
          }
/*...................................................................*/
          if(nodcon[no1] == nodcon[no2] && nodcon[no1] == nel1){
            VET(nel1,j,nelcon,maxViz) = -2;
            nodcon[no1] = nodcon[no2]   =  -1;
            imiss       = true;
            (*nEdge)++;
          }
/*...................................................................*/
        }
      }
    }
/*...................................................................*/
  }while(imiss);  

/*...*/  
  for(nel1=0;nel1<numel;nel1++)
    for(j=0;j<maxViz;j++)
      VET(nel1,j,nelcon,maxViz) += 1;
/*...................................................................*/
 
}
/*********************************************************************/
