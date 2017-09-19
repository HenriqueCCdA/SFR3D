#include<CellLoop.h>
/*********************************************************************
 * Biblioteca de celulas                                             *
 *-------------------------------------------------------------------*
 * Celulas 2D                                                        *
 *-------------------------------------------------------------------*
 * ------------------ INCOMPRESSIVEl ------------------------------- *
 *                                                                   *
 * CELLSIMPLEPRES2D: Celula 2D para equacao de correcao de pressoa   *
 * metodo simple em escoamento imcompressivel                        *
 *                                                                   *
 * CELLSIMPLENONORTHPRES2D: correcao nao ortogonal da Celula 2D      *
 * para equacao de correcao de pressao                               *
 *                                                                   *
 * ------------------ LEVEMENTE COMPRESSIVEl ------------------------*
 *                                                                   *
 * CELLSIMPLEPRES2D: Celula 2D para equacao de correcao de pressoa   *
 * metodo simple em escoamento imcompressivel                        *
 *                                                                   *
 * CELLSIMPLENONORTHPRES2D: correcao nao ortogonal da Celula 2D      *
 * para equacao de correcao de pressao                               *
 *                                                                   *
 *-------------------------------------------------------------------*
 * Celulas 3D                                                        *
 *-------------------------------------------------------------------*
 * ------------------ INCOMPRESSIVEl ------------------------------- *
 *                                                                   *
 * CELLSIMPLEPRES3D: Celula 3D para equacao de correcao de pressoa   *
 * metodo simple em escoamento imcompressivel                        *
 *                                                                   *
 * CELLSIMPLENONORTHPRES3D: correcao nao ortogonal da Celula 2D      *
 * para equacao de correcao de pressao                               *
 *                                                                   *
 *-------------------------------------------------------------------*
*********************************************************************/

/*********************************************************************
 * Data de criacao    : 30/06/2016                                   *
 * Data de modificaco : 15/02/2017                                   *
 *-------------------------------------------------------------------*
 * CELLSIMPLEVEL2D: Celula 2D para velocidade do metodo simple       *
 * em escoamento imcompressivel                                      *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * loadsVel  -> definicoes de cargas de velocidades                  *
 * loadsPres -> definicoes de cargas de pressao                      *
 * advVel    -> tecnica da discretizacao do termo advecao            *
 * diffVel   -> tecnica da discretizacao do termo difusivo           *
 * typeSimple-> tipo do metodo simple                                *
 * lnFace    -> numero de faces da celula central e seus vizinhos    *
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    *
 * lprop     -> propriedade fisicas das celulas                      *
 * lViz      -> viznhos da celula central                            *
 * lId       -> numeracoes das equacoes das celulas                  *
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *              vizinhos desta                                       *
 * mKsi      -> modulo do vetor ksi                                  *
 * eta       -> vetores paralelos as faces das celulas               *
 * mEta      -> modulo do vetor eta                                  *
 * normal    -> vetores normais as faces das celulas                 *
 * area      -> area da celula central                               *
 * xm        -> pontos medios das faces da celula central            *
 * xmcc      -> vetores que unem o centroide aos pontos medios das   *
 *              faces da celula central                              *
 * vSkew     -> vetor entre o ponto medio a intersecao que une os    *
 *            centrois compartilhado nessa face da celula central    *
 * mvSkew    -> distacia entre o ponto medio a intersecao que une os *
 *              centrois compartilhado nessa face da celula central  *
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               *
 * lDensity  -> massa especifica sem variacao temporal               *
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * faceVelR  -> restricoes por elemento de velocidades               *
 * faceVelL  -> carga por elemento de velocidades                    *
 * facePresR -> restricoes por elemento de pressao                   *
 * facePresL -> carga por elemento de pressao                        *
 * pres      -> campo de pressao conhecido                           *
 * gradPes   -> gradiente reconstruido da pressao                    *
 * vel       -> campo de velocidade conhecido                        *
 * gradVel   -> gradiente rescontruido das velocidades               *
 * dField    -> matriz D do metodo simple                            *
 * cc        -> centroides da celula centra e seus vizinhos          *
 * underU    -> parametro de sobre relaxamento                       *
 * nEn       -> numero de nos da celula central                      *
 * sPressure -> reconstrucao de segunda ordem para pressoes nas      *
 *              faces                                                *
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
 *                                                                   *
 * gradVel(nFace+1,ndf,ndm) -> gradVel(nFace+1,2,2)                  *
 *                                                                   *
 *                  | du1dx1 du1dx2 |                                *
 * grad(viz1,*,*) = |               |                                *
 *                  | du2dx1 du2dx2 |                                *
 *                                                                   *
 *                  | du1dx1 du1dx2 |                                *
 * grad(viz2,*,*) = |               |                                *
 *                  | du2dx1 du2dx2 |                                *
 *                                                                   *
 *                  | du1dx1 du1dx2 |                                *
 * grad(P   ,*,*) = |               |                                *
 *                  | du2dx1 du2dx2 |                                *
 *********************************************************************/
void cellSimpleVel2D(Loads *loadsVel     ,Loads *loadsPres 
            ,Advection advVel            ,Diffusion diffVel 
            ,short const typeSimple 
            ,short *RESTRICT lGeomType   ,DOUBLE *RESTRICT prop
            ,INT *RESTRICT lViz          ,INT *RESTRICT lId  
            ,DOUBLE *RESTRICT ksi        ,DOUBLE *RESTRICT mKsi
            ,DOUBLE *RESTRICT eta        ,DOUBLE *RESTRICT mEta
            ,DOUBLE *RESTRICT normal     ,DOUBLE *RESTRICT area   
            ,DOUBLE *RESTRICT xm         ,DOUBLE *RESTRICT xmcc
            ,DOUBLE *RESTRICT dcca       ,DOUBLE *RESTRICT lDensity
            ,DOUBLE *RESTRICT vSkew      ,DOUBLE *RESTRICT mvSkew
            ,DOUBLE *RESTRICT lA         ,DOUBLE *RESTRICT lB
            ,DOUBLE *RESTRICT lRcell     ,Temporal const ddt
            ,short  *RESTRICT lFaceVelR  ,short *RESTRICT lFaceVelL
            ,short  *RESTRICT lFacePresR ,short *RESTRICT lFacePresL
            ,DOUBLE *RESTRICT pres       ,DOUBLE *RESTRICT gradPres 
            ,DOUBLE *RESTRICT vel        ,DOUBLE *RESTRICT gradVel
            ,DOUBLE *RESTRICT dField     ,DOUBLE *RESTRICT cc
            ,DOUBLE const underU         ,const bool sPressure
            ,const short nEn             ,short const nFace    
            ,const short ndm             ,INT const nel)
{ 
  DOUBLE viscosityC ,viscosityV,viscosity;
  DOUBLE densityC   ,densityV  ,density;
/*...*/
  DOUBLE rCell[2],dt,dt0;
  DOUBLE p[2],sP,sPc[2];
/*...*/
  DOUBLE v[2],lKsi[2],lNormal[2],lXmcc[2],wf[2],ccV[2];
  DOUBLE dPviz,lModKsi,lModEta,du[2],duDksi[2],lXm[2];
  DOUBLE coef,lAn,tmp;
/*...*/
  DOUBLE dfd,cv,cvc[2],lvSkew[2];
/*... nonOrtogonal*/
  DOUBLE e[2],t[2],modE,dfdc[2];
/*... interpolacao linear*/
  DOUBLE alpha,alphaMenosUm;
  DOUBLE aP,tA[2];
/*... */
  DOUBLE pFace,pf[2],p1,p2;
/*... */
  DOUBLE wfn,velC[2],velV[2],presC,presV;
  DOUBLE gradPresC[2],gradPresV[2];
  DOUBLE gradVelC[2][2],gradVelV[2][2],gf[2][2],gfKsi[2];
  DOUBLE gradVelComp[2][2];
/*...*/
  short iCodAdv1 = advVel.iCod1;
  short iCodAdv2 = advVel.iCod2;
  short iCodDif  = diffVel.iCod;
/*...*/
  short idCell = nFace;
  short nAresta,nCarg,typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt[0];
  dt0      = ddt.dt[1];
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = lDensity[idCell];
/*...................................................................*/

/*... propriedades da celula*/
  viscosityC = MAT2D(idCell, DYNAMICVISCOSITY,prop,MAXPROP);
/*...................................................................*/

/*... | du1/dx1 du1/dx2*/
  gradVelC[0][0] = MAT3D(idCell,0,0,gradVel,2,ndm);
  gradVelC[0][1] = MAT3D(idCell,0,1,gradVel,2,ndm);
/*... | du2/dx1 du2/dx2*/
  gradVelC[1][0] = MAT3D(idCell,1,0,gradVel,2,ndm);
  gradVelC[1][1] = MAT3D(idCell,1,1,gradVel,2,ndm);
/*...................................................................*/

  velC[0]   = MAT2D(idCell,0,vel,ndm);
  velC[1]   = MAT2D(idCell,1,vel,ndm);

  presC         = pres[idCell];
  gradPresC[0]  = MAT2D(idCell,0,gradPres,ndm);
  gradPresC[1]  = MAT2D(idCell,1,gradPres,ndm);

/*...................................................................*/

  p[0]         = 0.0e0;
  p[1]         = 0.0e0;
  pf[0]        = 0.0e0;
  pf[1]        = 0.0e0;
  sP           = 0.0e0;
	sPc[0]       = 0.0e0;
	sPc[1]       = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    vizNel     = lViz[nAresta];
    lNormal[0] = MAT2D(nAresta,0,normal,ndm);
    lNormal[1] = MAT2D(nAresta,1,normal,ndm);
    lModEta    = mEta[nAresta];
    lXmcc[0]   = MAT2D(nAresta,0,xmcc,ndm);
    lXmcc[1]   = MAT2D(nAresta,1,xmcc,ndm);
/*... dominio*/
    if( vizNel  > -1 ){
/*...*/
      velV[0]        = MAT2D(nAresta,0,vel,ndm);
      velV[1]        = MAT2D(nAresta,1,vel,ndm);
      presV          = pres[nAresta];
      densityV       = lDensity[nAresta];
/*..*/
      lKsi[0]        = MAT2D(nAresta,0,ksi,ndm);
      lKsi[1]        = MAT2D(nAresta,1,ksi,ndm);
      lModKsi        = mKsi[nAresta];
/*..*/
      lvSkew[0]      = MAT2D(nAresta,0,vSkew,ndm);
      lvSkew[1]      = MAT2D(nAresta,1,vSkew,ndm);
/*..*/
      duDksi[0]      = (velV[0] - velC[0]) / lModKsi;
      duDksi[1]      = (velV[1] - velC[1]) / lModKsi;
/*... | du1/dx1 du1/dx2*/
      gradVelV[0][0] = MAT3D(nAresta,0,0,gradVel,2,ndm);
      gradVelV[0][1] = MAT3D(nAresta,0,1,gradVel,2,ndm);
/*... | du2/dx1 du2/dx2*/
      gradVelV[1][0] = MAT3D(nAresta,1,0,gradVel,2,ndm);
      gradVelV[1][1] = MAT3D(nAresta,1,1,gradVel,2,ndm);
/*...................................................................*/
      gradPresV[0] =  MAT2D(nAresta,0,gradPres,ndm);
      gradPresV[1] =  MAT2D(nAresta,1,gradPres,ndm);

      ccV[0]       =  MAT2D(nAresta,0,cc,ndm);
      ccV[1]       =  MAT2D(nAresta,1,cc,ndm);

      lXm[0]       =  MAT2D(nAresta,0,xm,ndm);
      lXm[1]       =  MAT2D(nAresta,1,xm,ndm);
/*...................................................................*/

/*... termo difusivo                
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/ 
      difusionScheme(lNormal ,lKsi 
                    ,lModEta ,lModKsi  
                    ,e       ,t   
                    ,ndm     ,iCodDif);
/*...................................................................*/

/*...*/
      v[0]         = lvSkew[0] + MAT2D(nAresta,0,xmcc,ndm);
      v[1]         = lvSkew[1] + MAT2D(nAresta,1,xmcc,ndm);
      dPviz        = sqrt(v[0]*v[0] + v[1]*v[1]);
      alpha        = dPviz/lModKsi;
      alphaMenosUm = 1.0e0 - alpha; 
/*...................................................................*/

/*... media harmonica*/
      viscosityV = MAT2D(nAresta, DYNAMICVISCOSITY,prop,MAXPROP);
      viscosity  = alpha/viscosityC + alphaMenosUm/viscosityV;
      viscosity  = 1.0e0/viscosity;
/*...................................................................*/

/*... difusao direta*/
      coef = viscosity;
      modE = sqrt(e[0]*e[0] + e[1]*e[1]);
      dfd  = coef*modE/lModKsi;
/*...................................................................*/
      
/*...*/
/*... | du1/dx1 du1/dx2 |*/
      gf[0][0] = alphaMenosUm*gradVelC[0][0] + alpha*gradVelV[0][0];
      gf[0][1] = alphaMenosUm*gradVelC[0][1] + alpha*gradVelV[0][1];
/*... | du2/dx1 du2/dx2* |*/
      gf[1][0] = alphaMenosUm*gradVelC[1][0] + alpha*gradVelV[1][0];
      gf[1][1] = alphaMenosUm*gradVelC[1][1] + alpha*gradVelV[1][1];
/*...*/
      wf[0]      = alphaMenosUm*velC[0] + alpha*velV[0];
      wf[1]      = alphaMenosUm*velC[1] + alpha*velV[1];
      density    = alphaMenosUm*densityC  + alpha*densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0]*lNormal[0] + wf[1]*lNormal[1];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi[0]      = gf[0][0]*lKsi[0] + gf[0][1]*lKsi[1];
/*...*/
      gfKsi[1]      = gf[1][0]*lKsi[0] + gf[1][1]*lKsi[1];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du[0]             = duDksi[0] - gfKsi[0];
      du[1]             = duDksi[1] - gfKsi[1];
/*...*/
      gradVelComp[0][0] = gf[0][0] + du[0]*lKsi[0];
      gradVelComp[0][1] = gf[0][1] + du[0]*lKsi[1];
/*...*/
      gradVelComp[1][0] = gf[1][0] + du[1]*lKsi[0];
      gradVelComp[1][1] = gf[1][1] + du[1]*lKsi[1];
/*...................................................................*/

/*... derivadas direcionais*/
/*...*/
      gfKsi[0] = gradVelComp[0][0]*t[0] 
               + gradVelComp[0][1]*t[1];
/*...*/
      gfKsi[1] = gradVelComp[1][0]*t[0] 
               + gradVelComp[1][1]*t[1];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc[0] = coef*gfKsi[0];
      dfdc[1] = coef*gfKsi[1];
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      cv   = density*wfn*lModEta;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      advectiveScheme(velC          ,velV
                     ,gradVelC[0]   ,gradVelV[0]
                     ,gradVelComp[0],lvSkew
                     ,lXmcc         ,v  
                     ,lKsi          ,lModKsi
                     ,cv            ,cvc
                     ,ndm
                     ,iCodAdv1, iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv,0.0e0);
      sP         += cv; 
/*... correcao nao ortogonal e do fluxo advectivo*/        
      p[0]       += dfdc[0] - cv*cvc[0];
      p[1]       += dfdc[1] - cv*cvc[1];
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem
      (forma conservativa)*/
      if(sPressure){
/*.. vetor que une o centroide da celula viznha ao ponto medio 
     da face*/
        v[0] = lXm[0] - ccV[0];
        v[1] = lXm[1] - ccV[1];

        p1 = presC + gradPresC[0]*lXmcc[0] + gradPresC[1]*lXmcc[1];
        p2 = presV + gradPresV[0]*v[0]     + gradPresV[1]*v[1];
     
        pFace = 0.5e0*(p1+p2);
      }
/*...................................................................*/

/*... interpolacao linear*/
      else
        pFace = alphaMenosUm*presC + alpha*presV;
/*...................................................................*/
      tmp   = pFace*lModEta;
      pf[0]+= tmp*lNormal[0];
      pf[1]+= tmp*lNormal[1];
/*...................................................................*/

/*... termos viscosos explicitos*/
      aP    = viscosity*lModEta;
      p[0] += aP*( gradVelV[0][0]*lNormal[0] 
                 + gradVelV[1][0]*lNormal[1]);
      p[1] += aP*( gradVelV[0][1]*lNormal[0] 
                 + gradVelV[1][1]*lNormal[1]);
/*...................................................................*/
    }
/*... contorno*/
    else{
      lA[nAresta] = 0.0e0;
      wfn = velC[0]*lNormal[0] + velC[1]*lNormal[1];

/*... termos viscosos explicitos*/
      aP    = viscosityC*lModEta;
      p[0] += aP*( gradVelC[0][0]*lNormal[0] 
                 + gradVelC[1][0]*lNormal[1]);
      p[1] += aP*( gradVelC[0][1]*lNormal[0] 
                 + gradVelC[1][1]*lNormal[1]);
/*...................................................................*/

/*...*/
      pFace = presC;
/*... pressao prescrita*/
      if(lFacePresR[nAresta]){
/*...cargas*/
        nCarg = lFacePresL[nAresta]-1;
        pLoadSimplePres(&sP        ,p
                       ,&pFace
                       ,viscosityC      ,densityC
                       ,wfn                 
                       ,lModEta         ,dcca[nAresta]
                       ,loadsPres[nCarg],false); 
      } 
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem*/
      if(sPressure)  
        pFace += gradPresC[0]*lXmcc[0] + gradPresC[1]*lXmcc[1];

      tmp   = pFace*lModEta;
      pf[0]+= tmp*lNormal[0];
      pf[1]+= tmp*lNormal[1];
/*...................................................................*/


/*... velocidades*/
      if(lFaceVelR[nAresta] > 0){
/*...cargas*/
        nCarg = lFaceVelL[nAresta]-1;
        pLoadSimple(sPc            ,p
                   ,tA             ,velC  
                   ,lNormal  
                   ,gradVelC[0]    ,lXmcc 
                   ,viscosityC     ,densityC
                   ,lModEta        ,dcca[nAresta]
                   ,loadsVel[nCarg],ndm
                   ,true           ,false);
      }  
/*...................................................................*/

/*... parede impermevavel*/
      else if(lFaceVelR[nAresta] == STATICWALL){
        aP    = viscosityC*lModEta/dcca[nAresta];
/*...*/
        sPc[0] += aP*(1.e0 - lNormal[0]*lNormal[0]);
        sPc[1] += aP*(1.e0 - lNormal[1]*lNormal[1]);
/*...................................................................*/

/*...*/
        p[0]   += aP*velC[1]*lNormal[1]*lNormal[0];           
        p[1]   += aP*velC[0]*lNormal[0]*lNormal[1]; 
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/
  
/*... distretizacao temporal*/
  if(fTime){
/*... EULER*/
    if(typeTime == EULER) 
      sP += densityC*area[idCell]/dt;
/*...BACKWARD*/
    else if(typeTime == BACKWARD){ 
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp*densityC*area[idCell];
    } 
  }
/*...................................................................*/

/*...*/
  lAn = 0.e0;
  if(nFace == 3){
    lAn        = lA[0] + lA[1] + lA[2];
  }
  else if(nFace == 4){
    lAn        = lA[0] + lA[1] + lA[2] + lA[3];
  }
  lA[idCell  ] = sP + sPc[0] + lAn;
	lA[idCell+1] = sP + sPc[1] + lAn;
/*...................................................................*/

/*... GradP*/
  p[0] -= pf[0];
  p[1] -= pf[1];
/*...................................................................*/

/*...*/
  rCell[0] = 0.0e0;
  rCell[1] = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    if( lViz[nAresta] > -1){
/*... pasando os valores conhecidos para o lado direito*/
      if(lId[nAresta] == -2){
        p[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        p[1] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
      }
      else{
/*residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        rCell[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
      } 
    }
  }
/*... residuo: R = F - KpUp*/ 
   rCell[0] += p[0] -lA[idCell  ]*velC[0];   
   rCell[1] += p[1] -lA[idCell+1]*velC[1];   
/*...................................................................*/

/*... under-relaxation(simple)*/
  lA[idCell]   = lA[idCell  ]/underU;
	lA[idCell+1] = lA[idCell+1]/underU;
  p[0]        += (1-underU)*lA[idCell  ]*velC[0];
  p[1]        += (1-underU)*lA[idCell+1]*velC[1];
/*...................................................................*/

/*...*/
   if(typeSimple == SIMPLEC){
    dField[0] = area[idCell]/(lA[idCell  ] - lAn);
	  dField[1] = area[idCell]/(lA[idCell+1] - lAn);
  }
  else if(typeSimple == SIMPLE){
    dField[0] = area[idCell]/lA[idCell  ];
		dField[1] = area[idCell]/lA[idCell+1];
  } 
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
  lB[0]     = p[0];
  lB[1]     = p[1];
  lRcell[0] = rCell[0];
  lRcell[1] = rCell[1];
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 20/08/2017                                   *
* Data de modificaco : 12/09/2017                                   *
*-------------------------------------------------------------------*
* CELLSIMPLEVEL2DLM: Celula 2D para velocidade do metodo simple     *
* em escoamento levemento compressivel (Low Mach)                   *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* loadsVel  -> definicoes de cargas de velocidades                  *
* loadsPres -> definicoes de cargas de pressao                      *
* advVel    -> tecnica da discretizacao do termo advecao            *
* diffVel   -> tecnica da discretizacao do termo difusivo           *
* tModel    -> modelo de turbulencia                                *
* typeSimple-> tipo do metodo simple                                *
* lnFace    -> numero de faces da celula central e seus vizinhos    *
* lGeomType -> tipo geometrico da celula central e seus vizinhos    *
* lprop     -> propriedade fisicas das celulas                      *
* lViz      -> viznhos da celula central                            *
* lId       -> numeracoes das equacoes das celulas                  *
* Ksi       -> vetores que unem centroide da celula central aos     *
*              vizinhos desta                                       *
* mKsi      -> modulo do vetor ksi                                  *
* eta       -> vetores paralelos as faces das celulas               *
* mEta      -> modulo do vetor eta                                  *
* normal    -> vetores normais as faces das celulas                 *
* area      -> area da celula central                               *
* xm        -> pontos medios das faces da celula central            *
* xmcc      -> vetores que unem o centroide aos pontos medios das   *
*              faces da celula central                              *
* vSkew     -> vetor entre o ponto medio a intersecao que une os    *
*            centrois compartilhado nessa face da celula central    *
* mvSkew    -> distacia entre o ponto medio a intersecao que une os *
*              centrois compartilhado nessa face da celula central  *
* dcca      -> menor distancia do centroide central a faces desta   *
*              celula                                               *
* lDensity  -> massa especifica sem variacao temporal               *
* lA        -> nao definido                                         *
* lB        -> nao definido                                         *
* lRcell    -> nao definido                                         *
* ddt       -> discretizacao temporal                               *
* faceVelR  -> restricoes por elemento de velocidades               *
* faceVelL  -> carga por elemento de velocidades                    *
* facePresR -> restricoes por elemento de pressao                   *
* facePresL -> carga por elemento de pressao                        *
* pres      -> campo de pressao conhecido                           *
* gradPes   -> gradiente reconstruido da pressao                    *
* vel       -> campo de velocidade conhecido                        *
* gradVel   -> gradiente rescontruido das velocidades               *
* lDensity  -> massa especifica sem variacao temporal               *
* lDviscosity-> viscosidade dinamica com variacao temporal          *
* dField    -> matriz D do metodo simple                            *
* cc        -> centroides da celula centra e seus vizinhos          *
* underU    -> parametro de sobre relaxamento                       *
* nEn       -> numero de nos da celula central                      *
* sPressure -> reconstrucao de segunda ordem para pressoes nas      *
*              faces                                                *
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
*                                                                   *
* gradVel(nFace+1,ndf,ndm) -> gradVel(nFace+1,2,2)                  *
*                                                                   *
*                  | du1dx1 du1dx2 |                                *
* grad(viz1,*,*) = |               |                                *
*                  | du2dx1 du2dx2 |                                *
*                                                                   *
*                  | du1dx1 du1dx2 |                                *
* grad(viz2,*,*) = |               |                                *
*                  | du2dx1 du2dx2 |                                *
*                                                                   *
*                  | du1dx1 du1dx2 |                                *
* grad(P   ,*,*) = |               |                                *
*                  | du2dx1 du2dx2 |                                *
*********************************************************************/
void cellSimpleVel2DLm(Loads *loadsVel   , Loads *loadsPres 
            , Advection advVel           , Diffusion diffVel 
            , Turbulence tModel          , short const typeSimple      
            , short *RESTRICT lGeomType  , DOUBLE *RESTRICT prop 
            , INT *RESTRICT lViz         , INT *RESTRICT lId 
            , DOUBLE *RESTRICT ksi       , DOUBLE *RESTRICT mKsi 
            , DOUBLE *RESTRICT eta       , DOUBLE *RESTRICT mEta 
            , DOUBLE *RESTRICT normal    , DOUBLE *RESTRICT area 
            , DOUBLE *RESTRICT xm        , DOUBLE *RESTRICT xmcc 
            , DOUBLE *RESTRICT dcca      , DOUBLE *RESTRICT cc 
            , DOUBLE *RESTRICT vSkew     , DOUBLE *RESTRICT mvSkew 
            , DOUBLE *RESTRICT lA        , DOUBLE *RESTRICT lB 
            , DOUBLE *RESTRICT lRcell    , Temporal const ddt 
            , short  *RESTRICT lFaceVelR , short *RESTRICT lFaceVelL 
            , short  *RESTRICT lFacePresR, short *RESTRICT lFacePresL 
            , DOUBLE *RESTRICT pres      , DOUBLE *RESTRICT gradPres 
            , DOUBLE *RESTRICT vel       , DOUBLE *RESTRICT gradVel 
            , DOUBLE *RESTRICT lDensity  , DOUBLE *RESTRICT lViscosity 
            , DOUBLE *RESTRICT dField      
            , DOUBLE const underU        , const bool sPressure 
            , const short nEn            , short const nFace 
            , const short ndm            , INT const nel)
{
/*...*/
  short iCodAdv1 = advVel.iCod1, iCodAdv2 = advVel.iCod2
      , iCodDif = diffVel.iCod;
/*...*/
  short idCell = nFace;
  short nAresta, nCarg, typeTime;
  bool fTime, fAbsultePressure = false, fRes = true,
       fTurb = tModel.fTurb, fGradRo = false;
  INT vizNel;
  DOUBLE viscosityC, viscosityV, viscosity, 
         eddyViscosityC, eddyViscosityV, effViscosityC, effViscosityV,
         densityC, densityV, density, densityRef;
/*...*/
  DOUBLE rCell[2], dt, dt0, p[2], sP, sPc[2], dFieldC[2], dFieldV[2],
         dFieldF[2], wf[2];
/*...*/
  DOUBLE v[2], lKsi[2], lNormal[2], lXmcc[2], ccV[2];
  DOUBLE dPviz, lModKsi, lModEta, du[2], duDksi[2], lXm[2];
  DOUBLE coef, lAn, tmp;
/*...*/
  DOUBLE dfd, cv, cvc[2], lvSkew[2];
/*... nonOrtogonal*/
  DOUBLE e[2], t[2], modE, dfdc[2];
/*... interpolacao linear*/
  DOUBLE alpha, alphaMenosUm, aP, tA[2];
/*... */
  DOUBLE pFace, pf[2], p1, p2, presC, presV, gradPresC[2], gradPresV[2];
/*... */
  DOUBLE wfn, velC[2], velV[2], g[2], gf[2][2], gfKsi[2],
         gradVelC[2][2], gradVelV[2][2], gradVelComp[2][2],gradRo[2];

/*...*/
  dt        = ddt.dt[0];  
  dt0       = ddt.dt[1];
  typeTime  = ddt.type;
  fTime     = ddt.flag;  
  g[0]      = gravity[0];
  g[1]      = gravity[1];
/*...................................................................*/

/*... propriedades da celula*/
  eddyViscosityC = eddyViscosityV = 0.0e0;
  densityC  = lDensity[idCell];
  viscosityC    = MAT2D(idCell, 0, lViscosity, 2);
  if(fTurb) eddyViscosityC= MAT2D(idCell, 1, lViscosity, 2);
  effViscosityC = viscosityC + eddyViscosityC;
  densityRef = MAT2D(idCell, DENSITY, prop, MAXPROP);
/*...................................................................*/

/*... | du1/dx1 du1/dx2*/
  gradVelC[0][0] = MAT3D(idCell, 0, 0, gradVel, 2, ndm);
  gradVelC[0][1] = MAT3D(idCell, 0, 1, gradVel, 2, ndm);
/*... | du2/dx1 du2/dx2*/
  gradVelC[1][0] = MAT3D(idCell, 1, 0, gradVel, 2, ndm);
  gradVelC[1][1] = MAT3D(idCell, 1, 1, gradVel, 2, ndm);
/*...................................................................*/

  velC[0] = MAT2D(idCell, 0, vel, ndm);
  velC[1] = MAT2D(idCell, 1, vel, ndm);
/*...*/
  presC        = pres[idCell];
  gradPresC[0] = MAT2D(idCell, 0, gradPres, ndm);
  gradPresC[1] = MAT2D(idCell, 1, gradPres, ndm);
  dFieldC[0]   = MAT2D(idCell, 0, dField, 2); 
  dFieldC[1]   = MAT2D(idCell, 1, dField, 2);
/*...................................................................*/
  sP = 0.0e0;
  p[0] = p[1] = 0.0e0;
  pf[0] = pf[1] = 0.0e0;
  sPc[0] = sPc[1] = 0.0e0;
  gradRo[0] = gradRo[1] = 0.e0;
  for (nAresta = 0; nAresta<nFace; nAresta++) {
    vizNel = lViz[nAresta];
    lNormal[0] = MAT2D(nAresta, 0, normal, ndm);
    lNormal[1] = MAT2D(nAresta, 1, normal, ndm);
    lModEta = mEta[nAresta];
    lXmcc[0] = MAT2D(nAresta, 0, xmcc, ndm);
    lXmcc[1] = MAT2D(nAresta, 1, xmcc, ndm);
/*... dominio*/
    if (vizNel  > -1) {
/*...*/
      densityV  = lDensity[nAresta];
      viscosityV     = MAT2D(nAresta, 0, lViscosity, 2);
      if(fTurb) eddyViscosityV = MAT2D(nAresta, 1, lViscosity, 2);
/*...*/
      velV[0] = MAT2D(nAresta, 0, vel, ndm);
      velV[1] = MAT2D(nAresta, 1, vel, ndm);
/*...*/
      presV        = pres[nAresta]; 
      gradPresV[0] = MAT2D(nAresta, 0, gradPres, ndm);
      gradPresV[1] = MAT2D(nAresta, 1, gradPres, ndm);
      dFieldV[0]   = MAT2D(nAresta, 0, dField, 2);
      dFieldV[1]   = MAT2D(nAresta, 1, dField, 2);
/*...*/
      lKsi[0] = MAT2D(nAresta, 0, ksi, ndm);
      lKsi[1] = MAT2D(nAresta, 1, ksi, ndm);
      lModKsi = mKsi[nAresta];
/*...*/
      lvSkew[0] = MAT2D(nAresta, 0, vSkew, ndm);
      lvSkew[1] = MAT2D(nAresta, 1, vSkew, ndm);
/*...*/
      duDksi[0] = (velV[0] - velC[0]) / lModKsi;
      duDksi[1] = (velV[1] - velC[1]) / lModKsi;
/*... | du1/dx1 du1/dx2*/
      gradVelV[0][0] = MAT3D(nAresta, 0, 0, gradVel, 2, ndm);
      gradVelV[0][1] = MAT3D(nAresta, 0, 1, gradVel, 2, ndm);
/*... | du2/dx1 du2/dx2*/
      gradVelV[1][0] = MAT3D(nAresta, 1, 0, gradVel, 2, ndm);
      gradVelV[1][1] = MAT3D(nAresta, 1, 1, gradVel, 2, ndm);
/*...................................................................*/
      gradPresV[0] = MAT2D(nAresta, 0, gradPres, ndm);
      gradPresV[1] = MAT2D(nAresta, 1, gradPres, ndm);

      ccV[0] = MAT2D(nAresta, 0, cc, ndm);
      ccV[1] = MAT2D(nAresta, 1, cc, ndm);

      lXm[0] = MAT2D(nAresta, 0, xm, ndm);
      lXm[1] = MAT2D(nAresta, 1, xm, ndm);
/*...................................................................*/

/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      difusionScheme(lNormal,lKsi
                    ,lModEta,lModKsi
                    ,e      ,t
                    ,ndm    ,iCodDif);
/*...................................................................*/

/*...*/
      v[0] = lvSkew[0] + MAT2D(nAresta, 0, xmcc, ndm);
      v[1] = lvSkew[1] + MAT2D(nAresta, 1, xmcc, ndm);
      dPviz = sqrt(v[0] * v[0] + v[1] * v[1]);
      alpha = dPviz / lModKsi;
      alphaMenosUm = 1.0e0 - alpha;
/*...................................................................*/

/*... media harmonica*/
      effViscosityV = viscosityV + eddyViscosityV;
      viscosity = alpha / effViscosityC + alphaMenosUm / effViscosityV;
      viscosity = 1.0e0 / viscosity;
/*...................................................................*/

/*... difusao direta*/
      coef = viscosity;
      modE = sqrt(e[0] * e[0] + e[1] * e[1]);
      dfd = coef*modE / lModKsi;
/*...................................................................*/

/*...*/
/*... | du1/dx1 du1/dx2 |*/
      gf[0][0] = alphaMenosUm*gradVelC[0][0] + alpha*gradVelV[0][0];
      gf[0][1] = alphaMenosUm*gradVelC[0][1] + alpha*gradVelV[0][1];
/*... | du2/dx1 du2/dx2* |*/
      gf[1][0] = alphaMenosUm*gradVelC[1][0] + alpha*gradVelV[1][0];
      gf[1][1] = alphaMenosUm*gradVelC[1][1] + alpha*gradVelV[1][1];
/*...*/
      wf[0]      = alphaMenosUm*velC[0] + alpha*velV[0];
      wf[1]      = alphaMenosUm*velC[1] + alpha*velV[1];
//    dFieldF[0] = alphaMenosUm*dFieldC[0] + alpha*dFieldV[0];
//    dFieldF[1] = alphaMenosUm*dFieldC[1] + alpha*dFieldV[1];
      density = alphaMenosUm*densityC + alpha*densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0]*lNormal[0] + wf[1]*lNormal[1];
/*    wfn = interpolFaceVel(velC         ,velV
                           ,presC        ,presV
                           ,gradPresC    ,gradPresV
                           ,lNormal      ,lKsi
                           ,lModKsi      ,dFieldF
                           ,alphaMenosUm ,alpha
                           ,ndm); */
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi[0] = gf[0][0] * lKsi[0] + gf[0][1] * lKsi[1];
/*...*/
      gfKsi[1] = gf[1][0] * lKsi[0] + gf[1][1] * lKsi[1];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du[0] = duDksi[0] - gfKsi[0];
      du[1] = duDksi[1] - gfKsi[1];
/*...*/
      gradVelComp[0][0] = gf[0][0] + du[0] * lKsi[0];
      gradVelComp[0][1] = gf[0][1] + du[0] * lKsi[1];
/*...*/
      gradVelComp[1][0] = gf[1][0] + du[1] * lKsi[0];
      gradVelComp[1][1] = gf[1][1] + du[1] * lKsi[1];
/*...................................................................*/

/*... derivadas direcionais*/
/*...*/
      gfKsi[0] = gradVelComp[0][0] * t[0]
        + gradVelComp[0][1] * t[1];
/*...*/
      gfKsi[1] = gradVelComp[1][0] * t[0]
        + gradVelComp[1][1] * t[1];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc[0] = coef*gfKsi[0];
      dfdc[1] = coef*gfKsi[1];
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      cv = density*wfn*lModEta;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      advectiveScheme(velC, velV
        , gradVelC[0], gradVelV[0]
        , gradVelComp[0], lvSkew
        , lXmcc, v
        , lKsi, lModKsi
        , cv, cvc
        , ndm
        , iCodAdv1, iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv, 0.0e0);
      sP += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p[0] += dfdc[0] - cv*cvc[0];
      p[1] += dfdc[1] - cv*cvc[1];
/*...................................................................*/

/*... gradiente da massa especifica */
      if(fGradRo){
        tmp = density*lModEta;
        gradRo[0] +=  tmp*lNormal[0];
        gradRo[1] +=  tmp*lNormal[1];
      }
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem
      (forma conservativa)*/
      if (sPressure) {
/*.. vetor que une o centroide da celula viznha ao ponto medio
        da face*/
        v[0] = lXm[0] - ccV[0];
        v[1] = lXm[1] - ccV[1];

        p1 = presC + gradPresC[0] * lXmcc[0] + gradPresC[1] * lXmcc[1];
        p2 = presV + gradPresV[0] * v[0] + gradPresV[1] * v[1];

        pFace = 0.5e0*(p1 + p2);
      }
/*...................................................................*/

/*... interpolacao linear*/
      else
        pFace = alphaMenosUm*presC + alpha*presV;
/*...................................................................*/
      tmp = pFace*lModEta;
      pf[0] += tmp*lNormal[0];
      pf[1] += tmp*lNormal[1];
/*...................................................................*/

/*... termos viscosos explicitos*/
      aP = viscosity*lModEta;
      p[0] += aP*(gradVelV[0][0] * lNormal[0]
                + gradVelV[1][0] * lNormal[1]);
      p[1] += aP*(gradVelV[0][1] * lNormal[0]
                + gradVelV[1][1] * lNormal[1]);
/*... divergente*/
      tmp = D2DIV3*aP*(gradVelV[0][0] + gradVelV[1][1]);
      p[0] -= tmp*lNormal[0];
      p[1] -= tmp*lNormal[1];
/*...................................................................*/
    }
/*... contorno*/
    else {
      lA[nAresta] = 0.0e0;
      wfn = velC[0] * lNormal[0] + velC[1] * lNormal[1];

/*... termos viscosos explicitos*/
      aP = effViscosityC*lModEta;
      p[0] += aP*(gradVelC[0][0] * lNormal[0]
                + gradVelC[1][0] * lNormal[1]);
      p[1] += aP*(gradVelC[0][1] * lNormal[0]
                + gradVelC[1][1] * lNormal[1]);
/*... divergente*/
      tmp = D2DIV3*aP*(gradVelC[0][0] + gradVelC[1][1]);
      p[0] -= tmp*lNormal[0];
      p[1] -= tmp*lNormal[1];
/*...................................................................*/

/*... gradiente da massa especifica */
      if(fGradRo){
        tmp = densityC*lModEta;
        gradRo[0] += tmp*lNormal[0];
        gradRo[1] += tmp*lNormal[1];
      }
/*...................................................................*/

/*...*/
      pFace = presC;
/*... pressao prescrita*/
      if (lFacePresR[nAresta]) {
/*...cargas*/
        nCarg = lFacePresL[nAresta] - 1;
        pLoadSimplePres(&sP, p
                        ,&pFace
                        ,effViscosityC, densityC
                        ,wfn
                        ,lModEta, dcca[nAresta]
                        ,loadsPres[nCarg], false);
      }
 /*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem*/
      if (sPressure)
        pFace += gradPresC[0] * lXmcc[0] + gradPresC[1] * lXmcc[1];

      tmp = pFace*lModEta;
      pf[0] += tmp*lNormal[0];
      pf[1] += tmp*lNormal[1];
/*...................................................................*/

/*... velocidades*/
      if (lFaceVelR[nAresta] > 0) {
/*...cargas*/
        nCarg = lFaceVelL[nAresta] - 1;
        pLoadSimple(sPc            , p,
                    tA             , velC,
                    lNormal,
                    gradVelC[0]    , lXmcc,
                    effViscosityC   , densityC,
                    lModEta        , dcca[nAresta],
                    loadsVel[nCarg], ndm,
                    true           , false);
      }
/*...................................................................*/

/*... parede impermevavel*/
      else if (lFaceVelR[nAresta] == STATICWALL) {
        aP = effViscosityC*lModEta / dcca[nAresta];
/*...*/
        sPc[0] += aP*(1.e0 - lNormal[0] * lNormal[0]);
        sPc[1] += aP*(1.e0 - lNormal[1] * lNormal[1]);
/*...................................................................*/

/*...*/
        p[0] += aP*velC[1] * lNormal[1] * lNormal[0];
        p[1] += aP*velC[0] * lNormal[0] * lNormal[1];
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*...*/
  if(fAbsultePressure){
    tmp   = densityC*area[idCell];
    p[0] += tmp*g[0];
    p[1] += tmp*g[1];
  }
  else if (fGradRo) {
    ccV[0] = MAT2D(idCell,0,cc,ndm);
    ccV[1] = MAT2D(idCell,1,cc,ndm);
    tmp = ccV[0]*g[0] + ccV[1]*g[1];
    if( nel == 9899 || nel == 9799){
       printf("nel %d %lf %lf %lf %lf\n",nel,densityC,tmp,gradRo[1],p[1]);
    }
    p[0] -= tmp*gradRo[0];
    p[1] -= tmp*gradRo[1];
  }
  else {
    tmp   = (densityC-densityRef)*area[idCell];  
    p[0] += tmp*g[0];
    p[1] += tmp*g[1];
  }
/*...................................................................*/

/*... distretizacao temporal*/
  if (fTime) {
/*... EULER*/
    if (typeTime == EULER)
      sP += densityC*area[idCell] / dt;
/*...BACKWARD*/
    else if (typeTime == BACKWARD) {
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp*densityC*area[idCell];
    }
  }
/*...................................................................*/

/*...*/
  lAn = 0.e0;
  if (nFace == 3) {
    lAn = lA[0] + lA[1] + lA[2];
  }
  else if (nFace == 4) {
    lAn = lA[0] + lA[1] + lA[2] + lA[3];
  }
  lA[idCell] = sP + sPc[0] + lAn;
  lA[idCell + 1] = sP + sPc[1] + lAn;
/*...................................................................*/

/*... GradP*/
  p[0] -= pf[0];
  p[1] -= pf[1];
/*...................................................................*/

  /*...*/
  rCell[0] = 0.0e0;
  rCell[1] = 0.0e0;
  for (nAresta = 0; nAresta<nFace; nAresta++) {
    if (lViz[nAresta] > -1) {
/*... pasando os valores conhecidos para o lado direito*/
      if (lId[nAresta] == -2) {
        p[0] += lA[nAresta] * MAT2D(nAresta, 0, vel, ndm);
        p[1] += lA[nAresta] * MAT2D(nAresta, 0, vel, ndm);
      }
      else {
/*residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell[0] += lA[nAresta] * MAT2D(nAresta, 0, vel, ndm);
        rCell[1] += lA[nAresta] * MAT2D(nAresta, 1, vel, ndm);
      }
    }
  }
/*... residuo: R = F - KpUp*/
  rCell[0] += p[0] - lA[idCell] * velC[0];
  rCell[1] += p[1] - lA[idCell + 1] * velC[1];
/*...................................................................*/

/*... under-relaxation(simple)*/
  lA[idCell] = lA[idCell] / underU;
  lA[idCell + 1] = lA[idCell + 1] / underU;
  if(!fRes){
    p[0] += (1 - underU)*lA[idCell] * velC[0];
    p[1] += (1 - underU)*lA[idCell + 1] * velC[1];
  }
/*...................................................................*/

/*...*/
  if (typeSimple == SIMPLEC) {
    MAT2D(idCell,0,dField,2) = area[idCell] / (lA[idCell] - lAn);
    MAT2D(idCell,1,dField,2) = area[idCell] / (lA[idCell + 1] - lAn);
  }
  else if (typeSimple == SIMPLE) {
    MAT2D(idCell,0,dField,2) = area[idCell] / lA[idCell];
    MAT2D(idCell,1,dField,2) = area[idCell] / lA[idCell + 1];
  }
/*...................................................................*/

/*...*/
  if (nFace == 3) {
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
  }
  else if (nFace == 4) {
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
    lA[3] *= -1.e0;
  }
/*...................................................................*/

/*...*/
  if(fRes){
    lB[0]      = rCell[0];
    lB[1]      = rCell[1];
  }
  else{
    lB[0]      = p[1];
    lB[1]      = p[1];
  }
  lRcell[0] = rCell[0];
  lRcell[1] = rCell[1];
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 05/09/2016                                   *
* Data de modificaco : 00/00/0000                                   *
*-------------------------------------------------------------------*
* CELLVELEXP2D: Celula 2D para velocidade do metodo simple          *
* em escoamento imcompressivel (Explicito)                          *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* loadsVel  -> definicoes de cargas de velocidades                  *
* loadsPres -> definicoes de cargas de pressao                      *
* advVel    -> tecnica da discretizacao do termo advecao            *
* diffVel   -> tecnica da discretizacao do termo difusivo           *
* lnFace    -> numero de faces da celula central e seus vizinhos    *
* lGeomType -> tipo geometrico da celula central e seus vizinhos    *
* lprop     -> propriedade fisicas das celulas                      *
* lViz      -> viznhos da celula central                            *
* lId       -> numeracoes das equacoes das celulas                  *
* Ksi       -> vetores que unem centroide da celula central aos     *
*              vizinhos desta                                       *
* mKsi      -> modulo do vetor ksi                                  *
* eta       -> vetores paralelos as faces das celulas               *
* mEta      -> modulo do vetor eta                                  *
* normal    -> vetores normais as faces das celulas                 *
* area      -> area da celula central                               *
* xm        -> pontos medios das faces da celula central            *
* xmcc      -> vetores que unem o centroide aos pontos medios das   *
*              faces da celula central                              *
* vSkew     -> vetor entre o ponto medio a intersecao que une os    *
*            centrois compartilhado nessa face da celula central    *
* mvSkew    -> distacia entre o ponto medio a intersecao que une os *
*              centrois compartilhado nessa face da celula central  *
* dcca      -> menor distancia do centroide central a faces desta   *
*              celula                                               *
* lDensity  -> massa especifica sem variacao temporal               *
* lB        -> nao definido                                         *
* ddt       -> discretizacao temporal                               *
* faceVelR  -> restricoes por elemento de velocidades               *
* faceVelL  -> carga por elemento de velocidades                    *
* facePresR -> restricoes por elemento de pressao                   *
* facePresL -> carga por elemento de pressao                        *
* pres      -> campo de pressao conhecido                           *
* gradPes   -> gradiente reconstruido da pressao                    *
* vel       -> campo de velocidade conhecido                        *
* gradVel   -> gradiente rescontruido das velocidades               *
* dField    -> matriz D do metodo simple                            *
* cc        -> centroides da celula centra e seus vizinhos          *
* bT        -> parte da discretizacao temporal                      *
* underU    -> parametro de sobre relaxamento                       *
* nEn       -> numero de nos da celula central                      *
* sPressure -> reconstrucao de segunda ordem para pressoes nas      *
*              faces                                                *
* fResidual -> calculo do residuo                                   *
* nEn       -> numero de nos da celula central                      *
* nFace     -> numero de faces da celula central                    *
* ndm       -> numero de dimensoes                                  *
* nel       -> numero da celula                                     *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
* lB        -> velocidade (calculo explicito) ou residuo            *
*-------------------------------------------------------------------*
* OBS:                                                              *
*-------------------------------------------------------------------*
*                                                                   *
* gradVel(nFace+1,ndf,ndm) -> gradVel(nFace+1,2,2)                  *
*                                                                   *
*                  | du1dx1 du1dx2 |                                *
* grad(viz1,*,*) = |               |                                *
*                  | du2dx1 du2dx2 |                                *
*                                                                   *
*                  | du1dx1 du1dx2 |                                *
* grad(viz2,*,*) = |               |                                *
*                  | du2dx1 du2dx2 |                                *
*                                                                   *
*                  | du1dx1 du1dx2 |                                *
* grad(P   ,*,*) = |               |                                *
*                  | du2dx1 du2dx2 |                                *
*                                                                   *
*********************************************************************/
void cellVelExp2D(Loads *loadsVel    ,Loads *loadsPres
               ,Advection advVel           ,Diffusion diffVel
               ,short *RESTRICT lGeomType  ,DOUBLE *RESTRICT prop
               ,INT *RESTRICT lViz         
               ,DOUBLE *RESTRICT ksi       ,DOUBLE *RESTRICT mKsi
               ,DOUBLE *RESTRICT eta       ,DOUBLE *RESTRICT mEta
               ,DOUBLE *RESTRICT normal    ,DOUBLE *RESTRICT area
               ,DOUBLE *RESTRICT xm        ,DOUBLE *RESTRICT xmcc
               ,DOUBLE *RESTRICT dcca      ,DOUBLE *RESTRICT lDensity
               ,DOUBLE *RESTRICT vSkew     ,DOUBLE *RESTRICT mvSkew
               ,DOUBLE *RESTRICT lB        ,Temporal const ddt
               ,short  *RESTRICT lFaceVelR ,short *RESTRICT lFaceVelL
               ,short  *RESTRICT lFacePresR,short *RESTRICT lFacePresL
               ,DOUBLE *RESTRICT pres      ,DOUBLE *RESTRICT gradPres
               ,DOUBLE *RESTRICT vel       ,DOUBLE *RESTRICT gradVel
               ,DOUBLE *RESTRICT dField    ,DOUBLE *RESTRICT cc
               ,DOUBLE *RESTRICT bT        ,DOUBLE const underU
               ,const bool sPressure       ,const bool fResidual
               ,const short nEn            ,short const nFace
               ,const short ndm            ,INT const nel)
{
  DOUBLE viscosityC, viscosityV, viscosity;
  DOUBLE densityC, densityV, density;
/*...*/
  DOUBLE dt, dt0;
  DOUBLE p[2], sP, sPc[2],lA[(MAX_NUM_FACE + 1)*MAX_NDF],velExp[2];
/*...*/
  DOUBLE v[2], lKsi[2], lNormal[2], lXmcc[2], wf[2], ccV[2];
  DOUBLE dPviz, lModKsi, lModEta, du[2], duDksi[2], lXm[2];
  DOUBLE coef, lAn, tmp;
/*...*/
  DOUBLE dfd, cv, cvc[2], lvSkew[2];
/*... nonOrtogonal*/
  DOUBLE e[2], t[2], modE, dfdc[2];
/*... interpolacao linear*/
  DOUBLE alpha, alphaMenosUm;
  DOUBLE aP, tA[2];
/*... */
  DOUBLE pFace, pf[2], p1, p2;
/*... */
  DOUBLE wfn, velC[2], velV[2], presC, presF;
  DOUBLE gradPresC[2], gradPresV[2];
  DOUBLE gradVelC[2][2], gradVelV[2][2], gf[2][2], gfKsi[2];
  DOUBLE gradVelComp[2][2];
/*...*/
  short iCodAdv1 = advVel.iCod1;
  short iCodAdv2 = advVel.iCod2;
  short iCodDif = diffVel.iCod;
/*...*/
  short idCell = nFace;
  short nAresta, nCarg, typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt[0];
  dt0      = ddt.dt[1];
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = lDensity[idCell];
/*...................................................................*/

/*... propriedades da celula*/
  viscosityC = MAT2D(idCell, DYNAMICVISCOSITY, prop, MAXPROP);
/*...................................................................*/

/*... | du1/dx1 du1/dx2*/
  gradVelC[0][0] = MAT3D(idCell, 0, 0, gradVel, 2, ndm);
  gradVelC[0][1] = MAT3D(idCell, 0, 1, gradVel, 2, ndm);
/*... | du2/dx1 du2/dx2*/
  gradVelC[1][0] = MAT3D(idCell, 1, 0, gradVel, 2, ndm);
  gradVelC[1][1] = MAT3D(idCell, 1, 1, gradVel, 2, ndm);
/*...................................................................*/

  velC[0] = MAT2D(idCell, 0, vel, ndm);
  velC[1] = MAT2D(idCell, 1, vel, ndm);

  presC = pres[idCell];
  gradPresC[0] = MAT2D(idCell, 0, gradPres, ndm);
  gradPresC[1] = MAT2D(idCell, 1, gradPres, ndm);
/*...................................................................*/

  p[0] = 0.0e0;
  p[1] = 0.0e0;
  pf[0] = 0.0e0;
  pf[1] = 0.0e0;
  sP = 0.0e0;
  sPc[0] = 0.0e0;
  sPc[1] = 0.0e0;
  for (nAresta = 0; nAresta<nFace; nAresta++) {
    vizNel = lViz[nAresta];
    lNormal[0] = MAT2D(nAresta, 0, normal, ndm);
    lNormal[1] = MAT2D(nAresta, 1, normal, ndm);
    lModEta = mEta[nAresta];
    lXmcc[0] = MAT2D(nAresta, 0, xmcc, ndm);
    lXmcc[1] = MAT2D(nAresta, 1, xmcc, ndm);
/*... dominio*/
    if (vizNel  > -1) {
/*...*/
      velV[0] = MAT2D(nAresta, 0, vel, ndm);
      velV[1] = MAT2D(nAresta, 1, vel, ndm);
      presF = pres[nAresta];
      densityV = lDensity[nAresta];
/*...*/
      lKsi[0] = MAT2D(nAresta, 0, ksi, ndm);
      lKsi[1] = MAT2D(nAresta, 1, ksi, ndm);
      lModKsi = mKsi[nAresta];
/*...*/
      lvSkew[0] = MAT2D(nAresta, 0, vSkew, ndm);
      lvSkew[1] = MAT2D(nAresta, 1, vSkew, ndm);
/*...*/
      duDksi[0] = (velV[0] - velC[0]) / lModKsi;
      duDksi[1] = (velV[1] - velC[1]) / lModKsi;
/*... | du1/dx1 du1/dx2*/
      gradVelV[0][0] = MAT3D(nAresta, 0, 0, gradVel, 2, ndm);
      gradVelV[0][1] = MAT3D(nAresta, 0, 1, gradVel, 2, ndm);
/*... | du2/dx1 du2/dx2*/
      gradVelV[1][0] = MAT3D(nAresta, 1, 0, gradVel, 2, ndm);
      gradVelV[1][1] = MAT3D(nAresta, 1, 1, gradVel, 2, ndm);
/*...................................................................*/
      gradPresV[0] = MAT2D(nAresta, 0, gradPres, ndm);
      gradPresV[1] = MAT2D(nAresta, 1, gradPres, ndm);

      ccV[0] = MAT2D(nAresta, 0, cc, ndm);
      ccV[1] = MAT2D(nAresta, 1, cc, ndm);

      lXm[0] = MAT2D(nAresta, 0, xm, ndm);
      lXm[1] = MAT2D(nAresta, 1, xm, ndm);
/*...................................................................*/

/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      difusionScheme(lNormal, lKsi
                     , lModEta, lModKsi
                     , e, t
                     , ndm, iCodDif);
/*...................................................................*/

/*...*/
      v[0] = lvSkew[0] + MAT2D(nAresta, 0, xmcc, ndm);
      v[1] = lvSkew[1] + MAT2D(nAresta, 1, xmcc, ndm);
      dPviz = sqrt(v[0] * v[0] + v[1] * v[1]);
      alpha = dPviz / lModKsi;
      alphaMenosUm = 1.0e0 - alpha;
/*...................................................................*/

/*... media harmonica*/
      viscosityV = MAT2D(nAresta, DYNAMICVISCOSITY, prop, MAXPROP);
      viscosity = alpha / viscosityC + alphaMenosUm / viscosityV;
      viscosity = 1.0e0 / viscosity;
/*...................................................................*/

/*... difusao direta*/
      coef = viscosity;
      modE = sqrt(e[0] * e[0] + e[1] * e[1]);
      dfd = coef*modE / lModKsi;
/*...................................................................*/

/*...*/
/*... | du1/dx1 du1/dx2 |*/
      gf[0][0] = alphaMenosUm*gradVelC[0][0] + alpha*gradVelV[0][0];
      gf[0][1] = alphaMenosUm*gradVelC[0][1] + alpha*gradVelV[0][1];
/*... | du2/dx1 du2/dx2* |*/
      gf[1][0] = alphaMenosUm*gradVelC[1][0] + alpha*gradVelV[1][0];
      gf[1][1] = alphaMenosUm*gradVelC[1][1] + alpha*gradVelV[1][1];
/*...*/
      wf[0] = alphaMenosUm*velC[0] + alpha*velV[0];
      wf[1] = alphaMenosUm*velC[1] + alpha*velV[1];
      density = alphaMenosUm*densityC + alpha*densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0] * lNormal[0] + wf[1] * lNormal[1];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi[0] = gf[0][0] * lKsi[0] + gf[0][1] * lKsi[1];
/*...*/
      gfKsi[1] = gf[1][0] * lKsi[0] + gf[1][1] * lKsi[1];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du[0] = duDksi[0] - gfKsi[0];
      du[1] = duDksi[1] - gfKsi[1];
/*...*/
      gradVelComp[0][0] = gf[0][0] + du[0] * lKsi[0];
      gradVelComp[0][1] = gf[0][1] + du[0] * lKsi[1];
/*...*/
      gradVelComp[1][0] = gf[1][0] + du[1] * lKsi[0];
      gradVelComp[1][1] = gf[1][1] + du[1] * lKsi[1];
/*...................................................................*/

/*... derivadas direcionais*/
/*...*/
      gfKsi[0] = gradVelComp[0][0] * t[0]
        + gradVelComp[0][1] * t[1];
/*...*/
      gfKsi[1] = gradVelComp[1][0] * t[0]
        + gradVelComp[1][1] * t[1];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc[0] = coef*gfKsi[0];
      dfdc[1] = coef*gfKsi[1];
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      cv = density*wfn*lModEta;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      advectiveScheme(velC, velV
                      , gradVelC[0], gradVelV[0]
                      , gradVelComp[0], lvSkew
                      , lXmcc, v
                      , lKsi, lModKsi
                      , cv, cvc
                      , ndm
                      , iCodAdv1, iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv, 0.0e0);
      sP += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p[0] += dfdc[0] - cv*cvc[0];
      p[1] += dfdc[1] - cv*cvc[1];
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem
      (forma conservativa)*/
      if (sPressure) {
/*.. vetor que une o centroide da celula viznha ao ponto medio
        da face*/
        v[0] = lXm[0] - ccV[0];
        v[1] = lXm[1] - ccV[1];

        p1 = presC + gradPresC[0] * lXmcc[0] + gradPresC[1] * lXmcc[1];
        p2 = presF + gradPresV[0] * v[0] + gradPresV[1] * v[1];

        pFace = 0.5e0*(p1 + p2);
      }
/*...................................................................*/

/*... interpolacao linear*/
      else
        pFace = alphaMenosUm*presC + alpha*presF;
/*...................................................................*/
      pf[0] += pFace*lModEta*lNormal[0];
      pf[1] += pFace*lModEta*lNormal[1];
/*...................................................................*/

/*... termos viscosos explicitos*/
      aP = viscosity*lModEta;
      p[0] += aP*(gradVelV[0][0] * lNormal[0]
                  + gradVelV[1][0] * lNormal[1]);
      p[1] += aP*(gradVelV[0][1] * lNormal[0]
                  + gradVelV[1][1] * lNormal[1]);
/*...................................................................*/
    }
/*... contorno*/
    else {
      lA[nAresta] = 0.0e0;
      wfn = velC[0] * lNormal[0] + velC[1] * lNormal[1];

/*... termos viscosos explicitos*/
      aP = viscosityC*lModEta;
      p[0] += aP*(gradVelC[0][0] * lNormal[0]
                  + gradVelC[1][0] * lNormal[1]);
      p[1] += aP*(gradVelC[0][1] * lNormal[0]
                  + gradVelC[1][1] * lNormal[1]);
/*...................................................................*/

/*...*/
      pFace = presC;
/*... pressao prescrita*/
      if (lFacePresR[nAresta]) {
/*...cargas*/
        nCarg = lFacePresL[nAresta] - 1;
        pLoadSimplePres(&sP, p
                        , &pFace
                        , viscosityC, densityC
                        , wfn
                        , lModEta, dcca[nAresta]
                        , loadsPres[nCarg], false);
      }
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem*/
      if (sPressure)
        pFace += gradPresC[0] * lXmcc[0] + gradPresC[1] * lXmcc[1];
      pf[0] += pFace*lModEta*lNormal[0];
      pf[1] += pFace*lModEta*lNormal[1];
/*...................................................................*/

/*... velocidades*/
      if (lFaceVelR[nAresta] > 0) {
/*...cargas*/
        nCarg = lFaceVelL[nAresta] - 1;
        pLoadSimple(sPc, p
                    , tA, velC
                    , lNormal
                    , gradVelC[0], lXmcc
                    , viscosityC, densityC
                    , lModEta, dcca[nAresta]
                    , loadsVel[nCarg], ndm
                    , true, false);
      }
/*...................................................................*/

/*... parede impermevavel*/
      else if (lFaceVelR[nAresta] == STATICWALL) {
        aP = viscosityC*lModEta / dcca[nAresta];
/*...*/
        sPc[0] += aP*(1.e0 - lNormal[0] * lNormal[0]);
        sPc[1] += aP*(1.e0 - lNormal[1] * lNormal[1]);
/*...................................................................*/

/*...*/
        p[0] += aP*velC[1] * lNormal[1] * lNormal[0];
        p[1] += aP*velC[0] * lNormal[0] * lNormal[1];
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... distretizacao temporal*/
  if (fTime) {
    p[0] += bT[0];
    p[1] += bT[1];
/*... EULER*/
    if (typeTime == EULER){
      sP += densityC*area[idCell] / dt;
    }
/*...................................................................*/

/*...BACKWARD*/
    else if (typeTime == BACKWARD) {
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp*densityC*area[idCell];
    }
  }
/*...................................................................*/

/*...*/
  lAn = 0.e0;
  if (nFace == 3) {
    lAn = lA[0] + lA[1] + lA[2];
  }
  else if (nFace == 4) {
    lAn = lA[0] + lA[1] + lA[2] + lA[3];
  }
  lA[idCell  ] = sP + sPc[0] + lAn;
  lA[idCell+1] = sP + sPc[1] + lAn;
/*...................................................................*/

/*... GradP*/
  p[0] -= pf[0];
  p[1] -= pf[1];
/*...................................................................*/

/*... calculo do residuo*/
  if(fResidual){
    for (nAresta = 0; nAresta<nFace; nAresta++) {
      if (lViz[nAresta] > -1) {
        p[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        p[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
      }
    }
/*... residuo: R = F - KpUp*/
    lB[0] = p[0] - lA[idCell  ]*velC[0];
    lB[1] = p[1] - lA[idCell+1]*velC[1];
/*...................................................................*/

/*...*/
    lB[2] = lA[idCell  ];
    lB[3] = lA[idCell+1];
/*...................................................................*/
  }
/*...................................................................*/

/*... atualizacao explicita*/
  else{
    for (nAresta = 0; nAresta<nFace; nAresta++) {
      if (lViz[nAresta] > -1) {
        p[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        p[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
      }
    }
/*... velocida explicita: vC = (-H[vF] - grad(P))/ac*/
    velExp[0] = p[0] / lA[idCell];
    velExp[1] = p[1] / lA[idCell + 1];
/*...................................................................*/

/*... under-relaxation(prime)*/
    lB[0] = velC[0] + underU*(velExp[0] - velC[0]);
    lB[1] = velC[1] + underU*(velExp[1] - velC[1]);
/*...................................................................*/

/*...*/
    dField[0] = area[idCell] / lA[idCell];
    dField[1] = area[idCell] / lA[idCell + 1];
/*...................................................................*/

  }
/*...................................................................*/

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 11/07/2016                                   *
 * Data de modificaco : 15/08/2016                                   * 
 *-------------------------------------------------------------------* 
 * CELLSIMPLEVE3D: Celula 3D para velocidade do metodo simple        * 
 * em escoamento imcompressivel                                      * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * loadsVel  -> definicoes de cargas de velocidades                  * 
 * loadsPres -> definicoes de cargas de pressao                      * 
 * advVel    -> tecnica da discretizacao do termo advecao            *
 * diffVel   -> tecnica da discretizacao do termo difusivo           *
 * typeSimple-> tipo do metodo simple                                *
 * lnFace    -> numero de faces da celula central e seus vizinhos    * 
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    * 
 * lprop     -> propriedade fisicas das celulas                      * 
 * lViz      -> viznhos da celula central                            * 
 * lId       -> numeracoes das equacoes das celulas                  * 
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *              vizinhos desta                                       * 
 * mKsi      -> modulo do vetor ksi                                  * 
 * eta       -> vetores paralelos as faces das celulas               * 
 * fArea     -> area da face                                         * 
 * normal    -> vetores normais as faces das celulas                 * 
 * volume    -> volume da celula central                             * 
 * xm        -> pontos medios das faces da celula central            * 
 * xmcc      -> vetores que unem o centroide aos pontos medios das   * 
 *              faces da celula central                              * 
 * vSkew     -> vetor entre o ponto medio a intersecao que une os    * 
 *            centrois compartilhado nessa face da celula central    * 
 * mvSkew    -> distacia entre o ponto medio a intersecao que une os * 
 *              centrois compartilhado nessa face da celula central  * 
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               * 
 * lDensity  -> massa especifica sem variacao temporal               * 
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * faceVelR  -> restricoes por elemento de velocidades               * 
 * faceVelL  -> carga por elemento de velocidades                    * 
 * facePresR -> restricoes por elemento de pressao                   * 
 * facePresL -> carga por elemento de pressao                        * 
 * pres      -> campo de pressao conhecido                           * 
 * gradPes   -> gradiente reconstruido da pressao                    * 
 * vel       -> campo de velocidade conhecido                        * 
 * gradVel   -> gradiente rescontruido das velocidades               * 
 * dField    -> matriz D do metodo simple                            * 
 * cc        -> centroides da celula centra e seus vizinhos          * 
 * nEn       -> numero de nos da celula central                      *
 * underU    -> parametro de sobre relaxamento                       * 
 * sPressure -> reconstrucao de segunda ordem para pressoes nas      *
 *              faces                                                *
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
 *                                                                   *  
 * gradVel(nFace+1,ndf,ndm) -> gradVel(nFace+1,2,2)                  * 
 *                                                                   *  
 *                  | du1dx1 du1dx2 du1dx3 |                         *
 * grad(viz1,*,*) = | du2dx1 du2dx2 du2dx3 |                         *
 *                  | du3dx1 du3dx2 du3dx3 |                         *
 *                                                                   *  
 *                  | du1dx1 du1dx2 du1dx3 |                         *
 * grad(viz2,*,*) = | du2dx1 du2dx2 du2dx3 |                         *
 *                  | du3dx1 du3dx2 du3dx3 |                         *
 *                                                                   *  
 *                  | du1dx1 du1dx2 du1dx3 |                         *
 * grad(P   ,*,*) = | du2dx1 du2dx2 du2dx3 |                         *
 *                  | du3dx1 du3dx2 du3dx3 |                         *
 *                                                                   *  
 *********************************************************************/
void cellSimpleVel3D(Loads *loadsVel     ,Loads *loadsPres 
            ,Advection advVel            ,Diffusion diffVel
            ,short const typeSimple 
            ,short *RESTRICT lGeomType   ,DOUBLE *RESTRICT prop
            ,INT *RESTRICT lViz          ,INT *RESTRICT lId  
            ,DOUBLE *RESTRICT ksi        ,DOUBLE *RESTRICT mKsi
            ,DOUBLE *RESTRICT eta        ,DOUBLE *RESTRICT fArea
            ,DOUBLE *RESTRICT normal     ,DOUBLE *RESTRICT volume 
            ,DOUBLE *RESTRICT xm         ,DOUBLE *RESTRICT xmcc
            ,DOUBLE *RESTRICT dcca       ,DOUBLE *RESTRICT lDensity
            ,DOUBLE *RESTRICT vSkew      ,DOUBLE *RESTRICT mvSkew
            ,DOUBLE *RESTRICT lA         ,DOUBLE *RESTRICT lB
            ,DOUBLE *RESTRICT lRcell     ,Temporal const ddt
            ,short  *RESTRICT lFaceVelR  ,short *RESTRICT lFaceVelL
            ,short  *RESTRICT lFacePresR ,short *RESTRICT lFacePresL
            ,DOUBLE *RESTRICT pres       ,DOUBLE *RESTRICT gradPres 
            ,DOUBLE *RESTRICT vel        ,DOUBLE *RESTRICT gradVel
            ,DOUBLE *RESTRICT dField     ,DOUBLE *RESTRICT cc
            ,DOUBLE const underU         ,const bool sPressure
            ,const short nEn             ,short const nFace    
            ,const short ndm             ,INT const nel)
{ 
  DOUBLE viscosityC ,viscosityV,viscosity;
  DOUBLE densityC   ,densityV  ,density;
/*...*/
  DOUBLE rCell[3],dt,dt0;
  DOUBLE p[3],sP,sPc[3];
/*...*/
  DOUBLE v[3],lKsi[3],lNormal[3],lXmcc[3],wf[3],ccV[3];
  DOUBLE dPviz,lModKsi,lFarea,du[3],duDksi[3],lXm[3];
  DOUBLE coef,lAn,tmp;
/*...*/
  DOUBLE dfd,cv,cvc[3],lvSkew[3];
/*... nonOrtogonal*/
	DOUBLE e[3], t[3], modE, dfdc[3];
/*... interpolacao linear*/
  DOUBLE alpha,alphaMenosUm;
  DOUBLE aP,tA[3];
/*... */
  DOUBLE pFace,pf[3],p1,p2;
/*... */
  DOUBLE wfn,velC[3],velV[3],presC,presF;
  DOUBLE gradPresC[3],gradPresV[3];
  DOUBLE gradVelC[3][3],gradVelV[3][3],gf[3][3],gfKsi[3];
  DOUBLE gradVelComp[3][3];
/*...*/
  short iCodAdv1= advVel.iCod1;
  short iCodAdv2= advVel.iCod2;
	short iCodDif = diffVel.iCod;
/*...*/
  short idCell = nFace;
  short nAresta,nCarg,typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt[0];
  dt0      = ddt.dt[1];
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = lDensity[idCell];
/*...................................................................*/

/*... propriedades da celula*/
  viscosityC = MAT2D(idCell, DYNAMICVISCOSITY,prop,MAXPROP);
/*...................................................................*/

/*... | du1/dx1 du1/dx2 du1/dx3*/
  gradVelC[0][0] = MAT3D(idCell,0,0,gradVel,3,ndm);
  gradVelC[0][1] = MAT3D(idCell,0,1,gradVel,3,ndm);
  gradVelC[0][2] = MAT3D(idCell,0,2,gradVel,3,ndm);
/*... | du2/dx1 du2/dx2 du2/dx3*/
  gradVelC[1][0] = MAT3D(idCell,1,0,gradVel,3,ndm);
  gradVelC[1][1] = MAT3D(idCell,1,1,gradVel,3,ndm);
  gradVelC[1][2] = MAT3D(idCell,1,2,gradVel,3,ndm);
/*... | du3/dx1 du3/dx2 du3/dx3*/
  gradVelC[2][0] = MAT3D(idCell,2,0,gradVel,3,ndm);
  gradVelC[2][1] = MAT3D(idCell,2,1,gradVel,3,ndm);
  gradVelC[2][2] = MAT3D(idCell,2,2,gradVel,3,ndm);
/*...................................................................*/

  velC[0]   = MAT2D(idCell,0,vel,ndm);
  velC[1]   = MAT2D(idCell,1,vel,ndm);
  velC[2]   = MAT2D(idCell,2,vel,ndm);

  presC         = pres[idCell];
  gradPresC[0]  = MAT2D(idCell,0,gradPres,ndm);
  gradPresC[1]  = MAT2D(idCell,1,gradPres,ndm);
  gradPresC[2]  = MAT2D(idCell,2,gradPres,ndm);
/*...................................................................*/

  p[0]   = 0.0e0;
  p[1]   = 0.0e0;
  p[2]   = 0.0e0;
  pf[0]  = 0.0e0;
  pf[1]  = 0.0e0;
  pf[2]  = 0.0e0;
  sP     = 0.0e0;
  sPc[0] = 0.0e0;
  sPc[1] = 0.0e0;
  sPc[2] = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    vizNel     = lViz[nAresta];
    lNormal[0] = MAT2D(nAresta,0,normal,ndm);
    lNormal[1] = MAT2D(nAresta,1,normal,ndm);
    lNormal[2] = MAT2D(nAresta,2,normal,ndm);
    lFarea     = fArea[nAresta];
    lXmcc[0]   = MAT2D(nAresta,0,xmcc,ndm);
    lXmcc[1]   = MAT2D(nAresta,1,xmcc,ndm);
    lXmcc[2]   = MAT2D(nAresta,2,xmcc,ndm);
/*... dominio*/
    if( vizNel  > -1 ){
/*...*/
      velV[0]        = MAT2D(nAresta,0,vel,ndm);
      velV[1]        = MAT2D(nAresta,1,vel,ndm);
      velV[2]        = MAT2D(nAresta,2,vel,ndm);
      presF          = pres[nAresta];
      densityV       = lDensity[nAresta];
/*..*/
      lKsi[0]        = MAT2D(nAresta,0,ksi,ndm);
      lKsi[1]        = MAT2D(nAresta,1,ksi,ndm);
      lKsi[2]        = MAT2D(nAresta,2,ksi,ndm);
      lModKsi        = mKsi[nAresta];
/*..*/
      lvSkew[0]      = MAT2D(nAresta,0,vSkew,ndm);
      lvSkew[1]      = MAT2D(nAresta,1,vSkew,ndm);
      lvSkew[2]      = MAT2D(nAresta,2,vSkew,ndm);
/*..*/
      duDksi[0]      = (velV[0] - velC[0]) / lModKsi;
      duDksi[1]      = (velV[1] - velC[1]) / lModKsi;
      duDksi[2]      = (velV[2] - velC[2]) / lModKsi;
/*... | du1/dx1 du1/dx2 du1/dx3*/
      gradVelV[0][0] = MAT3D(nAresta,0,0,gradVel,3,ndm);
      gradVelV[0][1] = MAT3D(nAresta,0,1,gradVel,3,ndm);
      gradVelV[0][2] = MAT3D(nAresta,0,2,gradVel,3,ndm);
/*... | du2/dx1 du2/dx2 du2/dx3*/
      gradVelV[1][0] = MAT3D(nAresta,1,0,gradVel,3,ndm);
      gradVelV[1][1] = MAT3D(nAresta,1,1,gradVel,3,ndm);
      gradVelV[1][2] = MAT3D(nAresta,1,2,gradVel,3,ndm);
/*... | du3/dx1 du3/dx2 du3/dx3*/
      gradVelV[2][0] = MAT3D(nAresta,2,0,gradVel,3,ndm);
      gradVelV[2][1] = MAT3D(nAresta,2,1,gradVel,3,ndm);
      gradVelV[2][2] = MAT3D(nAresta,2,2,gradVel,3,ndm);
/*...................................................................*/
      gradPresV[0] =  MAT2D(nAresta,0,gradPres,ndm);
      gradPresV[1] =  MAT2D(nAresta,1,gradPres,ndm);
      gradPresV[2] =  MAT2D(nAresta,2,gradPres,ndm);
 
      ccV[0]       =  MAT2D(nAresta,0,cc,ndm);
      ccV[1]       =  MAT2D(nAresta,1,cc,ndm);
      ccV[2]       =  MAT2D(nAresta,2,cc,ndm);
 
      lXm[0]       =  MAT2D(nAresta,0,xm,ndm);
      lXm[1]       =  MAT2D(nAresta,1,xm,ndm);
      lXm[2]       =  MAT2D(nAresta,2,xm,ndm);
/*...................................................................*/

/*... termo difusivo
grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
			difusionScheme(lNormal,lKsi
			            	,lFarea ,lModKsi
				            ,e      ,t
				            ,ndm    ,iCodDif);
/*...................................................................*/

/*...*/
      v[0]         = lvSkew[0] + MAT2D(nAresta,0,xmcc,ndm);
      v[1]         = lvSkew[1] + MAT2D(nAresta,1,xmcc,ndm);
      v[2]         = lvSkew[2] + MAT2D(nAresta,2,xmcc,ndm);
      dPviz        = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
      alpha        = dPviz/lModKsi;
      alphaMenosUm = 1.0e0 - alpha; 
/*...................................................................*/

/*... media harmonica*/
      viscosityV = MAT2D(nAresta, DYNAMICVISCOSITY,prop,MAXPROP);
      viscosity  = alpha/viscosityC + alphaMenosUm/viscosityV;
      viscosity  = 1.0e0/viscosity;
/*...................................................................*/

/*... difusao direta*/
			coef = viscosity;
			modE = sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
			dfd  = coef*modE/lModKsi;
/*...................................................................*/
      
/*...*/
      gf[0][0] = alphaMenosUm*gradVelC[0][0] + alpha*gradVelV[0][0];
      gf[0][1] = alphaMenosUm*gradVelC[0][1] + alpha*gradVelV[0][1];
      gf[0][2] = alphaMenosUm*gradVelC[0][2] + alpha*gradVelV[0][2];
/*...*/
      gf[1][0] = alphaMenosUm*gradVelC[1][0] + alpha*gradVelV[1][0];
      gf[1][1] = alphaMenosUm*gradVelC[1][1] + alpha*gradVelV[1][1];
      gf[1][2] = alphaMenosUm*gradVelC[1][2] + alpha*gradVelV[1][2];
/*...*/
      gf[2][0] = alphaMenosUm*gradVelC[2][0] + alpha*gradVelV[2][0];
      gf[2][1] = alphaMenosUm*gradVelC[2][1] + alpha*gradVelV[2][1];
      gf[2][2] = alphaMenosUm*gradVelC[2][2] + alpha*gradVelV[2][2];
/*...*/
      wf[0]    =   alphaMenosUm*velC[0] + alpha*velV[0];
      wf[1]    =   alphaMenosUm*velC[1] + alpha*velV[1];
      wf[2]    =   alphaMenosUm*velC[2] + alpha*velV[2];
      density  = alphaMenosUm*densityC  + alpha*densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0]*lNormal[0] + wf[1]*lNormal[1] + wf[2]*lNormal[2];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi[0]      = gf[0][0]*lKsi[0] 
                    + gf[0][1]*lKsi[1] 
                    + gf[0][2]*lKsi[2];
/*...*/
      gfKsi[1]      = gf[1][0]*lKsi[0] 
                    + gf[1][1]*lKsi[1] 
                    + gf[1][2]*lKsi[2];
/*...*/
      gfKsi[2]      = gf[2][0]*lKsi[0] 
                    + gf[2][1]*lKsi[1] 
                    + gf[2][2]*lKsi[2];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du[0]             = duDksi[0] - gfKsi[0];
      du[1]             = duDksi[1] - gfKsi[1];
      du[2]             = duDksi[2] - gfKsi[2];
/*...*/
      gradVelComp[0][0] = gf[0][0] + du[0]*lKsi[0];
      gradVelComp[0][1] = gf[0][1] + du[0]*lKsi[1];
      gradVelComp[0][2] = gf[0][2] + du[0]*lKsi[2];
/*...*/
      gradVelComp[1][0] = gf[1][0] + du[1]*lKsi[0];
      gradVelComp[1][1] = gf[1][1] + du[1]*lKsi[1];
      gradVelComp[1][2] = gf[1][2] + du[1]*lKsi[2];
/*...*/
      gradVelComp[2][0] = gf[2][0] + du[2]*lKsi[0];
      gradVelComp[2][1] = gf[2][1] + du[2]*lKsi[1];
      gradVelComp[2][2] = gf[2][2] + du[2]*lKsi[2];
/*...................................................................*/

/*... derivadas direcionais*/
/*...*/
      gfKsi[0] = gradVelComp[0][0]*t[0] 
               + gradVelComp[0][1]*t[1] 
               + gradVelComp[0][2]*t[2];
/*...*/
      gfKsi[1] = gradVelComp[1][0]*t[0] 
               + gradVelComp[1][1]*t[1] 
               + gradVelComp[1][2]*t[2];
/*...*/
      gfKsi[2] = gradVelComp[2][0]*t[0] 
               + gradVelComp[2][1]*t[1] 
               + gradVelComp[2][2]*t[2];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc[0] = coef*gfKsi[0];
      dfdc[1] = coef*gfKsi[1];
      dfdc[2] = coef*gfKsi[2];
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      cv   = density*wfn*lFarea;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      v[2] = lXm[2] - ccV[2];
      advectiveScheme(velC          ,velV
                     ,gradVelC[0]   ,gradVelV[0]
                     ,gradVelComp[0],lvSkew
                     ,lXmcc         ,v  
                     ,lKsi          ,lModKsi
                     ,cv            ,cvc
                     ,ndm
                     ,iCodAdv1      ,iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv,0.0e0);
      sP         += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p[0]       += dfdc[0] - cv*cvc[0];
      p[1]       += dfdc[1] - cv*cvc[1];
      p[2]       += dfdc[2] - cv*cvc[2];
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem
      (forma conservativa)*/
      if(sPressure){
/*.. vetor que une o centroide da celula viznha ao ponto medio 
     da face*/
        v[0] = lXm[0] - ccV[0];
        v[1] = lXm[1] - ccV[1];
        v[2] = lXm[2] - ccV[2];

        p1 = presC + gradPresC[0]*lXmcc[0] 
                   + gradPresC[1]*lXmcc[1] 
                   + gradPresC[2]*lXmcc[2];

        p2 = presF + gradPresV[0]*v[0]
                   + gradPresV[1]*v[1] 
                   + gradPresV[2]*v[2];
     
        pFace = 0.5e0*(p1+p2);
      }
/*...................................................................*/

/*... interpolacao linear*/
      else
        pFace = alphaMenosUm*presC + alpha*presF;
/*...................................................................*/
      tmp   = pFace*lFarea;
      pf[0]+= tmp*lNormal[0];
      pf[1]+= tmp*lNormal[1];
      pf[2]+= tmp*lNormal[2];
/*...................................................................*/

/*... termos viscosos explicitos*/
      aP    = viscosity*lFarea;
      p[0] += aP*( gradVelV[0][0]*lNormal[0] 
                 + gradVelV[1][0]*lNormal[1]  
                 + gradVelV[2][0]*lNormal[2]);
 
      p[1] += aP*( gradVelV[0][1]*lNormal[0] 
                 + gradVelV[1][1]*lNormal[1]  
                 + gradVelV[2][1]*lNormal[2]);
      
      p[2] += aP*( gradVelV[0][2]*lNormal[0] 
                 + gradVelV[1][2]*lNormal[1]  
                 + gradVelV[2][2]*lNormal[2]);
/*...................................................................*/
    }
/*... contorno*/
    else{
      lA[nAresta] = 0.0e0;
      wfn = velC[0]*lNormal[0] 
          + velC[1]*lNormal[1] 
          + velC[2]*lNormal[2];

/*... termos viscosos explicitos*/
      aP    = viscosityC*lFarea;
      p[0] += aP*( gradVelC[0][0]*lNormal[0] 
                 + gradVelC[1][0]*lNormal[1]  
                 + gradVelC[2][0]*lNormal[2]);

      p[1] += aP*( gradVelC[0][1]*lNormal[0] 
                 + gradVelC[1][1]*lNormal[1]
                 + gradVelC[2][1]*lNormal[2]);
      
      p[2] += aP*( gradVelC[0][2]*lNormal[0] 
                 + gradVelC[1][2]*lNormal[1]
                 + gradVelC[2][2]*lNormal[2]);
/*...................................................................*/

/*...*/
      pFace = presC;
/*... pressao prescrita*/
      if(lFacePresR[nAresta]){
/*...cargas*/
        nCarg = lFacePresL[nAresta]-1;
        pLoadSimplePres(&sP             ,p
                       ,&pFace
                       ,viscosityC      ,densityC
                       ,wfn                 
                       ,lFarea          ,dcca[nAresta]
                       ,loadsPres[nCarg],false); 
      } 
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem*/
      if(sPressure)  
        pFace += gradPresC[0]*lXmcc[0] 
               + gradPresC[1]*lXmcc[1]
               + gradPresC[2]*lXmcc[2];

      tmp   = pFace*lFarea;
      pf[0]+= tmp*lNormal[0];
      pf[1]+= tmp*lNormal[1];
      pf[2]+= tmp*lNormal[2];
/*...................................................................*/


/*... velocidades*/
      if(lFaceVelR[nAresta] > 0){
/*...cargas*/
        nCarg = lFaceVelL[nAresta]-1;
        pLoadSimple(sPc            ,p
                   ,tA             ,velC  
                   ,lNormal  
                   ,gradVelC[0]    ,lXmcc 
                   ,viscosityC     ,densityC
                   ,lFarea         ,dcca[nAresta]
                   ,loadsVel[nCarg],ndm
                   ,true           ,false);
      }  
/*...................................................................*/

/*... parede impermevavel*/
      else if(lFaceVelR[nAresta] == STATICWALL){
        aP   = viscosityC*lFarea /dcca[nAresta];
/*...*/
        sPc[0] += aP*(1.e0 - lNormal[0]*lNormal[0]);
        sPc[1] += aP*(1.e0 - lNormal[1]*lNormal[1]);
        sPc[2] += aP*(1.e0 - lNormal[2]*lNormal[2]);
/*...................................................................*/

/*... x*/
        p[0]+= aP*(velC[1]*lNormal[0]*lNormal[1]
                  +velC[2]*lNormal[0]*lNormal[2]);
/*... y*/
        p[1]+= aP*(velC[0]*lNormal[0]*lNormal[1]
                  +velC[2]*lNormal[1]*lNormal[2]);
/*... z*/
        p[2]+= aP*(velC[0]*lNormal[0]*lNormal[2]
                  +velC[1]*lNormal[1]*lNormal[2]);
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
    else if(typeTime == BACKWARD){
      tmp = 1.e0/dt + 1.e0/(dt+dt0);
      sP += tmp*densityC*volume[idCell];
    }
  }
/*...................................................................*/


/*...*/
  lAn = 0.e0;
  if(nFace == 4){
    lAn = lA[0] + lA[1] + lA[2] + lA[3];
  }
  else if(nFace == 6){
    lAn = lA[0] + lA[1] + lA[2] + lA[3] + lA[4] + lA[5];
  }
  lA[idCell  ] = sP + sPc[0] + lAn;
  lA[idCell+1] = sP + sPc[1] + lAn;
  lA[idCell+2] = sP + sPc[2] + lAn;
/*...................................................................*/

/*... under-relaxation(simple)*/
  lA[idCell]   = lA[idCell  ]/underU;
  lA[idCell+1] = lA[idCell+1]/underU;
  lA[idCell+2] = lA[idCell+2]/underU;
  p[0]        += (1-underU)*lA[idCell  ]*velC[0];
  p[1]        += (1-underU)*lA[idCell+1]*velC[1];
  p[2]        += (1-underU)*lA[idCell+2]*velC[2];
/*...................................................................*/

/*... GradP*/
  p[0] -= pf[0];
  p[1] -= pf[1];
  p[2] -= pf[2];
/*...................................................................*/

/*...*/
  rCell[0] = 0.0e0;
  rCell[1] = 0.0e0;
  rCell[2] = 0.0e0;
  for(nAresta=0;nAresta<nFace;nAresta++){
    if( lViz[nAresta] > -1){
/*... pasando os valores conhecidos para o lado direito*/
      if(lId[nAresta] == -2){
        p[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        p[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
        p[2] += lA[nAresta]*MAT2D(nAresta,2,vel,ndm);
      }
      else{
/*residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        rCell[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
        rCell[2] += lA[nAresta]*MAT2D(nAresta,2,vel,ndm);
      } 
    }
  }
/*... residuo: R = F - KpUp*/ 
   rCell[0] += p[0] -lA[idCell  ]*velC[0];
   rCell[1] += p[1] -lA[idCell+1]*velC[1];
   rCell[2] += p[2] -lA[idCell+2]*velC[2];
/*...................................................................*/

/*...*/
   if(typeSimple == SIMPLEC){ 
    dField[0] = volume[idCell]/(lA[idCell  ] - lAn);
    dField[1] = volume[idCell]/(lA[idCell+1] - lAn);
    dField[2] = volume[idCell]/(lA[idCell+2] - lAn);
  }
  else if(typeSimple == SIMPLE){ 
    dField[0] = volume[idCell]/lA[idCell];
    dField[1] = volume[idCell]/lA[idCell+1];
    dField[2] = volume[idCell]/lA[idCell+2];
  } 
/*...................................................................*/

/*...*/
  if(nFace == 4){
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
    lA[3] *= -1.e0;
  }
  else if(nFace == 6){
    lA[0] *= -1.e0;
    lA[1] *= -1.e0;
    lA[2] *= -1.e0;
    lA[3] *= -1.e0;
    lA[4] *= -1.e0;
    lA[5] *= -1.e0;
  }
/*...................................................................*/


/*...*/
  lB[0]     = p[0];
  lB[1]     = p[1];
  lB[2]     = p[2];
  lRcell[0] = rCell[0];
  lRcell[1] = rCell[1];
  lRcell[2] = rCell[2];
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 11/07/2016                                   *
 * Data de modificaco : 15/08/2016                                   *
 *-------------------------------------------------------------------*
 * CELLSIMPLEPRES3D: Celula 3D para equacao de correcao de pressoa   *
 * metodo simple em escoamento imcompressivel                        * 
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * loadsVel  -> definicoes de cargas de velocidades                  *
 * loadsPres -> definicoes de cargas de pressao                      *
 * advVel    -> tecnica da discretizacao do termo advecao            *
 * diffVel   -> tecnica da discretizacao do termo difusivo           *
 * typeSimple-> tipo do metodo simple                                *
 * lnFace    -> numero de faces da celula central e seus vizinhos    *
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    *
 * lprop     -> propriedade fisicas das celulas                      *
 * lViz      -> viznhos da celula central                            *
 * lId       -> numeracoes das equacoes das celulas                  *
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *              vizinhos desta                                       *
 * mKsi      -> modulo do vetor ksi                                  *
 * eta       -> vetores paralelos as faces das celulas               *
 * fArea     -> area da face                                         *
 * normal    -> vetores normais as faces das celulas                 *
 * volume    -> volume da celula central                             *
 * xm        -> pontos medios das faces da celula central            *
 * xmcc      -> vetores que unem o centroide aos pontos medios das   *
 *              faces da celula central                              *
 * vSkew     -> vetor entre o ponto medio a intersecao que une os    *
 *            centrois compartilhado nessa face da celula central    *
 * mvSkew    -> distacia entre o ponto medio a intersecao que une os *
 *              centrois compartilhado nessa face da celula central  *
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               *
 * lDensity  -> massa especifica sem variacao temporal               *
 * lB        -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * faceVelR  -> restricoes por elemento de velocidades               *
 * faceVelL  -> carga por elemento de velocidades                    *
 * facePresR -> restricoes por elemento de pressao                   *
 * facePresL -> carga por elemento de pressao                        *
 * pres      -> campo de pressao conhecido                           *
 * gradPes   -> gradiente reconstruido da pressao                    *
 * vel       -> campo de velocidade conhecido                        *
 * gradVel   -> gradiente rescontruido das velocidades               *
 * dField    -> matriz D do metodo simple                            *
 * cc        -> centroides da celula centra e seus vizinhos          *
 * bT        -> parte da discretizacao temporal                      *
 * underU    -> parametro de sobre relaxamento                       *
 * nEn       -> numero de nos da celula central                      *
 * sPressure -> reconstrucao de segunda ordem para pressoes nas      *
 *              faces                                                *
 * fResidual -> calculo do residuo                                   *
 * nEn       -> numero de nos da celula central                      *
 * nFace     -> numero de faces da celula central                    *
 * ndm       -> numero de dimensoes                                  *
 * nel       -> numero da celula                                     *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * lB        -> velocidade (calculo explicito) ou residuo            *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *                                                                   *
 * gradVel(nFace+1,ndf,ndm) -> gradVel(nFace+1,2,2)                  *
 *                                                                   *
 *                  | du1dx1 du1dx2 du1dx3 |                         *
 * grad(viz1,*,*) = | du2dx1 du2dx2 du2dx3 |                         *
 *                  | du3dx1 du3dx2 du3dx3 |                         *
 *                                                                   *
 *                  | du1dx1 du1dx2 du1dx3 |                         *
 * grad(viz2,*,*) = | du2dx1 du2dx2 du2dx3 |                         *
 *                  | du3dx1 du3dx2 du3dx3 |                         *
 *                                                                   *
 *                  | du1dx1 du1dx2 du1dx3 |                         *
 * grad(P   ,*,*) = | du2dx1 du2dx2 du2dx3 |                         *
 *                  | du3dx1 du3dx2 du3dx3 |                         *
 *                                                                   *
 *********************************************************************/
void cellVelExp3D(Loads *loadsVel            ,Loads *loadsPres
                 ,Advection advVel           ,Diffusion diffVel
                 ,short *RESTRICT lGeomType  ,DOUBLE *RESTRICT prop
                 ,INT *RESTRICT lViz         
                 ,DOUBLE *RESTRICT ksi       ,DOUBLE *RESTRICT mKsi
                 ,DOUBLE *RESTRICT eta       ,DOUBLE *RESTRICT fArea
                 ,DOUBLE *RESTRICT normal    ,DOUBLE *RESTRICT volume
                 ,DOUBLE *RESTRICT xm        ,DOUBLE *RESTRICT xmcc
                 ,DOUBLE *RESTRICT dcca      ,DOUBLE *RESTRICT lDensity
                 ,DOUBLE *RESTRICT vSkew     ,DOUBLE *RESTRICT mvSkew
                 ,DOUBLE *RESTRICT lB        ,Temporal const ddt
                 ,short  *RESTRICT lFaceVelR , short *RESTRICT lFaceVelL
                 ,short  *RESTRICT lFacePresR, short *RESTRICT lFacePresL
                 ,DOUBLE *RESTRICT pres      ,DOUBLE *RESTRICT gradPres
                 ,DOUBLE *RESTRICT vel       ,DOUBLE *RESTRICT gradVel
                 ,DOUBLE *RESTRICT dField    ,DOUBLE *RESTRICT cc
                 ,DOUBLE *RESTRICT bT        ,DOUBLE const underU        
                 ,const bool sPressure       ,const bool fResidual
                 ,const short nEn            ,short const nFace
                 ,const short ndm            ,INT const nel)
{
  DOUBLE viscosityC, viscosityV, viscosity;
  DOUBLE densityC, densityV, density;
/*...*/
  DOUBLE dt, dt0;
  DOUBLE p[3],sP,sPc[3],lA[(MAX_NUM_FACE+1)*MAX_NDF],velExp[3];
/*...*/
  DOUBLE v[3], lKsi[3], lNormal[3], lXmcc[3], wf[3], ccV[3];
  DOUBLE dPviz, lModKsi, lFarea, du[3], duDksi[3], lXm[3];
  DOUBLE coef, lAn, tmp;
/*...*/
  DOUBLE dfd, cv, cvc[3], lvSkew[3];
/*... nonOrtogonal*/
  DOUBLE e[3], t[3], modE, dfdc[3];
/*... interpolacao linear*/
  DOUBLE alpha, alphaMenosUm;
  DOUBLE aP, tA[3];
/*... */
  DOUBLE pFace, pf[3], p1, p2;
/*... */
  DOUBLE wfn, velC[3], velV[3], presC, presF;
  DOUBLE gradPresC[3], gradPresV[3];
  DOUBLE gradVelC[3][3], gradVelV[3][3], gf[3][3], gfKsi[3];
  DOUBLE gradVelComp[3][3];
/*...*/
  short iCodAdv1 = advVel.iCod1;
  short iCodAdv2 = advVel.iCod2;
  short iCodDif = diffVel.iCod;
/*...*/
  short idCell = nFace;
  short nAresta, nCarg, typeTime;
  INT vizNel;
  bool fTime;

/*...*/
  dt       = ddt.dt[0];
  dt0      = ddt.dt[1];
  typeTime = ddt.type;
  fTime    = ddt.flag;
  densityC = lDensity[idCell];
/*...................................................................*/

/*... propriedades da celula*/
  viscosityC = MAT2D(idCell, DYNAMICVISCOSITY, prop, MAXPROP);
/*...................................................................*/

/*... | du1/dx1 du1/dx2 du1/dx3*/
  gradVelC[0][0] = MAT3D(idCell, 0, 0, gradVel, 3, ndm);
  gradVelC[0][1] = MAT3D(idCell, 0, 1, gradVel, 3, ndm);
  gradVelC[0][2] = MAT3D(idCell, 0, 2, gradVel, 3, ndm);
/*... | du2/dx1 du2/dx2 du2/dx3*/
  gradVelC[1][0] = MAT3D(idCell, 1, 0, gradVel, 3, ndm);
  gradVelC[1][1] = MAT3D(idCell, 1, 1, gradVel, 3, ndm);
  gradVelC[1][2] = MAT3D(idCell, 1, 2, gradVel, 3, ndm);
/*... | du3/dx1 du3/dx2 du3/dx3*/
  gradVelC[2][0] = MAT3D(idCell, 2, 0, gradVel, 3, ndm);
  gradVelC[2][1] = MAT3D(idCell, 2, 1, gradVel, 3, ndm);
  gradVelC[2][2] = MAT3D(idCell, 2, 2, gradVel, 3, ndm);
/*...................................................................*/

  velC[0] = MAT2D(idCell, 0, vel, ndm);
  velC[1] = MAT2D(idCell, 1, vel, ndm);
  velC[2] = MAT2D(idCell, 2, vel, ndm);

  presC = pres[idCell];
  gradPresC[0] = MAT2D(idCell, 0, gradPres, ndm);
  gradPresC[1] = MAT2D(idCell, 1, gradPres, ndm);
  gradPresC[2] = MAT2D(idCell, 2, gradPres, ndm);
/*...................................................................*/

  p[0] = 0.0e0;
  p[1] = 0.0e0;
  p[2] = 0.0e0;
  pf[0] = 0.0e0;
  pf[1] = 0.0e0;
  pf[2] = 0.0e0;
  sP = 0.0e0;
  sPc[0] = 0.0e0;
  sPc[1] = 0.0e0;
  sPc[2] = 0.0e0;
  for (nAresta = 0; nAresta<nFace; nAresta++) {
    vizNel = lViz[nAresta];
    lNormal[0] = MAT2D(nAresta, 0, normal, ndm);
    lNormal[1] = MAT2D(nAresta, 1, normal, ndm);
    lNormal[2] = MAT2D(nAresta, 2, normal, ndm);
    lFarea = fArea[nAresta];
    lXmcc[0] = MAT2D(nAresta, 0, xmcc, ndm);
    lXmcc[1] = MAT2D(nAresta, 1, xmcc, ndm);
    lXmcc[2] = MAT2D(nAresta, 2, xmcc, ndm);
/*... dominio*/
    if (vizNel  > -1) {
/*...*/
      velV[0] = MAT2D(nAresta, 0, vel, ndm);
      velV[1] = MAT2D(nAresta, 1, vel, ndm);
      velV[2] = MAT2D(nAresta, 2, vel, ndm);
      presF = pres[nAresta];
      densityV = lDensity[nAresta];
/*...*/
      lKsi[0] = MAT2D(nAresta, 0, ksi, ndm);
      lKsi[1] = MAT2D(nAresta, 1, ksi, ndm);
      lKsi[2] = MAT2D(nAresta, 2, ksi, ndm);
      lModKsi = mKsi[nAresta];
/*...*/
      lvSkew[0] = MAT2D(nAresta, 0, vSkew, ndm);
      lvSkew[1] = MAT2D(nAresta, 1, vSkew, ndm);
      lvSkew[2] = MAT2D(nAresta, 2, vSkew, ndm);
/*...*/
      duDksi[0] = (velV[0] - velC[0]) / lModKsi;
      duDksi[1] = (velV[1] - velC[1]) / lModKsi;
      duDksi[2] = (velV[2] - velC[2]) / lModKsi;
/*... | du1/dx1 du1/dx2 du1/dx3*/
      gradVelV[0][0] = MAT3D(nAresta, 0, 0, gradVel, 3, ndm);
      gradVelV[0][1] = MAT3D(nAresta, 0, 1, gradVel, 3, ndm);
      gradVelV[0][2] = MAT3D(nAresta, 0, 2, gradVel, 3, ndm);
/*... | du2/dx1 du2/dx2 du2/dx3*/
      gradVelV[1][0] = MAT3D(nAresta, 1, 0, gradVel, 3, ndm);
      gradVelV[1][1] = MAT3D(nAresta, 1, 1, gradVel, 3, ndm);
      gradVelV[1][2] = MAT3D(nAresta, 1, 2, gradVel, 3, ndm);
/*... | du3/dx1 du3/dx2 du3/dx3*/
      gradVelV[2][0] = MAT3D(nAresta, 2, 0, gradVel, 3, ndm);
      gradVelV[2][1] = MAT3D(nAresta, 2, 1, gradVel, 3, ndm);
      gradVelV[2][2] = MAT3D(nAresta, 2, 2, gradVel, 3, ndm);
/*...................................................................*/
      gradPresV[0] = MAT2D(nAresta, 0, gradPres, ndm);
      gradPresV[1] = MAT2D(nAresta, 1, gradPres, ndm);
      gradPresV[2] = MAT2D(nAresta, 2, gradPres, ndm);

      ccV[0] = MAT2D(nAresta, 0, cc, ndm);
      ccV[1] = MAT2D(nAresta, 1, cc, ndm);
      ccV[2] = MAT2D(nAresta, 2, cc, ndm);

      lXm[0] = MAT2D(nAresta, 0, xm, ndm);
      lXm[1] = MAT2D(nAresta, 1, xm, ndm);
      lXm[2] = MAT2D(nAresta, 2, xm, ndm);
/*...................................................................*/

/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      difusionScheme(lNormal, lKsi
                     , lFarea, lModKsi
                     , e, t
                     , ndm, iCodDif);
/*...................................................................*/

/*...*/
      v[0] = lvSkew[0] + MAT2D(nAresta, 0, xmcc, ndm);
      v[1] = lvSkew[1] + MAT2D(nAresta, 1, xmcc, ndm);
      v[2] = lvSkew[2] + MAT2D(nAresta, 2, xmcc, ndm);
      dPviz = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
      alpha = dPviz / lModKsi;
      alphaMenosUm = 1.0e0 - alpha;
/*...................................................................*/

/*... media harmonica*/
      viscosityV = MAT2D(nAresta, DYNAMICVISCOSITY, prop, MAXPROP);
      viscosity = alpha / viscosityC + alphaMenosUm / viscosityV;
      viscosity = 1.0e0 / viscosity;
/*...................................................................*/

/*... difusao direta*/
      coef = viscosity;
      modE = sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
      dfd = coef*modE / lModKsi;
/*...................................................................*/

/*...*/
      gf[0][0] = alphaMenosUm*gradVelC[0][0] + alpha*gradVelV[0][0];
      gf[0][1] = alphaMenosUm*gradVelC[0][1] + alpha*gradVelV[0][1];
      gf[0][2] = alphaMenosUm*gradVelC[0][2] + alpha*gradVelV[0][2];
/*...*/
      gf[1][0] = alphaMenosUm*gradVelC[1][0] + alpha*gradVelV[1][0];
      gf[1][1] = alphaMenosUm*gradVelC[1][1] + alpha*gradVelV[1][1];
      gf[1][2] = alphaMenosUm*gradVelC[1][2] + alpha*gradVelV[1][2];
/*...*/
      gf[2][0] = alphaMenosUm*gradVelC[2][0] + alpha*gradVelV[2][0];
      gf[2][1] = alphaMenosUm*gradVelC[2][1] + alpha*gradVelV[2][1];
      gf[2][2] = alphaMenosUm*gradVelC[2][2] + alpha*gradVelV[2][2];
/*...*/
      wf[0] = alphaMenosUm*velC[0] + alpha*velV[0];
      wf[1] = alphaMenosUm*velC[1] + alpha*velV[1];
      wf[2] = alphaMenosUm*velC[2] + alpha*velV[2];
      density = alphaMenosUm*densityC + alpha*densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = wf[0] * lNormal[0] + wf[1] * lNormal[1] + wf[2] * lNormal[2];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi[0] = gf[0][0] * lKsi[0]
        + gf[0][1] * lKsi[1]
        + gf[0][2] * lKsi[2];
/*...*/
      gfKsi[1] = gf[1][0] * lKsi[0]
        + gf[1][1] * lKsi[1]
        + gf[1][2] * lKsi[2];
/*...*/
      gfKsi[2] = gf[2][0] * lKsi[0]
        + gf[2][1] * lKsi[1]
        + gf[2][2] * lKsi[2];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du[0] = duDksi[0] - gfKsi[0];
      du[1] = duDksi[1] - gfKsi[1];
      du[2] = duDksi[2] - gfKsi[2];
/*...*/
      gradVelComp[0][0] = gf[0][0] + du[0] * lKsi[0];
      gradVelComp[0][1] = gf[0][1] + du[0] * lKsi[1];
      gradVelComp[0][2] = gf[0][2] + du[0] * lKsi[2];
/*...*/
      gradVelComp[1][0] = gf[1][0] + du[1] * lKsi[0];
      gradVelComp[1][1] = gf[1][1] + du[1] * lKsi[1];
      gradVelComp[1][2] = gf[1][2] + du[1] * lKsi[2];
/*...*/
      gradVelComp[2][0] = gf[2][0] + du[2] * lKsi[0];
      gradVelComp[2][1] = gf[2][1] + du[2] * lKsi[1];
      gradVelComp[2][2] = gf[2][2] + du[2] * lKsi[2];
/*...................................................................*/

/*... derivadas direcionais*/
/*...*/
      gfKsi[0] = gradVelComp[0][0] * t[0]
        + gradVelComp[0][1] * t[1]
        + gradVelComp[0][2] * t[2];
/*...*/
      gfKsi[1] = gradVelComp[1][0] * t[0]
        + gradVelComp[1][1] * t[1]
        + gradVelComp[1][2] * t[2];
/*...*/
      gfKsi[2] = gradVelComp[2][0] * t[0]
        + gradVelComp[2][1] * t[1]
        + gradVelComp[2][2] * t[2];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc[0] = coef*gfKsi[0];
      dfdc[1] = coef*gfKsi[1];
      dfdc[2] = coef*gfKsi[2];
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      cv = density*wfn*lFarea;
/*...................................................................*/

/*... correcao do fluxo advectivo*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      v[2] = lXm[2] - ccV[2];
      advectiveScheme(velC, velV
                      , gradVelC[0], gradVelV[0]
                      , gradVelComp[0], lvSkew
                      , lXmcc, v
                      , lKsi, lModKsi
                      , cv, cvc
                      , ndm
                      , iCodAdv1, iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv, 0.0e0);
      sP += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p[0] += dfdc[0] - cv*cvc[0];
      p[1] += dfdc[1] - cv*cvc[1];
      p[2] += dfdc[2] - cv*cvc[2];
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem
      (forma conservativa)*/
      if (sPressure) {
/*.. vetor que une o centroide da celula viznha ao ponto medio
        da face*/
        v[0] = lXm[0] - ccV[0];
        v[1] = lXm[1] - ccV[1];
        v[2] = lXm[2] - ccV[2];

        p1 = presC + gradPresC[0] * lXmcc[0]
          + gradPresC[1] * lXmcc[1]
          + gradPresC[2] * lXmcc[2];

        p2 = presF + gradPresV[0] * v[0]
          + gradPresV[1] * v[1]
          + gradPresV[2] * v[2];

        pFace = 0.5e0*(p1 + p2);
      }
/*...................................................................*/

/*... interpolacao linear*/
      else
        pFace = alphaMenosUm*presC + alpha*presF;
/*...................................................................*/
      tmp = pFace*lFarea;
      pf[0] += tmp*lNormal[0];
      pf[1] += tmp*lNormal[1];
      pf[2] += tmp*lNormal[2];
/*...................................................................*/

/*... termos viscosos explicitos*/
      aP = viscosity*lFarea;
      p[0] += aP*(gradVelV[0][0] * lNormal[0]
                  + gradVelV[1][0] * lNormal[1]
                  + gradVelV[2][0] * lNormal[2]);

      p[1] += aP*(gradVelV[0][1] * lNormal[0]
                  + gradVelV[1][1] * lNormal[1]
                  + gradVelV[2][1] * lNormal[2]);

      p[2] += aP*(gradVelV[0][2] * lNormal[0]
                  + gradVelV[1][2] * lNormal[1]
                  + gradVelV[2][2] * lNormal[2]);
/*...................................................................*/
    }
/*... contorno*/
    else {
      lA[nAresta] = 0.0e0;
      wfn = velC[0] * lNormal[0]
        + velC[1] * lNormal[1]
        + velC[2] * lNormal[2];

/*... termos viscosos explicitos*/
      aP = viscosityC*lFarea;
      p[0] += aP*(gradVelC[0][0] * lNormal[0]
                  + gradVelC[1][0] * lNormal[1]
                  + gradVelC[2][0] * lNormal[2]);

      p[1] += aP*(gradVelC[0][1] * lNormal[0]
                  + gradVelC[1][1] * lNormal[1]
                  + gradVelC[2][1] * lNormal[2]);

      p[2] += aP*(gradVelC[0][2] * lNormal[0]
                  + gradVelC[1][2] * lNormal[1]
                  + gradVelC[2][2] * lNormal[2]);
/*...................................................................*/

/*...*/
      pFace = presC;
/*... pressao prescrita*/
      if (lFacePresR[nAresta]) {
/*...cargas*/
        nCarg = lFacePresL[nAresta] - 1;
        pLoadSimplePres(&sP, p
                        , &pFace
                        , viscosityC, densityC
                        , wfn
                        , lFarea, dcca[nAresta]
                        , loadsPres[nCarg], false);
      }
/*...................................................................*/

/*... gradiente da pressao com resconstrucao de segunda ordem*/
      if (sPressure)
        pFace += gradPresC[0] * lXmcc[0]
        + gradPresC[1] * lXmcc[1]
        + gradPresC[2] * lXmcc[2];

      pf[0] += pFace*lFarea*lNormal[0];
      pf[1] += pFace*lFarea*lNormal[1];
      pf[2] += pFace*lFarea*lNormal[2];
/*...................................................................*/

/*... velocidades*/
      if (lFaceVelR[nAresta] > 0) {
/*...cargas*/
        nCarg = lFaceVelL[nAresta] - 1;
        pLoadSimple(sPc, p
                    , tA, velC
                    , lNormal
                    , gradVelC[0], lXmcc
                    , viscosityC, densityC
                    , lFarea, dcca[nAresta]
                    , loadsVel[nCarg], ndm
                    , true, false);
      }
/*...................................................................*/

/*... parede impermevavel*/
      else if (lFaceVelR[nAresta] == STATICWALL) {
        aP = viscosityC*lFarea / dcca[nAresta];
/*...*/
        sPc[0] += aP*(1.e0 - lNormal[0] * lNormal[0]);
        sPc[1] += aP*(1.e0 - lNormal[1] * lNormal[1]);
        sPc[2] += aP*(1.e0 - lNormal[2] * lNormal[2]);
/*...................................................................*/

/*... x*/
        p[0] += aP*(velC[1] * lNormal[0] * lNormal[1]
                    + velC[2] * lNormal[0] * lNormal[2]);
/*... y*/
        p[1] += aP*(velC[0] * lNormal[0] * lNormal[1]
                    + velC[2] * lNormal[1] * lNormal[2]);
/*... z*/
        p[2] += aP*(velC[0] * lNormal[0] * lNormal[2]
                    + velC[1] * lNormal[1] * lNormal[2]);
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... distretização temporal*/
  if (fTime) {
    p[0] += bT[0];
    p[1] += bT[1];
    p[2] += bT[2];
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
  lAn = 0.e0;
  if (nFace == 4) {
    lAn = lA[0] + lA[1] + lA[2] + lA[3];
  }
  else if (nFace == 6) {
    lAn = lA[0] + lA[1] + lA[2] + lA[3] + lA[4] + lA[5];
  }
  lA[idCell]     = sP + sPc[0] + lAn;
  lA[idCell + 1] = sP + sPc[1] + lAn;
  lA[idCell + 2] = sP + sPc[2] + lAn;
/*...................................................................*/

/*... GradP*/
  p[0] -= pf[0];
  p[1] -= pf[1];
  p[2] -= pf[2];
/*...................................................................*/

/*... calculo do residuo*/
  if (fResidual) {
    for (nAresta = 0; nAresta<nFace; nAresta++) {
      if (lViz[nAresta] > -1) {
        p[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        p[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
        p[2] += lA[nAresta]*MAT2D(nAresta,2,vel,ndm);
      }
    }
/*... residuo: R = F - KpUp*/
    lB[0] = p[0] - lA[idCell  ]*velC[0];
    lB[1] = p[1] - lA[idCell+1]*velC[1];
    lB[2] = p[2] - lA[idCell+2]*velC[2];
/*...................................................................*/

/*...*/
    lB[3] = lA[idCell];
    lB[4] = lA[idCell + 1];
    lB[5] = lA[idCell + 2];
/*...................................................................*/
  }
/*...................................................................*/

/*... atualizacao explicita*/
  else {
    for (nAresta = 0; nAresta<nFace; nAresta++) {
      if (lViz[nAresta] > -1) {
        p[0] += lA[nAresta]*MAT2D(nAresta,0,vel,ndm);
        p[1] += lA[nAresta]*MAT2D(nAresta,1,vel,ndm);
        p[2] += lA[nAresta]*MAT2D(nAresta,2,vel,ndm);
      }
    }
/*... velocida explicita: vC = (-H[vF] - grad(P))/ac*/
    velExp[0] = p[0] / lA[idCell];
    velExp[1] = p[1] / lA[idCell+1];
    velExp[2] = p[2] / lA[idCell+2];
/*...................................................................*/

/*... under-relaxation(prime)*/
    lB[0] = velC[0] + underU*(velExp[0] - velC[0]);
    lB[1] = velC[1] + underU*(velExp[1] - velC[1]);
    lB[2] = velC[2] + underU*(velExp[2] - velC[2]);
/*...................................................................*/

/*...*/
    dField[0] = volume[idCell]/lA[idCell];
    dField[1] = volume[idCell]/lA[idCell + 1];
    dField[2] = volume[idCell]/lA[idCell + 2];
/*...................................................................*/
  }
/*...................................................................*/
}
/*********************************************************************/

