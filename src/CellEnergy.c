#include<CellLib.h>
/*********************************************************************
* Biblioteca de celulas                                             *
*-------------------------------------------------------------------*
* Celulas 2D                                                        *
*-------------------------------------------------------------------*
* cellEnergy2D- Celula de transporte da equacao de energia          *
*-------------------------------------------------------------------*
* Celulas 3D                                                        *
*-------------------------------------------------------------------*
* cellEnergy3D- Celula de transporte da equacao de energia          *
********************************************************************/

/*********************************************************************
* Data de criacao    : 20/08/2017                                   *
* Data de modificaco : 27/08/2019                                   *
*-------------------------------------------------------------------*
* CELLENERGY2D: Celula 2D para transporte                           *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* loads     -> definicoes de cargas                                 *
* model     -> modelo da equacao de energia                         *
* advT      -> tecnica da discretizacao do termo advecao            *
* diffT     -> tecnica da discretizacao do termo difusivo           *
* tModel    -> modelo de turbulencia                                *
* vProp     -> propedades variaveis (true|false)                    *
* lnFace    -> numero de faces da celula central e seus vizinhos    *
* lGeomType -> tipo geometrico da celula central e seus vizinhos    *
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
* lSheat    -> calor especifico com variacao temporal               *
* lDviscosity-> viscosidade dinamica com variacao temporal          *
* lTconductivity-> condutividade termica com variacao temporal      *
* dField    -> matriz D do metodo simple                            *
* wallPar   -> parametros de parede  ( yPlus, uPlus, uFri,sTressW)  *
* lA        -> nao definido                                         *
* lB        -> nao definido                                         *
* lRcell    -> nao definido                                         *
* ddt       -> discretizacao temporal                               *
* faceR     -> restricoes por elmento                               *
* faceL     -> carga por elemento                                   *
* u0        -> solucao conhecida                                    *
* gradU0    -> gradiente rescontruido da solucao conhecida          *
* vel       -> campo de velocidade conhecido                        *
* gradVel   -> gradiente rescontruido da velocidade                 *
* pres      -> pressao do tempo atual e do tempo anterior           *
* gradPres  -> gradiente de pressao do tempo atual                  *
* cc        -> centroides da celula centra e seus vizinhos          *
* underU    -> parametro de sob relaxamento                         *
* fSheat    -> calor especifico com variacao com a Temperatura      *
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
void cellEnergy2D(Loads *loads , Loads *loadsVel
  , Advection *advT            , Diffusion *diffT
  , Turbulence *tModel         , EnergyModel *model
  , PropVarFluid *vProp        , short *RESTRICT lGeomType  
  , INT *RESTRICT lViz         , INT *RESTRICT lId
  , DOUBLE *RESTRICT ksi       , DOUBLE *RESTRICT mKsi
  , DOUBLE *RESTRICT eta       , DOUBLE *RESTRICT mEta
  , DOUBLE *RESTRICT normal    , DOUBLE *RESTRICT volume
  , DOUBLE *RESTRICT xm        , DOUBLE *RESTRICT xmcc
  , DOUBLE *RESTRICT dcca      , DOUBLE *RESTRICT cc
  , DOUBLE *RESTRICT vSkew     , DOUBLE *RESTRICT mvSkew
  , DOUBLE *RESTRICT lA        , DOUBLE *RESTRICT lB
  , DOUBLE *RESTRICT lRcell    , Temporal const ddt
  , short  *RESTRICT lFaceR    , short *RESTRICT lFaceL
  , short  *RESTRICT lFaceVelR , short *RESTRICT lFaceVelL
  , DOUBLE *RESTRICT u0        , DOUBLE *RESTRICT gradU0
  , DOUBLE *RESTRICT vel       , DOUBLE *RESTRICT gradVel
  , DOUBLE *RESTRICT pres      , DOUBLE *RESTRICT gradPres
  , DOUBLE *RESTRICT lDensity  , DOUBLE *RESTRICT lSheat
  , DOUBLE *RESTRICT lViscosity, DOUBLE *RESTRICT lTconductivity
  , DOUBLE *RESTRICT dField    , DOUBLE *RESTRICT wallPar
  , DOUBLE const underU
  , const short nEn            , short const nFace
  , const short ndm            , INT const nel)
{
  bool fTime, fDisp, fRes, fPresWork, fTemp, fTurb, fWallModel, fKelvin
    , fSheat;
  short iCodAdv1, iCodAdv2, iCodDif, wallType, idCell, nAresta, nCarg1
    , nCarg2, typeTime, iCodPolFace;
/*...*/
  INT vizNel;
/*...*/
  DOUBLE thermCoefC, thermCoefV, densityC, densityV, densityM,
    diffEffC, diffEffV, diffEff, sHeatC, sHeatV, sHeatM,
    eddyViscosityC, eddyViscosityV, viscosityC, tA, coef,
    tmp, tmp1, prTwall, prTsgs;
  DOUBLE p, sP, dfd, gfKsi, lvSkew[2], alpha, alphaMenosUm;
  DOUBLE v[2], gradUcomp[2], lKsi[2], lNormal[2], gf[2];
  DOUBLE lModKsi, lModEta, du, duDksi, lXmcc[2], lXm[2];
  DOUBLE gradUp[2], gradUv[2], ccV[2], gradVelC[2][2], rCell, dt, dt0;
  DOUBLE uC, uV;
/*... nonOrtogonal*/
  DOUBLE e[2], t[2], s[2], modE, dfdc, xx[3];
/*... */
  DOUBLE presC, presC0, presV, gradPresC[2], gradPresV[2], wfn
    , velC[2], velV[2], dFieldC[2], dFieldV[2], dFieldF[2], cv, cvc;

/*...*/
  DOUBLE phi, psi, lambda;
  DOUBLE tW, tC;
/*...*/
  DOUBLE pAdv[NPADV];

/*...*/
  idCell      = nFace;
  iCodAdv1    = advT->iCod1;
  iCodAdv2    = advT->iCod2;
  pAdv[0]     = advT->par[0];
  iCodDif     = diffT->iCod;
  iCodPolFace = INTPOLFACELINEAR;
/*...................................................................*/
  dt         = ddt.dt[0];
  dt0        = ddt.dt[1];
  typeTime   = ddt.type;
  fTime      = ddt.flag;
  fDisp      = model->fDissipation;
  fRes       = model->fRes;
  fPresWork  = model->fPresWork;
  fTemp      = model->fTemperature;
  fKelvin    = model->fKelvin;
  fTurb      = tModel->fTurb;
  prTwall    = tModel->PrandltTwall;
  prTsgs     = tModel->PrandltTsgs;
  fWallModel = tModel->fWall;
  wallType   = tModel->wallType;
  fSheat     = vProp->fSpecificHeat;
/*...................................................................*/

/*... propriedades da celula*/
  sHeatM = eddyViscosityC = eddyViscosityV = 0.e0;
  densityC = lDensity[idCell];
  sHeatC = lSheat[idCell];
  thermCoefC = lTconductivity[idCell];
  viscosityC = MAT2D(idCell, 0, lViscosity, 2);
  if (fTurb) eddyViscosityC = MAT2D(idCell, 1, lViscosity, 2);
  lambda = -D2DIV3 * (viscosityC + eddyViscosityC);
/*...................................................................*/

/*...*/
  if (fTemp)
    diffEffC = thermCoefC + sHeatC * viscosityC / prTsgs;
  else
    diffEffC = thermCoefC / sHeatC + viscosityC / prTsgs;
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell, 0, gradU0, ndm);
  gradUp[1] = MAT2D(idCell, 1, gradU0, ndm);
  velC[0] = MAT2D(idCell, 0, vel, ndm);
  velC[1] = MAT2D(idCell, 1, vel, ndm);
  uC = u0[idCell];
/*... p(n)*/
  presC0 = MAT2D(idCell, 0, pres, 2);
/*... p(n+1)*/
  presC = MAT2D(idCell, 1, pres, 2);
/*...*/
  gradPresC[0] = MAT2D(idCell, 0, gradPres, ndm);
  gradPresC[1] = MAT2D(idCell, 1, gradPres, ndm);
  dFieldC[0] = MAT2D(idCell, 0, dField, 2);
  dFieldC[1] = MAT2D(idCell, 1, dField, 2);
/*...................................................................*/

  p = 0.0e0;
  sP = 0.0e0;
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
      densityV = lDensity[nAresta];
      sHeatV = lSheat[nAresta];
      thermCoefV = lTconductivity[nAresta];
      if (fTurb) eddyViscosityV = MAT2D(nAresta, 1, lViscosity, 2);
/*... p(n+1)*/
      presV = MAT2D(nAresta, 1, pres, 2);
      gradPresV[0] = MAT2D(nAresta, 0, gradPres, ndm);
      gradPresV[1] = MAT2D(nAresta, 1, gradPres, ndm);
      dFieldV[0] = MAT2D(nAresta, 0, dField, 2);
      dFieldV[1] = MAT2D(nAresta, 1, dField, 2);
/*...*/
      uV = u0[nAresta];
      velV[0] = MAT2D(nAresta, 0, vel, ndm);
      velV[1] = MAT2D(nAresta, 1, vel, ndm);
/*...*/
      lKsi[0] = MAT2D(nAresta, 0, ksi, ndm);
      lKsi[1] = MAT2D(nAresta, 1, ksi, ndm);
      lModKsi = mKsi[nAresta];
/*...*/
      lvSkew[0] = MAT2D(nAresta, 0, vSkew, ndm);
      lvSkew[1] = MAT2D(nAresta, 1, vSkew, ndm);
/*...*/
      duDksi = (uV - uC) / lModKsi;
/*...*/
      gradUv[0] = MAT2D(nAresta, 0, gradU0, ndm);
      gradUv[1] = MAT2D(nAresta, 1, gradU0, ndm);

      ccV[0] = MAT2D(nAresta, 0, cc, ndm);
      ccV[1] = MAT2D(nAresta, 1, cc, ndm);

      lXm[0] = MAT2D(nAresta, 0, xm, ndm);
      lXm[1] = MAT2D(nAresta, 1, xm, ndm);
/*...................................................................*/

/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      s[0] = lModEta * lNormal[0];
      s[1] = lModEta * lNormal[1];
/*...*/
      difusionSchemeNew(s, lKsi, e, t, ndm, iCodDif);
/*...................................................................*/

/*...*/
      alpha = interpolFace(lvSkew, lXmcc
        , mEta[idCell], mEta[nAresta]
        , lModKsi, ndm
        , iCodPolFace);
      alphaMenosUm = 1.0e0 - alpha;
/*...................................................................*/

/*... media harmonica*/
      if (fTemp) {
        diffEffV = thermCoefV + sHeatV * eddyViscosityV / prTsgs;
        diffEff = alpha / diffEffC + alphaMenosUm / diffEffV;
        diffEff = 1.0e0 / diffEff;
      }
      else {
        diffEffV = thermCoefV / sHeatV + eddyViscosityV / prTsgs;
        diffEff = alpha / diffEffC + alphaMenosUm / diffEffV;
        diffEff = 1.0e0 / diffEff;
      }
/*...................................................................*/

/*... difusao direta*/
      coef = diffEff;
      modE = sqrt(e[0] * e[0] + e[1] * e[1]);
      dfd = coef * modE / lModKsi;
/*...................................................................*/

/*...*/
      gf[0] = alphaMenosUm * gradUp[0] + alpha * gradUv[0];
      gf[1] = alphaMenosUm * gradUp[1] + alpha * gradUv[1];
      dFieldF[0] = alphaMenosUm * dFieldC[0] + alpha * dFieldV[0];
      dFieldF[1] = alphaMenosUm * dFieldC[1] + alpha * dFieldV[1];
      densityM = alphaMenosUm * densityC + alpha * densityV;
      if (fTemp)
        sHeatM = alphaMenosUm * sHeatC + alpha * sHeatV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = interpolFaceVel(velC, velV
        , presC, presV
        , gradPresC, gradPresV
        , lNormal, lKsi
        , lModKsi, dFieldF
        , alphaMenosUm, alpha
        , ndm);
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gf[0] * lKsi[0] + gf[1] * lKsi[1];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du = duDksi - gfKsi;
      gradUcomp[0] = gf[0] + du * lKsi[0];
      gradUcomp[1] = gf[1] + du * lKsi[1];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gradUcomp[0] * t[0] + gradUcomp[1] * t[1];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc = coef * gfKsi;
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      if (fTemp)
        cv = sHeatM * densityM*wfn*lModEta;
      else
        cv = densityM * wfn*lModEta;
/*...................................................................*/

/*...*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      advectiveSchemeScalar(uC, uV
        , gradUp, gradUv
        , gradUcomp, lvSkew
        , lXmcc, v
        , lKsi, lModKsi
        , cv, &cvc
        , alphaMenosUm, alpha
        , pAdv, ndm
        , iCodAdv1, iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nAresta] = dfd - min(cv, 0e0);
      sP += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p += dfdc - cv * cvc;
/*...................................................................*/
    }
/*... contorno*/
    else {
      lA[nAresta] = 0.e0;
      if (lFaceR[nAresta]) {
        wfn = velC[0] * lNormal[0] + velC[1] * lNormal[1];
/*...cargas*/
        nCarg1 = lFaceL[nAresta] - 1;
        xx[0] = MAT2D(nAresta, 0, xm, 2);
        xx[1] = MAT2D(nAresta, 1, xm, 2);
        xx[2] = 0.e0;
        pLoadEnergy(vProp
                  , &sP, &p
                  , &tA, velC
                  , uC, lNormal
                  , thermCoefC, densityC
                  , viscosityC, sHeatC
                  , prTwall, xx
                  , lModEta, dcca[nAresta]
                  , &loads[nCarg1]
                  , wallPar, ndm
                  , true, fTemp
                  , fKelvin, fSheat
                  , fWallModel, wallType);
/*...................................................................*/
      }
/*...................................................................*/

/*...*/
      else {
/*... inertial sub-layer*/
        if (wallPar[0] > 11.81e0) {
/*... energia na forma da temperatura*/
          if (fTemp)
            tW = uC;
/*...................................................................*/

/*... energia na forma da entalpia*/
          else {
            tC = specificEnthalpyForTemp(&vProp->sHeat     ,298.15
                                        ,uC, sHeatC, fSheat, fKelvin);
            tW = tC;
            tW = tempForSpecificEnthalpy(&vProp->sHeat
                                        , tW, sHeatC, fSheat, fKelvin);
          }
/*...................................................................*/

/*...*/
          tmp = thermCoefC * lModEta / dcca[nAresta];
          if (!fTemp) tmp /= sHeatC;
          sP += tmp;
          p += tmp * tW;
/*...................................................................*/
        }
/*...................................................................*/
      }
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... | du1/dx1 du1/dx2*/
  gradVelC[0][0] = MAT3D(idCell, 0, 0, gradVel, 2, ndm);
  gradVelC[0][1] = MAT3D(idCell, 0, 1, gradVel, 2, ndm);
/*... | du2/dx1 du2/dx2*/
  gradVelC[1][0] = MAT3D(idCell, 1, 0, gradVel, 2, ndm);
  gradVelC[1][1] = MAT3D(idCell, 1, 1, gradVel, 2, ndm);
/*...................................................................*/

/*...*/
  if (fDisp) {
    tmp = gradVelC[0][0] + gradVelC[1][1];
    psi = tmp * tmp;
/*...*/
    tmp = gradVelC[0][0] * gradVelC[0][0] + gradVelC[1][1] * gradVelC[1][1];
    tmp1 = gradVelC[0][1] + gradVelC[1][0];
    phi = 2.0*tmp + tmp1 * tmp1;
    tmp = (lambda*psi + (viscosityC + eddyViscosityC)*phi);
/*...*/
    p += tmp * volume[idCell];
  }
/*.....................................................................*/

/*...*/
  if (fPresWork) {
/*... derivada materia da pressao*/
    tmp = (presC - presC0) / dt
      + velC[0] * gradPresC[0] + velC[1] * gradPresC[1];
/*...*/
    p += tmp * volume[idCell];
/*.....................................................................*/
  }
/*.....................................................................*/

/*... distretizacao temporal*/
  if (fTemp)
    tmp1 = sHeatC * densityC;
  else
    tmp1 = densityC;

  if (fTime) {
/*... EULER*/
    if (typeTime == EULER)
      sP += tmp1 * volume[idCell] / dt;
/*...BACKWARD*/
    else if (typeTime == BACKWARD) {
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp * tmp1*volume[idCell];
    }
  }
/*...................................................................*/

/*...*/
  if (nFace == 3) {
    lA[idCell] = sP + lA[0] + lA[1] + lA[2];
  }
  else if (nFace == 4) {
    lA[idCell] = sP + lA[0] + lA[1] + lA[2] + lA[3];
  }
/*...................................................................*/

/*...*/
  rCell = 0.0e0;
  for (nAresta = 0; nAresta<nFace; nAresta++) {
    if (lViz[nAresta] > -1) {
/*... pasando os valoeres conhecidos para o lado direito*/
      if (lId[nAresta] == -2)
        p += lA[nAresta] * u0[nAresta];
      else
/*residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell += lA[nAresta] * u0[nAresta];
    }
  }
/*... residuo: R = F - KpUp*/
  rCell += p - lA[idCell] * u0[idCell];
/*...................................................................*/

/*... under-relaxation(simple)*/
  lA[idCell] = lA[idCell] / underU;
  if (!fRes)
    p += (1.e0 - underU)*lA[idCell] * u0[idCell];
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
  if (fRes)
    lB[0] = rCell;
  else
    lB[0] = p;
/*...*/
  lRcell[0] = rCell;
/*...................................................................*/
}
/*********************************************************************/

/********************************************************************
 * Data de criacao    : 03/10/2017                                   *
 * Data de modificaco : 08/10/2019                                   *
 *-------------------------------------------------------------------*
 * CELLENERGY3D: Celula 3D para transporte                           *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * loads     -> definicoes de cargas                                 *
 * model     -> modelo da equacao de energia                         *
 * advT      -> tecnica da discretizacao do termo advecao            *
 * diffT     -> tecnica da discretizacao do termo difusivo           *
 * tModel    -> modelo de turbulencia                                *
 * vProp     -> propedades variaveis (true|false)                    *
 * lnFace    -> numero de faces da celula central e seus vizinhos    *
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    *
 * lprop     -> propriedade fisicas das celulas                      *
 * lViz      -> viznhos da celula central                            *
 * lId       -> numeracoes das equacoes das celulas                  *
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *            vizinhos destas                                        *
 * mKsi      -> modulo do vetor ksi                                  *
 * eta       -> vetores paralelos as faces das celulas               *
 * fArea     -> area das faces                                       *
 * normal    -> vetores normais as faces das celulas                 *
 * area      -> area da celula central                               *
 * xm        -> pontos medios das faces da celula central            *
 * xmcc      -> vetores que unem o centroide aos pontos medios das   *
 *            faces da celula central                                *
 * vSkew  -> vetor entre o ponto medio a intersecao que une os       *
 *            centrois compartilhado nessa face da celula central    *
 * mvSkew -> distacia entre o ponto medio a intersecao que une os    *
 *            centrois compartilhado nessa face da celula central    *
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               *
 * dField    -> matriz D do metodo simple                            *
 * wallPar   -> parametros de parede  ( yPlus, uPlus, uFri,sTressW)  *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * faceR     -> restricoes por elmento                               *
 * faceL     -> carga por elemento                                   *
 * u0        -> solucao conhecida                                    *
 * gradU0    -> gradiente rescontruido da solucao conhecida          *
 * vel       -> campo de velocidade conhecido                        *
 * gradVel   -> gradiente rescontruido da velocidade                 *
 * pres      -> pressao do tempo atual e do tempo anterior           *
 * gradPres  -> gradiente de pressao do tempo atual                  *
 * lDensity  -> massa especifica com variacao temporal               *
 * lSheat    -> calor especifico com variacao temporal               *
 * lDviscosity-> viscosidade dinamica com variacao temporal          *
 * lTconductivity-> condutividade termica com variacao temporal      *
 * lEnthalpyk -> Entalpia das especies                               *
 * lGradY    -> gradiente das especies                               *
 * yFac      -> fracoes massicas                                     *
 * diffY     -> coeficiente de difusao massica por especies          *
 * rateHeat  -> energia liberada pela combustao (KJ/s )              *
 * cc        -> centroides da celula centra e seus vizinhos          *
 * underU    -> parametro de sob relaxamento                         *
 * fSheat    -> calor especifico com variacao com a Temperatura      *
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
 * OBS: Energia KJ/KG                                                *
 *-------------------------------------------------------------------*
 *********************************************************************/
void cellEnergy3D(Loads *loads               , Loads *lVel
                , Advection *advT            , Diffusion *diffT
                , Turbulence *tModel         , EnergyModel *eModel
                , Combustion *cModel         , PropVarFluid *vProp
                , short *RESTRICT lGeomType  
                , INT *RESTRICT lViz         , INT *RESTRICT lId
                , DOUBLE *RESTRICT ksi       , DOUBLE *RESTRICT mKsi
                , DOUBLE *RESTRICT eta       , DOUBLE *RESTRICT fArea
                , DOUBLE *RESTRICT normal    , DOUBLE *RESTRICT volume
                , DOUBLE *RESTRICT xm        , DOUBLE *RESTRICT xmcc
                , DOUBLE *RESTRICT dcca      , DOUBLE *RESTRICT cc
                , DOUBLE *RESTRICT vSkew     , DOUBLE *RESTRICT mvSkew
                , DOUBLE *RESTRICT lA        , DOUBLE *RESTRICT lB
                , DOUBLE *RESTRICT lRcell    , Temporal *ddt
                , short  *RESTRICT lFaceR    , short  *RESTRICT lFaceVelR 
                , DOUBLE *RESTRICT u0        , DOUBLE *RESTRICT gradU0
                , DOUBLE *RESTRICT vel       , DOUBLE *RESTRICT gradVel
                , DOUBLE *RESTRICT pres      , DOUBLE *RESTRICT gradPres
                , DOUBLE *RESTRICT lDensity  , DOUBLE *RESTRICT lSheat
                , DOUBLE *RESTRICT lViscosity, DOUBLE *RESTRICT lTconductivity
                , DOUBLE *RESTRICT enthalpyk , DOUBLE *RESTRICT gradY 
                , DOUBLE *RESTRICT diffY     , DOUBLE *RESTRICT yFrac     
                , DOUBLE const rateHeat  
                , DOUBLE *RESTRICT dField    , DOUBLE *RESTRICT wallPar
                , DOUBLE const underU
                , const short nEn            , short const nFace
                , const short ndm            , INT const nel)
{
  bool fTime, fDisp, fRes, fPresWork, fTemp, fTurb, fWallModel, fKelvin
    , fSheat,fEntalpy;
  short iCodAdv1, iCodAdv2, iCodDif, wallType, idCell, nf, nCarg1
    , nCarg2, typeTime, iCodPolFace, k, ns, i;
/*...*/
  INT vizNel;
/*...*/
  DOUBLE thermCoefF, thermCoefC, thermCoefV
       , densityC  , densityV  , densityM
       , diffEffC  , diffEffV  , diffEff
       , sHeatC    , sHeatV    , sHeatF
       , diffCoefC , diffCoefV , coef
       , diffTurbF , eddyViscosityC , eddyViscosityV, viscosityC
       , diffYC[MAXSPECIES], yFracC[MAXSPECIES], dV;
  DOUBLE tmp, tmp1, tmp2, tmp3, prTwall, prTsgs;
  DOUBLE hskV,hskC[MAXSPECIES],hsk;
  DOUBLE gradYC[MAXSPECIES][3],gradYV[3];
  DOUBLE p, sP, dfd, gfKsi, lvSkew[3], alpha, alphaMenosUm;
  DOUBLE v[3], gradUcomp[3], lKsi[3], lNormal[3], gf[3];
  DOUBLE lModKsi, lFarea, du, duDksi, lXmcc[3], lXm[3];
  DOUBLE gradUp[3], gradUv[3], ccV[3], gradVelC[3][3], rCell, dt, dt0;
  DOUBLE uC, uV;
/*... nonOrtogonal*/
  DOUBLE e[3], t[3], s[3], modE, dfdc, xx[3];
/*... */
  DOUBLE presC, presC0, presV, gradPresC[3], gradPresV[3], Pth[3];
  DOUBLE velC[3], velV[3], dFieldC[3], dFieldV[3], dFieldF[3], wfn;
  DOUBLE cv, cvc;
/*...*/
  DOUBLE phi, psi, lambda;
  DOUBLE tW, tC, tA;
/*...*/
  DOUBLE pAdv[NPADV];
/*...*/
  bool fComb;

/*...*/
  idCell      = nFace;
  iCodAdv1    = advT->iCod1;
  iCodAdv2    = advT->iCod2;
  pAdv[0]     = advT->par[0];
  iCodDif     = diffT->iCod;
  iCodPolFace = INTPOLFACELINEAR;
/*...................................................................*/

/*...*/
  dt        = ddt->dt[0];
  dt0       = ddt->dt[1];
  typeTime  = ddt->type;
  fTime     = ddt->flag;
  fDisp     = eModel->fDissipation;
  fRes      = eModel->fRes;
  fPresWork = eModel->fPresWork;
  fTemp     = eModel->fTemperature;
  fKelvin   = eModel->fKelvin;
  fTurb     = tModel->fTurb;
  prTwall   = tModel->PrandltTwall;
  prTsgs    = tModel->PrandltTsgs;
  fWallModel= tModel->fWall;
  wallType  = tModel->wallType;
  fSheat    = vProp->fSpecificHeat;
  fComb     = cModel->fCombustion;
  ns        = cModel->nOfSpecies;
  fEntalpy  = eModel->fDiffEnergy;
  Pth[0]    = thDynamic.pTh[0];
  Pth[1]    = thDynamic.pTh[1];
  Pth[2]    = thDynamic.pTh[2];
/*...................................................................*/

/*... propriedades da celula*/
  eddyViscosityC = eddyViscosityV = 0.e0;
  densityC       = lDensity[idCell];
  sHeatC         = lSheat[idCell];
  thermCoefC     = lTconductivity[idCell];

  if(fTemp)
    diffEffC       = diffCoefC = thermCoefC;
  else
    diffEffC       = diffCoefC = thermCoefC/sHeatC;

  viscosityC     = MAT2D(idCell, 0, lViscosity, 2);
  if (fTurb)
  {
    eddyViscosityC = MAT2D(idCell, 1, lViscosity, 2);
    diffEffC       = diffCoefC + eddyViscosityC/prTsgs;
  }
  lambda = -D2DIV3 * (viscosityC + eddyViscosityC); 
  if(fEntalpy)
  {
    for(k=0;k<ns;k++)
    {
/*... propriedades da celula*/
      diffYC[k] = MAT2D(idCell, k, diffY, ns);
/*...*/
      gradYC[k][0] = MAT3D(idCell, k, 0, gradY, ns, 3);
      gradYC[k][1] = MAT3D(idCell, k, 1, gradY, ns, 3);
      gradYC[k][2] = MAT3D(idCell, k, 2, gradY, ns, 3);
/*...*/
      hskC[k] = MAT2D(idCell, k, enthalpyk, ns);
/*...*/
      yFracC[k] = MAT2D(idCell, k, yFrac, ns);
    }
  }
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell, 0, gradU0, 3);
  gradUp[1] = MAT2D(idCell, 1, gradU0, 3);
  gradUp[2] = MAT2D(idCell, 2, gradU0, 3);
  velC[0]   = MAT2D(idCell, 0, vel, 3);
  velC[1]   = MAT2D(idCell, 1, vel, 3);
  velC[2]   = MAT2D(idCell, 2, vel, 3);
  uC = u0[idCell];
/*... p(n)*/
  presC0 = MAT2D(idCell, 0, pres, 2);
/*... p(n+1)*/
  presC = MAT2D(idCell, 1, pres, 2);
/*...*/
  gradPresC[0] = MAT2D(idCell, 0, gradPres, 3);
  gradPresC[1] = MAT2D(idCell, 1, gradPres, 3);
  gradPresC[2] = MAT2D(idCell, 2, gradPres, 3);
  dFieldC[0]   = MAT2D(idCell, 0, dField, 3);
  dFieldC[1]   = MAT2D(idCell, 1, dField, 3);
  dFieldC[2]   = MAT2D(idCell, 2, dField, 3);
/*...................................................................*/

  p = 0.0e0;
  sP = 0.0e0;
  for (nf = 0; nf<nFace; nf++) {
    vizNel = lViz[nf];
    lFarea = fArea[nf];
    lNormal[0] = MAT2D(nf, 0, normal, 3);
    lNormal[1] = MAT2D(nf, 1, normal, 3);
    lNormal[2] = MAT2D(nf, 2, normal, 3);
    lXmcc[0] = MAT2D(nf, 0, xmcc, 3);
    lXmcc[1] = MAT2D(nf, 1, xmcc, 3);
    lXmcc[2] = MAT2D(nf, 2, xmcc, 3);
/*... dominio*/
    if (vizNel  > -1) {
/*...*/
      densityV   = lDensity[nf];
      sHeatV     = lSheat[nf];
      thermCoefV = lTconductivity[nf];
      diffEffV   = diffCoefV = thermCoefV/sHeatV;
      if (fTurb)
      {
        eddyViscosityV = MAT2D(nf, 1, lViscosity, 2);
        diffEffV       = diffCoefV + eddyViscosityV/prTsgs;
      }
/*... p(n+1)*/
      presV   = MAT2D(nf, 1, pres, 2);
      uV      = u0[nf]; 
      lModKsi = mKsi[nf];
      duDksi = (uV - uC) / lModKsi;
      for (i = 0; i<3; i++)
      {
        gradPresV[i] = MAT2D(nf, i, gradPres, 3);
        dFieldV[i]   = MAT2D(nf, i, dField, 3);
        velV[i]      = MAT2D(nf, i, vel, 3);
        lKsi[i]      = MAT2D(nf, i, ksi, 3);
        lvSkew[i]    = MAT2D(nf, i, vSkew, 3);
/*...*/
        gradUv[i] = MAT2D(nf, i, gradU0, 3);
        ccV[i] = MAT2D(nf, i, cc, 3);
        lXm[i] = MAT2D(nf, i, xm, 3); 
      }
/*...................................................................*/

/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      s[0] = lFarea * lNormal[0];
      s[1] = lFarea * lNormal[1];
      s[2] = lFarea * lNormal[2];
/*...*/
      difusionSchemeNew(s, lKsi, e, t, ndm, iCodDif);
/*...................................................................*/

/*...*/
      alpha = interpolFace(lvSkew        , lXmcc
                         , volume[idCell], volume[nf]
                         , lModKsi       , ndm
                         , iCodPolFace);
      alphaMenosUm = 1.0e0 - alpha;
/*...................................................................*/

/*... media harmonica*/
      diffEff = mediaHarmonica(diffEffV      ,diffEffC
                              ,alphaMenosUm  ,alpha);
/*...................................................................*/

/*... difusao direta*/
      coef = diffEff;
      modE = sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
      dfd = coef * modE / lModKsi;
/*...................................................................*/

/*...*/
      gf[0] = alphaMenosUm * gradUp[0] + alpha * gradUv[0];
      gf[1] = alphaMenosUm * gradUp[1] + alpha * gradUv[1];
      gf[2] = alphaMenosUm * gradUp[2] + alpha * gradUv[2];
      dFieldF[0] = alphaMenosUm * dFieldC[0] + alpha * dFieldV[0];
      dFieldF[1] = alphaMenosUm * dFieldC[1] + alpha * dFieldV[1];
      dFieldF[2] = alphaMenosUm * dFieldC[2] + alpha * dFieldV[2];
      densityM = alphaMenosUm * densityC + alpha * densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = interpolFaceVel(velC       , velV
                         , presC       , presV
                         , gradPresC   , gradPresV
                         , lNormal     , lKsi
                         , lModKsi     , dFieldF
                         , alphaMenosUm, alpha
                         , ndm);
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gf[0] * lKsi[0] + gf[1] * lKsi[1] + gf[2] * lKsi[2];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du = duDksi - gfKsi;
      gradUcomp[0] = gf[0] + du * lKsi[0];
      gradUcomp[1] = gf[1] + du * lKsi[1];
      gradUcomp[2] = gf[2] + du * lKsi[2];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gradUcomp[0] * t[0]
            + gradUcomp[1] * t[1]
            + gradUcomp[2] * t[2];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc = coef * gfKsi;
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      if(fTemp)
      {
        sHeatF =  alphaMenosUm *  sHeatC  + alpha *  sHeatV ;
        cv = sHeatF * densityM * wfn*lFarea;
      }
      else
        cv = densityM * wfn*lFarea;
/*...................................................................*/

/*...*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      v[2] = lXm[2] - ccV[2];
      advectiveSchemeScalar(uC          , uV
                          , gradUp      , gradUv
                          , gradUcomp   , lvSkew
                          , lXmcc       , v
                          , lKsi        , lModKsi
                          , wfn         , &cvc
                          , alphaMenosUm, alpha
                          , pAdv        , ndm
                          , iCodAdv1    , iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nf] = dfd - min(cv, 0e0);
      sP += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p += dfdc - cv * cvc;
/*...................................................................*/

/*...*/
      if(fEntalpy)
      {
        for(k=0,tmp3=0.e0;k<ns;k++)
        {
/*...*/
          gradYV[0] = MAT3D(nf    ,k, 0, gradY, ns ,3);
          gradYV[1] = MAT3D(nf    ,k, 1, gradY, ns ,3);
          gradYV[2] = MAT3D(nf    ,k, 2, gradY, ns ,3);
/*...*/
          dV        = MAT2D(nf    , k, diffY, ns);
/*...*/
          hskV      = MAT2D(nf    , k, enthalpyk, ns);
          hsk       = alphaMenosUm*hskC[k]   + alpha*hskV;
          diffEff   = alphaMenosUm*diffYC[k] + alpha*dV;
/*...*/
          tmp2      = (alphaMenosUm*gradYC[k][0] + alpha*gradYV[0])*lNormal[0] 
                    + (alphaMenosUm*gradYC[k][1] + alpha*gradYV[1])*lNormal[1] 
                    + (alphaMenosUm*gradYC[k][2] + alpha*gradYV[2])*lNormal[2]; 

          tmp3      += hsk*diffEff*tmp2; 
        }
        p+=densityM*lFarea*tmp3;
      }
/*...................................................................*/
    }
/*... contorno*/
    else {

/*...*/
      if(fEntalpy)
      {
        for(k=0,tmp3=0.e0;k<ns;k++)
        {     
/*...*/
          hsk  = hskC[k]*diffYC[k];
/*...*/
          tmp2 = gradYC[k][0]*lNormal[0] 
               + gradYC[k][1]*lNormal[1] 
               + gradYC[k][2]*lNormal[2]; 
          tmp3 += hsk*tmp2; 
        }
        p+=densityC*lFarea*tmp3;
      }
/*...................................................................*/

/*...*/
      lA[nf] = 0.e0;
      if (lFaceR[nf])
      {
        wfn = velC[0] * lNormal[0]
          + velC[1] * lNormal[1]
          + velC[2] * lNormal[2];
/*...cargas*/
        nCarg1 = lFaceR[nf] - 1;
        xx[0] = MAT2D(nf, 0, xm, 3);
        xx[1] = MAT2D(nf, 1, xm, 3);
        xx[2] = MAT2D(nf, 2, xm, 3);
        pLoadEnergy(vProp
                  , &sP           , &p
                  , &tA           , velC
                  , uC            , lNormal
                  , thermCoefC    , densityC
                  , viscosityC    , sHeatC
                  , prTwall       , xx
                  , lFarea        , dcca[nf]
                  , &loads[nCarg1]
                  , wallPar       , ndm
                  , true          , fTemp
                  , fKelvin       , fSheat
                  , fWallModel    , wallType);
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... | du1/dx1 du1/dx2 du1/dx3*/
  gradVelC[0][0] = MAT3D(idCell, 0, 0, gradVel, 3, 3);
  gradVelC[0][1] = MAT3D(idCell, 0, 1, gradVel, 3, 3);
  gradVelC[0][2] = MAT3D(idCell, 0, 2, gradVel, 3, 3);
/*... | du2/dx1 du2/dx2 du2/dx3*/
  gradVelC[1][0] = MAT3D(idCell, 1, 0, gradVel, 3, 3);
  gradVelC[1][1] = MAT3D(idCell, 1, 1, gradVel, 3, 3);
  gradVelC[1][2] = MAT3D(idCell, 1, 2, gradVel, 3, 3);
/*... | du3/dx1 du3/dx2 du3/dx3*/
  gradVelC[2][0] = MAT3D(idCell, 2, 0, gradVel, 3, 3);
  gradVelC[2][1] = MAT3D(idCell, 2, 1, gradVel, 3, 3);
  gradVelC[2][2] = MAT3D(idCell, 2, 2, gradVel, 3, 3);
/*...................................................................*/

/*...*/
  if (fDisp)
  {
    tmp = gradVelC[0][0] + gradVelC[1][1] + gradVelC[2][2];
    psi = tmp * tmp;
/*...*/
    tmp = gradVelC[0][0] * gradVelC[0][0]
        + gradVelC[1][1] * gradVelC[1][1]
        + gradVelC[2][2] * gradVelC[2][2];
    tmp1 = gradVelC[0][1] + gradVelC[1][0];
    tmp2 = gradVelC[0][2] + gradVelC[2][0];
    tmp3 = gradVelC[1][2] + gradVelC[2][1];
    phi = 2.0*tmp + tmp1 * tmp1 + tmp2 * tmp2 + tmp3 * tmp3;
    tmp = (lambda*psi + (viscosityC + eddyViscosityC)*phi);
/*...*/
    p += tmp * volume[idCell];
  }
/*.....................................................................*/

/*...*/
  if (fPresWork)
  {
/*... derivada materia da pressao*/  
/*... pressao termodinamica (pressao em Pa)*/
    tmp1 = 0.5e+0*(Pth[2] - Pth[0]) / dt;  
/*... pressao fluidodinamica*/
    tmp = (presC - presC0) / dt
        + velC[0] * gradPresC[0]
        + velC[1] * gradPresC[1]
        + velC[2] * gradPresC[2];
/*...*/
    p += (tmp + tmp1) * volume[idCell];
/*.....................................................................*/
  }
/*.....................................................................*/

/*...*/
  if(fComb)
    p += rateHeat*volume[idCell];
/*.....................................................................*/

/*... distretizacao temporal*/
  if (fTemp)
    tmp1 = sHeatC * densityC;
  else
    tmp1 = densityC;

  if (fTime) {
/*... EULER*/
    if (typeTime == EULER)
      sP += tmp1 * volume[idCell] / dt;
/*...BACKWARD*/
    else if (typeTime == BACKWARD) {
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp * tmp1*volume[idCell];
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
  for (nf = 0; nf<nFace; nf++) {
    if (lViz[nf] > -1) {
/*... pasando os valoeres conhecidos para o lado direito*/
      if (lId[nf] == -2)
        p += lA[nf] * u0[nf];
      else
/*... residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell += lA[nf] * u0[nf];
    }
  }
/*... residuo: R = F - KpUp*/
  rCell += p - lA[idCell] * u0[idCell];
/*...................................................................*/

/*... under-relaxation(simple)*/
  lA[idCell] = lA[idCell] / underU;
  if (!fRes)
    p += (1.e0 - underU)*lA[idCell] * u0[idCell];
/*...................................................................*/

/*...*/
  for (nf = 0; nf<nFace; nf++)
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


/********************************************************************
 * Data de criacao    : 03/10/2017                                   *
 * Data de modificaco : 08/10/2019                                   *
 *-------------------------------------------------------------------*
 * CELLENERGY3D: Celula 3D para transporte                           *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * loads     -> definicoes de cargas                                 *
 * model     -> modelo da equacao de energia                         *
 * advT      -> tecnica da discretizacao do termo advecao            *
 * diffT     -> tecnica da discretizacao do termo difusivo           *
 * tModel    -> modelo de turbulencia                                *
 * vProp     -> propedades variaveis (true|false)                    *
 * lnFace    -> numero de faces da celula central e seus vizinhos    *
 * lGeomType -> tipo geometrico da celula central e seus vizinhos    *
 * lprop     -> propriedade fisicas das celulas                      *
 * lViz      -> viznhos da celula central                            *
 * lId       -> numeracoes das equacoes das celulas                  *
 * Ksi       -> vetores que unem centroide da celula central aos     *
 *            vizinhos destas                                        *
 * mKsi      -> modulo do vetor ksi                                  *
 * eta       -> vetores paralelos as faces das celulas               *
 * fArea     -> area das faces                                       *
 * normal    -> vetores normais as faces das celulas                 *
 * area      -> area da celula central                               *
 * xm        -> pontos medios das faces da celula central            *
 * xmcc      -> vetores que unem o centroide aos pontos medios das   *
 *            faces da celula central                                *
 * vSkew  -> vetor entre o ponto medio a intersecao que une os       *
 *            centrois compartilhado nessa face da celula central    *
 * mvSkew -> distacia entre o ponto medio a intersecao que une os    *
 *            centrois compartilhado nessa face da celula central    *
 * lA        -> nao definido                                         *
 * lB        -> nao definido                                         *
 * dcca      -> menor distancia do centroide central a faces desta   *
 *              celula                                               *
 * dField    -> matriz D do metodo simple                            *
 * wallPar   -> parametros de parede  ( yPlus, uPlus, uFri,sTressW)  *
 * lRcell    -> nao definido                                         *
 * ddt       -> discretizacao temporal                               *
 * faceR     -> restricoes por elmento                               *
 * faceL     -> carga por elemento                                   *
 * u0        -> solucao conhecida                                    *
 * gradU0    -> gradiente rescontruido da solucao conhecida          *
 * vel       -> campo de velocidade conhecido                        *
 * gradVel   -> gradiente rescontruido da velocidade                 *
 * pres      -> pressao do tempo atual e do tempo anterior           *
 * gradPres  -> gradiente de pressao do tempo atual                  *
 * lDensity  -> massa especifica com variacao temporal               *
 * lSheat    -> calor especifico com variacao temporal               *
 * lDviscosity-> viscosidade dinamica com variacao temporal          *
 * lTconductivity-> condutividade termica com variacao temporal      *
 * lEnthalpyk -> Entalpia das especies                               *
 * lGradY    -> gradiente das especies                               *
 * yFac      -> fracoes massicas                                     *
 * diffY     -> coeficiente de difusao massica por especies          *
 * rateHeat  -> energia liberada pela combustao (KJ/s )              *
 * cc        -> centroides da celula centra e seus vizinhos          *
 * underU    -> parametro de sob relaxamento                         *
 * fSheat    -> calor especifico com variacao com a Temperatura      *
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
 * OBS: Energia KJ/KG                                                *
 *-------------------------------------------------------------------*
 *********************************************************************/
void cellEnergy3DOld(Loads *loads               , Loads *lVel
                , Advection *advT            , Diffusion *diffT
                , Turbulence *tModel         , EnergyModel *eModel
                , Combustion *cModel         , PropVarFluid *vProp
                , short *RESTRICT lGeomType  
                , INT *RESTRICT lViz         , INT *RESTRICT lId
                , DOUBLE *RESTRICT ksi       , DOUBLE *RESTRICT mKsi
                , DOUBLE *RESTRICT eta       , DOUBLE *RESTRICT fArea
                , DOUBLE *RESTRICT normal    , DOUBLE *RESTRICT volume
                , DOUBLE *RESTRICT xm        , DOUBLE *RESTRICT xmcc
                , DOUBLE *RESTRICT dcca      , DOUBLE *RESTRICT cc
                , DOUBLE *RESTRICT vSkew     , DOUBLE *RESTRICT mvSkew
                , DOUBLE *RESTRICT lA        , DOUBLE *RESTRICT lB
                , DOUBLE *RESTRICT lRcell    , Temporal *ddt
                , short  *RESTRICT lFaceR    , short  *RESTRICT lFaceVelR 
                , DOUBLE *RESTRICT u0        , DOUBLE *RESTRICT gradU0
                , DOUBLE *RESTRICT vel       , DOUBLE *RESTRICT gradVel
                , DOUBLE *RESTRICT pres      , DOUBLE *RESTRICT gradPres
                , DOUBLE *RESTRICT lDensity  , DOUBLE *RESTRICT lSheat
                , DOUBLE *RESTRICT lViscosity, DOUBLE *RESTRICT lTconductivity
                , DOUBLE *RESTRICT enthalpyk , DOUBLE *RESTRICT gradY 
                , DOUBLE *RESTRICT diffY     , DOUBLE *RESTRICT yFrac     
                , DOUBLE const rateHeat  
                , DOUBLE *RESTRICT dField    , DOUBLE *RESTRICT wallPar
                , DOUBLE const underU
                , const short nEn            , short const nFace
                , const short ndm            , INT const nel)
{
  bool fTime, fDisp, fRes, fPresWork, fTemp, fTurb, fWallModel, fKelvin
    , fSheat,fEntalpy;
  short iCodAdv1, iCodAdv2, iCodDif, wallType, idCell, nf, nCarg1
    , nCarg2, typeTime, iCodPolFace, k, ns;
/*...*/
  INT vizNel;
/*...*/
  DOUBLE thermCoefF,thermCoefC, thermCoefV, densityC, densityV, densityM,
    diffEffC, diffEffV, diffEff, sHeatC, sHeatV, sHeatF,
    diffTurbF, eddyViscosityC, eddyViscosityV, viscosityC,
    hskV,hskC[MAXSPECIES],hsk, gradYC[MAXSPECIES][3],gradYV[3], 
    diffYC[MAXSPECIES], yFracC[MAXSPECIES], dV, tA, coef,
    tmp, tmp1, tmp2, tmp3, prTwall, prTsgs;
  DOUBLE p, sP, dfd, gfKsi, lvSkew[3], alpha, alphaMenosUm;
  DOUBLE v[3], gradUcomp[3], lKsi[3], lNormal[3], gf[3];
  DOUBLE lModKsi, lFarea, du, duDksi, lXmcc[3], lXm[3];
  DOUBLE gradUp[3], gradUv[3], ccV[3], gradVelC[3][3], rCell, dt, dt0;
  DOUBLE uC, uV;
/*... nonOrtogonal*/
  DOUBLE e[3], t[3], s[3], modE, dfdc, xx[3];
/*... */
  DOUBLE presC, presC0, presV, gradPresC[3], gradPresV[3], wfn
    , velC[3], velV[3], dFieldC[3], dFieldV[3], dFieldF[3], cv, cvc,Pth[3];
/*...*/
  DOUBLE phi, psi, lambda;
  DOUBLE tW, tC;
/*...*/
  DOUBLE pAdv[NPADV];
/*...*/
  bool fComb;

/*...*/
  idCell      = nFace;
  iCodAdv1    = advT->iCod1;
  iCodAdv2    = advT->iCod2;
  pAdv[0]     = advT->par[0];
  iCodDif     = diffT->iCod;
  iCodPolFace = INTPOLFACELINEAR;
/*...................................................................*/

/*...*/
  dt        = ddt->dt[0];
  dt0       = ddt->dt[1];
  typeTime  = ddt->type;
  fTime     = ddt->flag;
  fDisp     = eModel->fDissipation;
  fRes      = eModel->fRes;
  fPresWork = eModel->fPresWork;
  fTemp     = eModel->fTemperature;
  fKelvin   = eModel->fKelvin;
  fTurb     = tModel->fTurb;
  prTwall   = tModel->PrandltTwall;
  prTsgs    = tModel->PrandltTsgs;
  fWallModel= tModel->fWall;
  wallType  = tModel->wallType;
  fSheat    = vProp->fSpecificHeat;
  fComb     = cModel->fCombustion;
  ns        = cModel->nOfSpecies;
  fEntalpy  = eModel->fDiffEnergy;
  Pth[0]    = thDynamic.pTh[0];
  Pth[1]    = thDynamic.pTh[1];
  Pth[2]    = thDynamic.pTh[2];
/*...................................................................*/

/*... propriedades da celula*/
  eddyViscosityC = eddyViscosityV = 0.e0;
  densityC = lDensity[idCell];
  sHeatC = lSheat[idCell];
  thermCoefC = lTconductivity[idCell];
  viscosityC = MAT2D(idCell, 0, lViscosity, 2);
  if (fTurb) eddyViscosityC = MAT2D(idCell, 1, lViscosity, 2);
  lambda = -D2DIV3 * (viscosityC + eddyViscosityC); 
  if(fEntalpy)
  {
    for(k=0;k<ns;k++)
    {
/*... propriedades da celula*/
      diffYC[k] = MAT2D(idCell, k, diffY, ns);
/*...*/
      gradYC[k][0] = MAT3D(idCell, k, 0, gradY, ns, 3);
      gradYC[k][1] = MAT3D(idCell, k, 1, gradY, ns, 3);
      gradYC[k][2] = MAT3D(idCell, k, 2, gradY, ns, 3);
/*...*/
      hskC[k] = MAT2D(idCell, k, enthalpyk, ns);
/*...*/
      yFracC[k] = MAT2D(idCell, k, yFrac, ns);
    }
  }
/*...................................................................*/

/*...*/
  gradUp[0] = MAT2D(idCell, 0, gradU0, 3);
  gradUp[1] = MAT2D(idCell, 1, gradU0, 3);
  gradUp[2] = MAT2D(idCell, 2, gradU0, 3);
  velC[0] = MAT2D(idCell, 0, vel, 3);
  velC[1] = MAT2D(idCell, 1, vel, 3);
  velC[2] = MAT2D(idCell, 2, vel, 3);
  uC = u0[idCell];
/*... p(n)*/
  presC0 = MAT2D(idCell, 0, pres, 2);
/*... p(n+1)*/
  presC = MAT2D(idCell, 1, pres, 2);
/*...*/
  gradPresC[0] = MAT2D(idCell, 0, gradPres, 3);
  gradPresC[1] = MAT2D(idCell, 1, gradPres, 3);
  gradPresC[2] = MAT2D(idCell, 2, gradPres, 3);
  dFieldC[0] = MAT2D(idCell, 0, dField, 3);
  dFieldC[1] = MAT2D(idCell, 1, dField, 3);
  dFieldC[2] = MAT2D(idCell, 2, dField, 3);
/*...................................................................*/

  p = 0.0e0;
  sP = 0.0e0;
  for (nf = 0; nf<nFace; nf++) {
    vizNel = lViz[nf];
    lFarea = fArea[nf];
    lNormal[0] = MAT2D(nf, 0, normal, 3);
    lNormal[1] = MAT2D(nf, 1, normal, 3);
    lNormal[2] = MAT2D(nf, 2, normal, 3);
    lXmcc[0] = MAT2D(nf, 0, xmcc, 3);
    lXmcc[1] = MAT2D(nf, 1, xmcc, 3);
    lXmcc[2] = MAT2D(nf, 2, xmcc, 3);
/*... dominio*/
    if (vizNel  > -1) {
/*...*/
      densityV = lDensity[nf];
      sHeatV = lSheat[nf];
      thermCoefV = lTconductivity[nf];
      if (fTurb) eddyViscosityV = MAT2D(nf, 1, lViscosity, 2);
/*... p(n+1)*/
      presV = MAT2D(nf, 1, pres, 2);
      gradPresV[0] = MAT2D(nf, 0, gradPres, 3);
      gradPresV[1] = MAT2D(nf, 1, gradPres, 3);
      gradPresV[2] = MAT2D(nf, 2, gradPres, 3);
      dFieldV[0] = MAT2D(nf, 0, dField, 3);
      dFieldV[1] = MAT2D(nf, 1, dField, 3);
      dFieldV[2] = MAT2D(nf, 2, dField, 3);
/*...*/
      uV = u0[nf];
      velV[0] = MAT2D(nf, 0, vel, 3);
      velV[1] = MAT2D(nf, 1, vel, 3);
      velV[2] = MAT2D(nf, 2, vel, 3);
/*...*/
      lKsi[0] = MAT2D(nf, 0, ksi, 3);
      lKsi[1] = MAT2D(nf, 1, ksi, 3);
      lKsi[2] = MAT2D(nf, 2, ksi, 3);
      lModKsi = mKsi[nf];
/*...*/
      lvSkew[0] = MAT2D(nf, 0, vSkew, 3);
      lvSkew[1] = MAT2D(nf, 1, vSkew, 3);
      lvSkew[2] = MAT2D(nf, 2, vSkew, 3);
/*...*/
      duDksi = (uV - uC) / lModKsi;
/*...*/
      gradUv[0] = MAT2D(nf, 0, gradU0, 3);
      gradUv[1] = MAT2D(nf, 1, gradU0, 3);
      gradUv[2] = MAT2D(nf, 2, gradU0, 3);

      ccV[0] = MAT2D(nf, 0, cc, 3);
      ccV[1] = MAT2D(nf, 1, cc, 3);
      ccV[2] = MAT2D(nf, 2, cc, 3);

      lXm[0] = MAT2D(nf, 0, xm, 3);
      lXm[1] = MAT2D(nf, 1, xm, 3);
      lXm[2] = MAT2D(nf, 2, xm, 3);
/*...................................................................*/

/*... termo difusivo
      grad(phi)*S = (grad(phi)*E)Imp + (grad(phi)*T)Exp*/
      s[0] = lFarea * lNormal[0];
      s[1] = lFarea * lNormal[1];
      s[2] = lFarea * lNormal[2];
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
      if (fTemp)
      {
        sHeatF     = alphaMenosUm*sHeatC + alpha*sHeatV;
        thermCoefF = alphaMenosUm*thermCoefC + alpha*thermCoefV; 
        diffTurbF  = (alphaMenosUm*eddyViscosityC 
                   + alpha*eddyViscosityV)/prTsgs; 
        diffEff = thermCoefF + sHeatF * diffTurbF;
      }
      else
      {
        thermCoefF = alphaMenosUm*thermCoefC + alpha*thermCoefV;   
        sHeatF     = alphaMenosUm*sHeatC + alpha*sHeatV;
        diffTurbF  = (alphaMenosUm*eddyViscosityC 
                   + alpha*eddyViscosityV)/prTsgs;
        diffEff = thermCoefF/sHeatF + diffTurbF;
      }
/*...................................................................*/

/*... difusao direta*/
      coef = diffEff;
      modE = sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
      dfd = coef * modE / lModKsi;
/*...................................................................*/

/*...*/
      gf[0] = alphaMenosUm * gradUp[0] + alpha * gradUv[0];
      gf[1] = alphaMenosUm * gradUp[1] + alpha * gradUv[1];
      gf[2] = alphaMenosUm * gradUp[2] + alpha * gradUv[2];
      dFieldF[0] = alphaMenosUm * dFieldC[0] + alpha * dFieldV[0];
      dFieldF[1] = alphaMenosUm * dFieldC[1] + alpha * dFieldV[1];
      dFieldF[2] = alphaMenosUm * dFieldC[2] + alpha * dFieldV[2];
      densityM = alphaMenosUm * densityC + alpha * densityV;
/*...................................................................*/

/*... velocidade normal a face*/
      wfn = interpolFaceVel(velC, velV
        , presC, presV
        , gradPresC, gradPresV
        , lNormal, lKsi
        , lModKsi, dFieldF
        , alphaMenosUm, alpha
        , ndm);
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gf[0] * lKsi[0] + gf[1] * lKsi[1] + gf[2] * lKsi[2];
/*...................................................................*/

/*... gradiente compacto (Darwish e Moukalled)*/
      du = duDksi - gfKsi;
      gradUcomp[0] = gf[0] + du * lKsi[0];
      gradUcomp[1] = gf[1] + du * lKsi[1];
      gradUcomp[2] = gf[2] + du * lKsi[2];
/*...................................................................*/

/*... derivadas direcionais*/
      gfKsi = gradUcomp[0] * t[0]
            + gradUcomp[1] * t[1]
            + gradUcomp[2] * t[2];
/*...................................................................*/

/*... correcao nao-ortogonal*/
      dfdc = coef * gfKsi;
/*...................................................................*/

/*... fluxo convectivo upwind de primeira ordem*/
      if (fTemp)
        cv = sHeatF * densityM*wfn*lFarea;
      else
        cv = densityM * wfn*lFarea;
/*...................................................................*/

/*...*/
      v[0] = lXm[0] - ccV[0];
      v[1] = lXm[1] - ccV[1];
      v[2] = lXm[2] - ccV[2];
      advectiveSchemeScalar(uC, uV
        , gradUp, gradUv
        , gradUcomp, lvSkew
        , lXmcc, v
        , lKsi, lModKsi
        , wfn, &cvc
        , alphaMenosUm, alpha
        , pAdv, ndm
        , iCodAdv1, iCodAdv2);
/*...................................................................*/

/*...*/
      lA[nf] = dfd - min(cv, 0e0);
      sP += cv;
/*... correcao nao ortogonal e do fluxo advectivo*/
      p += dfdc - cv * cvc;
/*...................................................................*/

/*...*/
      if(fEntalpy)
      {
        for(k=0,tmp3=0.e0;k<ns;k++)
        {
/*...*/
          gradYV[0] = MAT3D(nf    ,k, 0, gradY, ns ,3);
          gradYV[1] = MAT3D(nf    ,k, 1, gradY, ns ,3);
          gradYV[2] = MAT3D(nf    ,k, 2, gradY, ns ,3);
/*...*/
          dV        = MAT2D(nf    , k, diffY, ns);
/*...*/
          hskV      = MAT2D(nf    , k, enthalpyk, ns);
          hsk       = alphaMenosUm*hskC[k]   + alpha*hskV;
          diffEff   = alphaMenosUm*diffYC[k] + alpha*dV;
/*...*/
          tmp2      = (alphaMenosUm*gradYC[k][0] + alpha*gradYV[0])*lNormal[0] 
                    + (alphaMenosUm*gradYC[k][1] + alpha*gradYV[1])*lNormal[1] 
                    + (alphaMenosUm*gradYC[k][2] + alpha*gradYV[2])*lNormal[2]; 

          tmp3      += hsk*diffEff*tmp2; 
        }
        p+=densityM*lFarea*tmp3;
      }
/*...................................................................*/
    }
/*... contorno*/
    else {

/*...*/
      if(fEntalpy)
      {
        for(k=0,tmp3=0.e0;k<ns;k++)
        {     
/*...*/
          hsk  = hskC[k]*diffYC[k];
/*...*/
          tmp2 = gradYC[k][0]*lNormal[0] 
               + gradYC[k][1]*lNormal[1] 
               + gradYC[k][2]*lNormal[2]; 
          tmp3 += hsk*tmp2; 
        }
        p+=densityC*lFarea*tmp3;
      }
/*...................................................................*/

/*...*/
      lA[nf] = 0.e0;
      if (lFaceR[nf])
      {
        wfn = velC[0] * lNormal[0]
          + velC[1] * lNormal[1]
          + velC[2] * lNormal[2];
/*...cargas*/
        nCarg1 = lFaceR[nf] - 1;
        nCarg2 = lFaceVelR[nf] - 1;
        xx[0] = MAT2D(nf, 0, xm, 3);
        xx[1] = MAT2D(nf, 1, xm, 3);
        xx[2] = MAT2D(nf, 2, xm, 3);
        pLoadEnergy(vProp
          , &sP           , &p
          , &tA           , velC
          , uC            , lNormal
          , thermCoefC    , densityC
          , viscosityC    , sHeatC
          , prTwall       , xx
          , lFarea        , dcca[nf]
          , &loads[nCarg1]
          , wallPar       , ndm
          , true          , fTemp
          , fKelvin       , fSheat
          , fWallModel    , wallType);
/*...................................................................*/
      }
/*...................................................................*/

/*...*/
      else
      {
/*... inertial sub-layer*/
        if (wallPar[0] > 11.81e0)
        {
/*... energia na forma da temperatura*/
          if (fTemp)
            tW = uC;
/*...................................................................*/

/*... energia na forma da entalpia*/
          else
          {
            if(fComb)
            {
              tC = specificEnthalpyForTempOfMix(&vProp->sHeat, 298.15
                                               , uC          , yFracC 
                                               , sHeatC      , ns
                                               , fSheat      , fKelvin
                                               , nel );
              tW = tC;
              tW = tempToSpecificEnthalpyMix(&vProp->sHeat, yFracC
                                            ,tW           , sHeatC
                                            ,ns  
                                            ,fSheat       , fKelvin);
            }
            else
            {
              tC = specificEnthalpyForTemp(&vProp->sHeat, 298.15
                                          , uC          , sHeatC
                                          , fSheat      , fKelvin);
              tW = tC;
              tW = tempForSpecificEnthalpy( &vProp->sHeat
                                          , tW           , sHeatC
                                          , fSheat       , fKelvin);
            }        
          }
/*...................................................................*/

/*...*/
          tmp = thermCoefC * lFarea / dcca[nf];
          if (!fTemp) tmp /= sHeatC;
          sP += tmp;
          p += tmp * tW;
/*...................................................................*/
        }
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*... | du1/dx1 du1/dx2 du1/dx3*/
  gradVelC[0][0] = MAT3D(idCell, 0, 0, gradVel, 3, 3);
  gradVelC[0][1] = MAT3D(idCell, 0, 1, gradVel, 3, 3);
  gradVelC[0][2] = MAT3D(idCell, 0, 2, gradVel, 3, 3);
/*... | du2/dx1 du2/dx2 du2/dx3*/
  gradVelC[1][0] = MAT3D(idCell, 1, 0, gradVel, 3, 3);
  gradVelC[1][1] = MAT3D(idCell, 1, 1, gradVel, 3, 3);
  gradVelC[1][2] = MAT3D(idCell, 1, 2, gradVel, 3, 3);
/*... | du3/dx1 du3/dx2 du3/dx3*/
  gradVelC[2][0] = MAT3D(idCell, 2, 0, gradVel, 3, 3);
  gradVelC[2][1] = MAT3D(idCell, 2, 1, gradVel, 3, 3);
  gradVelC[2][2] = MAT3D(idCell, 2, 2, gradVel, 3, 3);
/*...................................................................*/

/*...*/
  if (fDisp)
  {
    tmp = gradVelC[0][0] + gradVelC[1][1] + gradVelC[2][2];
    psi = tmp * tmp;
/*...*/
    tmp = gradVelC[0][0] * gradVelC[0][0]
      + gradVelC[1][1] * gradVelC[1][1]
      + gradVelC[2][2] * gradVelC[2][2];
    tmp1 = gradVelC[0][1] + gradVelC[1][0];
    tmp2 = gradVelC[0][2] + gradVelC[2][0];
    tmp3 = gradVelC[1][2] + gradVelC[2][1];
    phi = 2.0*tmp + tmp1 * tmp1 + tmp2 * tmp2 + tmp3 * tmp3;
    tmp = (lambda*psi + (viscosityC + eddyViscosityC)*phi);
/*...*/
    p += tmp * volume[idCell];
  }
/*.....................................................................*/

/*...*/
  if (fPresWork)
  {
/*... derivada materia da pressao*/  
/*... pressao termodinamica*/
    tmp1 = 0.5e-03*(Pth[2] - Pth[0]) / dt;  
/*... pressao fluidodinamica*/
    tmp = (presC - presC0) / dt
        + velC[0] * gradPresC[0]
        + velC[1] * gradPresC[1]
        + velC[2] * gradPresC[2];
/*...*/
    p += (tmp + tmp1) * volume[idCell];
/*.....................................................................*/
  }
/*.....................................................................*/

/*...*/
  if(fComb)
    p += rateHeat*volume[idCell];
/*.....................................................................*/

/*... distretizacao temporal*/
  if (fTemp)
    tmp1 = sHeatC * densityC;
  else
    tmp1 = densityC;

  if (fTime) {
/*... EULER*/
    if (typeTime == EULER)
      sP += tmp1 * volume[idCell] / dt;
/*...BACKWARD*/
    else if (typeTime == BACKWARD) {
      tmp = 1.e0 / dt + 1.e0 / (dt + dt0);
      sP += tmp * tmp1*volume[idCell];
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
  for (nf = 0; nf<nFace; nf++) {
    if (lViz[nf] > -1) {
/*... pasando os valoeres conhecidos para o lado direito*/
      if (lId[nf] == -2)
        p += lA[nf] * u0[nf];
      else
/*... residuo (R = F-KvizUviz ) e valores prescritos por elemento*/
        rCell += lA[nf] * u0[nf];
    }
  }
/*... residuo: R = F - KpUp*/
  rCell += p - lA[idCell] * u0[idCell];
/*...................................................................*/

/*... under-relaxation(simple)*/
  lA[idCell] = lA[idCell] / underU;
  if (!fRes)
    p += (1.e0 - underU)*lA[idCell] * u0[idCell];
/*...................................................................*/

/*...*/
  for (nf = 0; nf<nFace; nf++)
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
