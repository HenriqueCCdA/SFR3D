#ifndef _ADJCENCY_H
  #define _ADJCENCY_H_
/*...*/
  #include<Memoria.h>
  #include<HccaStdBool.h>
  #include<Define.h>
/*...................................................................*/
  void viz(Memoria *m       ,INT *el           ,INT *nelcon
          ,short *nen       ,INT const nnode   ,INT const numel  
          ,short const maxNo,short const maxViz,short const ndm);

/*... malha mista de triangulos e quadrilateros*/
  void adj2d(INT *el          ,INT *nodcon       ,INT *nelcon
            ,short *nen       ,INT const numel   ,INT const nnode 
            ,short const maxNo,short const maxViz,INT *nEdeg);
/*...................................................................*/
  
/*... malha de tetraedros*/
  void adjTetra4(INT *RESTRICT el         ,INT *RESTRICT nodcon
	        ,INT *RESTRICT nelcon
            ,INT const nnode    , INT const numel 
            ,short const maxNo,short const maxViz);

  void tetra4fNod(INT const nEl     ,short const face
             ,INT *RESTRICT el   ,INT *RESTRICT nodeFace
             ,short const maxNo);
       
  short tetra4face(INT const nEl         ,INT *RESTRICT el
                 ,INT *RESTRICT nodeFace ,short const maxNo);
/*...................................................................*/
  
/*... malha de hexaedros*/
  void adjHex8(INT *el         ,INT *nodcon         ,INT *nelcon
            ,INT const nnode    , INT const numel 
            ,short const maxNo,short const maxViz);

  void hexa8fNod(INT const nEl     ,short const face
             ,INT *RESTRICT el   ,INT *RESTRICT nodeFace
             ,short const maxNo);
       
  short hexa8face(INT const nEl         ,INT *RESTRICT el
                 ,INT *RESTRICT nodeFace ,short const maxNo);
/*...................................................................*/


#endif/*_MESH_*/

