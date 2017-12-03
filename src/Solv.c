#include<Solv.h>
/**********************************************************************
 * Data de criacao    : 00/00/0000                                    *
 * Data de modificaco : 27/08/2016                                    *
 * -------------------------------------------------------------------*
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
 * bOmp    -> Openmp                                                  *
 *  x      -> vetor da solucao                                        *
 * tol     -> tolerancia do solver                                    *
 * maxit   -> numero maximo de iteracao do solver iterativo           *
 * storage -> tecnica de armazenamento da matriz esparsa              * 
 * solver  -> PCG                                                     *
 * fSolvLog-> arquivo de log para solver                              *
 * fLog    -> log de arquivo (true|false)                             *
 * newX    -> vetor inicial iniciado com zero                         *
 * unsym   -> matriz nao simetrica                                    *
 * -------------------------------------------------------------------*
 * Parametro de saida :                                               * 
 * -------------------------------------------------------------------*
 * x      - atualizado                                                *
 * b      - modificado                                                *
 * -------------------------------------------------------------------*
 * OBS: csrD e csrC sao iguais para matrizes simetricas               *
 **********************************************************************/
void solverC(Memoria *m    
            ,INT const nEq       ,INT const nEqNov  
            ,INT const nAd       ,INT const nAdR
            ,INT *ia             ,INT *ja   
            ,DOUBLE *al          ,DOUBLE *ad,DOUBLE *au
            ,DOUBLE *b           ,DOUBLE *x
            ,Interface *iNeq     ,BufferOmp *bOmp
            ,DOUBLE const tol    ,unsigned int maxIt
            ,short const storage ,short const solver
            ,FILE* fSolvLog      ,bool const fLog
            ,bool const newX     ,bool const unSym    )
{
  DOUBLE *z=NULL,*r=NULL,*pc=NULL,*t=NULL,*v=NULL,*p=NULL,*h=NULL
        ,*i=NULL,*o=NULL,*s=NULL,*d=NULL;
  void   (*matVecC)();
  DOUBLE (*dotC)();
  INT nn;
  unsigned short nKrylov=50;
  bool fPrint = false,openMp = ompVar.fSolver;
/*...*/
	dotC    = NULL;
	matVecC = NULL;
/*...................................................................*/

  switch(solver){
/*... gradientes conjugados com precondicionador diagonal*/
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
      HccaAlloc(DOUBLE,m,r,nEq,"r",false);
			HccaAlloc(DOUBLE,m,p,nEq,"p",false);
      zero(z,nEq,DOUBLEC);
      zero(r,nEq,DOUBLEC);
			zero(p,nEq, DOUBLEC);
/*...................................................................*/
      
/*...*/
			setMatVec(&matVecC,storage,unSym,openMp);
/*...................................................................*/

/*...*/
			setDot(&dotC, DOT);
/*...................................................................*/

/*... gradientes conjugados*/
      tm.pcg = getTimeC() - tm.pcg;
      callCg(nEq        ,nEqNov
             ,nAd        ,nAdR
             ,ia         ,ja
             ,al         ,ad
             ,pc         ,b   
             ,x          ,z      
             ,r          ,p
             ,tol
             ,maxIt      ,newX
             ,fSolvLog   ,fLog
             ,fPrint     
             ,iNeq       ,bOmp
             ,matVecC    ,dotC);
/*...................................................................*/
      tm.pcg = getTimeC() - tm.pcg;
/*...................................................................*/
      
/*... liberando arranjos auxiliares do pcg*/
			HccaDealloc(m,p,"p",false);
      HccaDealloc(m,r,"r",false);
      HccaDealloc(m,z,"z",false);
/*...................................................................*/

/*... liberando arranjos do precondicionador*/
      HccaDealloc(m,pc,"pc",false);
/*...................................................................*/
    break;
/*...................................................................*/

/*... gradientes conjugados bi-ortoganilizado com precondicionador
     diagonal*/
    case PBICGSTAB:
/*... precondiconador diagonal*/
      HccaAlloc(DOUBLE,m,pc,nEqNov,"pc",false);
      zero(pc,nEqNov,DOUBLEC);
      tm.precondDiag = getTimeC() - tm.precondDiag;
      preCondDiag(pc,ad,nEqNov);
      tm.precondDiag = getTimeC() - tm.precondDiag;
/*...................................................................*/

/*... arranjos auxiliares do pbicgstab*/
      HccaAlloc(DOUBLE,m,z,nEq,"z",false);
      HccaAlloc(DOUBLE,m,r,nEq,"r",false);
      HccaAlloc(DOUBLE,m,t,nEq,"tt",false);
      HccaAlloc(DOUBLE,m,v,nEq,"vv",false);
      HccaAlloc(DOUBLE,m,p,nEq,"pp",false);
			HccaAlloc(DOUBLE,m,h,nEq,"hh", false);
      zero(z,nEq,DOUBLEC);
      zero(r,nEq,DOUBLEC);
      zero(t,nEq,DOUBLEC);
      zero(v,nEq,DOUBLEC);
      zero(p,nEq,DOUBLEC);
			zero(h,nEq,DOUBLEC);
/*...................................................................*/

/*...*/
			setMatVec(&matVecC, storage, unSym,openMp);
/*...................................................................*/
      
/*...*/
			setDot(&dotC,DOT);
/*...................................................................*/

/*... gradientes conjugados bi-ortoganilizado*/
      tm.pbicgstab = getTimeC() - tm.pbicgstab;
/*...*/
      callBicgStab(nEq   ,nEqNov
                 ,nAd    ,nAdR
                 ,ia     ,ja
                 ,al     ,ad    
                 ,pc     ,b     
                 ,x      ,t  
                 ,v      ,r
                 ,p      ,z
                 ,h 
                 ,tol    ,maxIt 
                 ,newX   ,fSolvLog
                 ,fLog   ,fPrint
                 ,iNeq   ,bOmp           
                 ,matVecC,dotC);  
/*...................................................................*/

/*...*/
      tm.pbicgstab = getTimeC() - tm.pbicgstab;
/*...................................................................*/
      
/*... liberando arranjos auxiliares do pbicgstab*/
			HccaDealloc(m,h,"hh",false);
      HccaDealloc(m,p,"pp",false);
      HccaDealloc(m,v,"vv",false);
      HccaDealloc(m,t,"tt",false);
      HccaDealloc(m,r,"r" ,false);
      HccaDealloc(m,z,"z" ,false);
/*...................................................................*/

/*... liberando arranjos do precondicionador*/
      HccaDealloc(m,pc,"pc",false);
/*...................................................................*/
    break;
/*...................................................................*/

/*... gradientes conjugados bi-ortoganilizado com precondicionador
      diagonal*/
    case PBICGSTABL2:
/*... precondiconador diagonal*/
      HccaAlloc(DOUBLE, m, pc, nEqNov, "pc", false);
      zero(pc, nEqNov, DOUBLEC);
      tm.precondDiag = getTimeC() - tm.precondDiag;
      preCondDiag(pc, ad, nEqNov);
      tm.precondDiag = getTimeC() - tm.precondDiag;
/*...................................................................*/

/*... arranjos auxiliares do pbicgstab*/
      HccaAlloc(DOUBLE, m, z, nEq, "z", false);
      HccaAlloc(DOUBLE, m, r, nEq, "r", false);
      HccaAlloc(DOUBLE, m, t, nEq, "tt", false);
      HccaAlloc(DOUBLE, m, v, nEq, "vv", false);
      HccaAlloc(DOUBLE, m, p, nEq, "pp", false);
      HccaAlloc(DOUBLE, m, h, nEq, "hh", false);
      HccaAlloc(DOUBLE, m, i, nEq, "ii", false);
      HccaAlloc(DOUBLE, m, o, nEq, "oo", false);
      HccaAlloc(DOUBLE, m, s, nEq, "ss", false);
      HccaAlloc(DOUBLE, m, d, nEq, "dd", false);
      zero(z, nEq, DOUBLEC);
      zero(r, nEq, DOUBLEC);
      zero(t, nEq, DOUBLEC);
      zero(v, nEq, DOUBLEC);
      zero(p, nEq, DOUBLEC);
      zero(h, nEq, DOUBLEC);
      zero(i, nEq, DOUBLEC);
      zero(o, nEq, DOUBLEC);
      zero(s, nEq, DOUBLEC);
      zero(d, nEq, DOUBLEC);
/*...................................................................*/

/*...*/
      setMatVec(&matVecC, storage, unSym, openMp);
/*...................................................................*/

/*...*/
      setDot(&dotC, DOT);
/*...................................................................*/

/*... gradientes conjugados bi-ortoganilizado*/
      tm.pbicgstab = getTimeC() - tm.pbicgstab;
/*...*/
      callBicgStabl2(nEq  , nEqNov
                    ,nAd ,nAdR
                    ,ia  ,ja
                    ,al  ,ad
                    ,pc  ,b
                    ,x   ,t
                    ,v   ,r
                    ,p   ,z
                    ,h   ,i
                    ,o   ,s
                    ,d     
                    ,tol , maxIt
                    ,newX, fSolvLog
                    ,fLog, fPrint
                    ,iNeq, bOmp
                    ,matVecC, dotC);
/*...................................................................*/

/*...*/
      tm.pbicgstab = getTimeC() - tm.pbicgstab;
/*...................................................................*/

/*... liberando arranjos auxiliares do pbicgstab*/
      HccaDealloc(m, d, "dd", false);
      HccaDealloc(m, s, "ss", false);
      HccaDealloc(m, o, "oo", false);
      HccaDealloc(m, i, "ii", false);
      HccaDealloc(m, h, "hh", false);
      HccaDealloc(m, p, "pp", false);
      HccaDealloc(m, v, "vv", false);
      HccaDealloc(m, t, "tt", false);
      HccaDealloc(m, r, "r", false);
      HccaDealloc(m, z, "z", false);
/*...................................................................*/

/*... liberando arranjos do precondicionador*/
      HccaDealloc(m, pc, "pc", false);
/*...................................................................*/
    break;
/*...................................................................*/

/*... GMRES*/
    case GMRES:
/*... precondiconador diagonal*/
      HccaAlloc(DOUBLE, m, pc, nEqNov, "pc", false);
      zero(pc, nEqNov, DOUBLEC);
/*...*/
      tm.precondDiag = getTimeC() - tm.precondDiag;
      preCondDiag(pc, ad, nEqNov);
      tm.precondDiag = getTimeC() - tm.precondDiag;
/*...................................................................*/

/*... arranjos auxiliares do pbicgstab*/
      nn = nEq*(nKrylov+1);
      HccaAlloc(DOUBLE, m, z, nn, "gg", false);
      zero(z, nn, DOUBLEC);
      
      nn = nKrylov*(nKrylov + 1);
      HccaAlloc(DOUBLE, m, h, nn,"hh", false);
      zero(h, nn, DOUBLEC);
      
      nn = nKrylov+1;
      HccaAlloc(DOUBLE, m, p, nn, "ee", false);
      zero(p, nn, DOUBLEC);
      
      nn = nKrylov;
      HccaAlloc(DOUBLE,m,t,nn,"cc", false);
      HccaAlloc(DOUBLE,m,v,nn,"ss", false);
      HccaAlloc(DOUBLE,m,d,nn,"yy", false);
      zero(t,nn,DOUBLEC);
      zero(v,nn,DOUBLEC);
      zero(d,nn,DOUBLEC);
/*...................................................................*/

/*...*/
      setMatVec(&matVecC, storage, unSym, openMp);
/*...................................................................*/

/*...*/
      setDot(&dotC, DOT);
/*...................................................................*/

/*... Gmres*/
      tm.gmres = getTimeC() - tm.gmres;
/*...*/
      callGmres(nEq    ,nEqNov
               ,nAd    ,nAdR
               ,ia     ,ja
               ,al     ,ad
               ,pc     ,b
               ,x      ,z
               ,h      ,d
               ,t      ,v
               ,p      ,nKrylov
               ,tol    ,maxIt
               ,newX   ,fSolvLog
               ,fLog   ,fPrint
               ,iNeq   ,bOmp
               ,matVecC,dotC);
/*...................................................................*/

/*...*/
      tm.gmres = getTimeC() - tm.gmres;
/*...................................................................*/

/*... liberando arranjos auxiliares do pbicgstab*/
       HccaDealloc(m, d, "yy", false);
       HccaDealloc(m, v, "ss", false);
       HccaDealloc(m, t, "cc", false);
       HccaDealloc(m, p, "ee", false);
       HccaDealloc(m, h, "hh", false);
       HccaDealloc(m, z, "gg", false);
/*...................................................................*/

/*... liberando arranjos do precondicionador*/
      HccaDealloc(m, pc, "pc", false);
/*...................................................................*/
    break;
/*...................................................................*/ 

/*... MINRES com precondicionador diagonal*/
    case MINRES:
/*... precondiconador diagonal*/
      HccaAlloc(DOUBLE,m,pc,nEqNov,"pc",false);
      zero(pc,nEqNov,DOUBLEC);
      tm.precondDiag = getTimeC() - tm.precondDiag;
      preCondDiag(pc,ad,nEqNov);
      tm.precondDiag = getTimeC() - tm.precondDiag;
/*...................................................................*/

/*... arranjos auxiliares do pbicgstab*/
      HccaAlloc(DOUBLE,m,z,nEq,"z",false);
      HccaAlloc(DOUBLE,m,r,nEq,"r",false);
      HccaAlloc(DOUBLE,m,t,nEq,"tt",false);
      HccaAlloc(DOUBLE,m,v,nEq,"vv",false);
      HccaAlloc(DOUBLE,m,p,nEq,"pp",false);
			HccaAlloc(DOUBLE,m,h,nEq,"hh", false);
      HccaAlloc(DOUBLE,m,o,nEq,"oo", false);
      HccaAlloc(DOUBLE,m,d,nEq,"dd", false);
      zero(z,nEq,DOUBLEC);
      zero(r,nEq,DOUBLEC);
      zero(t,nEq,DOUBLEC);
      zero(v,nEq,DOUBLEC);
      zero(p,nEq,DOUBLEC);
			zero(h,nEq,DOUBLEC);
      zero(o,nEq,DOUBLEC);
      zero(d,nEq,DOUBLEC);
/*...................................................................*/

/*...*/
			setMatVec(&matVecC, storage, unSym,openMp);
/*...................................................................*/
      
/*...*/
			setDot(&dotC,DOT);
/*...................................................................*/

/*... gradientes conjugados bi-ortoganilizado*/
      tm.minres = getTimeC() - tm.minres;
/*...*/
      callMinRes(nEq    , nEqNov
                ,nAd    , nAdR
                ,ia     , ja
                ,al     , ad    
                ,pc     , b     
                ,x      , t  
                ,v      , r
                ,p      , z
                ,h      , o
                ,d
                ,tol    , maxIt 
                ,newX   , fSolvLog
                ,fLog   , fPrint
                ,iNeq   , bOmp           
                ,matVecC, dotC);  
/*...................................................................*/

/*...*/
      tm.minres = getTimeC() - tm.minres;
/*...................................................................*/
      
/*... liberando arranjos auxiliares do pbicgstab*/
      HccaDealloc(m,d,"dd",false);
      HccaDealloc(m,o,"oo",false);
			HccaDealloc(m,h,"hh",false);
      HccaDealloc(m,p,"pp",false);
      HccaDealloc(m,v,"vv",false);
      HccaDealloc(m,t,"tt",false);
      HccaDealloc(m,r,"r" ,false);
      HccaDealloc(m,z,"z" ,false);
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
  else if(!strcmp(word,"PBICGSTAB"))
    *solver = PBICGSTAB;
  else if (!strcmp(word, "PBICGSTABL2"))
    *solver = PBICGSTABL2;
  else if (!strcmp(word, "GMRES"))
    *solver = GMRES;
 else if (!strcmp(word, "MINRES"))
    *solver = MINRES;

} 
/*********************************************************************/      

/**********************************************************************
 *SETSOLVER : escolhe o solver                                        *
 **********************************************************************/
void setSolverConfig(char *word,Solv *solv,FILE *fileIn)
{
/*...*/
  if(!strcmp(word,"PCG")){
    fscanf(fileIn,"%u" ,&solv->maxIt);
    fscanf(fileIn,"%lf",&solv->tol);
    solv->solver = PCG;   

    if(solv->tol == 0.e0) 
      solv->tol = smachn();

    if(!mpiVar.myId ) {
      printf("Solver    : PCG\n");
      printf("MaxIt     : %d\n",solv->maxIt);
      printf("Tol       : %e\n",solv->tol);
    }
  }
/*...................................................................*/

/*...*/
  else if(!strcmp(word,"PBICGSTAB")){
    fscanf(fileIn,"%u" ,&solv->maxIt);
    fscanf(fileIn,"%lf",&solv->tol);
    solv->solver = PBICGSTAB; 

    if(solv->tol == 0.e0) 
      solv->tol = smachn(); 

    if(!mpiVar.myId ) {
      printf("Solver    : PBICGSTAB\n");
      printf("MaxIt     : %d\n",solv->maxIt);
      printf("Tol       : %e\n",solv->tol);
    }
  }
/*...................................................................*/

/*...*/
  else if (!strcmp(word, "PBICGSTABL2")){
    fscanf(fileIn,"%u" ,&solv->maxIt);
    fscanf(fileIn,"%lf",&solv->tol);
    solv->solver = PBICGSTABL2; 
 
    if(solv->tol == 0.e0) 
      solv->tol = smachn();

    if(!mpiVar.myId ) {
      printf("Solver    : PBICGSTABL2\n");
      printf("MaxIt     : %d\n",solv->maxIt);
      printf("Tol       : %e\n",solv->tol);
    }
  }
/*...................................................................*/

/*...*/
  else if (!strcmp(word, "GMRES")){
    fscanf(fileIn,"%u" ,&solv->maxIt);
    fscanf(fileIn,"%lf",&solv->tol);
    solv->solver = GMRES;  
 
    if(solv->tol == 0.e0) 
      solv->tol = smachn();

    if(!mpiVar.myId ) {
      printf("Solver    : GMRES\n");
      printf("MaxIt     : %d\n",solv->maxIt);
      printf("Tol       : %e\n",solv->tol);
    }
  }
/*...................................................................*/

/*...*/
  else if(!strcmp(word,"MINRES")){
    fscanf(fileIn,"%u" ,&solv->maxIt);
    fscanf(fileIn,"%lf",&solv->tol);
    solv->solver = MINRES;   

    if(solv->tol == 0.e0) 
      solv->tol = smachn();

    if(!mpiVar.myId ) {
      printf("Solver    : MINRES\n");
      printf("MaxIt     : %d\n",solv->maxIt);
      printf("Tol       : %e\n",solv->tol);
    }
  }
/*...................................................................*/  

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

/**********************************************************************
* Data de criacao :    22 / 07 / 2016                                 *
* Data de modificaco : 00 / 00 / 0000  															  *
* ------------------------------------------------------------------- *
* SETDOT :escolhe o produto interno                                   *
* ------------------------------------------------------------------- *
* Parametro de entrada                                                *
* ------------------------------------------------------------------- *
* dotC -> -> nao definido                                             *
* iCod -> versao do produto interno  																	*
* ------------------------------------------------------------------- *
* Parametro de saida :																							  *
* ------------------------------------------------------------------- *
* dotC -> funcao escolhida                                            *
* ------------------------------------------------------------------- *
* OBS :                                                               *
**********************************************************************/
void setDot(DOUBLE(**dotC)(),short const iCod) {

/*...*/
	switch(iCod){
		case DOT:
      if(ompVar.fSolver)
        *dotC = dotOmp;
      else
			  *dotC = dot;
			break;
		case DOTI2:
      if (ompVar.fSolver)
        *dotC = dotOmpI2;
      else
	      *dotC = dotI2;
			break;
		case DOTI4:
      if (ompVar.fSolver)
        *dotC = dotOmpI4;
      else
	      *dotC = dotI4;
			break;
		case DOTI6:
      if (ompVar.fSolver)
        *dotC = dotOmpI6;
      else
	      *dotC = dotI6;
			break;
		case DOTI8:
      if (ompVar.fSolver)
        *dotC = dotOmpI8;
      else
	      *dotC = dotI8;
			break;
		case DOTO2:
      if (ompVar.fSolver)
        *dotC = dotOmpO2;
      else
	      *dotC = dotO2;
			break;
		case DOTO4:
      if (ompVar.fSolver)
        *dotC = dotOmpO4;
      else
	      *dotC = dotO4;
			break;
		case  DOTO6:
      if (ompVar.fSolver)
        *dotC = dotOmpO6;
      else
	      *dotC = dotO6;
			break;
		case  DOTO8:
      if (ompVar.fSolver)
        *dotC = dotOmpO8;
      else
	      *dotC = dotO8;
			break;
		case  DOTO2I2:
      if (ompVar.fSolver)
        *dotC = dotOmpO2I2;
      else
	      *dotC = dotO2I2;
		  break;
    default:
			ERRO_OP(__FILE__, __func__, iCod);
		break;	
	}
/*...................................................................*/
}
/*********************************************************************/

/**********************************************************************
* Data de criacao :    22 / 07 / 2016                                 *
* Data de modificaco : 27 / 08 / 2016  															  *
* ------------------------------------------------------------------- *
* SETMATVEC: escolhe o produto matriz vetor desejado                  *
* ------------------------------------------------------------------- *
* Parametro de entrada                                                *
* ------------------------------------------------------------------- *
* matVecC -> nao definido                                             *
* storage -> tecnica de armazenamento																	*
* unSym   -> matriz nao-simetrica (true:false 												*
* ------------------------------------------------------------------- *
* Parametro de saida :																							  *
* ------------------------------------------------------------------- *
* matVecC -> funcao escolhida                                         *
* ------------------------------------------------------------------- *
* OBS :                                                               *
**********************************************************************/
void setMatVec(void (**matVecC)(),short const storage
              ,bool const unSym  ,bool const openMp) {
	
	bool fCoo = false;
/*... estrutura de dados de armazenamentos da matriz esparcas*/
	switch (storage) {
/*... armazenamento CSR(a)*/
	case CSR:
		*matVecC = NULL;
		break;
/*...................................................................*/

/*... armazenamento CSRD(a,ad)*/
/*...CSRD+COO(symetric)*/
	case CSRDCOO:
		fCoo = true;
	case CSRD:
/*... nao - simetrica*/
		if (unSym) {
/*... mpi*/
			if (mpiVar.nPrcs > 1) {
				*matVecC = mpiMatVecCsrD;
			}
/*...................................................................*/

/*... sequencial*/
			else {
/*... Omp*/
        if (ompVar.fSolver)
          *matVecC = matVecCsrDomp;
/*... sequencial*/
        else
          *matVecC = matVecCsrD;
			}
/*...................................................................*/
		}
/*...................................................................*/

/*... simetrica*/
		else {
/*... mpi*/
			if (mpiVar.nPrcs > 1) {
/*... CSRD+COO*/
				if (fCoo) {
					*matVecC = mpiMatVecCsrDcooSym;
				}
/*...................................................................*/

/*... CSRD+CSR*/
				else {
					*matVecC = mpiMatVecCsrDSym;
/*...................................................................*/
				}
			}
/*..................................................................*/

/*... sequencial*/
			else {
/*... Omp*/
        if (ompVar.fSolver)
          *matVecC = matVecCsrDsymOmp;
/*... sequencial*/
        else
  				*matVecC = matVecCsrDSym;
			}
/*..................................................................*/
		}
/*..................................................................*/
		break;
/*...................................................................*/

/*... armazenamento CSRC(ad,au,al)*/
/*...CSRC+COO*/
	case CSRCCOO:
		fCoo = true;
	case CSRC:
/*... nao - simetrica*/
		if (unSym) {
/*... mpi*/
			if (mpiVar.nPrcs > 1) {
/*... CSRC+COO*/
				if (fCoo) {
					*matVecC = mpiMatVecCsrCcoo;
				}
/*...................................................................*/

/*... CSRC+CSR*/
				else {
					*matVecC = mpiMatVecCsrC;
/*...................................................................*/
				}
			}
/*..................................................................*/
		
/*... sem MPI*/
			else {
/*... Omp*/
        if (ompVar.fSolver)
          *matVecC = matVecCsrComp;
/*... sequencial*/
        else
				  *matVecC = matVecCsrC;
			}
/*..................................................................*/
    }		
/*..................................................................*/

/*... simetrica*/
		else {
/*... mpi*/
			if (mpiVar.nPrcs > 1) {
/*... CSRD+COO*/
				if (fCoo) {
					*matVecC = mpiMatVecCsrDcooSym;
				}
/*...................................................................*/

/*... CSRD+CSR*/
				else {
					*matVecC = mpiMatVecCsrDSym;
/*...................................................................*/
				}
			}
/*..................................................................*/

/*... sequencial*/
			else {
        if(ompVar.fSolver)
          *matVecC = matVecCsrDsymOmp;
        else
				  *matVecC = matVecCsrDSym;
			}
/*..................................................................*/
		}
/*..................................................................*/
  break;
/*...................................................................*/

/*... armazenamento ELLPACK(ad,a)*/
	case ELLPACK:
		/*... mpil*/
		if (mpiVar.nPrcs > 1) {
			*matVecC = mpiMatVecEllPack;
		}
/*...................................................................*/

/*... sequencial*/
		else {
			*matVecC = matVecEllPack;
			//          matVecC = matVecEllPackO2;
			//          matVecC = matVecEllPackO4;
		}
/*...................................................................*/
		break;
/*...................................................................*/

/*...*/
	default:
		ERRO_OP(__FILE__, __func__, storage);
		break;
/*...................................................................*/
	}
/*...................................................................*/

}
/*********************************************************************/

/**********************************************************************
* Data de criacao    : 27/08/2016                                    *
* Data de modificaco : 00/00/0000                                    *
* -------------------------------------------------------------------*
* CALLCG : chama o gradiente conjugados escolhido                    *
* -------------------------------------------------------------------*
* Parametro de entrada                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* Parametro de saida :                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* OBS:                                                               *
**********************************************************************/
void callCg(INT const nEq      ,INT const nEqNov
            ,INT const nAd      ,INT const nAdR
            ,INT *RESTRICT ia   ,INT *RESTRICT ja
            ,DOUBLE *RESTRICT al,DOUBLE *RESTRICT ad
            ,DOUBLE *RESTRICT m ,DOUBLE *RESTRICT b 
            ,DOUBLE *RESTRICT x ,DOUBLE *RESTRICT z
            ,DOUBLE *RESTRICT r ,DOUBLE *RESTRICT p
            ,DOUBLE const tol   ,unsigned int maxIt 
            ,bool const newX    ,FILE* fSolvLog     
            ,bool const fLog    ,bool const fPrint  
            ,Interface *iNeq    ,BufferOmp *bOmp
            ,void(*matVec)()    ,DOUBLE(*dot)())
{

/*... PCG-MPI*/
  if (mpiVar.nPrcs > 1)
    mpiPcg(nEq     ,nEqNov
          ,nAd     ,nAdR
          ,ia      ,ja
          ,al      ,ad 
          ,m       ,b  
          ,x       ,z
          ,r 
          ,tol     ,maxIt
          ,newX    ,fSolvLog
          ,fLog    ,fPrint
          ,iNeq
          ,matVec  ,dot);
/*...................................................................*/

/*... PCG*/
  else{
/*... OpenMp*/
    if(ompVar.fSolver){
      pcgOmp(nEq     ,nAd
            ,ia      ,ja
            ,al      ,ad
            ,m       ,b
            ,x       ,z
            ,r       ,p
            ,tol     ,maxIt 
            ,newX    ,fSolvLog
            ,NULL    ,fLog    
            ,false   ,fPrint
            ,bOmp
            ,matVec  ,dot);
    }
/*...................................................................*/

/*... sequencial*/
    else{  
      pcg(nEq     ,nAd
         ,ia      ,ja
         ,al      ,ad  
         ,m       ,b    
         ,x       ,z   
         ,r       ,p
         ,tol     ,maxIt  
         ,newX    ,fSolvLog
         ,NULL    ,fLog 
         ,false   ,fPrint
         ,matVec  ,dot);
    }  
/*...................................................................*/
  }
/*...................................................................*/
}
/*********************************************************************/

/**********************************************************************
* Data de criacao    : 27/08/2016                                    *
* Data de modificaco : 00/00/0000                                    *
* -------------------------------------------------------------------*
* CALLBICGCSTAB : chama o gradiente biconjugados estabilizados       *                   *
* -------------------------------------------------------------------*
* Parametro de entrada                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* Parametro de saida :                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* OBS:                                                               *
**********************************************************************/
void callBicgStab(INT const nEq     ,INT const nEqNov
                 ,INT const nAd     ,INT const nAdR
                 ,INT *RESTRICT ia  ,INT *RESTRICT ja
                 ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                 ,DOUBLE *RESTRICT m,DOUBLE *RESTRICT b
                 ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT t
                 ,DOUBLE *RESTRICT v,DOUBLE *RESTRICT r
                 ,DOUBLE *RESTRICT p,DOUBLE *RESTRICT z
                 ,DOUBLE *RESTRICT h
                 ,DOUBLE const tol  ,unsigned int maxIt
                 ,bool const newX   ,FILE* fSolvLog
                 ,bool const fLog   ,bool const fPrint
                 ,Interface *iNeq   ,BufferOmp *bOmp
                 ,void(*matVec)()   ,DOUBLE(*dot)())
{

/*... MPI*/
  if (mpiVar.nPrcs > 1)
    mpiPbicgstab(nEq     ,nEqNov
                ,nAd     ,nAdR
                ,ia      ,ja
                ,a       ,ad  
                ,m       ,b   
                ,x       ,t   
                ,v       ,r
                ,p       ,z  
                ,tol
                ,maxIt   ,newX
                ,fSolvLog,fLog
                ,fPrint  ,iNeq
                ,matVec  ,dot);
/*...................................................................*/

/*... */
  else {
/*... OpenMp*/
    if (ompVar.fSolver) {
      pbicgstabOmp(nEq   ,nAd
                  ,ia    ,ja
                  ,a     ,ad
                  ,m     ,b
                  ,x     ,t
                  ,v     ,r
                  ,p     ,z
                  ,h
                  ,tol   ,maxIt
                  ,newX  ,fSolvLog
                  ,NULL  ,fLog
                  ,false ,fPrint
                  ,bOmp
                  ,matVec,dot);
    }
/*...................................................................*/

/*... sequencial*/
    else {
      pbicgstab(nEq   ,nAd
               ,ia    ,ja
               ,a     ,ad
               ,m     ,b 
               ,x     ,t   
               ,v     ,r
               ,p     ,z 
               ,h
               ,tol   ,maxIt  
               ,newX  ,fSolvLog
               ,NULL  ,fLog  
               ,false ,fPrint
               ,matVec,dot);
    }
/*...................................................................*/
  }
/*...................................................................*/
}
/*********************************************************************/

/**********************************************************************
* Data de criacao    : 01/09/2016                                    *
* Data de modificaco : 00/00/0000                                    *
* -------------------------------------------------------------------*
* CALLBICGCSTAB(2) : chama o gradiente biconjugados estabilizados    *                   
* -------------------------------------------------------------------*
* Parametro de entrada                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* Parametro de saida :                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* OBS:                                                               *
**********************************************************************/
void callBicgStabl2(INT const nEq     ,INT const nEqNov
                   ,INT const nAd     ,INT const nAdR
                   ,INT *RESTRICT ia  ,INT *RESTRICT ja
                   ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
                   ,DOUBLE *RESTRICT m,DOUBLE *RESTRICT b
                   ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT t
                   ,DOUBLE *RESTRICT v,DOUBLE *RESTRICT r
                   ,DOUBLE *RESTRICT u,DOUBLE *RESTRICT r0
                   ,DOUBLE *RESTRICT w,DOUBLE *RESTRICT s
                   ,DOUBLE *RESTRICT p,DOUBLE *RESTRICT h
                   ,DOUBLE *RESTRICT z
                   ,DOUBLE const tol  ,unsigned int maxIt
                   ,bool const newX   ,FILE* fSolvLog
                   ,bool const fLog   ,bool const fPrint
                   ,Interface *iNeq   ,BufferOmp *bOmp
                   ,void(*matVec)()   ,DOUBLE(*dot)())
{

/*... MPI*/
  if (mpiVar.nPrcs > 1)
    mpiPbicgstab(nEq, nEqNov
                 , nAd, nAdR
                 , ia, ja
                 , a, ad
                 , m, b
                 , x, t
                 , v, r
                 , p, z
                 , tol
                 , maxIt, newX
                 , fSolvLog, fLog
                 , fPrint, iNeq
                 , matVec, dot);  
/*...................................................................*/

/*... */
  else {
/*... OpenMp*/
    if (ompVar.fSolver) {
      pbicgstabl2Omp(nEq   ,nAd
                    ,ia    ,ja
                    ,a     ,ad
                    ,m     ,b
                    ,x     ,t
                    ,v     ,r
                    ,u     ,r0
                    ,w     ,s
                    ,p     ,h
                    ,z
                    ,tol   ,maxIt
                    ,newX  ,fSolvLog
                    ,NULL  ,fLog
                    ,false ,fPrint
                    ,bOmp 
                    ,matVec,dot);
    }
/*...................................................................*/

/*... sequencial*/
    else {
      pbicgstabl2(nEq   ,nAd
               ,ia    ,ja
               ,a     ,ad
               ,m     ,b
               ,x     ,t
               ,v     ,r
               ,u     ,r0
               ,w     ,s
               ,p     ,h
               ,z
               ,tol   ,maxIt
               ,newX  ,fSolvLog
               ,NULL  ,fLog
               ,false ,fPrint
               ,matVec,dot);
    }
/*...................................................................*/
  }
/*...................................................................*/
}
/*********************************************************************/

/**********************************************************************
* Data de criacao    : 04/09/2016                                    *
* Data de modificaco : 00/00/0000                                    *
* -------------------------------------------------------------------*
* CALLGMRES : chama o GMRES                                          *
* -------------------------------------------------------------------*
* Parametro de entrada                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* Parametro de saida :                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* OBS:                                                               *
**********************************************************************/
void callGmres(INT const nEq     ,INT const nEqNov
              ,INT const nAd     ,INT const nAdR
              ,INT *RESTRICT ia  ,INT *RESTRICT ja
              ,DOUBLE *RESTRICT a,DOUBLE *RESTRICT ad
              ,DOUBLE *RESTRICT m,DOUBLE *RESTRICT b
              ,DOUBLE *RESTRICT x,DOUBLE *RESTRICT g
              ,DOUBLE *RESTRICT h,DOUBLE *RESTRICT y
              ,DOUBLE *RESTRICT c,DOUBLE *RESTRICT s
              ,DOUBLE *RESTRICT e,short const nKrylov
              ,DOUBLE const tol  ,unsigned int maxIt
              ,bool const newX   ,FILE* fSolvLog
              ,bool const fLog   ,bool const fPrint
              ,Interface *iNeq   ,BufferOmp *bOmp
              ,void(*matVec)()   ,DOUBLE(*dot)())
{

/*... MPI*/
  if (mpiVar.nPrcs > 1);
/*...................................................................*/

/*... */
  else {
/*... OpenMp*/
    if (ompVar.fSolver)
      gmresOmp(nEq  ,nAd
                   ,ia   ,ja
                   ,a    ,ad
                   ,m    ,b
                   ,x    ,g
                   ,h    ,y
                   ,c    ,s
                   ,e    ,nKrylov
                   ,tol  ,maxIt
                   ,newX ,fSolvLog
                   ,NULL ,fLog
                   ,fLog ,fPrint
                   ,bOmp
                   ,matVec,dot);
/*...................................................................*/

/*... sequencial*/
    else {
      gmres(nEq    ,nAd
           ,ia     ,ja
           ,a      ,ad
           ,m      ,b
           ,x      ,g
           ,h      ,y
           ,c      ,s
           ,e      ,nKrylov
           ,tol    ,maxIt
           ,newX   ,fSolvLog
           ,NULL   ,fLog
           ,fLog   ,fPrint
           ,matVec ,dot);
    }
/*...................................................................*/
  }
/*...................................................................*/

}
/*********************************************************************/


/**********************************************************************
* Data de criacao    : 18/09/2017                                    *
* Data de modificaco : 00/00/0000                                    *
* -------------------------------------------------------------------*
* CALLMINRES : chama o minres                                        *                   
* -------------------------------------------------------------------*
* Parametro de entrada                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* Parametro de saida :                                               *
* -------------------------------------------------------------------*
* -------------------------------------------------------------------*
* OBS:                                                               *
**********************************************************************/
void callMinRes(INT const nEq     ,INT const nEqNov
               ,INT const nAd     ,INT const nAdR
               ,INT *RESTRICT ia  ,INT *RESTRICT ja
               , DOUBLE *RESTRICT al, DOUBLE *RESTRICT ad
	             , DOUBLE *RESTRICT m , DOUBLE *RESTRICT b
               , DOUBLE *RESTRICT x , DOUBLE *RESTRICT v0
               , DOUBLE *RESTRICT v , DOUBLE *RESTRICT w
               , DOUBLE *RESTRICT w0, DOUBLE *RESTRICT w00
               , DOUBLE *RESTRICT z , DOUBLE *RESTRICT z0
               , DOUBLE *RESTRICT p
               ,DOUBLE const tol  ,unsigned int maxIt
               ,bool const newX   ,FILE* fSolvLog
               ,bool const fLog   ,bool const fPrint
               ,Interface *iNeq   ,BufferOmp *bOmp
               ,void(*matVec)()   ,DOUBLE(*dot)())
{

/*... MPI*/
  if (mpiVar.nPrcs > 1);
/*  mpiPbicgstab(nEq, nEqNov
                 , nAd, nAdR
                 , ia, ja
                 , a, ad
                 , m, b
                 , x, t
                 , v, r
                 , p, z
                 , tol
                 , maxIt, newX
                 , fSolvLog, fLog
                 , fPrint, iNeq
                 , matVec, dot);  */
/*...................................................................*/

/*... */
  else {
/*... OpenMp*/
    if (ompVar.fSolver) {
/*    pbicgstabl2Omp(nEq   ,nAd
                    ,ia    ,ja
                    ,a     ,ad
                    ,m     ,b
                    ,x     ,t
                    ,v     ,r
                    ,u     ,r0
                    ,w     ,s
                    ,p     ,h
                    ,z
                    ,tol   ,maxIt
                    ,newX  ,fSolvLog
                    ,NULL  ,fLog
                    ,false ,fPrint
                    ,bOmp 
                    ,matVec,dot);*/
    }
/*...................................................................*/

/*... sequencial*/
    else {
      pminres(nEq   ,nAd
             ,ia    ,ja
             ,al    ,ad
             ,m     ,b
             ,x     ,v0
             ,v     ,w
             ,w0    ,w00
             ,z     ,z0
             ,p 
             ,tol   ,maxIt
             ,newX  ,fSolvLog
             ,NULL  ,fLog
             ,false ,fPrint
             ,matVec,dot);  
     }
/*...................................................................*/
  }
/*...................................................................*/
}
/*********************************************************************/