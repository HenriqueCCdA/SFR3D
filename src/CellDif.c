#include<CellLoop.h>
/********************************************************************* 
 * CELLDIF2D: Celula 2D para difusao pura                            * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
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
 * mkm       -> distacia entre o ponto medio a intersecao que une os * 
 *            centrois compartilhado nessa face da celula central    * 
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               * 
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * lRcell    -> nao definido                                         *
 * u0        -> solucao conhecida                                    * 
 * gradU0    -> gradiente rescontruido da solucao conhecida          * 
 * faceR     -> restricoes por elmento                               * 
 * faceS     -> carga por elemento                                   * 
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
void cellDif2D(short *restrict lGeomType,DOUBLE *restrict prop
              ,INT *restrict lViz       ,INT *restrict lId  
              ,DOUBLE *restrict ksi     ,DOUBLE *restrict mKsi
              ,DOUBLE *restrict eta     ,DOUBLE *restrict mEta
              ,DOUBLE *restrict normal  ,DOUBLE *restrict volume
              ,DOUBLE *restrict xm      ,DOUBLE *restrict xmcc
              ,DOUBLE *restrict dcca    ,DOUBLE *restrict mkm
              ,DOUBLE *restrict lA      ,DOUBLE *restrict lB
              ,DOUBLE *restrict lRcell                        
              ,short  *restrict lFaceR  ,DOUBLE *restrict lFaceS
              ,DOUBLE *restrict u0      ,DOUBLE *restrict gradU0
              ,const short nEn          ,short const nFace    
              ,const short ndm          ,INT const nel)
{ 

  DOUBLE coefDifC,coefDif,coefDifV,rCell;
  DOUBLE p,aP,sP,nk,dfd,dfdc,gfKsi,modE,lmkm;
  DOUBLE v[2],gradUcomp[2],lKsi[2],lNormal[2],lEta[2],gf[2];
  DOUBLE dPviz,lModKsi,lModEta,du,duDksi;
  DOUBLE gradUp[2],gradUv[2],nMinusKsi[2];
  DOUBLE alpha,alphaMenosUm;
  short idCell = nFace;
  short nAresta;
  INT vizNel;

/*... propriedades da celula*/
  coefDifC = MAT2D(idCell,0,prop,MAXPROP);
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell,0,gradU0,ndm);
  gradUp[1] = MAT2D(idCell,1,gradU0,ndm);
/*...................................................................*/

  p  = 0.0e0;
  aP = 0.0e0;
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
      lEta[0]    = MAT2D(nAresta,0,eta,ndm);
      lEta[1]    = MAT2D(nAresta,1,eta,ndm);
      lModKsi    = mKsi[nAresta];
      lModEta    = mEta[nAresta];
      lmkm       = mkm[nAresta];
      duDksi     = (u0[nAresta] - u0[idCell]) / lModKsi;
      gradUv[0]  = MAT2D(nAresta,0,gradU0,ndm);
      gradUv[1]  = MAT2D(nAresta,1,gradU0,ndm);
/*...................................................................*/

/*... produtos internos*/
      nk = lNormal[0] * lKsi[0] + lNormal[1] * lKsi[1];
/*...................................................................*/
      
/*... correcao sobre-relaxada*/
      modE       = 1.0e0/nk;
/*... correcao residuo minimo*/
//    modE       = nk;
/*...................................................................*/

/*...*/
      v[0]  = lmkm*lEta[0] + MAT2D(nAresta,0,xmcc,ndm);
      v[1]  = lmkm*lEta[1] + MAT2D(nAresta,1,xmcc,ndm);
      dPviz = sqrt(v[0]*v[0] + v[1]*v[1]);
      alpha        = dPviz/lModKsi;
      alphaMenosUm = 1.0e0 - alpha; 
/*...................................................................*/

/*... media harmonica*/
      coefDifV = MAT2D(nAresta,0,prop,MAXPROP); 
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

/*...*/
      lA[nAresta] = dfd;
      p          += dfdc;
/*...................................................................*/
    }
/*... contorno*/
    else{
      lA[nAresta] = 0.0e0;
/*... fluxo prescrito*/
      if(lFaceR[nAresta] == 0){ 
        p +=  mEta[nAresta]*lFaceS[nAresta];
      }
/*... temperatura prescrita*/
      else{
        aP  = coefDifC*mEta[nAresta]/dcca[nAresta];
        sP += aP;
        p  += aP*lFaceS[nAresta]; 
      }
    }
/*...................................................................*/
  }

/*...*/
  lA[idCell] = sP;
  for(nAresta=0;nAresta<nFace;nAresta++){
    lA[idCell] += lA[nAresta];
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
  
  for(nAresta=0;nAresta<nFace;nAresta++){
   lA[nAresta] *= -1.e0;
  }

/*...*/
  lB[0]     = p;
  lRcell[0] = rCell;
/*...................................................................*/
}
/*********************************************************************/

