#include<Solv.h>
/**********************************************************************
 * SOLVERC: resolucao do sistema linear(Ax=b)                         *
 * -------------------------------------------------------------------*
 * Parametro de entrada                                               *
 * -------------------------------------------------------------------*
 * m       -> vetor de memoria                                        *
 * neq     -> numero de equacoes                                      *
 * neqNov  -> numero de equacoes nao sobrepostas                      *
 * nad     -> numero de elemetos nao nulos fora da diagonal principal *
 * nAdR    -> numero de termos nao nulos na parte retangular         *
 * ia      -> vetor das linhas da estrutura de dados                  *
 * ja      -> vetor das colunas da estrutura de dados                 *
 * al      -> parte inferior da matriz a                              *
 * ad      -> diagonal principal da matriz a                          *
 * au      -> parte superior da matriz a                              *
 *  b      -> vetor de forcas                                         *
 * iNeq    -> mapa de interface de equacoes                           *
 *  x      -> vetor da solucao                                        *
 * tol     -> tolerancia do solver                                    *
 * maxit   -> numero maximo de iteracao do solver iterativo           *
 * storage -> tecnica de armazenamento da matriz esparsa              * 
 * solver  -> PCG                                                     *
 * fSolvLog-> arquivo de log para solver                              *
 * fLog    -> log de arquivo (true|false)                             *
 * newX    -> vetor inicial iniciado com zero                         *
 * openmp  -> flag do openmp true|false                               *
 * unsym   -> matriz nao simetrica                                    *
 * loopwise-                                                          *
 * -------------------------------------------------------------------*
 * Parametro de saida :                                               * 
 * -------------------------------------------------------------------*
 * x      - atualizado                                                *
 * b      - modificado                                                *
 * -------------------------------------------------------------------*
**********************************************************************/
void solverC(Memoria *m    
            ,INT const nEq       ,INT const nEqNov  
            ,INT const nAd       ,INT const nAdR
            ,INT *ia             ,INT *ja   
            ,DOUBLE *al          ,DOUBLE *ad,DOUBLE *au
            ,DOUBLE *b           ,DOUBLE *x
            ,Interface *iNeq       
            ,DOUBLE const tol    ,unsigned int maxIt
            ,short const storage ,short const solver
            ,FILE* fSolvLog      ,bool const fLog
            ,bool const newX     ,bool const openMp   
            ,bool const unSym    ,bool const loopWise)
{
  DOUBLE *z=NULL,*r=NULL,*pc=NULL;
  bool fCoo=false;
  void   (*matVecC)();
  DOUBLE (*dotC)();

  switch(solver){
/*gradientes conjugados com precondicionador diagonal*/
    case PCG:
/*... precondiconador diagonal*/
      HccaAlloc(DOUBLE,m,pc,nEqNov,"pc",false);
      zero(pc,nEqNov,DOUBLEC);
      tm.precondDiag = getTimeC() - tm.precondDiag;
      preCondDiag(pc,ad,nEqNov);
      tm.precondDiag = getTimeC() - tm.precondDiag;
/*...................................................................*/

/*... arranjos auxiliares do pcg*/
      HccaAlloc(DOUBLE,m,z,nEq,"z",false);
      zero(z,nEq,DOUBLEC);
      HccaAlloc(DOUBLE,m,r,nEq,"r",false);
      zero(r,nEq,DOUBLEC);
/*...................................................................*/
      
/*... estrutura de dados de armazenamentos da matriz esparcas*/
      switch(storage){
/*... armazenamento CSR(a)*/
        case CSR:
          matVecC = NULL;
        break;
/*...................................................................*/

/*... armazenamento CSRD(a,ad)*/
/*...CSRD+COO(symetric)*/
        case CSRDCOO:
          fCoo = true;
        case CSRD:
/*... nao - simetrica*/
          if(unSym)
            matVecC = matVecCsrD;
/*... simetrica*/
          else
/*... mpi*/
            if( mpiVar.nPrcs > 1){
/*... CSRD+COO*/
              if(fCoo) 
                matVecC = mpiMatVecCsrDcooSym;
/*... CSRD+CSR*/
              else 
                matVecC = mpiMatVecCsrDSym;
            }
/*..................................................................*/

/*...*/
            else
              matVecC = matVecCsrDSym;
/*..................................................................*/
        break;
/*...................................................................*/

/*... armazenamento CSRC(ad,au,al)*/
        case CSRC:
          matVecC = NULL;
        break;
/*...................................................................*/

/*... armazenamento ELLPACK(ad,a)*/
        case ELLPACK:
//        matVecC = matVecEllPack;
          matVecC = matVecEllPackO2;
//        matVecC = matVecEllPackO4;
        break;
/*...................................................................*/

/*...*/
        default:
          ERRO_OP(__FILE__,__func__,storage);
        break;
/*...................................................................*/
      }
/*...................................................................*/
      
/*...*/
      dotC    = dot;
//    dotC    = dotL2;
//    dotC    = dotL4;
//    dotC    = dotL6;
//    dotC    = dotL8;
//    dotC    = dotO2;
//    dotC    = dotO4;
//    dotC    = dotO6;
//    dotC    = dotO8;
//    dotC    = dotO2L2;
/*...................................................................*/

/*... gradientes conjugados*/
      tm.pcg = getTimeC() - tm.pcg;
/*... PCG-MPI*/
      if( mpiVar.nPrcs > 1)
        mpiPcg(nEq      ,nEqNov    
              ,nAd      ,nAdR
              ,ia       ,ja
              ,al       ,ad   ,au
              ,pc       ,b    ,x
              ,z        ,r    ,tol
              ,maxIt    ,true 
              ,fSolvLog ,fLog
              ,false 
              ,iNeq           
              ,matVecC ,dotC);   
/*...................................................................*/
      
/*... PCG*/
      else
        pcg(nEq         
           ,nAd
           ,ia      ,ja
           ,al      ,ad   ,au
           ,pc      ,b    ,x
           ,z       ,r    ,tol
           ,maxIt   ,true 
           ,fSolvLog,fLog
           ,false 
           ,matVecC ,dotC);   
/*...................................................................*/
      tm.pcg = getTimeC() - tm.pcg;
/*...................................................................*/
      
/*... liberando arranjos auxiliares do pcg*/
      HccaDealloc(m,r,"r",false);
      HccaDealloc(m,z,"z",false);
/*...................................................................*/

/*... liberando arranjos do precondicionador*/
      HccaDealloc(m,pc,"pc",false);
/*...................................................................*/
    break;
/*...................................................................*/

/*...*/
    default:
      ERRO_OP(__FILE__,__func__,solver);
    break;
/*...................................................................*/
 
  }
/*...................................................................*/
}
/*********************************************************************/      

/**********************************************************************
 *SETSOLVER : escolhe o solver                                        *
 **********************************************************************/
void setSolver(char *word,short *solver)
{

  if(!strcmp(word,"PCG"))
   *solver = PCG;

} 
/*********************************************************************/      


/**********************************************************************
 *SMACHN: calcula a precisao da maquina para double                   *
 **********************************************************************/
DOUBLE smachn(){
  
  DOUBLE mq = 1.e0;

  while(mq+1.e0 != 1.e0)
    mq/=2.e0;

  return mq;
}
/*********************************************************************/      

