#ifndef _VTK_
  #define _VTK_
  #include<stdio.h>
  #include<stdlib.h>
  #include<Mystdbool.h>
  #include<Mesh.h>
  void headVtk(char *,bool, FILE *);
  void writeVtkCoor(double *x,long nnode     ,int ndm,bool cod, FILE *f);
  void writeVtkCell(int *el  ,short int *nen , short int *type
                   ,long numel ,short int maxno,bool cod
                   ,FILE *f);
  void writeVtkCellProp(int *iprop,double *dprop,long int numel     
                       ,int gdl       ,char *s      ,bool cod1 
                       ,short int cod2,FILE *f);
  void writeVtkNodeProp(int *iprop,double *dprop,short cod1,short cod2
                       ,int nnode ,short ndf    ,char *s   ,bool cod
                       ,FILE *f);

   

#endif
