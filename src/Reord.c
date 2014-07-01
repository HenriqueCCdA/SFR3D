#include<Reord.h>
/********************************************************************* 
 * REORD : reordanacao do grapho dos elementos                       * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * num   -> indefinido                                               * 
 * graph  -> grafo dos elementos                                     * 
 * numel -> numero de elementos                                      * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * num   -> elementos renumerados                                    * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *********************************************************************/
void reord(Memoria *m,long *num,long const *adj ,short const*nViz
          ,short const maxViz  ,long numel, bool flag){

  long *xAdj=NULL,*adjncy=NULL,*perm=NULL;
  long i,nDeg;

/*...*/  
  if(flag){
/*...*/
/*...................................................................*/

/*... armazena a malha no formato CSR*/
/*... calculo do vetor xAdj*/    
    Myalloc(long,m,xAdj  ,numel+1      ,"xAdj"  ,_AD_);
    convGraph(xAdj,adjncy,adj,nViz,maxViz,numel,true,false);
/*... calculo do vetor adjncy*/    
    nDeg = xAdj[numel] -xAdj[0];
    Myalloc(long,m,adjncy,nDeg,"adjncy",_AD_);
    convGraph(xAdj,adjncy,adj,nViz,maxViz,numel,false,true);
/*... ordena o grafo CSR em ordem crescente*/
    sortGraphCsr(xAdj,adjncy,numel);
/*... soma + 1 em todas as posicoes dos vetores*/    
    vectorPlusOne(xAdj,numel+1,i);
    vectorPlusOne(adjncy,nDeg,i);
/*...................................................................*/
//  for(i=0;i<numel+1;i++)
//    printf("%3ld %3ld\n",i+1,xAdj[i]); 
//  for(i=0;i<nDeg;i++)
//    printf("%3ld %3ld\n",i+1,adjncy[i]); 
/*...................................................................*/

/*...*/    
    Myalloc(long,m,perm  ,numel        ,"perm"  ,_AD_);
/*...................................................................*/

/*...*/    
    genrcm (numel,xAdj,adjncy,perm);
/*...................................................................*/
 
    
    for(i=0;i<numel;i++){
      num[i] = perm[i];
    }
    
/*...................................................................*/
//  for(i=0;i<numel;i++)
//    printf("%3ld %3ld %3ld\n",i+1,perm[i],num[i]); 
/*...................................................................*/

    Mydealloc(m,perm  ,"perm"  ,false);
    Mydealloc(m,adjncy,"adjncy",false);
    Mydealloc(m,adjncy,"xAdj"  ,false);
    
  } 
/*....................................................................*/  

/*... numeracao inicial*/  
  else
    for(i=0;i<numel;i++){
      num[i] = i+1;
    }
  
/*....................................................................*/  

}
/*********************************************************************/ 

