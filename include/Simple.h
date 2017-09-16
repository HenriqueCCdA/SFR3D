#ifndef _SIMPLE_H_
  #define _SMIPLE_H_
/*...*/ 
  #include<stdlib.h>
  #include<stdio.h>
 
/*...*/
  #include<File.h>
  #include<Memoria.h>
  #include<Mesh.h>
  #include<CellLoop.h>
  #include<Mesh.h>
  #include<WriteVtk.h>
  #include<Sisteq.h>
  #include<Solv.h>
  #include<Transient.h>
  #include<Turbulence.h>
  #include<Properties.h>
/*...................................................................*/
  void simpleSolver3D(Memoria *m        
                   ,Loads *loadsVel   ,Loads *loadsPres 
                   ,Mesh *mesh0       ,Mesh *mesh       
                   ,SistEq *sistEqVel ,SistEq *sistEqPres
                   ,Solv *solvVel     ,Solv *solvPres 
                   ,Simple *sp
                   ,Scheme sc         ,PartMesh *pMesh 
                   ,FileOpt opt       ,char *preName  
                   ,char *nameOut     ,FILE *fileOut);
  
  void simpleSolverLm(Memoria *m          , PropVar prop
                    , Loads *loadsVel     , Loads *loadsPres 
                    , Loads *loadsEnergy  , EnergyModel eModel
                    , Turbulence turbModel, ThermoDynamic *thDynamic
                    , Mesh *mesh0         , Mesh *mesh
                    , SistEq *sistEqVel   , SistEq *sistEqPres
                    , SistEq *sistEqEnergy  
                    , Solv *solvVel       , Solv *solvPres 
                    , Solv *solvEnergy      
                    , Simple *sp            
                    , Scheme sc           , PartMesh *pMesh
                    , FileOpt opt         , char *preName
                    , char *nameOut       , FILE *fileOut);

/*...*/
  void updateCellSimpleVel(DOUBLE *RESTRICT w
                ,DOUBLE *RESTRICT u1 ,DOUBLE *RESTRICT u2
                ,INT *RESTRICT id    ,INT const nEl
                ,short const ndm);
  
  void updateCellSimpleVel3D(DOUBLE *RESTRICT w
                ,DOUBLE *RESTRICT u1 ,DOUBLE *RESTRICT u2
                ,DOUBLE *RESTRICT u3     
                ,INT *RESTRICT id    ,INT const nEl
                ,short const ndm);

  void updateCellSimpleVelR(DOUBLE  *RESTRICT w  ,DOUBLE  *RESTRICT u1
                           ,DOUBLE  *RESTRICT u2 ,DOUBLE  *RESTRICT u3
                           ,INT  *RESTRICT id    ,INT const nEl
                           ,short const ndm);


  void updateCellSimplePres(DOUBLE *RESTRICT presC,DOUBLE *RESTRICT xp
                           ,INT *RESTRICT id,INT const nEl);

  void simpleUpdate(DOUBLE *RESTRICT w     ,DOUBLE *RESTRICT pressure
                 ,DOUBLE *RESTRICT PresC ,DOUBLE *RESTRICT GradPresC
                 ,DOUBLE *RESTRICT dField         
                 ,INT const nEl          ,short const ndm
                 ,DOUBLE const alphaPres);
  
  void residualSimple(DOUBLE *RESTRICT vel
                 ,DOUBLE *RESTRICT rCellVel,DOUBLE *RESTRICT rCellMass
                 ,DOUBLE *RESTRICT adVel
                 ,DOUBLE *RESTRICT rU      ,DOUBLE * rMass
                 ,INT  *RESTRICT idVel     
                 ,INT const nEl            ,INT const nEqVel
                 ,short const ndm          ,short iCod);
  
  void residualSimpleLm(DOUBLE *RESTRICT vel ,DOUBLE *RESTRICT energy
            ,DOUBLE *RESTRICT rCellVel   ,DOUBLE *RESTRICT rCellMass
            ,DOUBLE *RESTRICT rCellEnergy
            ,DOUBLE *RESTRICT adVel       ,DOUBLE *RESTRICT adEnergy 
            ,DOUBLE *RESTRICT rU          ,DOUBLE *rMass
            ,DOUBLE *rEnergy 
            ,INT  *RESTRICT idVel         ,INT  *RESTRICT idEnergy 
            ,INT const nEl                ,INT const nEqVel
            ,short const ndm              ,short iCod);
/*...................................................................*/

/*...*/
  void setSimpleScheme(char *word,Simple *sp,FILE *fileIn);
  void setSimpleLmScheme(char *word, Simple *sp, FILE *fileIn);
/*...................................................................*/

#endif/*_SIMPLE_H_*/
