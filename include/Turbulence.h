#ifndef _TURBULENCE_H_
  #define  _TURBULENCE_H_
/*...*/
  #include<math.h>
/*...................................................................*/

/*...*/
  #include<CellLoop.h>
  #include<Define.h>
  #include<Erro.h>
  #include<Memoria.h>
  #include<Mesh.h>
  #include<Jacobi.h>
/*...................................................................*/

  #define E_WALLMODEL   9.793e0
  #define VANDRIEST    26.e0
  #define VONKARMAN     0.4187e0
  #define DYNAMIC_CLIP  0.23e0

/*...*/
  void turbulence(Memoria *m
      , Loads *lVel                         , Turbulence tModel             
      , INT    *RESTRICT el                 , INT    *RESTRICT nelcon 
      , short  *RESTRICT nen                , short  *RESTRICT nFace 
      , short  *RESTRICT geomType           , DOUBLE *RESTRICT prop  
      , short  *RESTRICT calType            , short  *RESTRICT mat        
      , DOUBLE *RESTRICT cc                 , DOUBLE *RESTRICT ksi                      
      , DOUBLE *RESTRICT mKsi               , DOUBLE *RESTRICT eta  
      , DOUBLE *RESTRICT fArea              , DOUBLE *RESTRICT normal  
      , DOUBLE *RESTRICT volume             , DOUBLE *RESTRICT xm  
      , DOUBLE *RESTRICT xmcc               , DOUBLE *RESTRICT vSkew    
      , DOUBLE *RESTRICT mvSkew             , DOUBLE *RESTRICT dcca  
      , short  *RESTRICT faceRvel           , short *RESTRICT faceVelL               
      , DOUBLE *RESTRICT vel                , DOUBLE *RESTRICT gradVel
      , DOUBLE *RESTRICT densityFluid       , DOUBLE *RESTRICT dViscosity        
      , DOUBLE *RESTRICT eddyViscosity      , DOUBLE *RESTRICT wallPar                        
      , short const maxNo                   , short const maxViz
      , short const ndm                     , INT const numel     
      , short const ndf);   
/*...................................................................*/

/*...*/
  void lesDynamicMean(INT    *RESTRICT nelcon   , short  *RESTRICT nen    
                   , short  *RESTRICT nFace   , DOUBLE *RESTRICT gVolume 
                   , DOUBLE *RESTRICT vel     , DOUBLE *RESTRICT gradVel 
                   , DOUBLE *RESTRICT density , DOUBLE *RESTRICT dynamic                                  
                   , short const maxNo        , short const maxViz
                   , short const ndm          , INT const numel     
                   , short const ndf); 
/*...................................................................*/

/*...*/
  void turbulenceCellLoop(Loads *lVel         , Turbulence tModel             
      , INT    *RESTRICT el                 , INT    *RESTRICT nelcon 
      , short  *RESTRICT nen                , short  *RESTRICT nFace 
      , short  *RESTRICT geomType           , DOUBLE *RESTRICT prop  
      , short  *RESTRICT calType            , short  *RESTRICT mat        
      , DOUBLE *RESTRICT cc                 , DOUBLE *RESTRICT gKsi                      
      , DOUBLE *RESTRICT gmKsi              , DOUBLE *RESTRICT gEta  
      , DOUBLE *RESTRICT gfArea             , DOUBLE *RESTRICT gNormal  
      , DOUBLE *RESTRICT gVolume            , DOUBLE *RESTRICT gXm  
      , DOUBLE *RESTRICT gXmcc              , DOUBLE *RESTRICT gvSkew    
      , DOUBLE *RESTRICT gmvSkew            , DOUBLE *RESTRICT gDcca  
      , short  *RESTRICT faceVelR           , short *RESTRICT faceVelL               
      , DOUBLE *RESTRICT vel                , DOUBLE *RESTRICT gradVel
      , DOUBLE *RESTRICT density            , DOUBLE *RESTRICT dViscosity        
      , DOUBLE *RESTRICT eddyViscosity      , DOUBLE *RESTRICT wallPar  
      , DOUBLE *RESTRICT dynamic                 
      , short const maxNo                   , short const maxViz
      , short const ndm                     , INT const numel     
      , short const ndf);  
/*...................................................................*/

/*...*/
  void cellLes(Loads *loadsVel         , Turbulence tModel           
          , short *RESTRICT lGeomType  , DOUBLE *RESTRICT prop
          , INT *RESTRICT lViz         , DOUBLE *RESTRICT ksi
          , DOUBLE *RESTRICT mKsi
          , DOUBLE *RESTRICT eta       , DOUBLE *RESTRICT mEta
          , DOUBLE *RESTRICT normal    , DOUBLE *RESTRICT area
          , DOUBLE *RESTRICT xm        , DOUBLE *RESTRICT xmcc
          , DOUBLE *RESTRICT dcca      , DOUBLE *RESTRICT cc
          , DOUBLE *RESTRICT vSkew     , DOUBLE *RESTRICT mvSkew
          , short  *RESTRICT faceVelR  , short *RESTRICT faceVelL      
          , DOUBLE *RESTRICT vel       , DOUBLE *RESTRICT gradVel   
          , DOUBLE *RESTRICT lDensity  , DOUBLE const dViscosity
          , DOUBLE *viscosity          
          , const short nEn            , short const nFace
          , const short ndm            , INT const nel);

  void cellLes3D(Loads *lVel             , Turbulence tModel           
          , short *RESTRICT lGeomType  , DOUBLE *RESTRICT prop 
          , INT *RESTRICT lViz         , DOUBLE *RESTRICT fArea
          , DOUBLE *RESTRICT normal    , DOUBLE *RESTRICT volume 
          , DOUBLE *RESTRICT dcca      , short *RESTRICT lFaceVelR  
          , short *RESTRICT lFaceVelL  , DOUBLE *RESTRICT vel       
          , DOUBLE *RESTRICT gradVel   , DOUBLE *RESTRICT lDensity  
          , DOUBLE const dViscosity    , DOUBLE *viscosity          
          , DOUBLE *RESTRICT wallPar   , DOUBLE *RESTRICT dynamic
          , const short nEn            , short const nFace 
          , const short ndm            , INT const nel); 
/*...................................................................*/

/*...*/
  void lesDynamic(INT *RESTRICT lViz         , DOUBLE *RESTRICT volume
                , DOUBLE *RESTRICT lDensity  , DOUBLE *restrict vel
                , DOUBLE *restrict gradVel   , DOUBLE *RESTRICT lDynamic
                , short const nFace  );
/*...................................................................*/

/*...*/
  void wallModel(DOUBLE const vt     , DOUBLE const viscosity
               , DOUBLE const density, DOUBLE const dWall 
               , DOUBLE *yP          , DOUBLE *uP
               , short const iCod);
  DOUBLE wallModelHeat(DOUBLE const yPlus,DOUBLE const prM
                     , DOUBLE const prT);
/*...................................................................*/

/*...*/
  bool  wallDist(Loads *lVel               
             , INT *RESTRICT lViz       , DOUBLE *RESTRICT v 
             , DOUBLE *RESTRICT normal  , DOUBLE *RESTRICT dcca
             , short *RESTRICT lFaceVelR, short *RESTRICT lFaceVelL
             , DOUBLE const viscosity   , DOUBLE const density
             , DOUBLE *RESTRICT wallPar , DOUBLE *dWall     
             , short const wallType     , short const nFace );
/*...................................................................*/

/*...*/
  DOUBLE sigmaModel(DOUBLE *RESTRICT s, DOUBLE *restrict gradVel 
                , short const nFace , short const ndm);
/*...................................................................*/

#endif /*_TURBULENCE_H_*/