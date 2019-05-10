#include<Combustion.h>

static void vectorMolarMass(DOUBLE *mW, Combustion *cModel);

/*********************************************************************
 * Data de criacao    : 30/07/2018                                   *
 * Data de modificaco : 02/05/2019                                   *
 *-------------------------------------------------------------------*
 * combustionModel : modelo de combustao                             *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 *********************************************************************/
void combustionModel(Memoria *m         , PropVarFluid *prop
                   , Loads *loadsComb   , Loads *loadsVel
                   , Turbulence *tModel , Combustion *cModel
                   , EnergyModel *eModel, Mesh *mesh
                   , SistEq *sistEqComb , Solv *solvComb  
                   , Simple *sp         , Scheme *sc        
                   , PartMesh *pMesh    , FileOpt *opt
                   , short itSimple    ) 
{
  bool fComb[MAX_COMB],fExit,rel
      ,fSheat = prop->fSpecificHeat;
  short itComb, conv, i, j, kZero, nComb, jj = 1, jPrint;
  INT desloc;
  DOUBLE tmp,tb[MAX_COMB],rCell[MAX_COMB],rCell0[MAX_COMB],tolComb;
  DOUBLE *b[MAX_COMB], *bPc, *xu[MAX_COMB];
  DOUBLE *ad[MAX_COMB],*al[MAX_COMB],*au[MAX_COMB],*rCellC[MAX_COMB];
  char slName[][10] = {"zFuel","zAir","zProd"},
       spName[][10] = {"zFuel","zO2" ,"zN2","zCO2","zH2O"};
/*... rel   - criterio de parada com vlores relativos
      kZero - iteracao na qual o resido e normalizadp  
      nComb - numero de especies transportadas
      jPrint - numero de iteracao que ser impresso o residuo na tela
*/
  rel     = true;         
  fExit   = false;
  nComb   = cModel->nComb;
  jPrint  = 50;
  tolComb = sp->tolComb;
  kZero   = sp->kZeroComb;
/*...................................................................*/

/*...*/
  for(i=0;i<nComb; i++)
  {
    fComb[i] = false; 

    desloc = sistEqComb->neq;
    b[i] = &sistEqComb->b[i*desloc];
  
    xu[i] = &sistEqComb->x[i*desloc]; 

    ad[i] = &sistEqComb->ad[i*desloc];

    desloc = 2*sistEqComb->nad + sistEqComb->nadr;
    al[i] = &sistEqComb->al[i*desloc];
 
    au[i] = &sistEqComb->au[i*desloc];

    desloc = mesh->numel;
    rCellC[i] = &mesh->elm.rCellComb[i*desloc];
  
    rCell0[i] = 1.e0;
  }
/*...................................................................*/

/*...*/
  if(opt->fItPlot && !mpiVar.myId)  
    fprintf(opt->fileItPlot[FITPLOTCOMB]
           ,"#itSimple = %d t = %lf\n",itSimple+1,sc->ddt.t);
/*...................................................................*/

/*...*/
  for(itComb = 0; itComb < 1;itComb++)
  {
/*... taxa de comsumo do combustivel*/
    rateFuelConsume(cModel                , mesh->elm.zComb
                  , mesh->elm.temp        , mesh->elm.rateFuel 
                  , mesh->elm.densityFluid,eModel->fKelvin
                  , mesh->numel);    
/*...................................................................*/

/*... reconstruindo do gradiente (Fuel)*/
    tm.rcGradComb   = getTimeC() - tm.rcGradComb;
    rcGradU(m                    , loadsComb
        , mesh->elm.node         , mesh->elm.adj.nelcon
        , mesh->node.x           
        , mesh->elm.nen          , mesh->elm.adj.nViz
        , mesh->elm.cellFace     , mesh->face.owner
        , mesh->elm.geom.volume  , mesh->elm.geom.dcca
        , mesh->elm.geom.xmcc    , mesh->elm.geom.cc
        , mesh->face.mksi        , mesh->face.ksi
        , mesh->face.eta         , mesh->face.area
        , mesh->face.normal      , mesh->face.xm
        , mesh->face.mvSkew      , mesh->face.vSkew
        , mesh->elm.geomType     , mesh->elm.material.prop
        , mesh->elm.material.type, mesh->elm.mat 
        , mesh->elm.leastSquare  , mesh->elm.leastSquareR
        , mesh->elm.faceResZcomb , mesh->elm.faceLoadZcomb
        , mesh->elm.zComb        , mesh->elm.gradZcomb
        , mesh->node.zComb       , sc->rcGrad
        , mesh->maxNo            , mesh->maxViz
        , nComb                  , mesh->ndm              
        , &pMesh->iNo            , &pMesh->iEl
        , mesh->numelNov         , mesh->numel
        , mesh->nnodeNov         , mesh->nnode);    
    tm.rcGradComb = getTimeC() - tm.rcGradComb;
/*.................................................................. */

/*... calculo de: A(i),b(i)*/
    tm.systFormComb = getTimeC() - tm.systFormComb;
    systFormComb(loadsComb             , loadsVel
             , &sc->advComb            , &sc->diffComb  
             , tModel                  , cModel
             , prop                    
             , mesh->elm.node          , mesh->elm.adj.nelcon
             , mesh->elm.nen           , mesh->elm.adj.nViz
             , mesh->elm.cellFace      , mesh->face.owner
             , mesh->elm.geom.volume   , mesh->elm.geom.dcca
             , mesh->elm.geom.xmcc     , mesh->elm.geom.cc
             , mesh->face.mksi         , mesh->face.ksi
             , mesh->face.eta          , mesh->face.area
             , mesh->face.normal       , mesh->face.xm
             , mesh->face.mvSkew       , mesh->face.vSkew
             , mesh->elm.geomType      , mesh->elm.material.prop
             , mesh->elm.material.type , mesh->elm.mat
             , sistEqComb->ia          , sistEqComb->ja
             , sistEqComb->al          , sistEqComb->ad
             , sistEqComb->b           , sistEqComb->id
             , mesh->elm.faceResZcomb  , mesh->elm.faceLoadZcomb
             , mesh->elm.faceRvel      , mesh->elm.faceLoadVel
             , mesh->elm.zComb         , mesh->elm.gradZcomb  
             , mesh->elm.rateFuel      , mesh->elm.vel           
             , mesh->elm.pressure0     , mesh->elm.pressure
             , mesh->elm.gradPres      , mesh->elm.rCellComb
             , mesh->elm.densityFluid  , mesh->elm.cDiffComb
             , mesh->elm.eddyViscosity , mesh->elm.wallParameters
             , sp->d
             , sc->ddt                , sp->alphaComb
             , sistEqComb->neq        , sistEqComb->neqNov
             , sistEqComb->nad        , sistEqComb->nadr
             , mesh->maxNo            , mesh->maxViz
             , mesh->ndm              , mesh->numelNov
             , cModel->nComb          , sistEqComb->storage
             , true                   , true
             , true                   , sistEqComb->unsym);
    tm.systFormComb = getTimeC() - tm.systFormComb;
/*...................................................................*/

/*... soma o vetor b(i) = b(i) + b0(i)*/
    addVector(1.0e0                   , sistEqComb->b
            , 1.0e0                   , sistEqComb->b0
            , sistEqComb->neqNov*nComb, sistEqComb->b);
/*...................................................................*/

/*... soma o vetor R(i) = R(i) + b0(i)*/
    updateCellValueSimple(mesh->elm.rCellComb, sistEqComb->b0
                      , sistEqComb->id       , &sistEqComb->iNeq
                      , mesh->numelNov       , sistEqComb->neqNov
                      , nComb
                      , true                 , false);  
/*...................................................................*/

/*...*/
    if(jj == jPrint && !mpiVar.myId)
    {
      printf("\nIt Combustion: %d\n", itComb + 1);
      printf("Residuo:\n");
    }
    if(opt->fItPlot && !mpiVar.myId)
      fprintf(opt->fileItPlot[FITPLOTCOMB],"%9d",itComb+1);
/*...................................................................*/
    
/*...*/
    for(i=0,conv=0;i<nComb;i++){
      tb[i] = sqrt(dot(b[i], b[i], sistEqComb->neqNov));
      if (itComb == 0) tmp = maxArray(tb,nComb);
/*...................................................................*/

 /*...*/
      fComb[i] = true; 
      if ( tb[i] < SZERO || tb[i] == 0.e0 ) fComb[i] = false;
/*...................................................................*/

/*...*/ 
      if( itComb == kZero && rel )
        rCell[i]  = rCell0[i] = sqrt(dot(rCellC[i],rCellC[i]
                                    ,mesh->numelNov));  
      rCell[i] = sqrt(dot(rCellC[i],rCellC[i],mesh->numelNov));
                
      if( jj == jPrint && !mpiVar.myId )
      {
        if(cModel->fLump)
        {
          printf("%5s : %20.8e\n", slName[i],rCell[i] / rCell0[i]);
        }
        else
        {
          printf("%5s : %20.8e\n", spName[i],rCell[i] / rCell0[i]);
        }  
      }
      if(opt->fItPlot && !mpiVar.myId)
        fprintf(opt->fileItPlot[FITPLOTCOMB]," %20.8e ",rCell[i]);
/*...................................................................*/

/*...*/
      if(rCell[i]/ rCell0[i] < tolComb || rCell[i] < 1.e-16) conv++;
      if(conv == nComb && itComb != 0)
      {
        fExit = true;
        break;
      }
/*...................................................................*/
    }
/*...................................................................*/

/*...*/
    if(fExit)
      break;  
/*...................................................................*/

/*...*/
    for(i=0,conv = 0;i<nComb;i++)
    {
/*...Ax=b*/
      tm.solvComb = getTimeC() - tm.solvComb;
      if(fComb[i])
        solverC(m
              , sistEqComb->neq    , sistEqComb->neqNov
              , sistEqComb->nad    , sistEqComb->nadr
              , sistEqComb->ia     , sistEqComb->ja
              , al[i]              , ad[i]           , au[i]
              , b[i]               , xu[i]              
              , &sistEqComb->iNeq  , &sistEqComb->omp
              , solvComb->tol      , solvComb->maxIt
              , sistEqComb->storage, solvComb->solver
              , solvComb->fileSolv , solvComb->log
              , true               , sistEqComb->unsym);
      else
        zero(xu[i],sistEqComb->neq,DOUBLEC);
      tm.solvComb = getTimeC() - tm.solvComb;    
    }
/*...................................................................*/

/*...*/
    if( jj == jPrint) jj = 0;
    jj++;
    if(opt->fItPlot && !mpiVar.myId) 
      fprintf(opt->fileItPlot[FITPLOTCOMB],"\n");
/*...................................................................*/

/*... x -> zComb*/
    updateCellValueBlock(mesh->elm.zComb , sistEqComb->x
                       , sistEqComb->id  , &sistEqComb->iNeq
                       , mesh->numel     , sistEqComb->neq 
                       , cModel->nComb   
                       , cModel->fRes    , true);  
/*...................................................................*/
   
  }
/*...................................................................*/

/*...*/
//regularZ(mesh->elm.zComb,mesh->numelNov
//        ,cModel->nComb  ,cModel->fLump);
/*...................................................................*/

/*...*/
  getSpeciesPrimitives(cModel
                      ,mesh->elm.yFrac,mesh->elm.zComb
                      ,mesh->numelNov);
/*...................................................................*/

/*...*/
  rateHeatRealeseCombustion(cModel            , &prop->sHeat                
                    , mesh->elm.rateHeatReComb, mesh->elm.temp     
                    , mesh->elm.zComb0        , mesh->elm.zComb
                    , mesh->elm.densityFluid  , mesh->elm.rateFuel 
                    , mesh->elm.material.prop , mesh->elm.mat    
                    , sc->ddt.dt[TIME_N]      , mesh->numelNov
                    , fSheat                  , eModel->fKelvin );
/*...................................................................*/

/*...*/
  if(opt->fItPlot)  
    fprintf(opt->fileItPlot[FITPLOTCOMB],"\n");
/*...................................................................*/

/*...*/
  prop->molarMass = mixtureMolarMass(cModel,mesh->elm.yFrac);
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 15/08/2018                                   *
 * Data de modificaco : 03/05/2019                                   *
 *-------------------------------------------------------------------*
 * regularZ : mantem o z entre 0 e 1                                 *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * z            -> fracao massica                                    *
 * numel        -> numero de elementos                               *
 * nOfSpecies   -> numero de especie agrupadas                       *
 * fLump        -> true  - especies agrupadas                        *
 *                 false - especies primitivas                       *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * z -> fracao de massa de regularizadas                             * 
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * z < 0.0 -> z = 0.e0                                               *
 * fLump = true                                                      *
 * sum(z) > 1.0 -> z[zProd] = 1.e0 - (zFuel + zAir)                  *
* fLump = false                                                      *
 * sum(z) > 1.0 -> z[zN2] = 1.e0 - (zFuel + zO2 + zCO2 + zH2O)       * 
 *********************************************************************/
void regularZ(DOUBLE *RESTRICT z    , INT const numel
            , short const nComb      , bool fLump)
{
  short j;
  INT nel;
  DOUBLE zSum;

/*... z < 0.0*/
  for(nel = 0 ; nel < numel; nel++)
  {
    for(j=0;j<nComb;j++)
      if(MAT2D(nel,j,z,nComb) < 0.e0)
        MAT2D(nel,j,z,nComb) = 0.e0;
  }

/*... z <= 1.0*/
  for(nel = 0 ; nel < numel; nel++)
  {
 // for(j=0,zSum = 0.e0;j<nComb;j++)
 //   zSum += MAT2D(nel,j,z,nComb);
 // if(zSum > 1.e0)
 // {
      if(fLump)
        MAT2D(nel,SL_PROD,z,nComb) = 1.e0 
        - (MAT2D(nel,SL_AIR,z,nComb) + MAT2D(nel,SL_PROD,z,nComb));
      else
        MAT2D(nel,SP_N2,z,nComb) = 1.e0 
        - (MAT2D(nel,SP_FUEL,z,nComb) + MAT2D(nel,SP_O2 ,z,nComb)
        +  MAT2D(nel,SP_CO2 ,z,nComb) + MAT2D(nel,SP_H2O,z,nComb));
//  } 
  }

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 15/08/2018                                   *
 * Data de modificaco : 03/05/2019                                   *
 *-------------------------------------------------------------------*
 * getSpeciesPrimitives : obtem as especies primitivas das especies  *
 * agrupadas                                                         *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * cModel  -> modelo de combustao                                    *
 * y       -> nao definido                                           *
 * zComb   -> fracao massica                                         *
 * numel   -> numero de elementos                                    *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * y -> fracao de massa de especies primitivas                       * 
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * z(nel,1) -> air (oxidante)                                        *
 * z(nel,2) -> fuel(comburente)                                      *
 * z(nel,3) -> produto                                               *
 *                                                                   * 
 * y[0] - CH4                                                        *
 * y[1] - O2                                                         *
 * y[2] - N2                                                         *
 * y[3] - CO2                                                        *
 * y[4] - H2O                                                        *
 * y[5] - CO                                                         *
 * y[6] - C                                                          *
 *********************************************************************/
void getSpeciesPrimitives(Combustion *cModel 
                        , DOUBLE *RESTRICT y,DOUBLE *RESTRICT z
                        , INT const numel   )
{
  short i,ns,nl;
  INT nel;
  DOUBLE *py,*pz,*a;

  if(cModel->fLump)
  {
    ns = cModel->nOfSpecies; 
    nl = cModel->nOfSpeciesLump;
    a  = cModel->lumpedMatrix;

    for(nel = 0 ; nel < numel; nel++)
    {
      py = &MAT2D(nel,0,y,ns);
      pz = &MAT2D(nel,0,z,nl);
      yLumpedMatrixZ(py,a,pz,ns,nl);
    }
  }
  else
  {
    ns = cModel->nOfSpecies; 
    for(nel = 0 ; nel < numel; nel++)
      for(i=0;i<ns;i++)
        MAT2D(nel,i,y,ns) = MAT2D(nel,i,z,ns);
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 04/05/2019                                   *
 *-------------------------------------------------------------------*
 * rateFuelConsume: calculo da taxa de consumo do combustivel        *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * cModel  -> modelo de combustao                                    *
 * zComb   -> fracao massica                                         *
 * temp    -> temperatura                                            *
 * rate    -> nao definido                                           * 
 * density -> densidade                                              *
 * numel   -> numero de elementos                                    *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * rate    -> taxa de consumo do combustivel                         * 
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * z(nel,1) -> air (oxidante)                                        *
 * z(nel,2) -> fuel(comburente)                                      *
 * z(nel,3) -> produto                                               *
 *********************************************************************/
void rateFuelConsume(Combustion *cModel       , DOUBLE *RESTRICT zComb
                   , DOUBLE *RESTRICT temp    , DOUBLE *RESTRICT rate
                   , DOUBLE *RESTRICT density , bool const fKelvin
                   , INT const numel)
{
  short nComb = cModel->nComb,iCod = cModel->reactionKinetic;
  INT nel;
  DOUBLE s,tMix;
  DOUBLE zFuel, zAir, zO2, omega, densityC, alpha, coefA;
  DOUBLE tmp1, tmp2, tmp3, tmp4;
  DOUBLE mWfuel,mWox, tempA, eOx, eFuel, ru, tc; 

/*...*/
  switch(iCod)
  {
/*...*/
    case ARRHENIUS:
      s      = cModel->sMolar;
      alpha  = cModel->arrhenius.alpha;
      mWfuel = cModel->mW_Fuel;
      mWox   = cModel->mW_Air;
      coefA  = cModel->arrhenius.a;
/*... KJ/(kmol*kelvin)*/
      ru     = IDEALGASR*1.e-03;
      tempA  = cModel->arrhenius.energyAtivation/ru; 
      coefA *= mWfuel/(mWfuel+pow(mWox,s));
      eOx     = s;
      eFuel   = 1.0;

      for(nel = 0; nel < numel; nel++)
      {

        if(fKelvin)
          tc = temp[nel];  
        else
          tc = CELSIUS_FOR_KELVIN(temp[nel]);

        densityC = MAT2D(nel, TIME_N, density, DENSITY_LEVEL);

        zAir  = MAT2D(nel,0,zComb,nComb);
        zFuel = MAT2D(nel,1,zComb,nComb);
   
        tmp1 = pow(temp[nel],alpha); 
        tmp2 = pow(densityC,s+1.0);
        tmp3 = pow(zFuel,eFuel)*pow(zAir,eOx);
        tmp4 = exp(-tempA/tc);
//      printf("%lf %lf\n",tmp3,tmp4);

        rate[nel] = coefA*tmp1*tmp2*tmp3*tmp4;
      }
    break;
/*...................................................................*/

/*...*/
    case EBU:
      if(cModel->fLump)
      {
        s    = cModel->sMassAir;
        tMix = cModel->tMix;
        for(nel = 0; nel < numel; nel++)
        {

          densityC = MAT2D(nel, TIME_N, density, DENSITY_LEVEL);

          zAir  = MAT2D(nel,SL_AIR ,zComb,nComb);
          zFuel = MAT2D(nel,SL_FUEL,zComb,nComb);
    
          omega = max(densityC*min(zFuel,zAir/s),0.e0);
          rate[nel] = omega/tMix;
        }
      }
      else
      {
        s    = cModel->sMassO2;
        tMix = cModel->tMix;
        for(nel = 0; nel < numel; nel++)
        {

          densityC = MAT2D(nel, TIME_N, density, DENSITY_LEVEL);

          zO2   = MAT2D(nel,SP_O2,zComb,nComb);
          zFuel = MAT2D(nel,SP_FUEL,zComb,nComb);
    
          omega = max(densityC*min(zFuel,zO2/s),0.e0);
          rate[nel] = omega/tMix;
        }
      }
      break;
/*...................................................................*/
  }
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * rateHeatRealeseCombustion: calculo da taxa de liberacao de calor  *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * cModel  -> modelo de combustao                                    *
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * q       -> nao definido                                           *
 * zComb0  -> fracao de massa agrupada do passo de tempo anterior    *
 * zComb   ->fracao de massa agrupada do passo de tempo atural       *
 * rateFuel-> taxa de consumo do combustivel                         *
 * prop   - propriedades por material                                *
 * mat    - material da celula                                       * 
 * dt      -> delta dessa passo de tempo                             * 
 * numel   -> numero de elementos                                    *
 * fSheat   - calor especifico com variacao com a Temperatura        *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * q -> taxa de energia liberada KJ/s                                * 
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void rateHeatRealeseCombustion(Combustion *cModel,PropPol *sHeat   
                   , DOUBLE *RESTRICT q      , DOUBLE *RESTRICT temp
                   , DOUBLE *RESTRICT zComb0 , DOUBLE *RESTRICT zComb
                   , DOUBLE *RESTRICT density, DOUBLE *RESTRICT rateFuel 
                   , DOUBLE *RESTRICT prop   , short  *RESTRICT mat
                   , DOUBLE const dt         , INT const numel
                   , bool const fsHeat       , bool const fKelvin)
{

  short lMat, iCod = cModel->typeHeatRealese;
  INT nel;
  DOUBLE *h,hc,H[3],nCO2p,nH2Op;
  DOUBLE z0,z1,sHeatRef;
  DOUBLE densityC,densityC0,tmp,dRoZ;

  if(iCod == HFORMATION)
  {
    h     = cModel->entalphyOfForm;
    nCO2p = cModel->stoichCO2p;
    nH2Op = cModel->stoichH2Op;
    for(nel = 0; nel < numel; nel++)
    {
      lMat  = mat[nel] - 1;
      sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);

      H[0] = h[SP_CO2 ] + tempForSpecificEnthalpySpecies(sHeat    , SP_CO2 
                                                       , temp[nel], sHeatRef
                                                       , fsHeat   , fKelvin);
      H[1] = h[SP_H2O ] + tempForSpecificEnthalpySpecies(sHeat    , SP_H2O 
                                                       , temp[nel], sHeatRef    
                                                       , fsHeat   , fKelvin);
      H[2] = h[SP_FUEL] + tempForSpecificEnthalpySpecies(sHeat    , SP_FUEL 
                                                       , temp[nel], sHeatRef    
                                                       , fsHeat   , fKelvin);
     
      tmp =  (nCO2p*H[0] + nH2Op*H[1] - H[2])/cModel->mW_Fuel;

      tmp *=rateFuel[nel];

      q[nel] = -tmp;
    }
  }
  else if (iCod == HCOMBUSTION)
  {
    hc = cModel->entalphyOfCombustion;
    for(nel = 0; nel < numel; nel++)
    {
      tmp = rateFuel[nel]*hc;
      q[nel] = -tmp;
    }
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * rateFuelConsume: calculo da taxa de consumo do combustivel        *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * q       -> nao definido                                           *
 * rate    -> taxa de consumo de combustivel                         * 
 * numel   -> numero de elementos                                    *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * q -> energia total liberada KJ                                    * 
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * entalphyOfFormLumped[0] - Air KJ/Kg                               *
 * entalphyOfFormLumped[1] - Fuel KJ/Kg                              *
 * entalphyOfFormLumped[2] - Froduto KJ/Kg                           *
 *********************************************************************/
DOUBLE totalHeatRealeseComb(DOUBLE *RESTRICT q, DOUBLE *RESTRICT vol  
                          , DOUBLE const dt   , INT const numel)
{

  INT nel;
  DOUBLE qTotal =0.e0, vTotal = 0.e0;

  for(nel = 0; nel < numel; nel++)
    qTotal += q[nel]*vol[nel];

  return qTotal*dt;
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 04/05/2019                                   *
 *-------------------------------------------------------------------*
 * initLumpedMatrix : inicializa a matriz de especies                *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * cModel  -> modelo de combustao                                    *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initLumpedMatrix(Combustion *cModel)
{

  short nComb = cModel->nComb;

/*... Fuel*/
  MAT2D(SP_FUEL,0,cModel->lumpedMatrix,nComb) = 1.e0; 
  MAT2D(SP_FUEL,1,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(SP_FUEL,2,cModel->lumpedMatrix,nComb) = 0.e0; 
/*...................................................................*/

/*... O2*/
  MAT2D(SP_O2,0,cModel->lumpedMatrix,nComb) = 0.e0;
  MAT2D(SP_O2,1,cModel->lumpedMatrix,nComb) = 0.2330e0; 
  MAT2D(SP_O2,2,cModel->lumpedMatrix,nComb) = 0.0e0; 
/*...................................................................*/

/*... N2*/
  MAT2D(SP_N2,0,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(SP_N2,1,cModel->lumpedMatrix,nComb) = 0.7670e0; 
  MAT2D(SP_N2,2,cModel->lumpedMatrix,nComb) = 0.7248e0; 
/*...................................................................*/

/*... CO2*/
  MAT2D(SP_CO2,0,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(SP_CO2,1,cModel->lumpedMatrix,nComb) = 0.0e0; 
  MAT2D(SP_CO2,2,cModel->lumpedMatrix,nComb) = 0.1514e0; 
/*...................................................................*/

/*... H2O*/
  MAT2D(SP_H2O,0,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(SP_H2O,1,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(SP_H2O,2,cModel->lumpedMatrix,nComb) = 0.1238e0; 
/*...................................................................*/

/*... CO*/
  MAT2D(5,0,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(5,1,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(5,2,cModel->lumpedMatrix,nComb) = 0.e0; 
/*...................................................................*/

/*... C*/
  MAT2D(6,0,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(6,1,cModel->lumpedMatrix,nComb) = 0.e0; 
  MAT2D(6,2,cModel->lumpedMatrix,nComb) = 0.e0; 
/*...................................................................*/

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * initLumpedMatrix : inicializa a matriz de especies                *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * y       -> nao definido                                           *
 * a       -> matriz das especies agrupadas                          *
 * z       -> especies agrupadas                                     *
 * ns      -> numero de especies primitivas                          *
 * nl      -> numero de especies agrupadas                           *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * y       -> especies primitivas                                    *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * y[0] - CH4                                                        *
 * y[1] - O2                                                         *
 * y[2] - N2                                                         *
 * y[3] - CO2                                                        *
 * y[4] - H2O                                                        *
 * y[5] - CO                                                         *
 * y[6] - C                                                          *
 *********************************************************************/
void yLumpedMatrixZ(DOUBLE *RESTRICT y, DOUBLE *RESTRICT a
                  , DOUBLE *RESTRICT z
                  , short const ns    , short const nl)
{
  short i,j;

/*...*/
  for(i=0;i<ns;i++){
    y[i] = 0.e0;
    for(j=0;j<nl;j++)
      y[i] += MAT2D(i,j,a,nl)*z[j];  
  }
/*...................................................................*/

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * initMolarMass: inicializa a massa molar das especies              *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initMolarMass(Combustion *cModel)
{
  
  short nO,nC,nH;
  DOUBLE pO2,pN2;

  nC = cModel->fuel.c;
  nO = cModel->fuel.o;
  nH = cModel->fuel.h;

  pO2 = cModel->O2InAir;
  pN2 = cModel->N2InAir;

/*... Fuel */
  cModel->mW_Fuel = nC*MW_C + nH*MW_H + nO*MW_O;

/*...O2*/
  cModel->mW_O2 = 2.0e0*MW_O;

/*...N2*/
  cModel->mW_N2 = 2.0e0*MW_N;

/*...H2O*/
  cModel->mW_H2O = 2.0e0*MW_H + MW_O;

/*...CO2*/
  cModel->mW_CO2 =MW_C +  2.0e0*MW_O;

/*...CO*/
  cModel->mW_CO = MW_C + MW_O;

/*...C*/
  cModel->mW_C = MW_C;

/*... Air*/
  cModel->mW_Air = pO2*cModel->mW_O2 + pN2*cModel->mW_N2;

}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 23/08/2018                                   *
 * Data de modificaco : 03/05/2019                                   *
 *-------------------------------------------------------------------*
 * stoichiometricCoeff:                                              *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *                                                                   *
 * 1*(CmHnOl) + nO2*(O2) + (nO2*pN2/pO2)*N2                          * 
 * -> nCO2*(CO2) + nH2O*(H2O) + nN2*(N2)                             *                              *
 *                                                                   *
 * C - m = nCO2   :.                                                 *
 * H - n = 2*nH2O :. nH2O = n/2                                      *
 * O - 2*nO2 +l = 2*nCO2 + nH2O :. nO2 = nCO2 + nH2O/2               *
 * :. nO2 = m + n/4 - l/2                                            *
 * N - 2*nO2*pN2/pO2 = 2*nN2 :. nO2*pN2/pO2 = nN2 :.                 *
 * :.  nN2 = (m + n/4)*pN2/pO2                                       *
 *                                                                   *
 * nAir = nO2/pCO2                                                   *
 *********************************************************************/
void stoichiometricCoeff(Combustion *cModel)
{
  short m,n,l;
  DOUBLE nAir,nProd,pO2,pN2,pN2r,nN2p,nCO2p,nH2Op,nTotal;
  DOUBLE pCO2r,pH2Or,nO2,nN2;

  m = cModel->fuel.c;
  n = cModel->fuel.h;
  l = cModel->fuel.o;

  pO2 = cModel->O2InAir;
  pN2 = cModel->N2InAir;

/* FUEL + nAir*AIR -> nPrpd*PROD*/

/*... Ar ( pO2*O2 + pN2*N2)*/
//nAir = 2.e0*(1.0e0/(pO2));

/*... porduto  */
//nCO2r  = 1.0;
//nH2Or  = 2.0;
//nN2r   = nAir*pN2;
//nTotal = nCO2r + nH2Or + nN2r;
//pCO2 = nCO2r/nTotal;
//pH2O = nH2Or/nTotal;
//pN2  = nN2r/nTotal;
//nProd = nN2r/pN2;

//cModel->CO2InPord = pCO2;
//cModel->H2OInPord = pH2O;

/*... Ar ( pO2*O2 + pN2*N2)*/
  nO2  = m + n/4.0e0 - l/2.0;
  nN2  = nO2*pN2/pO2;
  nAir = nO2*(1.0e0/pO2);

/*... produto  */
  nN2p  = (m + n/4.e0)*pN2/pO2;
  nCO2p = m;
  nH2Op = n/2.e0;  

  nTotal = nCO2p + nH2Op + nN2p;
  pCO2r = nCO2p/nTotal;
  pH2Or = nH2Op/nTotal;
  pN2r = nN2p/nTotal;
  nProd = (nN2p/pN2r+nCO2p/pCO2r+nH2Op/pH2Or)/3.0;

  cModel->stoichO2   = nO2;
  cModel->stoichN2   = nN2;
  cModel->stoichH2Op = nH2Op;
  cModel->stoichCO2p = nCO2p;
  cModel->stoichN2p  = nN2p;

  cModel->CO2InProd = pCO2r;
  cModel->H2OInProd = pH2Or;
  cModel->N2InProd  = pN2r;

/*...*/
  cModel->sMolar    = nAir;
  cModel->sMassAir  = nAir*cModel->mW_Air/cModel->mW_Fuel;
  cModel->sMassO2   = nO2*cModel->mW_O2/cModel->mW_Fuel;
  cModel->sMassN2   = nN2*cModel->mW_N2/cModel->mW_Fuel;
  cModel->sMassCO2p = nCO2p*cModel->mW_CO2/cModel->mW_Fuel;
  cModel->sMassH2Op = nH2Op*cModel->mW_H2O/cModel->mW_Fuel;
  cModel->sMassN2p  = nN2p*cModel->mW_N2/cModel->mW_Fuel;
/*..................................................................*/

  fprintf(fileLogExc,"\nReaction:\n\n"); 
/*...*/
  if(cModel->fLump)
  {
    fprintf(fileLogExc,"C%dH%dO%d" 
           " + %lf (%lf O2 + %lf N2)\n"
           " -> %lf (%lf CO2 + %lf H2O + %lf N2)\n\n"
          ,m    ,n     ,l
          ,nAir ,pO2   ,pN2
          ,nProd,pCO2r, pH2Or,pN2r);
    
    fprintf(fileLogExc,"1 kg C%dH%dO%d" 
           " + %lf (%lf O2 + %lf N2)\n"
           " -> %lf (%lf CO2 + %lf H2O + %lf N2)\n\n"
          ,m                ,n     ,l
          ,cModel->sMassAir    ,pO2   ,pN2
          ,cModel->sMassAir + 1,pCO2r,pH2Or,pN2r);

  } 
/*..................................................................*/

/*...*/
  else
  {
    fprintf(fileLogExc,"C%dH%dO%d" 
           " + %lf O2 + %lf N2\n"
           " -> %lf CO2 + %lf H2O + %lf N2\n\n"
          ,m    ,n    ,l
          ,nO2  ,nO2*pN2/pO2
          ,nCO2p,nH2Op,nN2p);

    fprintf(fileLogExc,"1 kg C%dH%dO%d" 
           " + %lf  kg O2 + %lf  kg N2\n"
           " -> %lf kg CO2 + %lf kg H2O + %lf kg N2\n\n"
          ,m    ,n    ,l
          ,cModel->sMassO2  ,cModel->sMassN2
          ,cModel->sMassCO2p,cModel->sMassH2Op,cModel->sMassN2p);
  }
/*..................................................................*/
}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 18/08/2018                                   *
 * Data de modificaco : 05/05/2019                                   *
 *-------------------------------------------------------------------*
 * initMolarMass: inicializa a massa molar das especies              *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initEntalpyOfFormation(Combustion *cModel)
{
  DOUBLE s = cModel->sMassAir,tmp;
  DOUBLE hFuelFormation,hCO2Formation;
  DOUBLE pCO2p,pH2Op,pN2p,nN2p,nCO2p,nH2Op,nProd;

  pCO2p = cModel->CO2InProd;
  pH2Op = cModel->H2OInProd;
  pN2p  = cModel->N2InProd;
  nN2p  = cModel->stoichN2p;
  nCO2p = cModel->stoichCO2p;
  nH2Op = cModel->stoichH2Op;

/*...  Fuel - KJ/kMol*/
  cModel->entalphyOfForm[SP_FUEL] = -74870.e0; 
/*... CO2 - KJ/kMol*/
  cModel-> entalphyOfForm[SP_CO2] = -393510.e0;
/*... H2O - KJ/kMol*/
  cModel-> entalphyOfForm[SP_H2O] = -241826.e0;
/*... CO - KJ/kMol*/
//cModel-> entalphyOfForm = -110500.e0;        

}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 18/08/2018                                   *
 * Data de modificaco : 05/05/2019                                   *
 *-------------------------------------------------------------------*
 * initMolarMass: inicializa a massa molar das especies              *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initEntalpyOfCombustion(Combustion *cModel)
{
  
  DOUBLE hFuelFormation,hCO2Formation;
  DOUBLE pCO2,pH2O,hFuel,hCO2,hH2O;

  pCO2 = cModel->CO2InProd;
  pH2O = cModel->H2OInProd;

/*...  Fuel - KJ/kMol*/
  hFuel = -74870.e0; 
/*... CO2 - KJ/kMol*/
  hCO2 = -393520.e0;
/*... H2O - KJ/kMol*/
  hH2O = -241830.e0;

/*... KJ/Kmol de fuel*/
  cModel->entalphyOfCombustion = (hCO2 + 2.0*hH2O) - hFuel;

  fprintf(fileLogExc,"Primitives species:\n");

  fprintf(fileLogExc,"Entalphy of combustion (KJ/Kmol) = %lf\n"  
                    ,cModel->entalphyOfCombustion);


/*... KJ/KG de fuel */
  cModel->entalphyOfCombustion  /= cModel->mW_Fuel;

  fprintf(fileLogExc,"Entalphy of combustion (KJ/KG)   = %lf\n\n"  
                    ,cModel->entalphyOfCombustion);
}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 02/05/2019                                   *
 *-------------------------------------------------------------------*
 * concetracionOfSpecies:                                            *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * cModel  -> nao definido                                           *
 * z       -> especies agrupadas   (cModel->fLump = true)            *
 *            especies primitivas  (cModel->fLump = false)           *
 * c       -> concetracao das especies primitivas                    *
 * density -> densidade da mistura                                   *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * c       -> concentracao das especies                              *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * c[0] - CH4                                                        *
 * c[1] - O2                                                         *
 * c[2] - N2                                                         *
 * c[3] - CO2                                                        *
*  c[4] - H2O                                                        *
 * c[5] - CO                                                         * 
 * c[6] - C                                                          *
 *********************************************************************/
void concetracionOfSpecies(Combustion *cModel,DOUBLE *RESTRICT z
                          ,DOUBLE *RESTRICT c,DOUBLE const density)
{
  bool fLump = cModel->fLump;
  short i, ns = cModel->nOfSpecies, nl = cModel->nOfSpeciesLump;
  DOUBLE y[MAXSPECIES],mW[MAXSPECIES];

  vectorMolarMass(mW,cModel);

  if (fLump) yLumpedMatrixZ(y,cModel->lumpedMatrix, z, ns, nl);
  
  for(i=0;i<ns;i++)
    c[i] = y[i]*density/mW[i];

}
/*********************************************************************/

/**********************************************************************
 * Data de criacao    : 15/08/2018                                    *
 * Data de modificaco : 10/05/2019                                    *
 *------------------------------------------------------------------- * 
 * sumFracZ : Soma todas as fracoes de massa                          *  
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * z     -> nao definido                                              * 
 * zComb -> fracoes de massa                                          * 
 * n     -> numero de linhas na matriz                                * 
 * nComb -> numero de componentes na mistura                          * 
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * ------------------------------------------------------------------ *
 * z -> soma ( deve ser igual 1)                                      * 
 * ------------------------------------------------------------------ *
 * OBS:                                                               *
 *------------------------------------------------------------------- *
 **********************************************************************/
void sumFracZ(DOUBLE *z       ,DOUBLE *zComb 
         ,INT const n     ,short const nComb)
{
  short j;
  INT i;

  for(i=0;i<n;i++)
  {
    z[i] = 0.e0;
    for(j=0;j<nComb;j++)
      z[i] += MAT2D(i,j,zComb,nComb);
  }
}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 12/08/2018                                   *
 * Data de modificaco : 25/08/2018                                   *
 *-------------------------------------------------------------------*
 * mixtureMolarMass: massa molar da mistura                          *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * cModel  -> nao definido                                           *
 * y       -> especies primitivas                                    *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE mixtureMolarMass(Combustion *cModel,DOUBLE *RESTRICT y) 
{                    

  short i, ns = cModel->nOfSpecies;
  DOUBLE mW[MAXSPECIES],tmp;

  vectorMolarMass(mW,cModel);

  for(i=0,tmp=0.e0;i<ns;i++)
    tmp += y[i]/mW[i];

  return 1.e0/tmp;

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 00/00/0000                                   *
 * Data de modificaco : 05/05/2019                                   *
 *-------------------------------------------------------------------*
 * vectorMolarMass : massa molar da mistura                          *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * mW -> nao definido                                                *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * mW -> massa molar                                                 *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
static void vectorMolarMass(DOUBLE *mW, Combustion *cModel)
{

  mW[SP_FUEL] = cModel->mW_Fuel;
  mW[SP_O2  ] = cModel->mW_O2;
  mW[SP_N2  ] = cModel->mW_N2;
  mW[SP_CO2 ] = cModel->mW_CO2;
  mW[SP_H2O ] = cModel->mW_H2O;
/*mW[5] = cModel->mW_CO;
  mW[6] = cModel->mW_C;*/

}
/*********************************************************************/ 

/*********************************************************************
 * Data de criacao    : 03/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * maxArray : retorna o valor maximo de vetor                        *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * x -> valores                                                      *
 * y -> dimensao de vetor                                            *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE maxArray(DOUBLE *RESTRICT x,INT const n)
{
  
  INT j;
  DOUBLE tmp;

  tmp = max(x[0], x[1]);
  for(j=2;j<n;j++)
    tmp = max(tmp, x[j]); 

  return tmp;

}
/*********************************************************************/ 

void printt(double *x, short n)
{
  int i;
  for(i=0;i<n;i++)
    printf("%d %e\n",i,x[i]);
}