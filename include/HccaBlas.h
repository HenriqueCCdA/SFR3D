#ifndef _HCCABLAS_H_
  #define _HCCABLAS_H_

/*...*/
  #include<Mesh.h>
  #include<ParallelMpi.h>
  #include<Erro.h>  
  #include<HccaStdBool.h>
  #include<Define.h>
  #include<HccaTime.h>
  #include<OpenMp.h>
/*...................................................................*/

/*...*/
  long flopDot(INT nDim);
  void prodVet(DOUBLE *RESTRICT a
              ,DOUBLE *RESTRICT b
              ,DOUBLE *RESTRICT c);
/*...................................................................*/

/*... normas*/
  LDOUBLE lnormInf(LDOUBLE *RESTRICT x,INT const lin,INT const col);

/*...................................................................*/

/*======================== level 1 =================================*/
/*... soma de vetores*/
  void addVector(DOUBLE const alpha,DOUBLE *RESTRICT a
                ,DOUBLE const beta ,DOUBLE *RESTRICT b
                ,INT const nDim    ,DOUBLE *RESTRICT c);
/*... multiplicacao de vetor por um escalar*/  
  void  alphaProdVector(DOUBLE const alpha,DOUBLE *RESTRICT a
                       ,INT const nDim    ,DOUBLE *RESTRICT c);
/*...................................................................*/

/*... produto enterno (MPI)*/
  DOUBLE dot(DOUBLE *RESTRICT a,DOUBLE *RESTRICT b,INT const nDim);
  DOUBLE dotO2I2(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotI2(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotI4(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotI6(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotI8(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotO2(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotO4(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotO6(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotO8(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
/*...................................................................*/

/*... produto enterno (OPENMP)*/
  DOUBLE dotOmp(DOUBLE *RESTRICT a,DOUBLE *RESTRICT b,INT const nDim);
  DOUBLE dotOmpO2I2(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2
                   ,INT const n);
  DOUBLE dotOmpO2I4(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2
                   ,INT const n);
  DOUBLE dotOmpI2(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpI4(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpI6(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpI8(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpO2(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpO4(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpO6(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
  DOUBLE dotOmpO8(DOUBLE *RESTRICT x1,DOUBLE *RESTRICT x2,INT const n);
/*...................................................................*/
/*==================================================================*/

/*======================== level 2 =================================*/

/*level 2*/
/* ... matriz cheia*/
  void matVecFull(DOUBLE *RESTRICT a
                 ,DOUBLE *RESTRICT x
                 ,DOUBLE *RESTRICT y
                 ,INT nLin          ,INT nCol);
  void matVecFullO2(DOUBLE *RESTRICT a
                 ,DOUBLE *RESTRICT x
                 ,DOUBLE *RESTRICT y
                 ,INT nLin          ,INT nCol);
  void matVecFullO4(DOUBLE *RESTRICT a
                   ,DOUBLE *RESTRICT x
                   ,DOUBLE *RESTRICT y
                   ,INT nLin          ,INT nCol);
  void matVecFullO2I2(DOUBLE *RESTRICT a
                     ,DOUBLE *RESTRICT x
                     ,DOUBLE *RESTRICT y
                     ,INT nLin          ,INT nCol);
  void matVecFullO4I2(DOUBLE *RESTRICT a
                     ,DOUBLE *RESTRICT x
                     ,DOUBLE *RESTRICT y
                     ,INT nLin          ,INT nCol);
  void matVecFullO4I4(DOUBLE *RESTRICT a
                     ,DOUBLE *RESTRICT x
                     ,DOUBLE *RESTRICT y
                     ,INT nLin          ,INT nCol);
  void lmatVecFull(LDOUBLE *RESTRICT a
                  ,LDOUBLE *RESTRICT x
                  ,LDOUBLE *RESTRICT y
                  ,INT nLin       
                  ,INT nCol);
/*...................................................................*/


/*... Csr*/
  void matVecCsr(INT const neq           
                    ,INT *RESTRICT ia   ,INT *RESTRICT ja
                    ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                    ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y);
/*...................................................................*/

/*... CsrSym*/
  void matVecCsrSym(INT const neq           
                   ,INT *RESTRICT ia   ,INT *RESTRICT ja
                   ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                   ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y);
/*...................................................................*/

/*... CsrDSym*/
  void matVecCsrDSym(INT const neq           
                    ,INT *RESTRICT ia   ,INT *RESTRICT ja
                    ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                    ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y);

  void matVecCsrDSymI2(INT const neq           
                  ,INT *RESTRICT ia   ,INT *RESTRICT ja
                  ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                  ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y);

  void matVecCsrDSymO2(INT const neq           
                  ,INT *RESTRICT ia   ,INT *RESTRICT ja
                  ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                  ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y);
/*...................................................................*/

/*... CsrDSymOmp*/
  void matVecCsrDsymOmp(INT const nEq
                       ,INT *RESTRICT ia, INT *RESTRICT ja
                       ,DOUBLE *RESTRICT al, DOUBLE *RESTRICT ad
                       ,DOUBLE *RESTRICT x, DOUBLE *RESTRICT y
                       ,INT  *RESTRICT thBegin, INT *RESTRICT thEnd
                       ,INT  *RESTRICT thHeight
                       ,DOUBLE *RESTRICT thY, int const nThreads);
/*...................................................................*/

/*... CsrD*/ 
  void     matVecCsrD(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void   matVecCsrDI2(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void   matVecCsrDI4(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void   matVecCsrDI6(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void   matVecCsrDO2(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void   matVecCsrDO4(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void   matVecCsrDO6(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void matVecCsrDO2I2(INT const neq           
                     ,INT *RESTRICT ia  ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
/*...................................................................*/

/*... OPENMP - CsrD*/
  void matVecCsrDomp(INT const neq
                    ,INT *RESTRICT ia      ,INT *RESTRICT ja
                    ,DOUBLE *RESTRICT a    ,DOUBLE *RESTRICT ad
                    ,DOUBLE *RESTRICT x    ,DOUBLE *RESTRICT y
                    ,INT  *RESTRICT thBegin,INT *RESTRICT thEnd
                    ,INT  *RESTRICT thHeight
                    ,DOUBLE *ddum          ,int const nThreads);
/*...................................................................*/

/*... MPI - CsrD */ 
/*... Simetrico*/
/*... CSRD+CSR*/
  void mpiMatVecCsrDSym(INT const nEq    ,INT const *nAd 
                     ,INT *RESTRICT ia   ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y
                     ,Interface *iNeq);
/*...................................................................*/

/*... CSRD+COO*/
  void mpiMatVecCsrDcooSym(INT const nEq      ,INT const *nAd       
                          ,INT *RESTRICT ia   ,INT *RESTRICT ja
                          ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
                          ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y
                          ,Interface *iNeq);
/*...................................................................*/

/*... Geral*/
  void mpiMatVecCsrD(INT const neq   ,INT const *nAd 
                  ,INT *RESTRICT ia  ,INT *RESTRICT ja
                  ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                  ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y
                  ,Interface *iNeq);
/*...................................................................*/

/*... MPI+OPENMP - CsrD */ 
/*... Simetrico*/
/*... CSRD+CSR*/
  void mpiMatVecCsrDSymOmp(INT const nEq       ,INT const *nAd      
                     ,INT *RESTRICT ia       ,INT *RESTRICT ja
                     ,DOUBLE *RESTRICT al    ,DOUBLE *RESTRICT ad
                     ,DOUBLE *RESTRICT x     ,DOUBLE *RESTRICT y
                      ,INT  *RESTRICT thBegin,INT *RESTRICT thEnd
                     ,INT  *RESTRICT thHeight,DOUBLE *RESTRICT thY   
                     ,int const nThreads     ,Interface *iNeq);    
/*...................................................................*/

/*... CsrC */ 
/*... Simetrico*/
/*... ESTRUTURALMENTE SIMETRICA*/
  void matVecCsrC(INT const neq                       
                 ,INT *RESTRICT ia  ,INT *RESTRICT ja
                 ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                 ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
/*...................................................................*/

/*... CsrCsymOmp*/
  void matVecCsrComp(INT const nEq
                    ,INT *RESTRICT ia      ,INT *RESTRICT ja
                    ,DOUBLE *RESTRICT a    ,DOUBLE *RESTRICT ad
                    ,DOUBLE *RESTRICT x    ,DOUBLE *RESTRICT y
                    ,INT  *RESTRICT thBegin,INT *RESTRICT thEnd
                    ,INT  *RESTRICT thHeight
                    ,DOUBLE *RESTRICT thY, int const nThreads);
/*...................................................................*/

/*... CSRD+CSR*/
  void mpiMatVecCsrC(INT const nEq      ,INT const *nAd      
                    ,INT *RESTRICT ia   ,INT *RESTRICT ja
                    ,DOUBLE *RESTRICT a ,DOUBLE *RESTRICT ad
                    ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y
                    ,Interface *iNeq);
/*...................................................................*/

/*... CSRC+COO*/
  void mpiMatVecCsrCcoo(INT const nEq      ,INT const *nAd      
                       ,INT *RESTRICT ia   ,INT *RESTRICT ja
                       ,DOUBLE *RESTRICT a ,DOUBLE *RESTRICT ad
                       ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT y
                       ,Interface *iNeq);  
/*...................................................................*/

/*... CSRD+CSR*/
  void mpiMatVecCsrComp(INT const nEq     ,INT const *nAd      
                  ,INT *RESTRICT ia       ,INT *RESTRICT ja
                  ,DOUBLE *RESTRICT a     ,DOUBLE *RESTRICT ad
                  ,DOUBLE *RESTRICT x     ,DOUBLE *RESTRICT y
                  ,INT  *RESTRICT thBegin ,INT *RESTRICT thEnd
                  ,INT  *RESTRICT thHeight,DOUBLE *RESTRICT thY 
                  ,int const nThreads     ,Interface *iNeq);  
/*...................................................................*/

/*... EllPack*/ 
  void matVecEllPack(INT const nEq           
                  ,INT *RESTRICT ia  ,INT *RESTRICT ja
                  ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                  ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void matVecEllPackO2(INT const nEq           
                      ,INT *RESTRICT ia  ,INT *RESTRICT ja
                      ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                      ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
  void matVecEllPackO4(INT const nEq           
                      ,INT *RESTRICT ia  ,INT *RESTRICT ja
                      ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                      ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y);
/*... MPI*/
  void mpiMatVecEllPack(INT const nEq  ,INT const *nAd 
                      ,INT *RESTRICT ia  ,INT *RESTRICT ja
                      ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                      ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT y
                      ,Interface *iNeq);
/*...................................................................*/


/*==================================================================*/

/*======================== level 3 =================================*/
  void hccaDgemm(INT const ni,INT const nj,INT const nk
                ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT b
                ,DOUBLE *RESTRICT c);
/*==================================================================*/

#endif /*_HCCABLAS_H_*/
