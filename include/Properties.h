#ifndef _PROPERTIES_H_
  #define  _PROPERTIES_H_
/*...*/
  #include<math.h>
  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
/*...................................................................*/

/*...*/
  #include<Combustion.h>
  #include<Define.h>
  #include<HccaStdBool.h>
  #include<Erro.h>
  #include<File.h>
  #include<Mesh.h>
/*...................................................................*/

/*...agua*/
  DOUBLE waterDensity(DOUBLE const t);
  DOUBLE waterSpecifiHeat(DOUBLE const t);
  DOUBLE waterDynamicViscosity(DOUBLE const t);
/*...................................................................*/

/*... gas ideal incompressivel(Ar)*/
  DOUBLE airDensity(Prop *den
                   ,DOUBLE const t, DOUBLE const presRef
                   ,DOUBLE const p, bool const fKelvin);
  DOUBLE airSpecifiHeat(Prop *sHeatPol
                       ,DOUBLE const t,bool const fKelvin);
  DOUBLE airDynamicViscosity(Prop *dVisc,DOUBLE const t
                            ,bool const fKelvin);
  DOUBLE airThermalConductvity(Prop *thC,DOUBLE const t
                              ,bool const fKelvin);
  DOUBLE specificEnthalpyForTemp(Prop *sHeatPol
                               , DOUBLE const hs, DOUBLE const sHeatRef
                               , bool const fSheat, bool const fKelvin); 
/*...*/
  DOUBLE tempForSpecificEnthalpy(Prop *sHeatPol
                               , DOUBLE const t, DOUBLE const sHeatRef
                               , bool const fSheat, bool const fKelvin);
/*...................................................................*/

/*...*/
  void updateDensity(Prop *pDen
                    ,DOUBLE *RESTRICT temp   , DOUBLE *RESTRICT pressure
                    ,DOUBLE *RESTRICT density                 
                    ,DOUBLE const alpha        ,bool const iKelvin 
                    ,INT const nEl             ,char  const iCod);

  void updateSpecificHeat(Prop *sHeatPol
                         ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT sHeat
                         ,bool const iKelvin  
                         ,INT const nEl        ,char  const iCod);
  void updateDynamicViscosity(Prop *dVisc
                            ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT visc    
                            ,bool const iKelvin   
                            ,INT const nEl);
  void updateThermalconductivity(Prop *thC
                              ,DOUBLE *RESTRICT t,DOUBLE *RESTRICT thCond    
                              ,bool const iKelvin       
                              ,INT const nEl);
  
  void updateDensityCD(Prop *pol, DOUBLE *RESTRICT u
                     , DOUBLE *RESTRICT density, INT nEl
                     , char  iCod);

  void updateProp(Prop *pol, DOUBLE *RESTRICT u
                , DOUBLE *RESTRICT coef, INT nEl);

  void initPropRef(PropVarFluid *propF ,DOUBLE *RESTRICT propMat
                ,short const lMat);
  void initPropTemp(PropVarFluid *propFluid
                  ,DOUBLE *RESTRICT prop    ,DOUBLE *RESTRICT t 
                  ,DOUBLE *RESTRICT pressure,DOUBLE *RESTRICT propMat
                  ,short *RESTRICT mat
                  ,short const np           ,INT const nCell 
                  ,bool const iKelvin       ,short const iProp);

  void initPropCD(Prop *pol, DOUBLE *RESTRICT prop
                 , DOUBLE *RESTRICT u, DOUBLE *RESTRICT propMat
                 , short *RESTRICT mat
                 , short np, INT    nCell
                 , short iProp);

  void initPropStructCD(PropVarCD *propVar, short const n);
/*...................................................................*/

/*...*/
  void getTempForEnergy(Prop *sHeatPol
                     ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                     ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                     ,INT const nCell      ,bool const fTemp
                     ,bool const fSheat    ,bool const fKelvin
                     ,bool const fOmp      ,short const nThreads );

  void getEnergyForTemp(Prop *sHeatPol
                     ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                     ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                     ,INT const nCell     
                     ,bool const fSheat    ,bool const fKelvin
                     ,bool const fOmp      ,short const nThreads );
/*...................................................................*/

/*...*/
  DOUBLE diffProp(Prop *pol, DOUBLE u);
/*...................................................................*/

/*...*/
  void specificMassRefOld(DOUBLE *RESTRICT density, DOUBLE *RESTRICT volume                  
                  , DOUBLE *RESTRICT prop      , short  *RESTRICT mat
                  , INT const nCell);
  DOUBLE specificMassRef(DOUBLE *RESTRICT density, DOUBLE *RESTRICT volume                  
                       , INT const nCell);
  void presRef(DOUBLE *RESTRICT temp0 , DOUBLE *RESTRICT temp  
             , DOUBLE *RESTRICT volume  , DOUBLE *pTh                  
             , INT const nCell          , bool const fKelvin);
  void presRefMix(Combustion *cModel
              , DOUBLE *RESTRICT temp0 , DOUBLE *RESTRICT temp  
              , DOUBLE *RESTRICT yFrac0, DOUBLE *RESTRICT yFrac
              , DOUBLE *RESTRICT volume, DOUBLE *pTh                               
              , INT const nCell        , bool const fKelvin);
  void initPresRef(DOUBLE *RESTRICT temp, DOUBLE *RESTRICT volume
               , DOUBLE *pTh            , DOUBLE const densityRef 
               , DOUBLE const molarMass                  
               , INT const nCell        , bool const fKelvin);
/*...................................................................*/

/*...*/
  void initDensityPol(Prop *prop, char *s, FILE *file);
  void initSheatPol(Prop *prop, char *s, FILE *file);
  void initDviscosityPol(Prop *prop, char *s, FILE *file);
  void initThCondPol(Prop *prop, char *s, FILE *file);
  void initDiffSp(Prop *prop, char *s, FILE *file);
/*...................................................................*/

/*... nasa pol*/
  DOUBLE polNasaCp(PolNasa *a, DOUBLE x);
  void nasaPolRange(PolNasa *a      , DOUBLE const x
                 ,DOUBLE **c      , DOUBLE *xNew
                 ,short const iCod);
  DOUBLE polNasaH(PolNasa *a     , DOUBLE const x, bool const fKelvin);
  DOUBLE polNasaCp2Derivada(PolNasa *a     , DOUBLE const x);
  DOUBLE intNum(PolNasa *a,DOUBLE const x0,DOUBLE const x1
               ,short const iCod);
/*... mistura gasosa*/
  void initLeornadJones(Combustion *cModel);
  DOUBLE collisionIntegral(DOUBLE const t,DOUBLE const ek);
  DOUBLE diffusionCollisionIntegral(DOUBLE const t
                                 ,DOUBLE const ekl,DOUBLE const eki); 
/*... massa especifica da mistura*/
  DOUBLE mixtureSpeciesDensity(Prop *den        ,DOUBLE const malorMassMix
                            ,DOUBLE const t      ,DOUBLE const p
                            ,DOUBLE const presRef,bool const fKelvin);
  void updateMixDensity(Prop *pDen         , Combustion *cModel
                 , DOUBLE *RESTRICT temp    , DOUBLE *RESTRICT pressure
                 , DOUBLE *RESTRICT density , DOUBLE *RESTRICT zComb
                 , DOUBLE const alpha       , bool const iKelvin    
                 , INT const nEl            , char  const iCod);
/*... calor especifico da mistura*/
  void initMixtureSpeciesfiHeat(Prop *prop, char *s,Combustion *cModel, FILE *file);
  DOUBLE mixtureSpecifiHeat(Prop *sHeat      , DOUBLE *yFrac
                         , DOUBLE const t    , short const nOfPrSp
                         , bool const fKelvin); 
  DOUBLE specieSpecifiHeat(Prop *sHeat     , short const kSpecie
                        , DOUBLE const t      , bool const fKelvin); 
  void updateMixSpecificHeat(Prop *sHeatPol
                         , DOUBLE *RESTRICT temp  , DOUBLE *RESTRICT yFrac  
                         , DOUBLE *RESTRICT sHeat , short const nOfPrSp
                         , bool const iKelvin
                         , INT const nEl          , char  const iCod);
/*... viscosidae dinamica da mistura*/
  DOUBLE mixtureDynamicViscosity(Prop *dVisc    ,Combustion *cModel
                            ,DOUBLE *RESTRICT yFrac,DOUBLE const t 

                            ,bool const fKelvin);
  DOUBLE specieViscosity(DOUBLE const molarMass
                        ,DOUBLE const sigmaA   ,DOUBLE const ek   
                        ,DOUBLE const t     ); 
  void updateMixDynamicViscosity(Prop *dVisc    ,Combustion *cModel
                          ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT yFrac
                          ,DOUBLE *RESTRICT visc ,short const nOfPrSp   
                          ,bool const iKelvin    ,INT const nEl);
/*... condutividade termica*/
  void updateMixDynamicThermalCond(PropVarFluid *PropF ,Combustion *cModel
                          ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT yFrac
                          ,DOUBLE *RESTRICT thc  ,short const nOfPrSp   
                          ,bool const iKelvin    ,INT const nEl);
  DOUBLE mixtureThermalConductvity(PropVarFluid *PropF ,Combustion *cModel 
                                ,DOUBLE *RESTRICT yFrac,DOUBLE const t 
                                ,bool const fKelvin);
/* ... coeficiente de diffusao da especies*/
  DOUBLE mixtureDiffusion(PropVarFluid *propF ,Combustion *cModel 
                       ,DOUBLE *RESTRICT yFrac,DOUBLE const t 
                       ,short const kSpecieA  ,short const kSpecieI 
                       ,INT const nEl         ,bool const fKelvin);
  DOUBLE specieDiffusionBinary(DOUBLE const mMassA,DOUBLE const mMassB
                            ,DOUBLE const sigmaA,DOUBLE const sigmaB  
                            ,DOUBLE const ekA   ,DOUBLE const ekB
                            ,DOUBLE const t     );

  void updateMixDiffusion(PropVarFluid *propF,Combustion *cModel 
                       ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT yFrac
                       ,DOUBLE *RESTRICT diff ,short const nOfPrSp 
                       ,short const nComb       
                       ,bool const iKelvin    ,INT const nEl);

/*...*/
  DOUBLE specificEnthalpyForTempOfMix(Prop *sHeatPol  , DOUBLE const t
                             , DOUBLE const hs        , DOUBLE *yFrac
                             , DOUBLE const sHeatRef  , short const nOfPrSp
                             , bool const fSheat      , bool const fKelvin
                             , INT const nel ); 

  DOUBLE tempToSpecificEnthalpyMix(Prop *sHeat    , DOUBLE *yFrac
                                , DOUBLE const t    , DOUBLE const sHeatRef
                                , short const nOfPrSp
                                , bool const fSheat , bool const fKelvin); 
  DOUBLE tempForSpecificEnthalpySpecies(Prop *sHeat, short const kSpecie
                               , DOUBLE const t    , DOUBLE const sHeatRef
                               , bool const fSheat , bool const fKelvin);

  void getEnergyFromTheTempMix(Prop *sHeatPol  ,DOUBLE *RESTRICT yFrac 
                        ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                        ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                        ,INT const nCell      ,short const nOfPrSp
                        ,bool const fSheat    ,bool const fKelvin
                        ,bool const fOmp      ,short const nThreads );

  void  getTempFromTheEnergy(Prop *sHeatPol    ,DOUBLE *RESTRICT yFrac
                        ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                        ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                        ,INT const nCell      ,short const nOfPrSp 
                        ,bool const fTemp     ,bool const fSheat    
                        ,bool const fKelvin
                        ,bool const fOmp      ,short const nThreads );

  void initPropTempMix(PropVarFluid *propF    , Combustion *cModel
                 ,DOUBLE *RESTRICT prop     ,DOUBLE *RESTRICT t       
                 ,DOUBLE *RESTRICT pressure ,DOUBLE *RESTRICT yFrac  
                 ,DOUBLE *RESTRICT propMat  ,short *RESTRICT mat  
                 ,short const nOfPrSp       ,short const np  
                 ,INT    const nCell        ,bool const iKelvin 
                 ,short const iProp);
  
  void initDiffMix(PropVarFluid *propFluid, Combustion *cModel
                ,DOUBLE *RESTRICT diff     ,DOUBLE *RESTRICT t  
                ,DOUBLE *RESTRICT pressure ,DOUBLE *RESTRICT yFrac 
                ,DOUBLE *RESTRICT propMat  ,short *RESTRICT mat    
                ,short const nOfPrSp       ,short const nComb   
                ,INT    const nCell        ,bool const iKelvin);
/*...................................................................*/

/*...*/
  void initCdPol(Prop *prop, char *s, FILE *file);
/*...................................................................*/

/*...*/
//DOUBLE mixtureMolarMass(Combustion *cModel,DOUBLE *RESTRICT z);
/*...................................................................*/

  int readFileLineSimple(DOUBLE *x, FILE *file);

#endif /*_PROPERTIES_H_*/