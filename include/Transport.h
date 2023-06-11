#ifndef _TRANSPORT_H_
  #define _TRANSPORT_H_

  #include<File.h>
  #include<Memoria.h>
  #include<Mesh.h>
  #include<CellLoop.h>
  #include<Mesh.h>
  #include<WriteVtk.h>
  #include<Sisteq.h>
  #include<Solv.h>
  #include<Transient.h>


void transport(Memoria *m     , Loads *loadsTrans, TransModel *tModel
             , Mesh *mesh0    , Mesh *mesh       , SistEq *sistEqT
             , Solv *solvT    , Scheme *sc       , PartMesh *pMesh
             , PropVarCD *prop, FileOpt *opt     , char *preName
             , char *nameOut  , FILE *fileOut);
#endif/*_TRANSPORT_H_*/
