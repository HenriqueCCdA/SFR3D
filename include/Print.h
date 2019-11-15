#ifndef _PRINT_H_
  #define _PRINT_H_
  
/*...*/
  #include<stdio.h>
  #include<stdlib.h>
/*...*/
  #include<Define.h>
  #include<HccaStdBool.h>
  #include<CellLoop.h>
  #include<Mesh.h>
  #include<Memoria.h>
  #include<File.h>
  #include<WriteVtk.h>
  #include<ParallelMpi.h>
  #include<Media.h>
  #include<TimeInterpol.h>

  void initPrintVtk(FileOpt *opt);
  
  void printCall(Memoria *m            ,PropVarFluid *propF
              ,Turbulence *tModel   ,EnergyModel *eModel 
              ,Combustion *cModel   ,TimeInterpol *ti
              ,PartMesh *pMesh      ,Scheme *sc
              ,Loads *loadsVel      ,Loads *loadsPres 
              ,Loads *loadsTemp     ,Loads *loadsComb
              ,FileOpt *opt
              ,Mesh *mesh0          ,Mesh *mesh  
              ,Mean *media      
              ,char *preName        ,char *nameOut);

  void print3D(Memoria *m          ,PropVarFluid *propF
           ,Turbulence *turbModel,EnergyModel *eModel 
           ,Combustion *cModel
           ,PartMesh *pMesh      ,Scheme *sc
           ,Loads *loadsVel      ,Loads *loadsPres 
           ,Loads *loadsTemp     ,Loads *loadsComb
           ,TimeInterpol *ti     ,FileOpt *opt
           ,Mesh *mesh0          ,Mesh *mesh  
           ,Mean *media          ,DOUBLE const ts
           ,char *preName        ,char *nameOut);

  void printFluid(Memoria *m         ,PropVarFluid *propF
               ,Turbulence *turbModel,EnergyModel *eModel
               ,PartMesh *pMesh      ,Scheme *sc
               ,Loads *loadsVel      ,Loads *loadsPres 
               ,Loads *loadsTemp     ,FileOpt *opt
               ,Mesh *mesh0          ,Mesh *mesh  
               ,Mean *media      
               ,char *preName        ,char *nameOut);
  
  void printCombustion(Memoria *m      ,PropVarFluid *propF
               ,Turbulence *turbModel,EnergyModel *eModel 
               ,Combustion *cModel
               ,PartMesh *pMesh      ,Scheme *sc
               ,Loads *loadsVel      ,Loads *loadsPres 
               ,Loads *loadsTemp     ,Loads *loadsComb
               ,FileOpt *opt
               ,Mesh *mesh0          ,Mesh *mesh  
               ,Mean *media      
               ,char *preName        ,char *nameOut);

  void printDiff(Memoria *m
               , PartMesh *pMesh, Scheme *sc
               , Loads *loadsD1 , FileOpt *opt
               , Mesh *mesh0    , Mesh *mesh
               , char *preName  , char *nameOut);

  void printTrans(Memoria *m
                , PartMesh *pMesh, Scheme *sc
                , Loads *loadsT1 , FileOpt *opt
                , Mesh *mesh0    , Mesh *mesh
                , char *preName  , char *nameOut);

  void printFace(Memoria *m   , Mesh *mesh
               , char* preName, bool fComb
               , bool bVtk    , FILE *fileOut);


  void reScaleMesh(Mesh *mesh,  FILE *fileIn);


#endif/*_WRITE_VTK_H_*/