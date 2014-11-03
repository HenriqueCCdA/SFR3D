#include<HccaBlas.h>

/********************************************************************* 
 * FLOPDOT: Calculo do numero de flops do produto interno            * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * nDim -> numero de dimensoes                                       * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * Numero de flops                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
long flopDot(INT nDim){
  return (2*nDim-1);
}
/*********************************************************************/ 

/********************************************************************* 
 * FLOPMATVECCSR  : Calculo do numero de flops da operacao de matiz  * 
 * por um vetor, onde a matriz e uma matriz esparsa no formato CSR   * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * neq  -> numero de linhas                                          * 
 * nad  -> numero total de elementos nao nulos                       * 
 * ty   -> tipo do formato csr                                       * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * Numero de flops                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
INT flopMatVecCsr(INT neq,INT nad,short ty){
  
  INT flop;
  switch(ty){
/*... armazenamento CSR(a)*/
    case CSR:
      flop = 2*nad;
    break;
/*... armazenamento CSRD(a,ad)*/
    case CSRD:
      flop = 2*nad + neq;
    break;
/*... armazenamento CSRC(ad,au,al)*/
    case CSRC:
      flop = 4*nad + neq;
    break;
/*...*/
    default:
      ERRO_OP(__FILE__,__func__,ty);
    break;
  }
/*...................................................................*/
  return flop;
}
/*********************************************************************/ 


/********************************************************************* 
 * PRODVET: produto vetorial                                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * a  - vetor a                                                      * 
 * b  - vetor b                                                      * 
 * c  - indefinido                                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * c  - produto vetoria entre a e b                                  * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void prodVet(DOUBLE *restrict a,DOUBLE *restrict b, DOUBLE *restrict c)
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
}
/********************************************************************/ 

/*======================== level 1 =================================*/

/********************************************************************* 
 * ALPAHAPRODVECTOR : produto de vetor por um escalar                * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * alpha -> escalar que multiplica o vetor a                         * 
 * a     -> vetor a                                                  * 
 * nDim  -> dimensao dos vetores                                     * 
 * c  - indefinido                                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * c  - produto vetoria entre alpha*a                                * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void alphaProdVector(DOUBLE const alpha,DOUBLE *restrict a
                    ,INT const nDim    ,DOUBLE *restrict c) 

{
  INT i;
 
  for(i=0;i<nDim;i++)
    c[i] = alpha*a[i];

}
/*********************************************************************/ 

/********************************************************************* 
 * ADDVECTOR : adicao de vetores                                     * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * alpha -> escalar que multiplica o vetor a                         * 
 * a     -> vetor a                                                  * 
 * beto  -> escalar que multiplica o vetor b                         * 
 * b     -> vetor b                                                  * 
 * nDim  -> dimensao dos vetores                                     * 
 * c  - indefinido                                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * c  - produto vetoria entre alpha*a e beta*b                       * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void addVector(DOUBLE const alpha,DOUBLE *restrict a
              ,DOUBLE const beta ,DOUBLE *restrict b
              ,INT const nDim    ,DOUBLE *restrict c) 

{
  INT i;
 
  for(i=0;i<nDim;i++)
    c[i] = alpha*a[i] + beta*b[i];

}
/*********************************************************************/ 


/********************************************************************* 
 * DOT: produto interno entre dois vetores                           * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dot(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i;
  DOUBLE tmp=0.e0;
  for(i=0;i<n;i++)
    tmp += x1[i]*x2[i];
  return tmp;
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTO2L2: produto interno entre dois vetores                       * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotO2L2(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp1=0.e0,tmp2=0.e0;
  
  resto = n%4;
  if(resto == 1)
     tmp1 = x1[0]*x2[0]; 
  else if(resto == 2)
     tmp1 = x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp1 = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
    
  for(i=resto;i<n;i+=4){
    tmp1 += x1[i  ]*x2[i  ] + x1[i+1]*x2[i+1]; 
    tmp2 += x1[i+2]*x2[i+2] + x1[i+3]*x2[i+3]; 
  }
  return (tmp1+tmp2);
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTL2: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotL2(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp1=0.e0,tmp2=0.e0;
  resto = n%2;
  
  if(resto)
    tmp1 = x1[0]*x2[0];
    
  for(i=resto;i<n;i+=2){
    tmp1 += x1[i  ]*x2[i  ]; 
    tmp2 += x1[i+1]*x2[i+1];
  }
  return (tmp1+tmp2);
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTL4: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotL4(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp1=0.e0,tmp2=0.e0,tmp3=0.e0,tmp4=0.e0;
  
  
  resto = n%4;
  if(resto == 1)
     tmp1 = x1[0]*x2[0]; 
  else if(resto == 2)
     tmp1 = x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp1 = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
    
  
  for(i=resto;i<n;i+=4){
    tmp1 += x1[i  ]*x2[i  ];
    tmp2 += x1[i+1]*x2[i+1];
    tmp3 += x1[i+2]*x2[i+2];
    tmp4 += x1[i+3]*x2[i+3];
  }
  return (tmp1+tmp2+tmp3+tmp4);
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTL6: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotL6(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp1=0.e0,tmp2=0.e0,tmp3=0.e0,tmp4=0.e0;
  DOUBLE tmp5=0.e0,tmp6=0.e0;
  
  
  resto = n%6;
  if(resto == 1)
     tmp1= x1[0]*x2[0]; 
  else if(resto == 2)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 3)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 4)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2] + x1[3]*x2[3]; 
  else if(resto == 5)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4];               
  
  for(i=resto;i<n;i+=6){
    tmp1 += x1[i  ]*x2[i  ];
    tmp2 += x1[i+1]*x2[i+1];
    tmp3 += x1[i+2]*x2[i+2];
    tmp4 += x1[i+3]*x2[i+3];
    tmp5 += x1[i+4]*x2[i+4];
    tmp6 += x1[i+5]*x2[i+5];
  }
  return (tmp1+tmp2+tmp3+tmp4+tmp5+tmp6);
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTL8: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotL8(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp1=0.e0,tmp2=0.e0,tmp3=0.e0,tmp4=0.e0;
  DOUBLE tmp5=0.e0,tmp6=0.e0,tmp7=0.e0,tmp8=0.e0;
  
  
  resto = n%8;
  if(resto == 1)
     tmp1= x1[0]*x2[0]; 
  else if(resto == 2)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 3)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 4)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2] + x1[3]*x2[3]; 
  else if(resto == 5)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4];               
  else if(resto == 6)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4] + x1[5]*x2[5]; 
  else if(resto == 7)
     tmp1= x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4] + x1[5]*x2[5]  
         + x1[6]*x2[6];
  
  for(i=resto;i<n;i+=8){
    tmp1 += x1[i  ]*x2[i  ];
    tmp2 += x1[i+1]*x2[i+1];
    tmp3 += x1[i+2]*x2[i+2];
    tmp4 += x1[i+3]*x2[i+3];
    tmp5 += x1[i+4]*x2[i+4];
    tmp6 += x1[i+5]*x2[i+5];
    tmp7 += x1[i+6]*x2[i+6];
    tmp8 += x1[i+7]*x2[i+7];
  }
  return (tmp1+tmp2+tmp3+tmp4+tmp5+tmp6+tmp7+tmp8);
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTO2: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotO2(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp=0.e0;
  resto = n%2;
  
  if(resto)
    tmp = x1[0]*x2[0];
    
  for(i=resto;i<n;i+=2)
    tmp += x1[i]*x2[i] + x1[i+1]*x2[i+1];
  return tmp;
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTO4: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotO4(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp=0.e0;
  
  resto = n%4;
  if(resto == 1)
     tmp = x1[0]*x2[0]; 
  else if(resto == 2)
     tmp = x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
    
  
  for(i=resto;i<n;i+=4)
    tmp += x1[i  ]*x2[i  ] + x1[i+1]*x2[i+1] 
         + x1[i+2]*x2[i+2] + x1[i+3]*x2[i+3];
  return tmp;
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTO6: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotO6(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp=0.e0;
  
  resto = n%6;
  if(resto == 1)
     tmp = x1[0]*x2[0]; 
  else if(resto == 2)
     tmp = x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 4)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2] + x1[3]*x2[3];
  else if(resto == 5)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2] 
         + x1[3]*x2[3] + x1[4]*x2[4];
    
  
  for(i=resto;i<n;i+=6)
    tmp += x1[i  ]*x2[i  ] + x1[i+1]*x2[i+1] 
         + x1[i+2]*x2[i+2] + x1[i+3]*x2[i+3] 
         + x1[i+4]*x2[i+4] + x1[i+5]*x2[i+5];
  return tmp;
} 
/*********************************************************************/ 

/********************************************************************* 
 * DOTO8: produto interno entre dois vetores                         * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * x1 - vetor x1                                                     * 
 * x2 - vetor x2                                                     * 
 * n  - numero de dimensoes                                          * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
DOUBLE dotO8(DOUBLE *restrict x1,DOUBLE *restrict x2,INT const n)
{
  INT i,resto;
  DOUBLE tmp=0.e0;
  
  resto = n%8;
  if(resto == 1)
     tmp = x1[0]*x2[0]; 
  else if(resto == 2)
     tmp = x1[0]*x2[0] + x1[1]*x2[1]; 
  else if(resto == 3)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 3)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2]; 
  else if(resto == 4)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] + x1[2]*x2[2] + x1[3]*x2[3]; 
  else if(resto == 5)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4];               
  else if(resto == 6)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4] + x1[5]*x2[5]; 
  else if(resto == 7)
     tmp = x1[0]*x2[0] + x1[1]*x2[1] 
         + x1[2]*x2[2] + x1[3]*x2[3]  
         + x1[4]*x2[4] + x1[5]*x2[5]  
         + x1[6]*x2[6];

  for(i=resto;i<n;i+=8)
    tmp += x1[i  ]*x2[i  ] + x1[i+1]*x2[i+1] 
         + x1[i+2]*x2[i+2] + x1[i+3]*x2[i+3]
         + x1[i+4]*x2[i+4] + x1[i+5]*x2[i+5]
         + x1[i+6]*x2[i+6] + x1[i+7]*x2[i+7];
  return tmp;
} 
/*********************************************************************/ 

/*==================================================================*/

/*======================== level 2 =================================*/

/********************************************************************* 
 * MATVECCSRDSYM:produto matriz vetor para uma matriz no formato CSRD* 
 * (y=Ax, A uma matriz simentrica)                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * neq -> numero de equacoes                                         * 
 * ia  -> vetor csr                                                  * 
 * ja  -> vetor csr                                                  * 
 * al  -> vetor com os valores inferiores da matriz                  * 
 * ad  -> vetor com os valores da diagonal principal da matriz       * 
 * x   -> vetor a ser multiplicado                                   * 
 * y   -> indefinido                                                 * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * y   -> vetor com o resultado da multiplicacao                     * 
 *-------------------------------------------------------------------* 
 * OBS: ja guarda os indiceis da para inferior da matriz             * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void matVecCsrDSym(INT const neq           
                  ,INT *restrict ia   ,INT *restrict ja
                  ,DOUBLE *restrict al,DOUBLE *restrict ad
                  ,DOUBLE *restrict x ,DOUBLE *restrict y)
{
  INT    i,k,jak;
  DOUBLE xi,tmp,sAu;
  
  y[0] = ad[0]*x[0]; 
  for(i=1;i<neq;i++){
    xi  = x[i];
    tmp = ad[i]*xi;
    for(k=ia[i];k<ia[i+1];k++){
      jak = ja[k];
      sAu = al[k];
/*... produto da linha i pelo vetor x*/
      tmp    += sAu*x[jak];
/*... produto dos coef. da parte superior da matriz por x(i)*/
      y[jak] += sAu*xi;
       
    }
/*... armazena o resultado em y(i) */
    y[i] = tmp;
  }
} 
/*********************************************************************/ 
/*==================================================================*/

/*======================== level 3 =================================*/
/*==================================================================*/
