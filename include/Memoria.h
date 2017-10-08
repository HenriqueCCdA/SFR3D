#ifndef _MEM_H_
  #define _MEM_H_
  #include<string.h>
  #include<stdio.h>
  #include<stdlib.h>
  #include<HccaStdBool.h>
  #include<HccaTime.h>
  #ifdef _MSC_VER
	  typedef __int64 iptx;
  #else
	  typedef long iptx; 
  #endif
/*...*/
  #ifdef _AD_
    #undef  _AD_
  #endif  
  #define _AD_ false
/*.....................................................................*/
  #define ALIGN         64
  #define CONV_BYTES    1024.0
  #define NPONTEIRO    500/*Numero de ponteiros maximo allocados*/
  #define MNOMEPONTEIRO 20/*Tamanho maximo do nome do vetor*/
  #define DIF   "***************************************************************"

/*...Tipo de variavel usado no enderessamento de vetores*/
  #define TYPEADRESS void

  iptx nmax;

  typedef struct{
    char *ia;/*arroanjo principal*/
    char nome_ponteiro[NPONTEIRO][MNOMEPONTEIRO+1];/*nome dos ponterios alocados*/
	  unsigned int npont;/*numero de ponteiros alocados*/
    iptx pont[NPONTEIRO][2];/*localizacao do ponteiro no ia*/    
	  TYPEADRESS **end[NPONTEIRO];/*endereco dos ponterios allocados*/
	  iptx iespont;/*localizacao do ultimo ponteiro allocado no ia*/
    double   tempmem;/*tempo total de excucao das funcoes*/
  }Memoria;
/******************************prototipo******************************/
  void   initMem        (Memoria*, iptx,bool );
  void   finalizeMem    (Memoria*,bool );
  void*  alloc          (Memoria*, TYPEADRESS**,int,char*,int,bool);
  void   setNamePoint   (Memoria*,char*,bool);
  int    locateNamePoint(Memoria*,char*,bool);
  void*  locate          (Memoria*,char*,bool);
  void   relloc          (Memoria*,int,bool);
  void   moveVector      (Memoria*,int,int);
  void   cleanNamePoint  (Memoria*,int);
  void*  dalloc          (Memoria*, char *,bool);
  iptx   usoMemoria      (Memoria* ,char*);
  void   mapVector       (Memoria*);
  double memoriaTotal    (char *);
  double memoriaVector   (Memoria*,char*,char*,bool);
  void   vzero(char*,long,char*);
/*********************************************************************/
/*...*/
/*Type - tipo alocado( ex int,double ...)              */
/*   m - vetor de memoria                              */
/*   x - ponterio alocado                              */
/*   n - numero de poiscoes                            */
/*name - nome do vetor                                 */
/*  ad - mode verdose das funcoes de alocacao de meoria*/
  #define HccaAlloc(type,m,x,n,name,ad) x=(type*)alloc(m,(TYPEADRESS**)&x,n,name,sizeof(type),ad);
  #define HccaDealloc(m,x,name,ad) x=dalloc(m,name,ad);
  #define zero(v,n,type) vzero((char*)v,n,type);
/*..................................................................*/
#endif /*_MEM_H_*/
