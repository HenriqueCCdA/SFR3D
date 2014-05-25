#ifndef MEM_H
#define MEM_H
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<Mystdbool.h>
#include<time.h>
#define CONV_BYTES    1024.0
#define NPONTEIRO     50/*Numero de ponteiros maximo allocados*/
#define MNOMEPONTEIRO 20/*Tamanho maximo do nome do vetor*/
#define NMAX          1000000000
#define DIF   "***************************************************************"
typedef struct Memoria{
  char *ia;/*arroanjo principal*/
  char nome_ponteiro[NPONTEIRO][MNOMEPONTEIRO+1];/*nome dos ponterios alocados*/
  long int pont[NPONTEIRO][2];/*lacalizacao do ponteiro no ia*/                  
  unsigned int npont;/*numero de ponteiros alocados*/
  long int **end[NPONTEIRO];/*endereco dos ponteriosallocados*/
  long int iespont;/*localizacao do ultimo paonteiro allocado no ia*/
  time_t tempmem;/*tempo total de excucao das funcoes*/
}Memoria;
/******************************prototipo******************************/
void   init_mem      (Memoria*,bool ); 
void   finalize_mem  (Memoria*,bool );
void*  alloc         (Memoria*,long**,int,char*,int,bool);
void   setnamepont   (Memoria*,char*,bool);
int    locatenamepont(Memoria*,char*,bool);
void*  locate        (Memoria*,char*,bool);
void   relloc        (Memoria*,int,bool);
void   movevector    (Memoria*,int,int);
void   cleannamepont (Memoria*,int);
void*  dalloc        (Memoria*, char *,bool);
long   uso_memoria   (Memoria* ,char*);
void   mapvetor      (Memoria*);
double memoria_total (char *);
double memoriavector (Memoria*,char*,char*,bool);
void   vzero(char*,long,char*);
/*********************************************************************/
/*...*/
/*...Tipo de variavel usado no enderessamento de vetores*/
#define TYPEADRESS long int
/*Type - tipo alocado( ex int,double ...)              */
/*   m - vetor de memoria                              */
/*   x - ponterio alocado                              */
/*   n - numero de poiscoes                            */
/*name - nome do vetor                                 */
/*  ad - mode verdose das funcoes de alocacao de meoria*/
#define Myalloc(type,m,x,n,name,ad) x=(type*)alloc(m,(TYPEADRESS**)&x,n,name,sizeof(type),ad);
#define Mydealloc(m,x,name,ad) x=dalloc(m,name,ad);
#define zero(v,n,type) vzero((char*)v,n,type);
/*..................................................................*/
#endif
