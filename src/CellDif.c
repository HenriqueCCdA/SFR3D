#include<CellLib.h>
/********************************************************************* 
 * CELLDIF2D: Celula 2D para difusao pura                            * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * loads     -> definicoes de cargas                                 * 
 * lnFace    -> numero de faces da celula central e seus vizinhos    * 
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    * 
 * lprop     -> propriedade fisicas das celulas                      * 
 * lViz      -> viznhos da celula central                            * 
 * lId       -> numeracoes das equacoes das celulas                  * 
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *            vizinhos destas                                        * 
 * mKsi      -> modulo do vetor ksi                                  * 
 * eta       -> vetores paralelos as faces das celulas               * 
 * mEta      -> modulo do vetor eta                                  * 
 * normal    -> vetores normais as faces das celulas                 * 
 * area      -> area da celula central                               * 
 * xm        -> pontos medios das faces da celula central            * 
 * xmcc      -> vetores que unem o centroide aos pontos medios das   * 
 *            faces da celula central                                * 
 * vSkew  -> vetor entre o ponto medio a intersecao que une os       * 
 *            centrois compartilhado nessa face da celula central    * 
 * mvSkew -> distacia entre o ponto medio a intersecao que une os    * 
 *            centrois compartilhado nessa face da celula central    * 
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               * 
 * lDensity  -> massa especifica com variacao temporal               * 
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * u0        -> solucao conhecida                                    * 
 * gradU0    -> gradiente rescontruido da solucao conhecida          * 
 * faceR     -> restricoes por elmento                               * 
 * faceL     -> carga por elemento                                   * 
 * nEn       -> numero de nos da celula central                      * 
 * nFace     -> numero de faces da celula central                    * 
 * ndm       -> numero de dimensoes                                  * 
 * nel       -> numero da celula                                     * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * lA        -> coeficiente da linha i                               *
 * lB        -> vetor de forca da linha i                            *
 * lRcell    -> residuo por celula                                   *
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void cellDif2D(Loads *loads
              ,short *RESTRICT lGeomType,DOUBLE *RESTRICT prop
              ,INT *RESTRICT lViz       ,INT *RESTRICT lId  
              ,DOUBLE *RESTRICT ksi     ,DOUBLE *RESTRICT mKsi
              ,DOUBLE *RESTRICT eta     ,DOUBLE *RESTRICT mEta
              ,DOUBLE *RESTRICT normal  ,DOUBLE *RESTRICT volume
              ,DOUBLE *RESTRICT xm      ,DOUBLE *RESTRICT xmcc
              ,DOUBLE *RESTRICT dcca    ,DOUBLE *RESTRICT lDensity
              ,DOUBLE *RESTRICT vSkew   ,DOUBLE *RESTRICT mvSkew
              ,DOUBLE *RESTRICT lA      ,DOUBLE *RESTRICT lB
              ,DOUBLE *RESTRICT lRcell  ,Temporal const ddt                
              ,short  *RESTRICT lFaceR  ,short *RESTRICT lFaceL
              ,DOUBLE *RESTRICT u0      ,DOUBLE *RESTRICT gradU0
              ,const short nEn          ,short const nFace    
              ,const short ndm          ,INT const nel)
{ 

  DOUBLE coefDifC,coefDif,coefDifV,rCell,*dum=NULL;
  DOUBLE densityC,dt,dt0;
  DOUBLE p,sP,nk,dfd,dfdc,gfKsi,modE,lvSkew[2];
  DOUBLE v[2],gradUcomp[2],lKsi[2],lNormal[2],gf[2];
  DOUBLE dPviz,lModKsi,lModEta,du,duDksi;
  DOUBLE gradUp[2],gradUv[2],nMinusKsi[2];
  DOUBLE alpha,alphaMenosUm;
  DOUBLE tA,tmp;
  DOUBLE xx[3];
  short idCell = nFace;
  short nAresta,nCarg,typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt[0];
  dt0      = ddt.dt[1];
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = *lDensity;
/*...................................................................*/

/*... propriedades da celula*/
  coefDifC = MAT2D(idCell,COEFDIF,prop,MAXPROP);
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell,0,gradU0,ndm);
  gradUp[1] = MAT2D(idCell,1,gradU0,ndm);
/*...................................................................*/

  p  = 0.0e0;
  sP = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    vizNel = lViz[nAresta];
/*... dominio*/
    if( vizNel  > -1 ){
/*...*/
      lKsi[0]    = MAT2D(nAresta,0,ksi,ndm);
      lKsi[1]    = MAT2D(nAresta,1,ksi,ndm);
      lNormal[0] = MAT2D(nAresta,0,normal,ndm);
      lNormal[1] = MAT2D(nAresta,1,normal,ndm);
      lModKsi    = mKsi[nAresta];
      lModEta    = mEta[nAresta];
      lvSkew[0]  = MAT2D(nAresta,0,vSkew,ndm);
      lvSkew[1]  = MAT2D(nAresta,1,vSkew,ndm);
      duDksi     = (u0[nAresta] - u0[idCell]) / lModKsi;
      gradUv[0]  = MAT2D(nAresta,0,gradU0,ndm);
      gradUv[1]  = MAT2D(nAresta,1,gradU0,ndm);
/*...................................................................*/

/*... produtos internos*/
      nk = lNormal[0] * lKsi[0] + lNormal[1] * lKsi[1];
/*...................................................................*/
      
/*... correcao sobre-relaxada*/
      modE       = 1.0e0/nk;
/*...................................................................*/

/*...*/
      v[0]         = lvSkew[0] + MAT2D(nAresta,0,xmcc,ndm);
      v[1]         = lvSkew[1] + MAT2D(nAresta,1,xmcc,ndm);
      dPviz        = sqrt(v[0]*v[0] + v[1]*v[1]);
      alpha        = dPviz/lModKsi;
      alphaMenosUm = 1.0e0 - alpha; 
/*...................................................................*/

/*... media harmonica*/
      coefDifV = MAT2D(nAresta,COEFDIF,prop,MAXPROP); 
      coefDif  = alpha/coefDifC + alphaMenosUm/coefDifV;
      coefDif  = 1.0e0/coefDif;
/*...................................................................*/

/*... difusao direta*/
      dfd = (coefDif*lModEta*modE)/lModKsi;
/*...................................................................*/
      
/*...*/
      gf[0] = alphaMenosUm*gradUp[0] + alpha*gradUv[0];
      gf[1] = alphaMenosUm*gradUp[1] + alpha*gradUv[1];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi        = gf[0] *lKsi[0] + gf[1] *lKsi[1];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du           = duDksi  - gfKsi;
      gradUcomp[0] = gf[0] + du*lKsi[0];
      gradUcomp[1] = gf[1] + du*lKsi[1];
/*...................................................................*/

/*... derivadas direcionais*/
      nMinusKsi[0] = lNormal[0] - modE*lKsi[0];
      nMinusKsi[1] = lNormal[1] - modE*lKsi[1];
      gfKsi = gradUcomp[0]*nMinusKsi[0] + gradUcomp[1]*nMinusKsi[1];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc = coefDif*lModEta*gfKsi;    
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd;
      p          += dfdc;
/*...................................................................*/
    }
/*... contorno*/
    else{
      lA[nAresta] = 0.0e0;
      if(lFaceR[nAresta]){
/*...cargas*/
        nCarg=lFaceL[nAresta]-1;
        xx[0] = MAT2D(nAresta,0,xm,2);
        xx[1] = MAT2D(nAresta,1,xm,2);
        xx[2] = 0.e0;                    
        pLoad(&sP           ,&p
             ,&tA           ,lNormal
             ,dum
             ,coefDifC      ,0.e0
             ,xx            ,mEta[nAresta] 
             ,dcca[nAresta] ,&loads[nCarg]
             ,2             ,true);
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... distretizacao temporal*/
  if (fTime) {
    /*... EULER*/
    if (typeTime == EULER)
      sP += densityC*volume[idCell] / dt;
    /*...BACKWARD*/
    else if (typeTime == BACKWARD) {
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp*densityC*volume[idCell];
    }
  }
/*...................................................................*/

/*...*/
  if(nFace == 3){
    lA[idCell] = sP + lA[0] + lA[1] + lA[2];
  }
  else if(nFace == 4){
    lA[idCell] = sP + lA[0] + lA[1] + lA[2] + lA[3];
  }
/*...................................................................*/

/*...*/
  rCell = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    if( lViz[nAresta] > -1){
/*... pasando os valoeres conhecidos para o lado direito*/
      if(lId[nAresta] == -2)
        p += lA[nAresta]*u0[nAresta]; 
      else
/*residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell += lA[nAresta]*u0[nAresta]; 
    }
  }
/*... residuo: R = F - KpUp*/ 
  rCell += p -lA[idCell]*u0[idCell];   
/*...................................................................*/

/*...*/
  if(nFace == 3){
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
  }
  else if(nFace == 4){
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
    lA[3] *= -1.e0;
  }
/*...................................................................*/

/*...*/
  lB[0]     = p;
  lRcell[0] = rCell;
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/01/2018                                   *
 * Data de modificaco : 06/10/2019                                   *
 * ----------------------------------------------------------------- *
 * CELLDIF3D: Celula 3D para difusao pura                            * 
 *------------------------------------------------------------------ * 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * loads     -> definicoes de cargas                                 * 
 * diff      -> tecnica da discretizacao do termo difusivo           *
 * lnFace    -> numero de faces da celula central e seus vizinhos    * 
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    * 
 * lViz      -> viznhos da celula central                            * 
 * lId       -> numeracoes das equacoes das celulas                  * 
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *            vizinhos destas                                        * 
 * mKsi      -> modulo do vetor ksi                                  * 
 * eta       -> vetores paralelos as faces das celulas               * 
 * fArea     -> area das faces da celula central                     * 
 * normal    -> vetores normais as faces das celulas                 * 
 * volume    -> volume da celula central                             * 
 * xm        -> pontos medios das faces da celula central            * 
 * xmcc      -> vetores que unem o centroide aos pontos medios das   * 
 *            faces da celula central                                * 
 * vSkew  -> vetor entre o ponto medio a intersecao que une os       * 
 *            centrois compartilhado nessa face da celula central    * 
 * mvSkew -> distacia entre o ponto medio a intersecao que une os    * 
 *            centrois compartilhado nessa face da celula central    * 
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               * 
 * lDensity  -> massa especifica com variacao temporal               * 
 * lCeofDiffD-> coeficiente de difusao com variacao temporal         *
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * u0        -> solucao conhecida                                    * 
 * gradU0    -> gradiente rescontruido da solucao conhecida          * 
 * faceR     -> restricoes por elemento                              * 
 * nEn       -> numero de nos da celula central                      * 
 * nFace     -> numero de faces da celula central                    * 
 * ndm       -> numero de dimensoes                                  * 
 * nel       -> numero da celula                                     * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * lA        -> coeficiente da linha i                               *
 * lB        -> vetor de forca da linha i                            *
 * lRcell    -> residuo por celula                                   *
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void cellDif3D(Loads *loads               ,Diffusion *diff
              ,DiffModel *dModel          ,short *RESTRICT lGeomType
              ,INT *RESTRICT lViz         ,INT *RESTRICT lId  
              ,DOUBLE *RESTRICT ksi       ,DOUBLE *RESTRICT mKsi
              ,DOUBLE *RESTRICT eta       ,DOUBLE *RESTRICT fArea
              ,DOUBLE *RESTRICT normal    ,DOUBLE *RESTRICT volume
              ,DOUBLE *RESTRICT xm        ,DOUBLE *RESTRICT xmcc
              ,DOUBLE *RESTRICT dcca      ,DOUBLE *RESTRICT lDensity
              ,DOUBLE *RESTRICT lCeofDiffD
              ,DOUBLE *RESTRICT vSkew     ,DOUBLE *RESTRICT mvSkew
              ,DOUBLE *RESTRICT lA        ,DOUBLE *RESTRICT lB
              ,DOUBLE *RESTRICT lRcell    ,Temporal *ddt             
              ,short  *RESTRICT lFaceR    
              ,DOUBLE *RESTRICT u0        ,DOUBLE *RESTRICT gradU0
              ,const short nEn            ,short const nFace    
              ,const short ndm            ,INT const nel)
{ 

  bool fTime,fRes;
  short idCell, nf, nCarg, typeTime, iCodPolFace, iCodDif;
  INT vizNel;
  DOUBLE coefDifC,coefDif,coefDifV,rCell,dt,dt0, *dum = NULL;
  DOUBLE densityC;
  DOUBLE p,sP,dfd,dfdc,gfKsi,lvSkew[3];
  DOUBLE gradUcomp[3],lKsi[3],lNormal[3],gf[3];
  DOUBLE lModKsi,lfArea,du,duDksi, lXmcc[3];
  DOUBLE gradUp[3],gradUv[3];
  DOUBLE alpha,alphaMenosUm;
/*... nonOrtogonal*/
  DOUBLE e[3], t[3], s[3], modE, xx[3];
  DOUBLE tA,tmp;

/*...*/
  idCell      = nFace;
  iCodDif     = diff->iCod;
  iCodPolFace = INTPOLFACELINEAR;
/*...................................................................*/

/*...*/
  dt       = ddt->dt[0];
  dt0      = ddt->dt[1];
  typeTime = ddt->type;
  fTime    = ddt->flag;
  densityC = *lDensity;
  fRes     = dModel->fRes;
/*...................................................................*/
  
/*... propriedades da celula*/
  coefDifC = lCeofDiffD[idCell];
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell,0,gradU0,ndm);
  gradUp[1] = MAT2D(idCell,1,gradU0,ndm);
  gradUp[2] = MAT2D(idCell,2,gradU0,ndm);
/*...................................................................*/

  p  = 0.0e0;
  sP = 0.0e0;
  for(nf=0;nf<nFace;nf++)
  {
    vizNel = lViz[nf];
    lfArea = fArea[nf];
    lNormal[0] = MAT2D(nf, 0, normal, 3);
    lNormal[1] = MAT2D(nf, 1, normal, 3);
    lNormal[2] = MAT2D(nf, 2, normal, 3);
    lXmcc[0] = MAT2D(nf, 0, xmcc, 3);
    lXmcc[1] = MAT2D(nf, 1, xmcc, 3);
    lXmcc[2] = MAT2D(nf, 2, xmcc, 3);
/*... dominio*/
    if( vizNel  > -1 )
    {
/*...*/
      lKsi[0]    = MAT2D(nf,0,ksi,3);
      lKsi[1]    = MAT2D(nf,1,ksi,3);
      lKsi[2]    = MAT2D(nf,2,ksi,3);
      lModKsi    = mKsi[nf];
      lvSkew[0]  = MAT2D(nf,0,vSkew,3);
      lvSkew[1]  = MAT2D(nf,1,vSkew,3);
      lvSkew[2]  = MAT2D(nf,2,vSkew,3);
      duDksi     = (u0[nf] - u0[idCell]) / lModKsi;
      gradUv[0]  = MAT2D(nf,0,gradU0,3);
      gradUv[1]  = MAT2D(nf,1,gradU0,3);
      gradUv[2]  = MAT2D(nf,2,gradU0,3);
/*...................................................................*/
     
/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      s[0] = lfArea * lNormal[0];
      s[1] = lfArea * lNormal[1];
      s[2] = lfArea * lNormal[2];
/*...*/
      difusionSchemeNew(s, lKsi, e, t, ndm, iCodDif);
/*...................................................................*/

/*...*/
      alpha = interpolFace(lvSkew, lXmcc
            , volume[idCell], volume[nf]
            , lModKsi, ndm
            , iCodPolFace);
      alphaMenosUm = 1.0e0 - alpha;
/*...................................................................*/

/*... media harmonica*/
      coefDifV = lCeofDiffD[nf];
      coefDif  = alpha/coefDifC + alphaMenosUm/coefDifV;
      coefDif  = 1.0e0/coefDif;
/*...................................................................*/

/*... difusao direta*/
      modE = sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
      dfd = coefDif*modE/lModKsi;
/*...................................................................*/
      
/*...*/
      gf[0] = alphaMenosUm*gradUp[0] + alpha*gradUv[0];
      gf[1] = alphaMenosUm*gradUp[1] + alpha*gradUv[1];
      gf[2] = alphaMenosUm*gradUp[2] + alpha*gradUv[2];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi        = gf[0] *lKsi[0]  
                   + gf[1] *lKsi[1] 
                   + gf[2] *lKsi[2];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du           = duDksi  - gfKsi;
      gradUcomp[0] = gf[0] + du*lKsi[0];
      gradUcomp[1] = gf[1] + du*lKsi[1];
      gradUcomp[2] = gf[2] + du*lKsi[2];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gradUcomp[0]*t[0] 
            + gradUcomp[1]*t[1] 
            + gradUcomp[2]*t[2];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc = coefDif*gfKsi;    
/*...................................................................*/

/*...*/
      lA[nf] = dfd;
      p     += dfdc;
/*...................................................................*/
    }
/*... contorno*/
    else
    {
      lA[nf] = 0.0e0;
      if(lFaceR[nf])
      {
/*...cargas*/
        nCarg = lFaceR[nf]-1;
        xx[0] = MAT2D(nf,0,xm,3);
        xx[1] = MAT2D(nf,1,xm,3);
        xx[2] = MAT2D(nf,2,xm,3);        
        pLoad(&sP           ,&p
             ,&tA           ,dum
             ,lNormal
             ,coefDifC      ,densityC
             ,xx            ,fArea[nf] 
             ,dcca[nf]      ,&loads[nCarg] 
             ,ndm           ,true);
      }
/*...................................................................*/
    }
/*...................................................................*/
  }

/*... distretizacao temporal*/
  if (fTime) 
  {
/*... EULER*/
    if (typeTime == EULER)
      sP += densityC*volume[idCell] / dt;
/*...BACKWARD*/
    else if (typeTime == BACKWARD) 
    { 
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp*densityC*volume[idCell];
    }
  }  
/*...................................................................*/

/*...*/
  lA[idCell] = sP;
  for(nf=0;nf<nFace;nf++)
    lA[idCell] += lA[nf];
/*...................................................................*/

/*...*/
  rCell = 0.0e0;
  for(nf=0;nf<nFace;nf++){
    if( lViz[nf] > -1){
/*... pasando os valoeres conhecidos para o lado direito*/
      if(lId[nf] == -2)
        p += lA[nf]*u0[nf]; 
      else
/*residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell += lA[nf]*u0[nf]; 
    }
  }
/*... residuo: R = F - KpUp*/ 
  rCell += p -lA[idCell]*u0[idCell]; 
/*...................................................................*/

/*...*/  
  for(nf=0;nf<nFace;nf++)
    lA[nf] *= -1.e0;
/*...................................................................*/

/*...*/
  if (fRes)
    lB[0] = rCell;
  else
    lB[0] = p;
/*...*/
  lRcell[0] = rCell;
/*...................................................................*/
}
/*********************************************************************/


