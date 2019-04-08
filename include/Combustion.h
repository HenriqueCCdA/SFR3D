#ifndef _COMBUSTION_H_
  #define _COMBUSTION_H_
/*...*/
  #include<stdio.h>
  #include<stdlib.h>
  #include<math.h>
/*...................................................................*/

/*...*/
  #include<Memoria.h>
  #include<Mesh.h>
  #include<HccaStdBool.h>
  #include<CellLoop.h>
  #include<Define.h>
//#include<Properties.h>
  #include<Sisteq.h> 
  #include<Solv.h>
/*...................................................................*/

/*...*/
void combustionSolver(Memoria *m          , PropVarFluid *propF
                    , Loads *loadsVel     , Loads *loadsPres
                    , Loads *loadsEnergy  , Loads *loadsKturb
                    , Loads *loadsComb
                    , EnergyModel *eModel , Combustion *cModel
                    , MassEqModel *eMass  , MomentumModel *ModelMomentum
                    , Turbulence *tModel  , ThermoDynamic *thDynamic
                    , Mesh *mesh0         , Mesh *mesh
                    , SistEq *sistEqVel   , SistEq *sistEqPres
                    , SistEq *sistEqEnergy, SistEq *sistEqKturb
                    , SistEq *sistEqComb
                    , Solv *solvVel       , Solv *solvPres
                    , Solv *solvEnergy    , Solv *solvKturb
                    , Solv *solvComb
                    , Simple *sp          , Scheme *sc
                    , PartMesh *pMesh     , Mean *media
                    , FileOpt *opt        , char *preName
                    , char *nameOut       , FILE *fileOut);
/*...................................................................*/

/*...*/
  void combustionModel(Memoria *m       , PropVarFluid *prop
                   , Loads *loadsComb   , Loads *loadsVel
                   , Turbulence *tModel , Combustion *cModel
                   , EnergyModel *eModel, Mesh *mesh
                   , SistEq *sistEqComb , Solv *solvComb  
                   , Simple *sp         , Scheme *sc        
                   , PartMesh *pMesh    , FileOpt *opt
                   , short itSimple); 
/*...................................................................*/

/*...*/
  void rateFuelConsume(Combustion *cModel    , DOUBLE *RESTRICT zComb
                   , DOUBLE *RESTRICT temp   , DOUBLE *RESTRICT rate
                   , DOUBLE *RESTRICT density, bool const iKelvin
                   , INT const numel);
/*...................................................................*/

/*...*/
  void rateHeatRealeseCombustion(Combustion *cModel   
                   , DOUBLE *RESTRICT q
                   , DOUBLE *RESTRICT zComb0 , DOUBLE *RESTRICT zComb
                   , DOUBLE *RESTRICT density, DOUBLE *RESTRICT rateFuel 
                   , DOUBLE const dt         , INT const numel);
/*...................................................................*/

/*...*/
  void getSpeciesPrimitives(Combustion *cModel 
                        , DOUBLE *RESTRICT y,DOUBLE *RESTRICT z
                        , INT const numel);
/*...................................................................*/

/*...*/
  void regularZ(DOUBLE *RESTRICT z, INT const numel
              , short const nLump);
  void initLumpedMatrix(Combustion *cModel);
  void yLumpedMatrixZ(DOUBLE *RESTRICT y, DOUBLE *RESTRICT a
                  , DOUBLE *RESTRICT z
                  , short const ni    , short const nj);
  void initMolarMass(Combustion *cModel);
  void initEntalpyOfFormation(Combustion *cModel);
  void stoichiometricCoeff(Combustion *cModel);
  void initEntalpyOfCombustion(Combustion *cModel);

  void sumFracZ(DOUBLE *z      ,DOUBLE *zComb 
              ,INT const n     ,short const nComb);

  DOUBLE totalHeatRealeseComb(DOUBLE *RESTRICT q, DOUBLE *RESTRICT vol  
                          , DOUBLE const dt     , INT const numel);

/*...*/
  DOUBLE mixtureMolarMass(Combustion *cModel,DOUBLE *RESTRICT z);
/*...................................................................*/

/*...................................................................*/
#endif/*_COMBUSTION_H_*/