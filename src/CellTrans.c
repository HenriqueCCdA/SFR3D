#include<CellLoop.h>
/********************************************************************* 
 * CELLTRANS2D: Celula 2D para transporte                            * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * loads     -> definicoes de cargas                                 * 
 * advT      -> tecnica da discretizacao do termo advecao            * 
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
 * faceR     -> restricoes por elmento                               * 
 * faceL     -> carga por elemento                                   * 
 * u0        -> solucao conhecida                                    * 
 * gradU0    -> gradiente rescontruido da solucao conhecida          * 
 * vel       -> campo de velocidade conhecido                        * 
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
void cellTrans2D(Loads *loads           ,Advection advT
              ,short *restrict lGeomType,DOUBLE *restrict prop
              ,INT *restrict lViz       ,INT *restrict lId  
              ,DOUBLE *restrict ksi     ,DOUBLE *restrict mKsi
              ,DOUBLE *restrict eta     ,DOUBLE *restrict mEta
              ,DOUBLE *restrict normal  ,DOUBLE *restrict volume
              ,DOUBLE *restrict xm      ,DOUBLE *restrict xmcc
              ,DOUBLE *restrict dcca    ,DOUBLE *restrict lDensity
              ,DOUBLE *restrict vSkew   ,DOUBLE *restrict mvSkew
              ,DOUBLE *restrict lA      ,DOUBLE *restrict lB
              ,DOUBLE *restrict lRcell  ,Temporal const ddt
              ,short  *restrict lFaceR  ,short *restrict lFaceL
              ,DOUBLE *restrict u0      ,DOUBLE *restrict gradU0
              ,DOUBLE *restrict vel                                
              ,const short nEn          ,short const nFace    
              ,const short ndm          ,INT const nel)
{ 

  DOUBLE coefDifC,coefDif,coefDifV,rCell,dt;
  DOUBLE densityC,densityF,densityM;
  DOUBLE p,sP,nk,dfd,dfdc,gfKsi,modE,lvSkew[2];
  DOUBLE v[2],gradUcomp[2],lKsi[2],lNormal[2],gf[2];
  DOUBLE dPviz,lModKsi,lModEta,du,duDksi;
  DOUBLE gradUp[2],gradUv[2],nMinusKsi[2];
  DOUBLE alpha,alphaMenosUm;
  DOUBLE tA;
  DOUBLE xx[3];
/*... */
  DOUBLE wfn,wf[2],velC[2],velF[2],cv,cvc;
  short iCod=advT.iCod;
/*...*/
  short idCell = nFace;
  short nAresta,nCarg,typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt;
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = lDensity[idCell];
/*...................................................................*/

/*... propriedades da celula*/
  coefDifC = MAT2D(idCell,COEFDIF,prop,MAXPROP);
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell,0,gradU0,ndm);
  gradUp[1] = MAT2D(idCell,1,gradU0,ndm);
  velC[0]   = MAT2D(idCell,0,vel,ndm);
  velC[1]   = MAT2D(idCell,1,vel,ndm);
/*...................................................................*/

  p          = 0.0e0;
  sP         = 0.0e0;
  lA[idCell] = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    vizNel     = lViz[nAresta];
    lNormal[0] = MAT2D(nAresta,0,normal,ndm);
    lNormal[1] = MAT2D(nAresta,1,normal,ndm);
    lModEta    = mEta[nAresta];
/*... dominio*/
    if( vizNel  > -1 ){
/*...*/
      lKsi[0]    = MAT2D(nAresta,0,ksi,ndm);
      lKsi[1]    = MAT2D(nAresta,1,ksi,ndm);
      lModKsi    = mKsi[nAresta];
      lvSkew[0]  = MAT2D(nAresta,0,vSkew,ndm);
      lvSkew[1]  = MAT2D(nAresta,1,vSkew,ndm);
      duDksi     = (u0[nAresta] - u0[idCell]) / lModKsi;
      gradUv[0]  = MAT2D(nAresta,0,gradU0,ndm);
      gradUv[1]  = MAT2D(nAresta,1,gradU0,ndm);
      velF  [0]  = MAT2D(nAresta,0,vel,ndm);
      velF  [1]  = MAT2D(nAresta,1,vel,ndm);
      densityF   = lDensity[nAresta];
/*...................................................................*/

/*... produtos interno*/
      nk  =    lKsi[0]*lNormal[0] + lKsi[1]*lNormal[1];
/*...................................................................*/
      
/*... correcao sobre-relaxada*/
      modE       = 1.0e0/nk;
/*...................................................................*/

/*...*/
      v[0]  = lvSkew[0] + MAT2D(nAresta,0,xmcc,ndm);
      v[1]  = lvSkew[1] + MAT2D(nAresta,1,xmcc,ndm);
      dPviz = sqrt(v[0]*v[0] + v[1]*v[1]);
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
      gf[0]    = alphaMenosUm*gradUp[0] + alpha*gradUv[0];
      gf[1]    = alphaMenosUm*gradUp[1] + alpha*gradUv[1];
      wf[0]    =   alphaMenosUm*velC[0] + alpha*velF[0];
      wf[1]    =   alphaMenosUm*velC[1] + alpha*velF[1];
      densityM = alphaMenosUm*densityC  + alpha*densityF;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0]*lNormal[0] + wf[1]*lNormal[1];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi        = gf[0] *lKsi[0]    + gf[1] *lKsi[1];
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

/*... fluxo convectivo upwind de primeira ordem*/
      cv   = densityM*wfn*lModEta;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      if(FOUP == iCod)
        cvc = 0.e0;
      else
        cvc = faceBaseTvd(nAresta    ,idCell
                         ,u0
                         ,gradUv     ,gradUp
                         ,lKsi       ,lModKsi   
                         ,cv
                         ,iCod       ,ndm);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv,0.0e0);
      sP         += cv;    
/*... correcao nao ortogonal e do fluxo advectivo*/        
      p          += dfdc - cv*cvc;
/*...................................................................*/
    }
/*... contorno*/
    else{
      lA[nAresta] = 0.0e0;
      if(lFaceR[nAresta]){
        wfn = velC[0]*lNormal[0] + velC[1]*lNormal[1];
/*...cargas*/
        nCarg=lFaceL[nAresta]-1;
        xx[0] = MAT2D(nAresta,0,xm,2);
        xx[1] = MAT2D(nAresta,1,xm,2);
        xx[2] = 0.e0;                    
        pLoad(&sP           ,&p
             ,&tA
             ,coefDifC      ,densityC
             ,wfn           ,xx 
             ,lModEta       ,dcca[nAresta]
             ,loads[nCarg],true);
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... distretização temporal*/
  if(fTime){
/*... EULER*/
    if(typeTime == EULER) 
      sP     += densityC*volume[idCell]/dt;
/*...BACKWARD*/
    else if(typeTime == BACKWARD) 
      sP     += 1.5e0*densityC*volume[idCell]/dt;
  }
/*...................................................................*/


/*...*/
  if(nFace == 3){
    lA[idCell] = sP + lA[0] + lA[1] + lA[2];
  }
  else if(nFace == 4){
    lA[idCell] = sP + lA[0] + lA[1] + lA[2] + lA[3];
  }
/*
  lA[idCell] = sP;
  for(nAresta=0;nAresta<nFace;nAresta++){
    lA[idCell] += lA[nAresta];
  }
*/
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

/*  
  for(nAresta=0;nAresta<nFace;nAresta++){
   lA[nAresta] *= -1.e0;
  }
*/
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
 * CELLTRANS3D: Celula 3D para transporte                            * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * loads     -> definicoes de cargas                                 * 
 * advT      -> tecnica da discretizacao do termo advecao            * 
 * lnFace    -> numero de faces da celula central e seus vizinhos    * 
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    * 
 * lprop     -> propriedade fisicas das celulas                      * 
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
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * faceR     -> restricoes por elemento                              * 
 * faceL     -> carga por elemento                                   * 
 * u0        -> solucao conhecida                                    * 
 * gradU0    -> gradiente rescontruido da solucao conhecida          * 
 * vel       -> campo de velocidade conhecido                        * 
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
void cellTrans3D(Loads *loads           ,Advection advT
              ,short *restrict lGeomType,DOUBLE *restrict prop
              ,INT *restrict lViz       ,INT *restrict lId  
              ,DOUBLE *restrict ksi     ,DOUBLE *restrict mKsi
              ,DOUBLE *restrict eta     ,DOUBLE *restrict fArea
              ,DOUBLE *restrict normal  ,DOUBLE *restrict volume
              ,DOUBLE *restrict xm      ,DOUBLE *restrict xmcc
              ,DOUBLE *restrict dcca    ,DOUBLE *restrict lDensity
              ,DOUBLE *restrict vSkew   ,DOUBLE *restrict mvSkew
              ,DOUBLE *restrict lA      ,DOUBLE *restrict lB
              ,DOUBLE *restrict lRcell  ,Temporal const ddt             
              ,short  *restrict lFaceR  ,short  *restrict lFaceL  
              ,DOUBLE *restrict u0      ,DOUBLE *restrict gradU0
              ,DOUBLE *restrict vel                                
              ,const short nEn          ,short const nFace    
              ,const short ndm          ,INT const nel)
{ 

  DOUBLE coefDifC,coefDif,coefDifV,rCell,dt;
  DOUBLE densityC,densityF,densityM;
  DOUBLE p,sP,nk,dfd,dfdc,gfKsi,modE,lvSkew[3];
  DOUBLE v[3],gradUcomp[3],lKsi[3],lNormal[3],gf[3];
  DOUBLE dPviz,lModKsi,lfArea,du,duDksi;
  DOUBLE gradUp[3],gradUv[3],nMinusKsi[3];
  DOUBLE alpha,alphaMenosUm;
  DOUBLE tA;
  DOUBLE xx[3];
/*...*/
  DOUBLE wfn,wf[3],velC[3],velF[3],cv,cvc;
  short iCod=advT.iCod;
/*...*/
  short idCell = nFace;
  short nf,nCarg,typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt;
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = lDensity[idCell];
/*...................................................................*/
  
/*... propriedades da celula*/
  coefDifC = MAT2D(idCell,COEFDIF,prop,MAXPROP);
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell,0,gradU0,ndm);
  gradUp[1] = MAT2D(idCell,1,gradU0,ndm);
  gradUp[2] = MAT2D(idCell,2,gradU0,ndm);
  velC[0]   = MAT2D(idCell,0,vel,ndm);
  velC[1]   = MAT2D(idCell,1,vel,ndm);
  velC[2]   = MAT2D(idCell,2,vel,ndm);
/*...................................................................*/
      
  p          = 0.0e0;
  sP         = 0.0e0;
  lA[idCell] = 0.0e0;
  for(nf=0;nf<nFace;nf++){
    vizNel     = lViz[nf];
    lNormal[0] = MAT2D(nf,0,normal,ndm);
    lNormal[1] = MAT2D(nf,1,normal,ndm);
    lNormal[2] = MAT2D(nf,2,normal,ndm);
    lfArea     = fArea[nf];
/*... dominio*/
    if( vizNel  > -1 ){
/*...*/
      lKsi[0]    = MAT2D(nf,0,ksi,ndm);
      lKsi[1]    = MAT2D(nf,1,ksi,ndm);
      lKsi[2]    = MAT2D(nf,2,ksi,ndm);
      lModKsi    = mKsi[nf];
      lvSkew[0]  = MAT2D(nf,0,vSkew,ndm);
      lvSkew[1]  = MAT2D(nf,1,vSkew,ndm);
      lvSkew[2]  = MAT2D(nf,2,vSkew,ndm);
      duDksi     = (u0[nf] - u0[idCell]) / lModKsi;
      gradUv[0]  = MAT2D(nf,0,gradU0,ndm);
      gradUv[1]  = MAT2D(nf,1,gradU0,ndm);
      gradUv[2]  = MAT2D(nf,2,gradU0,ndm);
      velF[0]    = MAT2D(nf,0,vel,ndm);
      velF[1]    = MAT2D(nf,1,vel,ndm);
      velF[2]    = MAT2D(nf,2,vel,ndm);
      densityF   = lDensity[nf];
/*...................................................................*/

/*... produtos internos*/
      nk = lNormal[0] * lKsi[0] 
         + lNormal[1] * lKsi[1] 
         + lNormal[2] * lKsi[2];
/*...................................................................*/
      
/*... correcao sobre-relaxada*/
      modE       = 1.0e0/nk;
/*...................................................................*/

/*...*/
      v[0]  = lvSkew[0] + MAT2D(nf,0,xmcc,ndm);
      v[1]  = lvSkew[1] + MAT2D(nf,1,xmcc,ndm);
      v[2]  = lvSkew[2] + MAT2D(nf,2,xmcc,ndm);
      dPviz = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
      alpha        = dPviz/lModKsi;
      alphaMenosUm = 1.0e0 - alpha; 
/*...................................................................*/

/*... media harmonica*/
      coefDifV = MAT2D(nf,COEFDIF,prop,MAXPROP); 
      coefDif  = alpha/coefDifC + alphaMenosUm/coefDifV;
      coefDif  = 1.0e0/coefDif;
/*...................................................................*/

/*... difusao direta*/
      dfd = (coefDif*lfArea*modE)/lModKsi;
/*...................................................................*/
      
/*...*/
      gf[0]    = alphaMenosUm*gradUp[0] + alpha*gradUv[0];
      gf[1]    = alphaMenosUm*gradUp[1] + alpha*gradUv[1];
      gf[2]    = alphaMenosUm*gradUp[2] + alpha*gradUv[2];
      wf[0]    =   alphaMenosUm*velC[0] + alpha*velF[0];
      wf[1]    =   alphaMenosUm*velC[1] + alpha*velF[1];
      wf[2]    =   alphaMenosUm*velC[2] + alpha*velF[2];
      densityM = alphaMenosUm*densityC  + alpha*densityF;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0]*lNormal[0] + wf[1]*lNormal[1] + wf[2]*lNormal[2];
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
      nMinusKsi[0] = lNormal[0] - modE*lKsi[0];
      nMinusKsi[1] = lNormal[1] - modE*lKsi[1];
      nMinusKsi[2] = lNormal[2] - modE*lKsi[2];
      gfKsi = gradUcomp[0]*nMinusKsi[0] 
            + gradUcomp[1]*nMinusKsi[1] 
            + gradUcomp[2]*nMinusKsi[2];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc = coefDif*lfArea*gfKsi;    
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      cv   = densityM*wfn*lfArea;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      if(FOUP == iCod)
        cvc = 0.e0;
      else
        cvc = faceBaseTvd(nf         ,idCell
                         ,u0
                         ,gradUv     ,gradUp
                         ,lKsi       ,lModKsi   
                         ,cv
                         ,iCod       ,ndm);
/*...................................................................*/

/*...*/
      lA[nf] = dfd - min(cv,0.e0);
      sP    += cv;                    
/*... correcao nao ortogonal e do fluxo advectivo*/        
      p     += dfdc - cv*cvc;
/*...................................................................*/
    }
/*... contorno*/
    else{
      lA[nf] = 0.0e0;
      if(lFaceR[nf]){
        wfn = velC[0]*lNormal[0] 
            + velC[1]*lNormal[1] 
            + velC[2]*lNormal[2];
/*...cargas*/
        nCarg=lFaceL[nf]-1;
        xx[0] = MAT2D(nf,0,xm,3);
        xx[1] = MAT2D(nf,1,xm,3);
        xx[2] = MAT2D(nf,2,xm,3);        
        pLoad(&sP           ,&p
             ,&tA
             ,coefDifC      ,densityC
             ,wfn           ,xx 
             ,fArea[nf]     ,dcca[nf]
             ,loads[nCarg]  ,true);
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*... distretização temporal*/
  if(fTime){
/*... EULER*/
    if(typeTime == EULER) 
      sP     = sP + densityC*volume[idCell]/dt;
/*...BACKWARD*/
    else if(typeTime == BACKWARD) 
      sP     = sP + 1.5e0*densityC*volume[idCell]/dt;
  }
/*...................................................................*/

/*...*/ 
  if(nf == 4){
    lA[idCell] = sP + lA[0] + lA[1] + lA[2] + lA[3];
  }
  else if(nf == 6){
    lA[idCell] = sP + lA[0] + lA[1] + lA[2] + lA[3] + lA[4] + lA[5];
  }

/*
  lA[idCell] = sP;
  for(nf=0;nf<nFace;nf++){
    lA[idCell] += lA[nf];
  }
*/
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
/*
  for(nf=0;nf<nFace;nf++){
   lA[nf] *= -1.e0;
  }
*/
  if(nf == 4){
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
    lA[3] *= -1.e0;
  }
  else if(nf == 6){
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
    lA[3] *= -1.e0;
    lA[4] *= -1.e0;
    lA[5] *= -1.e0;
  }
/*...................................................................*/

/*...*/
  lB[0]     = p;
  lRcell[0] = rCell;
/*...................................................................*/
}
/*********************************************************************/

