#include<Print.h>
/********************************************************************* 
 * Data de criacao    : 16/08/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * globalCombCel: glabalizacao as arranjos para impressao            * 
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
static void globalCombCel(Memoria *m
                         ,Mesh *mesh0     ,Mesh *mesh
                         ,PartMesh *pMesh ,FileOpt *opt
                         ,short const nSp ,short const ndfVel
                         ,short const ndm ,short const ndfComb )
{

/*... pres (Cel)*/
  if(opt->pres)
    dGlobalCel(m                  , pMesh
             , mesh0->elm.pressure, mesh->elm.pressure
             , mesh->numelNov
             , 1              , 1);
/*...................................................................*/

/*... GradPres (Cel)*/
  if(opt->gradPres)
    dGlobalCel(m                  , pMesh
             , mesh0->elm.gradPres, mesh->elm.gradPres
             , mesh->numelNov
             , ndm                , 1);
/*...................................................................*/

/*... vel (Cel)*/
  if(opt->vel)
    dGlobalCel(m              , pMesh
             , mesh0->elm.vel , mesh->elm.vel
             , mesh->numelNov
             , ndm            , 1);
/*...................................................................*/

/*... GradVel (Cel)*/
  if(opt->gradPres)
    dGlobalCel(m                  , pMesh
             , mesh0->elm.gradVel , mesh->elm.gradVel
             , mesh->numelNov
             , ndm                , ndm);
/*...................................................................*/

/*... temp (Cel)*/
  if(opt->energy)
    dGlobalCel(m              , pMesh
             , mesh0->elm.temp, mesh->elm.temp
             , mesh->numelNov
             , 1              , 1);
/*...................................................................*/

/*... gradTemp (Cel)*/
  if(opt->gradEnergy)
    dGlobalCel(m                  , pMesh
             , mesh0->elm.gradTemp, mesh->elm.gradTemp
             , mesh->numelNov
             , ndm            , 1);
/*...................................................................*/

/*... yFrac (Cel)*/
  if(opt->yFrac)
    dGlobalCel(m                 , pMesh
             , mesh0->elm.yFrac  , mesh->elm.yFrac
             , mesh->numelNov
             , nSp               , 1);
/*...................................................................*/

/*... gradY (Cel)*/
  if(opt->gradY)
    dGlobalCel(m                 , pMesh
             , mesh0->elm.gradY  , mesh->elm.gradY
             , mesh->numelNov
             , nSp               , ndm);
/*...................................................................*/

/*... zComb (Cel)*/
  if(opt->zComb)
    dGlobalCel(m                 , pMesh
             , mesh0->elm.zComb  , mesh->elm.zComb
             , mesh->numelNov
             , ndfComb               , 1);
/*...................................................................*/

/*... gradZ (Cel)*/
  if(opt->gradZcomb)
    dGlobalCel(m                   , pMesh
             , mesh0->elm.gradZcomb, mesh->elm.gradZcomb
             , mesh->numelNov
             , ndfComb             , ndm);
/*...................................................................*/

/*... enthalpyk */
  if(opt->enthalpyk)
    dGlobalCel(m                   , pMesh
             , mesh0->elm.enthalpyk, mesh->elm.enthalpyk
             , mesh->numelNov
             , nSp                 , 1  );
/*...................................................................*/

/*... tReactor (Cel)*/
  if(opt->tReactor)
    dGlobalCel(m                  , pMesh
             , mesh0->elm.tReactor, mesh->elm.tReactor
             , mesh->numelNov
             , N_TERMS_REACTOR    , 1);
/*...................................................................*/

/*... density (Cel)*/
  if(opt->densityFluid)
    dGlobalCel(m                      , pMesh
             , mesh0->elm.densityFluid, mesh->elm.densityFluid
             , mesh->numelNov
             , DENSITY_LEVEL          , 1);
/*...................................................................*/

/*... viscosity (Cel)*/
  if(opt->dViscosity)
    dGlobalCel(m                   , pMesh
             , mesh0->elm.dViscosity, mesh->elm.dViscosity
             , mesh->numelNov
             , 1                    , 1);
/*...................................................................*/

/*... specificHeat (Cel)*/
  if(opt->specificHeat)
    dGlobalCel(m                      , pMesh
             , mesh0->elm.specificHeat, mesh->elm.specificHeat
             , mesh->numelNov
             , SHEAT_LEVEL            , 1);
/*...................................................................*/

/*... tCondutivity (Cel)*/
  if(opt->tConductivity)
    dGlobalCel(m                       , pMesh
             , mesh0->elm.tConductivity, mesh->elm.tConductivity
             , mesh->numelNov
             , 1                       , 1);
/*...................................................................*/

/*... tCondutivity (Cel)*/
  if(opt->tConductivity)
    dGlobalCel(m                       , pMesh
             , mesh0->elm.tConductivity, mesh->elm.tConductivity
             , mesh->numelNov
             , 1                       , 1);
/*...................................................................*/

/*... tCondutivity (Cel)*/
  if(opt->coefDiffSp)
    dGlobalCel(m                       , pMesh
             , mesh0->elm.cDiffComb    , mesh->elm.cDiffComb
             , mesh->numelNov
             , nSp                     , 1);
/*...................................................................*/

/*... eddyViscosity (Cel)*/
  if(opt->eddyViscosity)
    dGlobalCel(m                       , pMesh
             , mesh0->elm.eddyViscosity, mesh->elm.eddyViscosity
             , mesh->numelNov
             , 1                       , 1);
/*...................................................................*/

/*... rateHeatRe    (Cel)*/
  if(opt->rateHeatComb)
    dGlobalCel(m                        , pMesh
             , mesh0->elm.rateHeatReComb, mesh->elm.rateHeatReComb
             , mesh->numelNov
             , 1                        , 1);
/*...................................................................*/

/*... rateHeatRe (Cel)*/
  if(opt->wk)
    dGlobalCel(m                        , pMesh
             , mesh0->elm.wk            , mesh->elm.wk              
             , mesh->numelNov
             , nSp                      , 1);
/*...................................................................*/

}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 02/12/2017                                   *
 * Data de modificaco : 14/10/2019                                   *
 *-------------------------------------------------------------------*
 * printFluid: impressao do fluido                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------*  
 * m       -> vetor de memoria principal                             * 
 * turbModel -> modelo de turbulencia                                *
 * eModel    -> modelo da equacao de energia                         *
 * loadVel   -> deficicao de cargas velocidade                       *
 * loadPres  -> deficicao de cargas pres                             *
 * loadTemp  -> deficicao de cargas temperatura                      *
 * opt       -> opcoes de arquivo                                    *
 * mesh0     -> malha global                                         *
 * mesh      -> malha particionada                                   *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*  
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void printFluid(Memoria *m           ,PropVarFluid *propF
               ,Turbulence *turbModel,EnergyModel *eModel
               ,PartMesh *pMesh      ,Scheme *sc
               ,Loads *loadsVel      ,Loads *loadsPres 
               ,Loads *loadsTemp     ,FileOpt *opt
               ,Mesh *mesh0          ,Mesh *mesh  
               ,Mean *media      
               ,char *preName        ,char *nameOut)
{
 
  short ndm = mesh->ndm;
  
  void *dum=NULL;
  short ndfVel;
  DOUBLE *nStressR=NULL,*nEddyV=NULL,*nDvisc=NULL;
  DOUBLE *nSheat=NULL,*nTCond=NULL,*nGradRho=NULL,*cell=NULL;
  DOUBLE *nMedVel=NULL,*nP2Vel=NULL,*nMedP2Vel=NULL;
  DOUBLE *nCdyn=NULL,*nWall=NULL,*nKturb=NULL;
  FILE *fileOut=NULL;

/*...*/
  ndfVel = max(mesh->ndfF - 1, mesh->ndfFt - 2);
/*...................................................................*/

/*...*/
  HccaAlloc(DOUBLE, m, cell, mesh->numel, "AuxCell"  , _AD_);
  if(opt->gradRho)
  {
    HccaAlloc(DOUBLE, m, nGradRho , mesh->nnode*ndm   , "nGR"   , _AD_);   
  } 
  if(opt->specificHeat)
    HccaAlloc(DOUBLE, m, nSheat   , mesh->nnode    , "nSheat"   , _AD_);
  if(opt->tConductivity)
    HccaAlloc(DOUBLE, m, nTCond   , mesh->nnode          , "nTcond"   , _AD_);
  if(opt->dViscosity)
    HccaAlloc(DOUBLE, m, nDvisc   , mesh->nnode          , "nVis"     , _AD_);
  if(opt->eddyViscosity)
    HccaAlloc(DOUBLE, m, nEddyV   , mesh->nnode          , "nEddyV"   , _AD_);
  if(opt->stressR)
    HccaAlloc(DOUBLE, m, nStressR , mesh->nnode*mesh->ntn, "nStressR" , _AD_);
  if(opt->cDynamic)
    HccaAlloc(DOUBLE, m, nCdyn    , mesh->nnode*2        , "nCdyn"    , _AD_); 
  if(opt->wallParameters)
    HccaAlloc(DOUBLE, m, nWall    , mesh->nnode*NWALLPAR , "nWall"    , _AD_);
  if(opt->kTurb)
    HccaAlloc(DOUBLE, m, nKturb   , mesh->nnode           , "nKturb"   , _AD_);
/*...................................................................*/

/*...*/
  if (media->fMedia) 
  {
    HccaAlloc(DOUBLE, m, nMedVel  , mesh->nnode*ndm, "nMediaVel", _AD_);
    zero(nMedVel, mesh->nnode*ndm,DOUBLEC);
    HccaAlloc(DOUBLE, m, nP2Vel   , mesh->nnode*ndm, "nP2Vel", _AD_);
    zero(nMedVel, mesh->nnode*ndm,DOUBLEC);
    HccaAlloc(DOUBLE, m,nMedP2Vel, mesh->nnode*ndm, "nMedP2Vel", _AD_);
    zero(nMedVel, mesh->nnode*ndm,DOUBLEC);
  }
/*...................................................................*/

/*... reconstruindo do gradiente (Pres)*/
  if(opt->gradPres)
  {
    tm.rcGradPres = getTimeC() - tm.rcGradPres;
    rcGradU(m                      , loadsPres
         , mesh->elm.node          , mesh->elm.adj.nelcon
         , mesh->node.x
         , mesh->elm.nen           , mesh->elm.adj.nViz
         , mesh->elm.cellFace      , mesh->face.owner
         , mesh->elm.geom.volume   , mesh->elm.geom.dcca
         , mesh->elm.geom.xmcc     , mesh->elm.geom.cc
         , mesh->face.mksi         , mesh->face.ksi
         , mesh->face.eta          , mesh->face.area
         , mesh->face.normal       , mesh->face.xm
         , mesh->face.mvSkew       , mesh->face.vSkew
         , mesh->elm.geomType      , mesh->elm.material.prop
         , mesh->elm.material.type 
         , mesh->elm.mat           , NULL
         , mesh->elm.leastSquare   , mesh->elm.leastSquareR
         , mesh->elm.faceRpres     
         , mesh->elm.pressure      , mesh->elm.gradPres                
         , mesh->node.pressure     
         , mesh->elm.densityFluid , mesh->elm.gradRhoFluid
         , propF->densityRef
         , &sc->rcGrad
         , mesh->maxNo             , mesh->maxViz
         , 1                       , mesh->ndm       
         , &pMesh->iNo             , &pMesh->iEl  
         , mesh->numelNov          , mesh->numel        
         , mesh->nnodeNov          , mesh->nnode
         , false);  
    tm.rcGradPres = getTimeC() - tm.rcGradPres;
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (GradPres)*/
  if(opt->gradPres && opt->fNode)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                 , loadsPres
              , mesh->elm.cellFace , mesh->face.owner
              , mesh->node.gradPres, mesh->elm.gradPres
              , mesh->elm.node     , mesh->elm.geomType            
              , mesh->elm.geom.cc  , mesh->node.x  
              , mesh->face.xm
              , mesh->elm.nen      , mesh->elm.adj.nViz
              , mesh->elm.faceRpres, &pMesh->iNo          
              , mesh->numelNov     , mesh->numel        
              , mesh->nnodeNov     , mesh->nnode 
              , mesh->maxNo        , mesh->maxViz   
              , mesh->ndm          , 1
              , mesh->ndm          , 2);
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (pres)*/
  if(opt->pres && opt->fNode )
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                 , loadsPres
               , mesh->elm.cellFace , mesh->face.owner
               , mesh->node.pressure, mesh->elm.pressure   
               , mesh->elm.node     , mesh->elm.geomType            
               , mesh->elm.geom.cc  , mesh->node.x  
               , mesh->face.xm        
               , mesh->elm.nen      , mesh->elm.adj.nViz
               , mesh->elm.faceRpres, &pMesh->iNo           
               , mesh->numelNov     , mesh->numel        
               , mesh->nnodeNov     , mesh->nnode 
               , mesh->maxNo        , mesh->maxViz   
               , 1                  , 1
               , mesh->ndm          , 2);
    boundaryNode(m                     , loadsPres  
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.pressure   , mesh->elm.pressure   
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRpres             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , 1                     ,1  
               , mesh->ndm       ); 
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... calculo da matrix jacobiana das velocidades
                            | du1dx1 du1dx2 du1dx3 |   
                            | du2dx1 du2dx2 du2dx3 |   
                            | du3dx1 du3dx2 du3dx3 |
*/        
  if(opt->gradVel)
  {
    tm.rcGradVel  = getTimeC() - tm.rcGradVel;
    rcGradU(m                        , loadsVel
         , mesh->elm.node          , mesh->elm.adj.nelcon
         , mesh->node.x
         , mesh->elm.nen           , mesh->elm.adj.nViz
         , mesh->elm.cellFace      , mesh->face.owner
         , mesh->elm.geom.volume   , mesh->elm.geom.dcca
         , mesh->elm.geom.xmcc     , mesh->elm.geom.cc
         , mesh->face.mksi         , mesh->face.ksi
         , mesh->face.eta          , mesh->face.area
         , mesh->face.normal       , mesh->face.xm
         , mesh->face.mvSkew       , mesh->face.vSkew
         , mesh->elm.geomType      , mesh->elm.material.prop
         , mesh->elm.material.type 
         , mesh->elm.mat           , NULL
         , mesh->elm.leastSquare   , mesh->elm.leastSquareR
         , mesh->elm.faceRvel       
         , mesh->elm.vel           , mesh->elm.gradVel                           
         , mesh->node.vel          
         , NULL                   , NULL
         , 0
         , &sc->rcGrad
         , mesh->maxNo             , mesh->maxViz
         , ndfVel                  , mesh->ndm
         , &pMesh->iNo             , &pMesh->iEl  
         , mesh->numelNov          , mesh->numel        
         , mesh->nnodeNov          , mesh->nnode
         , false); 
    tm.rcGradVel = getTimeC() - tm.rcGradVel;  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (gradVel)*/
    if(opt->fNode)
    {
      tm.interCellNode = getTimeC() - tm.interCellNode;
      interCellNode(m                   , loadsVel
                  , mesh->elm.cellFace , mesh->face.owner
                  , mesh->node.gradVel , mesh->elm.gradVel    
                  , mesh->elm.node     , mesh->elm.geomType            
                  , mesh->elm.geom.cc  , mesh->node.x  
                  , mesh->face.xm        
                  , mesh->elm.nen      , mesh->elm.adj.nViz
                  , mesh->elm.faceRvel , &pMesh->iNo           
                  , mesh->numelNov     , mesh->numel        
                  , mesh->nnodeNov     , mesh->nnode 
                  , mesh->maxNo        , mesh->maxViz   
                  , ndfVel             , mesh->ndm
                  , mesh->ndm          , 2);
      tm.interCellNode = getTimeC() - tm.interCellNode;
    }  
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (vel)*/
  if(opt->fNode && opt->vel)
  {
    interCellNode(m                  , loadsVel
               , mesh->elm.cellFace, mesh->face.owner
               , mesh->node.vel    , mesh->elm.vel        
               , mesh->elm.node    , mesh->elm.geomType            
               , mesh->elm.geom.cc , mesh->node.x  
               , mesh->face.xm   
               , mesh->elm.nen     , mesh->elm.adj.nViz
               , mesh->elm.faceRvel, &pMesh->iNo          
               , mesh->numelNov    , mesh->numel        
               , mesh->nnodeNov    , mesh->nnode 
               , mesh->maxNo       , mesh->maxViz   
               , mesh->ndm         , 1
               , mesh->ndm         , 2);

    boundaryNode(m                     , loadsVel  
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.vel        , mesh->elm.vel    
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRvel             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , ndfVel                , 1  
               , mesh->ndm       );
  }
/*...................................................................*/

/*... medias*/
  if (media->fMedia) {

/*... medias das velociade*/
    if(media->fVel)
      interCellNode(m                   , loadsVel
                   , mesh->elm.cellFace , mesh->face.owner
                   , nMedVel            , media->mVel        
                   , mesh->elm.node     , mesh->elm.geomType            
                   , mesh->elm.geom.cc  , mesh->node.x  
                   , mesh->face.xm        
                   , mesh->elm.nen      , mesh->elm.adj.nViz
                   , mesh->elm.faceRvel , &pMesh->iNo           
                   , mesh->numelNov     , mesh->numel        
                   , mesh->nnodeNov     , mesh->nnode 
                   , mesh->maxNo        , mesh->maxViz   
                   , mesh->ndm          , 1
                   , mesh->ndm          , 2);
/*...................................................................*/
  }
/*...................................................................*/

/*...*/
  if(mesh->ndfFt)
  {
/*... reconstruindo do gradiente (gradRho)*/
    if (opt->gradRho)
    {
      getColFromMatrix(cell        ,mesh->elm.densityFluid
                      ,mesh->numel ,DENSITY_LEVEL
                      ,TIME_N);
      rcGradU(m                     , loadsRhoFluid
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
           , mesh->elm.material.type
           , mesh->elm.mat          , NULL
           , mesh->elm.leastSquare  , mesh->elm.leastSquareR
           , mesh->elm.faceRrho  
           , cell                   , mesh->elm.gradRhoFluid
           , mesh->node.rhoFluid    
           , NULL                   , NULL
           , 0
           , &sc->rcGrad
           , mesh->maxNo            , mesh->maxViz
           , 1                      , mesh->ndm       
           , &pMesh->iNo            , &pMesh->iEl 
           , mesh->numelNov         , mesh->numel
           , mesh->nnodeNov         , mesh->nnode
           , true); 

/*.................................................................. */

/*... interpolacao das variaveis da celulas para pos nos (GradRho)*/
      if(opt->fNode && opt->gradRho)
      {
        interCellNode(m              , loadsRhoFluid  
              , mesh->elm.cellFace   , mesh->face.owner
              , nGradRho             , mesh->elm.gradRhoFluid 
              , mesh->elm.node       , mesh->elm.geomType
              , mesh->elm.geom.cc    , mesh->node.x
              , mesh->face.xm          
              , mesh->elm.nen        , mesh->elm.adj.nViz
              , mesh->elm.faceRrho   , &pMesh->iNo            
              , mesh->numelNov       , mesh->numel
              , mesh->nnodeNov       , mesh->nnode
              , mesh->maxNo          , mesh->maxViz
              , mesh->ndm            , 1
              , mesh->ndm            , 2);  
      }
/*...................................................................*/
    }
/*...................................................................*/

/*... reconstruindo do gradiente (Energia)*/
    if(opt->gradEnergy)
      rcGradU(m                     , loadsTemp
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
           , mesh->elm.material.type
           , mesh->elm.mat          , mesh->elm.tConductivity
           , mesh->elm.leastSquare  , mesh->elm.leastSquareR
           , mesh->elm.faceRenergy  
           , mesh->elm.temp         , mesh->elm.gradTemp  
           , mesh->node.temp        
           , NULL                   , NULL
           , 0
           , &sc->rcGrad
           , mesh->maxNo            , mesh->maxViz
           , 1                      , mesh->ndm
           , &pMesh->iNo            , &pMesh->iEl
           , mesh->numelNov         , mesh->numel
           , mesh->nnodeNov         , mesh->nnode
           , false); 
/*.................................................................. */

/*... interpolacao das variaveis da celulas para pos nos (GradEnergy)*/
    if(opt->fNode && opt->gradEnergy)
      interCellNode(m                  , loadsTemp  
              , mesh->elm.cellFace   , mesh->face.owner
              , mesh->node.gradTemp  , mesh->elm.gradTemp  
              , mesh->elm.node       , mesh->elm.geomType
              , mesh->elm.geom.cc    , mesh->node.x
              , mesh->face.xm          
              , mesh->elm.nen        , mesh->elm.adj.nViz
              , mesh->elm.faceRenergy, &pMesh->iNo            
              , mesh->numelNov       , mesh->numel
              , mesh->nnodeNov       , mesh->nnode
              , mesh->maxNo          , mesh->maxViz
              , mesh->ndm            , 1
              , mesh->ndm            , 2);  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (energy)*/
    if(opt->fNode && opt->temp)
    {
     interCellNode(m                , loadsTemp
             , mesh->elm.cellFace   , mesh->face.owner
             , mesh->node.temp      , mesh->elm.temp
             , mesh->elm.node       , mesh->elm.geomType
             , mesh->elm.geom.cc    , mesh->node.x
             , mesh->face.xm          
             , mesh->elm.nen        , mesh->elm.adj.nViz
             , mesh->elm.faceRenergy, &pMesh->iNo            
             , mesh->numelNov       , mesh->numel
             , mesh->nnodeNov       , mesh->nnode
             , mesh->maxNo          , mesh->maxViz
             , 1                    , 1
             , mesh->ndm            , 2); 

      boundaryNode(m                   , loadsTemp   
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.temp       , mesh->elm.temp 
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRenergy             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , 1                     , 1  
               , mesh->ndm       );
    }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (density)*/
    if(opt->fNode && opt->densityFluid)
      interCellNode(m             ,  loadsRhoFluid 
             , mesh->elm.cellFace , mesh->face.owner
             , mesh->node.rhoFluid, mesh->elm.densityFluid
             , mesh->elm.node     , mesh->elm.geomType
             , mesh->elm.geom.cc  , mesh->node.x
             , mesh->face.xm      
             , mesh->elm.nen      , mesh->elm.adj.nViz
             , dum                , &pMesh->iNo         
             , mesh->numelNov     , mesh->numel
             , mesh->nnodeNov     , mesh->nnode
             , mesh->maxNo        , mesh->maxViz
             , 3                  , 1
             , mesh->ndm          , 2);  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (dViscosity)*/
    if(opt->fNode && opt->dViscosity)
      interCellNode(m                  , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nDvisc               , mesh->elm.dViscosity
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (sHeat)*/
    if(opt->fNode && opt->specificHeat)
    {
      getColFromMatrix(cell        ,mesh->elm.specificHeat
                      ,mesh->numel ,SHEAT_LEVEL
                      ,TIME_N);

      interCellNode(m                    , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nSheat               , cell
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
    }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (sHeat)*/
    if(opt->fNode && opt->tConductivity)
      interCellNode(m                    , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nTCond               , mesh->elm.tConductivity
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
/*...................................................................*/
  }
/*...................................................................*/

/*...*/
  if (turbModel->fTurb) 
  {
/*... viscisidade turbulenta*/
    if(opt->eddyViscosity)       
      interCellNode(m                  , loadsVel
                   , mesh->elm.cellFace, mesh->face.owner
                   , nEddyV            , mesh->elm.eddyViscosity        
                   , mesh->elm.node    , mesh->elm.geomType            
                   , mesh->elm.geom.cc , mesh->node.x  
                   , mesh->face.xm   
                   , mesh->elm.nen     , mesh->elm.adj.nViz
                   , dum               , &pMesh->iNo          
                   , mesh->numelNov    , mesh->numel        
                   , mesh->nnodeNov    , mesh->nnode 
                   , mesh->maxNo       , mesh->maxViz   
                   , 1                 , 1
                   , mesh->ndm         , 2);
/*...................................................................*/

/*... tensor residual (modelos estruturais)*/
    if(opt->stressR)
      interCellNode(m                     , loadsVel
                , mesh->elm.cellFace      , mesh->face.owner
                , nStressR                , mesh->elm.stressR              
                , mesh->elm.node          , mesh->elm.geomType            
                , mesh->elm.geom.cc       , mesh->node.x  
                , mesh->face.xm             
                , mesh->elm.nen           , mesh->elm.adj.nViz
                , dum                     , &pMesh->iNo                
                , mesh->numelNov          , mesh->numel        
                , mesh->nnodeNov          , mesh->nnode 
                , mesh->maxNo             , mesh->maxViz   
                , 6                       , 1
                , mesh->ndm               , 2);
/*...................................................................*/

/*... coeficiente dinamico*/
    if(opt->cDynamic)
      interCellNode(m                      , loadsVel
                , mesh->elm.cellFace       , mesh->face.owner
                , nCdyn                    , mesh->elm.cd              
                , mesh->elm.node           , mesh->elm.geomType            
                , mesh->elm.geom.cc        , mesh->node.x  
                , mesh->face.xm              
                , mesh->elm.nen            , mesh->elm.adj.nViz
                , dum                      , &pMesh->iNo                 
                , mesh->numelNov           , mesh->numel        
                , mesh->nnodeNov           , mesh->nnode 
                , mesh->maxNo              , mesh->maxViz   
                , 2                        , 1
                , mesh->ndm                , 2);
/*...................................................................*/

/*... parametros de parede*/
    if(opt->wallParameters)
      interCellNode(m                     , loadsVel
                , mesh->elm.cellFace      , mesh->face.owner
                , nWall                   , mesh->elm.wallParameters              
                , mesh->elm.node          , mesh->elm.geomType            
                , mesh->elm.geom.cc       , mesh->node.x  
                , mesh->face.xm             
                , mesh->elm.nen           , mesh->elm.adj.nViz
                , dum                     , &pMesh->iNo                
                , mesh->numelNov          , mesh->numel        
                , mesh->nnodeNov          , mesh->nnode 
                , mesh->maxNo             , mesh->maxViz   
                , 4                       , 1
                , mesh->ndm               , 2);
/*...................................................................*/

/*... energia cinetica turbulenta*/
    if(opt->kTurb)       
      interCellNode(m              , loadsVel
            , mesh->elm.cellFace   , mesh->face.owner
            , nKturb               , mesh->elm.kTurb                
            , mesh->elm.node       , mesh->elm.geomType            
            , mesh->elm.geom.cc    , mesh->node.x  
            , mesh->face.xm      
            , mesh->elm.nen        , mesh->elm.adj.nViz
            , mesh->elm.faceReKturb, &pMesh->iNo          
            , mesh->numelNov       , mesh->numel        
            , mesh->nnodeNov       , mesh->nnode 
            , mesh->maxNo          , mesh->maxViz   
            , 1                    , 1
            , mesh->ndm            , 2);
/*...................................................................*/

  }                                 
/*...................................................................*/

/*...*/
  if(!mpiVar.myId ){
    fName(preName,sc->ddt.timeStep,0,21,nameOut);
/*...*/
    wResVtkFluid(m                        , mesh0->node.x
               , mesh0->elm.geom.cc       
               , mesh0->elm.node          , mesh0->elm.mat    
               , mesh0->elm.nen           , mesh0->elm.geomType
               , mesh0->elm.pressure      , mesh0->node.pressure
               , mesh0->elm.gradPres      , mesh0->node.gradPres  
               , mesh0->elm.vel           , mesh0->node.vel      
               , mesh0->elm.gradVel       , mesh0->node.gradVel 
               , mesh0->elm.temp          , mesh0->node.temp   
               , mesh0->elm.gradTemp      , mesh0->node.gradTemp
               , mesh0->elm.eddyViscosity , nEddyV
               , mesh0->elm.densityFluid  , mesh0->node.rhoFluid
               , mesh0->elm.dViscosity    , nDvisc
               , mesh0->elm.stressR       , nStressR
               , mesh0->elm.cd            , nCdyn
               , mesh0->elm.wallParameters, nWall
               , mesh0->elm.kTurb         , nKturb
               , media->mVel              , nMedVel
               , mesh0->elm.specificHeat  , nSheat
               , mesh0->elm.tConductivity , nTCond    
               , mesh0->elm.gradRhoFluid  , nGradRho                                                           
               , mesh0->nnode             , mesh0->numel  
               , mesh0->ndm               , mesh0->maxNo 
               , mesh0->numat             , ndfVel
               , mesh0->ntn               
               , nameOut                  , opt
               , eModel->fKelvin          , media
               , &sc->ddt                 , fileOut);   
/*...................................................................*/
  }
/*...................................................................*/

/*... desalocando memoria*/
  if (media->fMedia) {
    HccaDealloc(m, nMedP2Vel, "nMedP2Vel", _AD_);
    HccaDealloc(m, nP2Vel   , "nP2Vel"   , _AD_);
    HccaDealloc(m, nMedVel  , "nMediaVel", _AD_);
  }
  if(opt->kTurb)
    HccaDealloc(m, nKturb   , "nKturb"   , _AD_);
  if(opt->wallParameters)
    HccaDealloc(m, nWall    , "nWall"    , _AD_);
  if(opt->cDynamic)
    HccaDealloc(m, nCdyn    , "nCdyn"    , _AD_); 
  if(opt->stressR)
    HccaDealloc(m, nStressR , "nStressR" , _AD_); 
  if(opt->eddyViscosity)
    HccaDealloc(m, nEddyV   , "nEddyV"   , _AD_); 
  if(opt->dViscosity)
    HccaDealloc(m, nDvisc   , "nVis"     , _AD_);   
  if(opt->tConductivity)
    HccaDealloc(m, nTCond, "nTcond", _AD_);
  if(opt->specificHeat)
    HccaDealloc(m, nSheat, "nSheat", _AD_);  
  if(opt->gradRho)
  {
    HccaDealloc(m, nGradRho, "nGR", _AD_);
  }
  HccaDealloc(m, cell    , "AuxCell", _AD_);
/*...................................................................*/

}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 05/08/2018                                   *
 * Data de modificaco : 26/10/2019                                   *
 *-------------------------------------------------------------------*
 * printCombustion: impressao do fluido                              * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------*  
 * m       -> vetor de memoria principal                             * 
 * turbModel -> modelo de turbulencia                                *
 * eModel    -> modelo da equacao de energia                         *
 * cModel    -> modelo de combustao
 * loadVel   -> deficicao de cargas velocidade                       *
 * loadPres  -> deficicao de cargas pres                             *
 * loadTemp  -> deficicao de cargas temperatura                      *
 * opt       -> opcoes de arquivo                                    *
 * mesh0     -> malha global                                         *
 * mesh      -> malha particionada                                   *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*  
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void printCombustion(Memoria *m      ,PropVarFluid *propF
               ,Turbulence *turbModel,EnergyModel *eModel 
               ,Combustion *cModel
               ,PartMesh *pMesh      ,Scheme *sc
               ,Loads *loadsVel      ,Loads *loadsPres 
               ,Loads *loadsTemp     ,Loads *loadsComb
               ,FileOpt *opt
               ,Mesh *mesh0          ,Mesh *mesh  
               ,Mean *media      
               ,char *preName        ,char *nameOut)
{
 
  short ndm,nSp;
  void *dum=NULL;
  short ndfVel,ndfZ;
  DOUBLE *nStressR=NULL,*nEddyV=NULL,*nDvisc=NULL;
  DOUBLE *nSheat=NULL,*nTCond=NULL,*nGradRho=NULL,*cell=NULL;
  DOUBLE *nMedVel=NULL,*nP2Vel=NULL,*nMedP2Vel=NULL;
  DOUBLE *nCdyn=NULL,*nWall=NULL,*nKturb=NULL,*nWk=NULL;
  DOUBLE *nYfrac=NULL,*nRaHeReComb=NULL,*nEnthalpyK=NULL,*nGradY=NULL;
  DOUBLE *nDiffY=NULL;
  FILE *fileOut=NULL;

/*...*/
  ndm     = mesh->ndm;
  ndfZ    = cModel->nComb;
  nSp     = cModel->nOfSpecies; 
  ndfVel  = max(mesh->ndfF - 1, mesh->ndfFt - 2);
/*...................................................................*/

/*...*/
  HccaAlloc(DOUBLE, m, cell, mesh->numel, "AuxCell"  , _AD_);
  if(opt->gradRho)
  {
    HccaAlloc(DOUBLE, m, nGradRho , mesh->nnode*ndm   , "nGR"   , _AD_);   
  }   
  if(opt->specificHeat)
    HccaAlloc(DOUBLE, m, nSheat   , mesh->nnode    , "nSheat"   , _AD_);
  if(opt->tConductivity)
    HccaAlloc(DOUBLE, m, nTCond   , mesh->nnode          , "nTcond"   , _AD_);
  if(opt->dViscosity)
    HccaAlloc(DOUBLE, m, nDvisc   , mesh->nnode          , "nVis"     , _AD_);
  if(opt->eddyViscosity)
    HccaAlloc(DOUBLE, m, nEddyV   , mesh->nnode          , "nEddyV"   , _AD_);
  if(opt->coefDiffSp)
    HccaAlloc(DOUBLE, m, nDiffY   , mesh->nnode*ndfZ     , "cDiffZ"   , _AD_);
  if(opt->stressR)
    HccaAlloc(DOUBLE, m, nStressR , mesh->nnode*mesh->ntn, "nStressR" , _AD_);
  if(opt->cDynamic)
    HccaAlloc(DOUBLE, m, nCdyn    , mesh->nnode*2        , "nCdyn"    , _AD_); 
  if(opt->wallParameters)
    HccaAlloc(DOUBLE, m, nWall    , mesh->nnode*NWALLPAR , "nWall"    , _AD_);
  if(opt->kTurb)
    HccaAlloc(DOUBLE, m, nKturb   , mesh->nnode           , "nKturb"   , _AD_);
/*...................................................................*/

/*...*/
  if(opt->yFrac)
    HccaAlloc(DOUBLE, m, nYfrac   , mesh->nnode*nSp, "nYfrac"   , _AD_);
/*...................................................................*/

/*...*/
  if(opt->wk)
    HccaAlloc(DOUBLE, m, nWk, mesh->nnode, "nWk", _AD_);
/*...................................................................*/

/*...*/
  if(opt->rateHeatComb)
    HccaAlloc(DOUBLE, m, nRaHeReComb, mesh->nnode, "nRaHeComb",_AD_);
/*...................................................................*/

/*...*/
  if (media->fMedia) 
  {
    HccaAlloc(DOUBLE, m, nMedVel  , mesh->nnode*ndm, "nMediaVel", _AD_);
    zero(nMedVel, mesh->nnode*ndm,DOUBLEC);
    HccaAlloc(DOUBLE, m, nP2Vel   , mesh->nnode*ndm, "nP2Vel", _AD_);
    zero(nMedVel, mesh->nnode*ndm,DOUBLEC);
    HccaAlloc(DOUBLE, m,nMedP2Vel, mesh->nnode*ndm, "nMedP2Vel", _AD_);
    zero(nMedVel, mesh->nnode*ndm,DOUBLEC);
  }
/*...................................................................*/

/*... reconstruindo do gradiente (Pres)*/
  if(opt->gradPres)
  {
    tm.rcGradPres = getTimeC() - tm.rcGradPres;
    rcGradU(m                      , loadsPres
         , mesh->elm.node          , mesh->elm.adj.nelcon
         , mesh->node.x
         , mesh->elm.nen           , mesh->elm.adj.nViz
         , mesh->elm.cellFace      , mesh->face.owner
         , mesh->elm.geom.volume   , mesh->elm.geom.dcca
         , mesh->elm.geom.xmcc     , mesh->elm.geom.cc
         , mesh->face.mksi         , mesh->face.ksi
         , mesh->face.eta          , mesh->face.area
         , mesh->face.normal       , mesh->face.xm
         , mesh->face.mvSkew       , mesh->face.vSkew
         , mesh->elm.geomType      , mesh->elm.material.prop
         , mesh->elm.material.type 
         , mesh->elm.mat           , NULL
         , mesh->elm.leastSquare   , mesh->elm.leastSquareR
         , mesh->elm.faceRpres     
         , mesh->elm.pressure      , mesh->elm.gradPres                
         , mesh->node.pressure     
         , mesh->elm.densityFluid , mesh->elm.gradRhoFluid
         , propF->densityRef
         , &sc->rcGrad
         , mesh->maxNo             , mesh->maxViz
         , 1                       , mesh->ndm       
         , &pMesh->iNo             , &pMesh->iEl  
         , mesh->numelNov          , mesh->numel        
         , mesh->nnodeNov          , mesh->nnode
         , false);  
    tm.rcGradPres = getTimeC() - tm.rcGradPres;
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (GradPres)*/
  if(opt->gradPres && opt->fNode)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                  , loadsPres
              , mesh->elm.cellFace , mesh->face.owner
              , mesh->node.gradPres, mesh->elm.gradPres
              , mesh->elm.node     , mesh->elm.geomType            
              , mesh->elm.geom.cc  , mesh->node.x  
              , mesh->face.xm
              , mesh->elm.nen      , mesh->elm.adj.nViz
              , mesh->elm.faceRpres, &pMesh->iNo          
              , mesh->numelNov     , mesh->numel        
              , mesh->nnodeNov     , mesh->nnode 
              , mesh->maxNo        , mesh->maxViz   
              , mesh->ndm          , 1
              , mesh->ndm          , 2);
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (pres)*/
  if(opt->pres && opt->fNode)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                 , loadsPres
               , mesh->elm.cellFace , mesh->face.owner
               , mesh->node.pressure, mesh->elm.pressure   
               , mesh->elm.node     , mesh->elm.geomType            
               , mesh->elm.geom.cc  , mesh->node.x  
               , mesh->face.xm        
               , mesh->elm.nen      , mesh->elm.adj.nViz
               , mesh->elm.faceRpres, &pMesh->iNo           
               , mesh->numelNov     , mesh->numel        
               , mesh->nnodeNov     , mesh->nnode 
               , mesh->maxNo        , mesh->maxViz   
               , 1                  , 1
               , mesh->ndm          , 2);
    boundaryNode(m                     , loadsPres  
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.pressure   , mesh->elm.pressure   
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRpres             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , 1                     ,1  
               , mesh->ndm       ); 
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... calculo da matrix jacobiana das velocidades
                            | du1dx1 du1dx2 du1dx3 |   
                            | du2dx1 du2dx2 du2dx3 |   
                            | du3dx1 du3dx2 du3dx3 |
*/  
  if(opt->gradVel)
  {      
    tm.rcGradVel  = getTimeC() - tm.rcGradVel;
    rcGradU(m                        , loadsVel
         , mesh->elm.node          , mesh->elm.adj.nelcon
         , mesh->node.x
         , mesh->elm.nen           , mesh->elm.adj.nViz
         , mesh->elm.cellFace      , mesh->face.owner
         , mesh->elm.geom.volume   , mesh->elm.geom.dcca
         , mesh->elm.geom.xmcc     , mesh->elm.geom.cc
         , mesh->face.mksi         , mesh->face.ksi
         , mesh->face.eta          , mesh->face.area
         , mesh->face.normal       , mesh->face.xm
         , mesh->face.mvSkew       , mesh->face.vSkew
         , mesh->elm.geomType      , mesh->elm.material.prop
         , mesh->elm.material.type 
         , mesh->elm.mat           , NULL
         , mesh->elm.leastSquare   , mesh->elm.leastSquareR
         , mesh->elm.faceRvel       
         , mesh->elm.vel           , mesh->elm.gradVel                           
         , mesh->node.vel          
         , NULL                   , NULL
         , 0
         , &sc->rcGrad
         , mesh->maxNo             , mesh->maxViz
         , ndfVel                  , mesh->ndm
         , &pMesh->iNo             , &pMesh->iEl  
         , mesh->numelNov          , mesh->numel        
         , mesh->nnodeNov          , mesh->nnode
         , false); 
    tm.rcGradVel = getTimeC() - tm.rcGradVel;  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (gradVel)*/
    if(opt->fNode)
    {
      tm.interCellNode = getTimeC() - tm.interCellNode;
      interCellNode(m                   , loadsVel
                , mesh->elm.cellFace , mesh->face.owner
               , mesh->node.gradVel , mesh->elm.gradVel    
               , mesh->elm.node     , mesh->elm.geomType            
               , mesh->elm.geom.cc  , mesh->node.x  
               , mesh->face.xm        
               , mesh->elm.nen      , mesh->elm.adj.nViz
               , mesh->elm.faceRvel , &pMesh->iNo           
               , mesh->numelNov     , mesh->numel        
               , mesh->nnodeNov     , mesh->nnode 
               , mesh->maxNo        , mesh->maxViz   
               , ndfVel             , mesh->ndm
               , mesh->ndm          , 2);
      tm.interCellNode = getTimeC() - tm.interCellNode;
    }
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (vel)*/
  if(opt->fNode && opt->vel)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                  , loadsVel
               , mesh->elm.cellFace, mesh->face.owner
               , mesh->node.vel    , mesh->elm.vel        
               , mesh->elm.node    , mesh->elm.geomType            
               , mesh->elm.geom.cc , mesh->node.x  
               , mesh->face.xm   
               , mesh->elm.nen     , mesh->elm.adj.nViz
               , mesh->elm.faceRvel, &pMesh->iNo          
               , mesh->numelNov    , mesh->numel        
               , mesh->nnodeNov    , mesh->nnode 
               , mesh->maxNo       , mesh->maxViz   
               , mesh->ndm         , 1
               , mesh->ndm         , 2);

    boundaryNode(m                     , loadsVel  
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.vel        , mesh->elm.vel    
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRvel             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , ndfVel                , 1  
               , mesh->ndm       );
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (density)*/
  if(opt->fNode && opt->densityFluid)
    interCellNode(m             ,  loadsRhoFluid 
             , mesh->elm.cellFace , mesh->face.owner
             , mesh->node.rhoFluid, mesh->elm.densityFluid
             , mesh->elm.node     , mesh->elm.geomType
             , mesh->elm.geom.cc  , mesh->node.x
             , mesh->face.xm      
             , mesh->elm.nen      , mesh->elm.adj.nViz
             , dum                , &pMesh->iNo         
             , mesh->numelNov     , mesh->numel
             , mesh->nnodeNov     , mesh->nnode
             , mesh->maxNo        , mesh->maxViz
             , 3                  , 1
             , mesh->ndm          , 2);  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (dViscosity)*/
  if(opt->fNode && opt->dViscosity)
    interCellNode(m                  , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nDvisc               , mesh->elm.dViscosity
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (sHeat)*/
  if(opt->fNode && opt->specificHeat)
  {
    getColFromMatrix(cell        ,mesh->elm.specificHeat
                    ,mesh->numel ,SHEAT_LEVEL
                    ,TIME_N);

    interCellNode(m                    , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nSheat               , cell
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (sHeat)*/
  if(opt->fNode && opt->tConductivity)
    interCellNode(m                    , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nTCond               , mesh->elm.tConductivity
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
/*...................................................................*/

/*... medias*/
  if (media->fMedia)
  {
/*... medias das velociade*/
    if(media->fVel)
    {
      tm.interCellNode = getTimeC() - tm.interCellNode;
      interCellNode(m                   , loadsVel
                   , mesh->elm.cellFace , mesh->face.owner
                   , nMedVel            , media->mVel        
                   , mesh->elm.node     , mesh->elm.geomType            
                   , mesh->elm.geom.cc  , mesh->node.x  
                   , mesh->face.xm        
                   , mesh->elm.nen      , mesh->elm.adj.nViz
                   , mesh->elm.faceRvel , &pMesh->iNo           
                   , mesh->numelNov     , mesh->numel        
                   , mesh->nnodeNov     , mesh->nnode 
                   , mesh->maxNo        , mesh->maxViz   
                   , mesh->ndm          , 1
                   , mesh->ndm          , 2);
      tm.interCellNode = getTimeC() - tm.interCellNode;
    }
/*...................................................................*/
  }
/*...................................................................*/

/*...*/
  if(mesh->ndfFt)
  {
/*... reconstruindo do gradiente (gradRho)*/
    if (opt->gradRho)
    {
      getColFromMatrix(cell        ,mesh->elm.densityFluid
                      ,mesh->numel ,DENSITY_LEVEL
                      ,TIME_N);
      rcGradU(m                     , loadsRhoFluid
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
           , mesh->elm.material.type
           , mesh->elm.mat          , NULL
           , mesh->elm.leastSquare  , mesh->elm.leastSquareR
           , mesh->elm.faceRrho  
           , cell                   , mesh->elm.gradRhoFluid
           , mesh->node.rhoFluid    
           , NULL                   , NULL
           , 0
           , &sc->rcGrad
           , mesh->maxNo            , mesh->maxViz
           , 1                      , mesh->ndm       
           , &pMesh->iNo            , &pMesh->iEl 
           , mesh->numelNov         , mesh->numel
           , mesh->nnodeNov         , mesh->nnode
           , true); 

/*.................................................................. */

/*... interpolacao das variaveis da celulas para pos nos (GradRho)*/
      if(opt->fNode && opt->gradRho)
      {
        interCellNode(m              , loadsRhoFluid  
              , mesh->elm.cellFace   , mesh->face.owner
              , nGradRho             , mesh->elm.gradRhoFluid 
              , mesh->elm.node       , mesh->elm.geomType
              , mesh->elm.geom.cc    , mesh->node.x
              , mesh->face.xm          
              , mesh->elm.nen        , mesh->elm.adj.nViz
              , mesh->elm.faceRrho   , &pMesh->iNo            
              , mesh->numelNov       , mesh->numel
              , mesh->nnodeNov       , mesh->nnode
              , mesh->maxNo          , mesh->maxViz
              , mesh->ndm            , 1
              , mesh->ndm            , 2);  
      }
/*...................................................................*/
    }
/*...................................................................*/

/*... reconstruindo do gradiente (Energia)*/
    if(opt->gradEnergy)
      rcGradU(m                     , loadsTemp
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
           , mesh->elm.material.type
           , mesh->elm.mat          , mesh->elm.tConductivity
           , mesh->elm.leastSquare  , mesh->elm.leastSquareR
           , mesh->elm.faceRenergy  
           , mesh->elm.temp         , mesh->elm.gradTemp  
           , mesh->node.temp        
           , NULL                   , NULL
           , 0
           , &sc->rcGrad
           , mesh->maxNo            , mesh->maxViz
           , 1                      , mesh->ndm
           , &pMesh->iNo            , &pMesh->iEl
           , mesh->numelNov         , mesh->numel
           , mesh->nnodeNov         , mesh->nnode
           , false); 
/*.................................................................. */

/*... interpolacao das variaveis da celulas para pos nos (GradEnergy)*/
    if(opt->fNode && opt->gradEnergy)
      interCellNode(m                  , loadsTemp  
              , mesh->elm.cellFace   , mesh->face.owner
              , mesh->node.gradTemp  , mesh->elm.gradTemp  
              , mesh->elm.node       , mesh->elm.geomType
              , mesh->elm.geom.cc    , mesh->node.x
              , mesh->face.xm          
              , mesh->elm.nen        , mesh->elm.adj.nViz
              , mesh->elm.faceRenergy, &pMesh->iNo            
              , mesh->numelNov       , mesh->numel
              , mesh->nnodeNov       , mesh->nnode
              , mesh->maxNo          , mesh->maxViz
              , mesh->ndm            , 1
              , mesh->ndm            , 2);  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (energy)*/
    if(opt->fNode && opt->temp)
    {
     interCellNode(m                , loadsTemp
             , mesh->elm.cellFace   , mesh->face.owner
             , mesh->node.temp      , mesh->elm.temp
             , mesh->elm.node       , mesh->elm.geomType
             , mesh->elm.geom.cc    , mesh->node.x
             , mesh->face.xm          
             , mesh->elm.nen        , mesh->elm.adj.nViz
             , mesh->elm.faceRenergy, &pMesh->iNo            
             , mesh->numelNov       , mesh->numel
             , mesh->nnodeNov       , mesh->nnode
             , mesh->maxNo          , mesh->maxViz
             , 1                    , 1
             , mesh->ndm            , 2); 

      boundaryNode(m                   , loadsTemp   
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.temp       , mesh->elm.temp 
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRenergy             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , 1                     , 1  
               , mesh->ndm       );
    }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (density)*/
    if(opt->fNode && opt->densityFluid)
      interCellNode(m             ,  loadsRhoFluid 
             , mesh->elm.cellFace , mesh->face.owner
             , mesh->node.rhoFluid, mesh->elm.densityFluid
             , mesh->elm.node     , mesh->elm.geomType
             , mesh->elm.geom.cc  , mesh->node.x
             , mesh->face.xm      
             , mesh->elm.nen      , mesh->elm.adj.nViz
             , dum                , &pMesh->iNo         
             , mesh->numelNov     , mesh->numel
             , mesh->nnodeNov     , mesh->nnode
             , mesh->maxNo        , mesh->maxViz
             , 3                  , 1
             , mesh->ndm          , 2);  
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (dViscosity)*/
    if(opt->fNode && opt->dViscosity)
      interCellNode(m                  , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nDvisc               , mesh->elm.dViscosity
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (sHeat)*/
    if(opt->fNode && opt->specificHeat)
    {
      getColFromMatrix(cell        ,mesh->elm.specificHeat
                      ,mesh->numel ,SHEAT_LEVEL
                      ,TIME_N);

      interCellNode(m                    , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nSheat               , cell
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
    }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (sHeat)*/
    if(opt->fNode && opt->tConductivity)
      interCellNode(m                    , loadsTemp
                , mesh->elm.cellFace   , mesh->face.owner
                , nTCond               , mesh->elm.tConductivity
                , mesh->elm.node       , mesh->elm.geomType
                , mesh->elm.geom.cc    , mesh->node.x
                , mesh->face.xm          
                , mesh->elm.nen        , mesh->elm.adj.nViz
                , dum                  , &pMesh->iNo            
                , mesh->numelNov       , mesh->numel
                , mesh->nnodeNov       , mesh->nnode
                , mesh->maxNo          , mesh->maxViz
                , 1                    , 1
                , mesh->ndm            , 2);
/*...................................................................*/
  }
/*...................................................................*/

/*...*/
  if (turbModel->fTurb) 
  {
/*... viscisidade turbulenta*/
    if(opt->eddyViscosity)       
      interCellNode(m                  , loadsVel
                   , mesh->elm.cellFace, mesh->face.owner
                   , nEddyV            , mesh->elm.eddyViscosity        
                   , mesh->elm.node    , mesh->elm.geomType            
                   , mesh->elm.geom.cc , mesh->node.x  
                   , mesh->face.xm   
                   , mesh->elm.nen     , mesh->elm.adj.nViz
                   , dum               , &pMesh->iNo          
                   , mesh->numelNov    , mesh->numel        
                   , mesh->nnodeNov    , mesh->nnode 
                   , mesh->maxNo       , mesh->maxViz   
                   , 1                 , 1
                   , mesh->ndm         , 2);
/*...................................................................*/

/*... tensor residual (modelos estruturais)*/
    if(opt->stressR)
      interCellNode(m                     , loadsVel
                , mesh->elm.cellFace      , mesh->face.owner
                , nStressR                , mesh->elm.stressR              
                , mesh->elm.node          , mesh->elm.geomType            
                , mesh->elm.geom.cc       , mesh->node.x  
                , mesh->face.xm             
                , mesh->elm.nen           , mesh->elm.adj.nViz
                , dum                     , &pMesh->iNo                
                , mesh->numelNov          , mesh->numel        
                , mesh->nnodeNov          , mesh->nnode 
                , mesh->maxNo             , mesh->maxViz   
                , 6                       , 1
                , mesh->ndm               , 2);
/*...................................................................*/

/*... coeficiente dinamico*/
    if(opt->cDynamic)
      interCellNode(m                      , loadsVel
                , mesh->elm.cellFace       , mesh->face.owner
                , nCdyn                    , mesh->elm.cd              
                , mesh->elm.node           , mesh->elm.geomType            
                , mesh->elm.geom.cc        , mesh->node.x  
                , mesh->face.xm              
                , mesh->elm.nen            , mesh->elm.adj.nViz
                , dum                      , &pMesh->iNo                 
                , mesh->numelNov           , mesh->numel        
                , mesh->nnodeNov           , mesh->nnode 
                , mesh->maxNo              , mesh->maxViz   
                , 2                        , 1
                , mesh->ndm                , 2);
/*...................................................................*/

/*... parametros de parede*/
    if(opt->wallParameters)
      interCellNode(m                     , loadsVel
                , mesh->elm.cellFace      , mesh->face.owner
                , nWall                   , mesh->elm.wallParameters              
                , mesh->elm.node          , mesh->elm.geomType            
                , mesh->elm.geom.cc       , mesh->node.x  
                , mesh->face.xm             
                , mesh->elm.nen           , mesh->elm.adj.nViz
                , dum                     , &pMesh->iNo                
                , mesh->numelNov          , mesh->numel        
                , mesh->nnodeNov          , mesh->nnode 
                , mesh->maxNo             , mesh->maxViz   
                , 4                       , 1
                , mesh->ndm               , 2);
/*...................................................................*/

/*... energia cinetica turbulenta*/
    if(opt->kTurb)       
      interCellNode(m              , loadsVel
            , mesh->elm.cellFace   , mesh->face.owner
            , nKturb               , mesh->elm.kTurb                
            , mesh->elm.node       , mesh->elm.geomType            
            , mesh->elm.geom.cc    , mesh->node.x  
            , mesh->face.xm      
            , mesh->elm.nen        , mesh->elm.adj.nViz
            , mesh->elm.faceReKturb, &pMesh->iNo          
            , mesh->numelNov       , mesh->numel        
            , mesh->nnodeNov       , mesh->nnode 
            , mesh->maxNo          , mesh->maxViz   
            , 1                    , 1
            , mesh->ndm            , 2);
/*...................................................................*/

  }                                 
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (wk)*/
  if(opt->wk && opt->fNode)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                     , loadsComb
                , mesh->elm.cellFace    , mesh->face.owner
                , nWk                   , mesh->elm.wk       
                , mesh->elm.node        , mesh->elm.geomType            
                , mesh->elm.geom.cc     , mesh->node.x  
                , mesh->face.xm           
                , mesh->elm.nen         , mesh->elm.adj.nViz
                , mesh->elm.faceResZcomb, &pMesh->iNo           
                , mesh->numelNov        , mesh->numel        
                , mesh->nnodeNov        , mesh->nnode 
                , mesh->maxNo           , mesh->maxViz   
                , 1                     , 1            
                , mesh->ndm             , 2);
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (wt)*/
  if(opt-> rateHeatComb && opt->fNode)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                     , loadsComb
                , mesh->elm.cellFace    , mesh->face.owner
                , nRaHeReComb           , mesh->elm.rateHeatReComb
                , mesh->elm.node        , mesh->elm.geomType            
                , mesh->elm.geom.cc     , mesh->node.x  
                , mesh->face.xm           
                , mesh->elm.nen         , mesh->elm.adj.nViz
                , mesh->elm.faceResZcomb, &pMesh->iNo           
                , mesh->numelNov        , mesh->numel        
                , mesh->nnodeNov        , mesh->nnode 
                , mesh->maxNo           , mesh->maxViz   
                , 1                     , 1            
                , mesh->ndm             , 2);
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*...*/
  if(opt->yFrac && opt->fNode)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                     , loadsComb
                , mesh->elm.cellFace    , mesh->face.owner
                , nYfrac                , mesh->elm.yFrac
                , mesh->elm.node        , mesh->elm.geomType            
                , mesh->elm.geom.cc     , mesh->node.x  
                , mesh->face.xm           
                , mesh->elm.nen         , mesh->elm.adj.nViz
                , mesh->elm.faceResZcomb, &pMesh->iNo           
                , mesh->numelNov        , mesh->numel        
                , mesh->nnodeNov        , mesh->nnode 
                , mesh->maxNo           , mesh->maxViz   
                , nSp                   , 1            
                , mesh->ndm             , 2);
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... calculo da matrix jacobiana gradZcomb
                            | dz1dx1 du1dx2 dz1dx3 |   
                            | dz2dx1 du2dx2 dz2dx3 |   
                            | dz3dx1 dz3dx2 dz3dx3 |
*/      
/*... reconstruindo do gradiente (gradZ)*/
  if(opt->gradZcomb)
  {
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
        , mesh->elm.material.type
        , mesh->elm.mat          , NULL
        , mesh->elm.leastSquare  , mesh->elm.leastSquareR
        , mesh->elm.faceResZcomb 
        , mesh->elm.zComb        , mesh->elm.gradZcomb
        , mesh->node.zComb       
        , NULL                   , NULL
        , 0
        , &sc->rcGrad
        , mesh->maxNo            , mesh->maxViz
        , ndfZ                   , mesh->ndm              
        , &pMesh->iNo            , &pMesh->iEl
        , mesh->numelNov         , mesh->numel
        , mesh->nnodeNov         , mesh->nnode
        , false);      
    tm.rcGradComb = getTimeC() - tm.rcGradComb;
/*.................................................................. */  

/*... interpolacao das variaveis da celulas para pos nos (gradZComb)*/
    if(opt->fNode)
    {
      tm.interCellNode = getTimeC() - tm.interCellNode;
      interCellNode(m                  , loadsComb
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.gradZcomb  , mesh->elm.gradZcomb  
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm           
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceResZcomb, &pMesh->iNo           
               , mesh->numelNov        , mesh->numel        
               , mesh->nnodeNov        , mesh->nnode 
               , mesh->maxNo           , mesh->maxViz   
               , ndfZ                  , mesh->ndm
               , mesh->ndm             , 2);
      tm.interCellNode = getTimeC() - tm.interCellNode;
    }
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (zComb)*/
  if(opt->fNode && opt->zComb)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                    , loadsComb
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.zComb      , mesh->elm.zComb  
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm           
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceResZcomb, &pMesh->iNo           
               , mesh->numelNov        , mesh->numel        
               , mesh->nnodeNov        , mesh->nnode 
               , mesh->maxNo           , mesh->maxViz   
               , ndfZ                  , 1
               , mesh->ndm             , 2);

      boundaryNode(m                   , loadsComb  
               , mesh->elm.cellFace    , mesh->face.owner
               , mesh->node.zComb      , mesh->elm.zComb   
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm         , mesh->elm.geom.xmcc 
               , mesh->elm.densityFluid, mesh->elm.gradRhoFluid
               , propF->densityRef 
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , mesh->elm.faceRenergy             
               , mesh->numelNov        , mesh->numel      
               , mesh->nnodeNov        , mesh->nnode   
               , mesh->maxNo           , mesh->maxViz
               , ndfZ                  , 1  
               , mesh->ndm       );
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (gradZComb)*/
  if(opt->fNode && opt->coefDiffSp)
  {
    tm.interCellNode = getTimeC() - tm.interCellNode;
    interCellNode(m                    , loadsComb
               , mesh->elm.cellFace    , mesh->face.owner
               , nDiffY                , mesh->elm.cDiffComb  
               , mesh->elm.node        , mesh->elm.geomType            
               , mesh->elm.geom.cc     , mesh->node.x  
               , mesh->face.xm           
               , mesh->elm.nen         , mesh->elm.adj.nViz
               , NULL                  , &pMesh->iNo           
               , mesh->numelNov        , mesh->numel        
               , mesh->nnodeNov        , mesh->nnode 
               , mesh->maxNo           , mesh->maxViz   
               , nSp                   , 1               
               , mesh->ndm             , 2);
    tm.interCellNode = getTimeC() - tm.interCellNode;
  }
/*...................................................................*/

/*... globalizacao das variaveis*/
  if(opt->fCell)
      globalCombCel(m
                   ,mesh0,mesh
                   ,pMesh,opt
                   ,nSp  ,ndfVel
                   ,ndm  ,ndfZ);
/*...................................................................*/

/*...*/
  if(!mpiVar.myId )
  {
    fName(preName,sc->ddt.timeStep,0,30,nameOut);
/*...*/
    wResVtkCombustion(m                   , cModel                 
               , mesh0->node.x            , mesh0->elm.geom.cc       
               , mesh0->elm.node          , mesh0->elm.mat    
               , mesh0->elm.nen           , mesh0->elm.geomType
               , mesh0->elm.pressure      , mesh0->node.pressure
               , mesh0->elm.gradPres      , mesh0->node.gradPres  
               , mesh0->elm.vel           , mesh0->node.vel      
               , mesh0->elm.gradVel       , mesh0->node.gradVel 
               , mesh0->elm.temp          , mesh0->node.temp   
               , mesh0->elm.gradTemp      , mesh0->node.gradTemp
               , mesh0->elm.zComb         , mesh0->node.zComb
               , mesh0->elm.gradZcomb     , mesh0->node.gradZcomb
               , mesh0->elm.eddyViscosity , nEddyV
               , mesh0->elm.densityFluid  , mesh0->node.rhoFluid
               , mesh0->elm.dViscosity    , nDvisc
               , mesh0->elm.stressR       , nStressR
               , mesh0->elm.cd            , nCdyn
               , mesh0->elm.wallParameters, nWall
               , mesh0->elm.kTurb         , nKturb
               , mesh0->elm.wk            , nWk      
               , mesh0->elm.yFrac         , nYfrac
               , mesh0->elm.gradY         , nGradY 
               , mesh0->elm.rateHeatReComb, nRaHeReComb
               , media->mVel              , nMedVel 
               , mesh0->elm.enthalpyk     , nEnthalpyK
               , mesh0->elm.specificHeat  , nSheat
               , mesh0->elm.tConductivity , nTCond
               , mesh0->elm.cDiffComb     , nDiffY  
               , mesh0->elm.gradRhoFluid  , nGradRho            
               , mesh0->elm.tReactor
               , mesh0->nnode             , mesh0->numel  
               , mesh0->ndm               , mesh0->maxNo 
               , mesh0->numat             , ndfVel
               , mesh0->ntn               
               , nameOut                  , opt
               , eModel->fKelvin          , media
               , sc->ddt                  , fileOut);   
/*...................................................................*/
  }
/*...................................................................*/

/*... desalocando memoria*/
  if (media->fMedia) 
  {
    HccaDealloc(m, nMedP2Vel, "nMedP2Vel", _AD_);
    HccaDealloc(m, nP2Vel   , "nP2Vel"   , _AD_);
    HccaDealloc(m, nMedVel  , "nMediaVel", _AD_);
  }

/*...*/
  if(opt->rateHeatComb)
    HccaDealloc( m, nRaHeReComb, "nRaHeComb",_AD_);
/*...................................................................*/

/*...*/
  if(opt->wk)
    HccaDealloc( m, nWk, "nWk", _AD_);
/*...................................................................*/

/*...*/  
  if(opt->yFrac)
    HccaDealloc(m, nYfrac   , "nYfrac"   , _AD_);
/*...................................................................*/
  if(opt->kTurb)
    HccaDealloc(m, nKturb   , "nKturb"   , _AD_);
  if(opt->wallParameters)
    HccaDealloc(m, nWall    , "nWall"    , _AD_);
  if(opt->cDynamic)
    HccaDealloc(m, nCdyn    , "nCdyn"    , _AD_); 
  if(opt->stressR)
    HccaDealloc(m, nStressR , "nStressR" , _AD_); 
  if(opt->coefDiffSp)
    HccaDealloc(m, nDiffY   , "cDiffZ"   , _AD_);
  if(opt->eddyViscosity)
    HccaDealloc(m, nEddyV   , "nEddyV"   , _AD_); 
  if(opt->dViscosity)
    HccaDealloc(m, nDvisc   , "nVis"     , _AD_);   
  if(opt->tConductivity)
    HccaDealloc(m, nTCond, "nTcond", _AD_);
  if(opt->specificHeat)
    HccaDealloc(m, nSheat, "nSheat", _AD_);  
  if(opt->gradRho)
  {
    HccaDealloc(m, nGradRho, "nGR", _AD_);
  }
  HccaDealloc(m, cell    , "AuxCell", _AD_);
/*...................................................................*/ 

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 02/06/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * printTrans:impressao da equacao de de transporte                  *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * m       -> vetor de memoria principal                             *
 * pMesh     -> modelo de turbulencia                                *
 * sc        -> modelo da equacao de energia                         *
 * loadsT1   -> deficicao de cargas                                  *
 * opt       -> opcoes de arquivo                                    *
 * mesh0     -> malha global                                         *
 * mesh      -> malha particionada                                   *
 * preName   -> prefixo do arquivo                                   *
 * nameOut   -> arquivo de saida                                     *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
*********************************************************************/
void printTrans(Memoria *m
              , PartMesh *pMesh, Scheme *sc
              , Loads *loadsT1 , FileOpt *opt
              , Mesh *mesh0    , Mesh *mesh
              , char *preName  , char *nameOut)
{
  void *dum = NULL;
  char i,str[10][100],*ps[10];
  FILE *fileOut = NULL;
  DOUBLE *nDen = NULL, *nCoefDiff = NULL, *nVel = NULL;

/*...*/
  for(i=0;i<10;ps[i] = str[i],i++);
/*....................................................................*/

/*...*/
  HccaAlloc(DOUBLE,m,nDen     ,mesh->nnode*DENSITY_LEVEL,"nDen",_AD_);
  HccaAlloc(DOUBLE,m,nCoefDiff,mesh->nnode          ,"nCeofDiff",_AD_);
  HccaAlloc(DOUBLE, m,nVel    , mesh->nnode*mesh->ndm, "nV", _AD_);
/*...................................................................*/

/*... reconstruindo do gradiente*/
  tm.rcGradT1 = getTimeC() - tm.rcGradT1;
  rcGradU(m                     , loadsT1
         , mesh->elm.node       , mesh->elm.adj.nelcon
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
         , mesh->elm.material.type
         , mesh->elm.mat          , NULL
         , mesh->elm.leastSquare  , mesh->elm.leastSquareR
         , mesh->elm.faceRt1      
         , mesh->elm.uT1          , mesh->elm.gradUt1
         , mesh->node.uT1         
         , NULL                   , NULL
         , 0
         , &sc->rcGrad
         , mesh->maxNo            , mesh->maxViz
         , mesh->ndfT[0]          , mesh->ndm
         , &pMesh->iNo            , &pMesh->iEl
         , mesh->numelNov         , mesh->numel
         , mesh->nnodeNov         , mesh->nnode
         , false);
  tm.rcGradT1 = getTimeC() - tm.rcGradT1;
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (Grad)*/
  interCellNode(m                 , loadsT1
              , mesh->elm.cellFace, mesh->face.owner
              , mesh->node.gradUt1, mesh->elm.gradUt1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , mesh->elm.faceRt1 , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , mesh->ndm         , 1
              , mesh->ndm         , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (uT1)*/
  interCellNode(m                 , loadsT1
              , mesh->elm.cellFace, mesh->face.owner
              , mesh->node.uT1    , mesh->elm.uT1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , mesh->elm.faceRt1 , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , mesh->ndfT[0]     , 1
              , mesh->ndm         , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (density)*/
  interCellNode(m                 , loadsT1
              , mesh->elm.cellFace, mesh->face.owner
              , nDen              , mesh->elm.densityUt1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , dum               , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , 3                 , 1
              , mesh->ndm         , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (coefDif)*/
  interCellNode(m                 , loadsD1
              , mesh->elm.cellFace, mesh->face.owner
              , nCoefDiff         , mesh->elm.cDiffT1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , dum               , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , 1                 , 1
              , mesh->ndm         , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (vel)*/
  interCellNode(m                  , loadsT1
               , mesh->elm.cellFace, mesh->face.owner
               , nVel              , mesh->elm.vel
               , mesh->elm.node    , mesh->elm.geomType
               , mesh->elm.geom.cc , mesh->node.x
               , mesh->face.xm
               , mesh->elm.nen     , mesh->elm.adj.nViz
               , dum               , &pMesh->iNo
               , mesh->numelNov    , mesh->numel
               , mesh->nnodeNov    , mesh->nnode
               , mesh->maxNo       , mesh->maxViz
               , mesh->ndm         , 1
               , mesh->ndm         , 2);
/*...................................................................*/

/*... globalizacao das variaveis*/
/*... uT1(Node)*/
  dGlobalNode(m              , pMesh
            , mesh0->node.uT1, mesh->node.uT1
            , mesh->ndfT[0]  , 1);
  /*... vel(Node)*/
  dGlobalNode(m              , pMesh
            , mesh0->node.vel, mesh->node.vel
            , mesh->ndm, 1);
/*... gradUt1(Node)*/
  dGlobalNode(m                  , pMesh
            , mesh0->node.gradUt1, mesh->node.gradUt1
            , mesh->ndm          , 1);
/*... uT1(Cel)*/
  dGlobalCel(m             , pMesh
           , mesh0->elm.uT1, mesh->elm.uT1
           , mesh->numelNov
           , mesh->ndfT[0] , 1);
/*... gradUt1(Cel)*/
  dGlobalCel(m                 , pMesh
           , mesh0->elm.gradUt1, mesh->elm.gradUt1
           , mesh->numelNov
           , mesh->ndm         , 1);
/*...................................................................*/

/*...*/
  if (!mpiVar.myId)
  {
    fName(preName, sc->ddt.timeStep, 0, 20, nameOut);

    strcpy(str[0], "elT1");
    strcpy(str[1], "noT1");
    strcpy(str[2], "elGradT1");
    strcpy(str[3], "noGradT1");
    strcpy(str[4], "elVel");
    strcpy(str[5], "nVel");
    strcpy(str[6], "eDensityT1");
    strcpy(str[7], "nDensityT1");
    strcpy(str[8], "nCoefDiffT1");
    strcpy(str[9], "nCoefDiffT1");
/*...*/
    wResVtkTrans(m           , mesh0->node.x
      , mesh0->elm.node      , mesh0->elm.mat
      , mesh0->elm.nen       , mesh0->elm.geomType
      , mesh0->elm.uT1       , mesh0->node.uT1
      , mesh0->elm.gradUt1   , mesh0->node.gradUt1
      , mesh0->elm.vel       , nVel
      , mesh0->elm.densityUt1, nDen
      , mesh0->elm.cDiffT1   , nCoefDiff
      , mesh0->nnode         , mesh0->numel
      , mesh0->ndm           , mesh0->maxNo
      , mesh0->numat         , mesh0->ndfT[0]
      , ps    
      , nameOut              , opt
      , &sc->ddt             , fileOut);
/*...................................................................*/
  }
/*...................................................................*/

/*... desalocando memoria*/
  HccaDealloc(m, nVel     , "nV"       , _AD_);
  HccaDealloc(m, nCoefDiff, "nCeofDiff", _AD_);
  HccaDealloc(m, nDen     , "nDen"     , _AD_);
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 01/05/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * printDiff: impressao da equacao de diff                           *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * m       -> vetor de memoria principal                             *
 * pMesh     -> modelo de turbulencia                                *
 * sc        -> modelo da equacao de energia                         *
 * loadsD1   -> deficicao de cargas velocidade                       *
 * opt       -> opcoes de arquivo                                    *
 * mesh0     -> malha global                                         *
 * mesh      -> malha particionada                                   *
 * preName   -> prefixo do arquivo                                   *
 * nameOut   -> arquivo de saida                                     *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void printDiff(Memoria *m
             , PartMesh *pMesh      , Scheme *sc
             , Loads *loadsD1       , FileOpt *opt
             , Mesh *mesh0          , Mesh *mesh
             , char *preName        , char *nameOut)
{
  void *dum = NULL;
  char str1[100], str2[100], str3[100], str4[100];
  FILE *fileOut = NULL;
  DOUBLE *nDenD = NULL, *nCoefDiffD = NULL;

/*...*/
  HccaAlloc(DOUBLE,m, nDenD,mesh->nnode * DENSITY_LEVEL,"nDenD", _AD_);
  HccaAlloc(DOUBLE,m,nCoefDiffD,mesh->nnode, "nCeofDiff", _AD_);
  /*...................................................................*/

/*... reconstruindo do gradiente*/
  tm.rcGradD1 = getTimeC() - tm.rcGradD1;
  rcGradU(m                      , loadsD1
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
        , mesh->elm.material.type
        , mesh->elm.mat          , NULL
        , mesh->elm.leastSquare  , mesh->elm.leastSquareR
        , mesh->elm.faceRd1      
        , mesh->elm.uD1          , mesh->elm.gradUd1
        , mesh->node.uD1         
        , NULL                   , NULL
        , 0
        , &sc->rcGrad
        , mesh->maxNo            , mesh->maxViz
        , mesh->ndfD[0]          , mesh->ndm
        , &pMesh->iNo            , &pMesh->iEl
        , mesh->numelNov         , mesh->numel
        , mesh->nnodeNov         , mesh->nnode
        , false);
  tm.rcGradD1 = getTimeC() - tm.rcGradD1;
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (Grad)*/
  interCellNode(m                 , loadsD1
               , mesh->elm.cellFace, mesh->face.owner
               , mesh->node.gradUd1, mesh->elm.gradUd1
               , mesh->elm.node    , mesh->elm.geomType
               , mesh->elm.geom.cc , mesh->node.x
               , mesh->face.xm
               , mesh->elm.nen     , mesh->elm.adj.nViz
               , mesh->elm.faceRd1 , &pMesh->iNo
               , mesh->numelNov    , mesh->numel
               , mesh->nnodeNov    , mesh->nnode
               , mesh->maxNo       , mesh->maxViz
               , mesh->ndm         , 1
               , mesh->ndm        , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (uD1)*/
  interCellNode(m                 , loadsD1
              , mesh->elm.cellFace, mesh->face.owner
              , mesh->node.uD1    , mesh->elm.uD1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , mesh->elm.faceRd1 , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , mesh->ndfD[0]     , 1
              , mesh->ndm         , 2);
/*...................................................................*/


/*... interpolacao das variaveis da celulas para pos nos (density)*/
  interCellNode(m                 , loadsD1
              , mesh->elm.cellFace, mesh->face.owner
              , nDenD             , mesh->elm.densityUd1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , dum               , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , 3                 , 1
              , mesh->ndm         , 2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (coefDif)*/
  interCellNode(m                 , loadsD1
              , mesh->elm.cellFace, mesh->face.owner
              , nCoefDiffD        , mesh->elm.cDiffD1
              , mesh->elm.node    , mesh->elm.geomType
              , mesh->elm.geom.cc , mesh->node.x
              , mesh->face.xm
              , mesh->elm.nen     , mesh->elm.adj.nViz
              , dum               , &pMesh->iNo
              , mesh->numelNov    , mesh->numel
              , mesh->nnodeNov    , mesh->nnode
              , mesh->maxNo       , mesh->maxViz
              , 1                 , 1
              , mesh->ndm         , 2);
/*...................................................................*/

/*... globalizacao das variaveis*/
/*... uD1(Node)*/
  dGlobalNode(m              , pMesh
            , mesh0->node.uD1, mesh->node.uD1
            , mesh->ndfD[0]  , 1);

/*... gradUd1(Node)*/
  dGlobalNode(m                   , pMesh
             , mesh0->node.gradUd1, mesh->node.gradUd1
             , mesh->ndm          , 1);
/*... uD1(Cel)*/
  dGlobalCel(m             , pMesh
           , mesh0->elm.uD1, mesh->elm.uD1
           , mesh->numelNov
           , mesh->ndfD[0] , 1);
/*... gradUd1(Cel)*/
  dGlobalCel(m                 , pMesh
           , mesh0->elm.gradUd1, mesh->elm.gradUd1
           , mesh->numelNov
           , mesh->ndm         , 1);
/*...................................................................*/

/*...*/
  if (!mpiVar.myId)
  {
    fName(preName, sc->ddt.timeStep, 0, 8, nameOut);

    strcpy(str1, "elD1");
    strcpy(str2, "noD1");
    strcpy(str3, "elGradD1");
    strcpy(str4, "noGradD1");
/*...*/
    wResVtkDif(m                    , mesh0->node.x
             , mesh0->elm.node      , mesh0->elm.mat
             , mesh0->elm.nen       , mesh0->elm.geomType
             , mesh0->elm.uD1       , mesh0->node.uD1
             , mesh0->elm.gradUd1   , mesh0->node.gradUd1
             , mesh0->elm.densityUd1, nDenD
             , mesh0->elm.cDiffD1   , nCoefDiffD
             , mesh0->nnode         , mesh0->numel
             , mesh0->ndm           , mesh0->maxNo
             , mesh0->numat         , mesh0->ndfD[0]
             , str1                 , str2
             , str3                 , str4
             , nameOut              , opt
             , &sc->ddt             , fileOut);
/*...................................................................*/
  }
/*...................................................................*/

/*... desalocando memoria*/
  HccaDealloc(m, nCoefDiffD, "nCeofDiff", _AD_);
  HccaDealloc(m, nDenD     , "nDenD"    , _AD_);
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 01/05/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * reScaleMesh : redimensio as coordenada da matriz                  *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * m       -> vetor de memoria principal                             *
 * pMesh     -> modelo de turbulencia                                *
 * sc        -> modelo da equacao de energia                         *
 * loadsD1   -> deficicao de cargas velocidade                       *
 * opt       -> opcoes de arquivo                                    *
 * mesh0     -> malha global                                         *
 * mesh      -> malha particionada                                   *
 * preName   -> prefixo do arquivo                                   *
 * nameOut   -> arquivo de saida                                     *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void reScaleMesh(DOUBLE *x,INT nnode, short ndm, FILE *fileIn)
{
  char word[WORD_SIZE], fNomeOut[MAX_STR_LEN_SUFIXO];
  short j;
  int i;
  DOUBLE sC[MAX_NDM];

/*... lendo os valores de escala*/  
  if(ndm == 2)
    fscanf(fileIn,"%lf %lf", &sC[0],&sC[1]);
  else if (ndm == 3)
    fscanf(fileIn, "%lf %lf %lf", &sC[0], &sC[1], &sC[2]);
  readMacro(fileIn, word, false);
/*...................................................................*/

/*... lendo o nome do arquivo de saida*/
  strcpy(fNomeOut,word);
/*...................................................................*/

/*...*/
  for(i=0;i<nnode;i++)
  {
    for (j = 0; j < ndm; j++) 
    {
      MAT2D(i,j,x,ndm) *= sC[j];
    }
  }
/*...................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 01/05/2018                                   *
 * Data de modificaco : 15/09/2019                                   *
 *-------------------------------------------------------------------*
 * reScaleMesh : redimensio as coordenada da matriz                  *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * m       -> vetor de memoria principal                             *
 * pMesh     -> modelo de turbulencia                                *
 * sc        -> modelo da equacao de energia                         *
 * loadsD1   -> deficicao de cargas velocidade                       *
 * opt       -> opcoes de arquivo                                    *
 * mesh0     -> malha global                                         *
 * mesh      -> malha particionada                                   *
 * preName   -> prefixo do arquivo                                   *
 * nameOut   -> arquivo de saida                                     *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initPrintVtk(FileOpt *opt)
{
  opt->bVtk          = false;    
  opt->fCell         = false;
  opt->fNode         = false;
  opt->gradPres      = false;
  opt->gradVel       = false;
  opt->gradEnergy    = false;
  opt->gradTemp      = false;
  opt->graduD1       = false;
  opt->graduT1       = false;
  opt->gradZcomb     = false;
  opt->uD1           = false;
  opt->uT1           = false;
  opt->vel           = false;
  opt->pres          = false;
  opt->presTotal     = false;
  opt->energy        = false;
  opt->temp          = false;
  opt->eddyViscosity = false;
  opt->densityFluid  = false;
  opt->specificHeat  = false;
  opt->dViscosity    = false;
  opt->tConductivity = false;
  opt->densityD1     = false;
  opt->coefDiffD1    = false;
  opt->densityT1     = false;
  opt->coefDiffT1    = false;
  opt->coefDiffSp    = false;
  opt->vorticity     = false;
  opt->wallParameters= false;
  opt->stress        = false;
  opt->kinetic       = false;
  opt->stressR       = false;
  opt->cDynamic      = false;
  opt->Qcriterion    = false;
  opt->kTurb         = false;
  opt->zComb         = false;
  opt->wk            = false;
  opt->yFrac         = false;
  opt->rateHeatComb  = false;
  opt->enthalpyk     = false;
  opt->gradY         = false;
  opt->tReactor      = false;
  opt->gradRho       = false;
  opt->bconditions   = false;
  opt->cc            = false;
  opt->pKelvin       = false;
  opt->stepPlot[0] = 5;
  opt->stepPlot[1] = opt->stepPlot[0];

}
/*********************************************************************
* Data de criacao    : 20/07/2018                                   *
* Data de modificaco : 05/05/2019                                   *
*-------------------------------------------------------------------*
* reScaleMesh : redimensio as coordenada da matriz                  *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* m       -> vetor de memoria principal                             *
* mesh      -> malha global                                         *
* preName   -> prefixo do arquivo                                   *
* fComb     -> modelo de combustao                                  *
* bVtk      -> arquivo vtk binario                                  *
* fileOut  -> ponteiro do arquivo de saida                          *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
*-------------------------------------------------------------------*
* OBS:                                                              *
*-------------------------------------------------------------------*
*********************************************************************/
void printFace(Memoria *m   , Mesh *mesh
             , char* preName, bool fComb 
             , bool bVtk
             , FILE *fileOut)
{

  char nameOut[SIZEMAX],aux[MAX_STR_LEN_SUFIXO];

/*...*/
  if (mesh->ndfF > 0)
  {
    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_vel");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m        , mesh->node.x
               , mesh->elm.node    , mesh->elm.nen
               , mesh->elm.geomType, mesh->elm.faceRvel
               , mesh->nnode       , mesh->numel
               , mesh->ndm         , mesh->maxViz
               , mesh->ndfF - 1    , mesh->maxNo
               , nameOut           , bVtk
               , fileOut);

    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_pres");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m                  , mesh->node.x
               , mesh->elm.node     , mesh->elm.nen
               , mesh->elm.geomType , mesh->elm.faceRpres
               , mesh->nnode        , mesh->numel
               , mesh->ndm          , mesh->maxViz
               , 1                  , mesh->maxNo
               , nameOut            , bVtk
               , fileOut);
  }
/*..................................................................*/

/*...*/
  if(mesh->ndfFt > 0)
  {
    aux[0] = '\0';
    strcpy(aux,preName);
    strcat(aux,"_energy");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m                   , mesh->node.x
               , mesh->elm.node      , mesh->elm.nen
              , mesh->elm.geomType   , mesh->elm.faceRenergy
              , mesh->nnode          , mesh->numel
              , mesh->ndm            , mesh->maxViz
              , 1                    , mesh->maxNo          
              , nameOut              , bVtk                 
              , fileOut);
    
    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_vel");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m                    , mesh->node.x
               , mesh->elm.node       , mesh->elm.nen
               , mesh->elm.geomType   , mesh->elm.faceRvel  
               , mesh->nnode          , mesh->numel
               , mesh->ndm            , mesh->maxViz
               , mesh->ndfFt-1        , mesh->maxNo
               , nameOut              , bVtk
               , fileOut);

    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_pres");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m                  , mesh->node.x
               , mesh->elm.node     , mesh->elm.nen
               , mesh->elm.geomType , mesh->elm.faceRpres
               , mesh->nnode        , mesh->numel
               , mesh->ndm          , mesh->maxViz
               , 1                  , mesh->maxNo
               , nameOut            , bVtk
               , fileOut);
  }
/*..................................................................*/

/*...*/
  if (mesh->ndfD[0] > 0)
  {
    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_D1");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m         , mesh->node.x
      , mesh->elm.node     , mesh->elm.nen
      , mesh->elm.geomType , mesh->elm.faceRd1   
      , mesh->nnode        , mesh->numel
      , mesh->ndm          , mesh->maxViz
      , 1                  , mesh->maxNo
      , nameOut            , bVtk
      , fileOut);
  }
/*..................................................................*/

/*...*/
  if (mesh->ndfT[0] > 0)
  {
    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_T1");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m                 , mesh->node.x
               , mesh->elm.node    , mesh->elm.nen
               , mesh->elm.geomType, mesh->elm.faceRt1 
               , mesh->nnode       , mesh->numel
               , mesh->ndm         , mesh->maxViz
               , 1                 , mesh->maxNo
               , nameOut           , bVtk
               , fileOut);
  }
/*..................................................................*/

/*...*/
  if (fComb)
  {
    aux[0] = '\0';
    strcpy(aux, preName);
    strcat(aux, "_combustion");
    fName(aux, 0, 0, 17, nameOut);
    wGeoFaceVtk2(m                     , mesh->node.x
               , mesh->elm.node        , mesh->elm.nen
               , mesh->elm.geomType    , mesh->elm.faceResZcomb 
               , mesh->nnode           , mesh->numel
               , mesh->ndm             , mesh->maxViz
               , 1                     , mesh->maxNo
               , nameOut               , bVtk
               , fileOut);
  }
/*..................................................................*/

}
/********************************************************************/
