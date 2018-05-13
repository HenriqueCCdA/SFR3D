#ifndef _FILE_H_
  #define _FILE_H_
/*...*/
  #include<math.h>
  #include<stdio.h>
  #include<stdlib.h>
/*...*/
  #include<Erro.h>
  #include<HccaStdBool.h>
  #include<string.h>
  #include<Define.h>
/*...*/
  #define MAX_STR_LEN_IN    129 /*tamanho do arquivo de entrada maximo*/
  #define MAX_STR_LEN_SUFIXO 80  /*tamanho do arquivo de entrada maximo*/
  #define SIZEMAX           200 /*tamanho do arquivo de saida maximo*/
  #define MAX_STR_NUMBER     65 /*INT integer*/
  #define MAX_EXT            50 /*numero maximo da extencao*/ 
  #define WORD_SIZE         120
  #define LINE_SIZE         120
  #define MAX_LINE          200

  #define FITPLOTD1           1
  #define FITPLOTD2           2
  #define FITPLOTT1           3
  #define FITPLOTT2           4
  #define FITPLOTTEMP         5
  #define FITPLOTSIMPLE       6
   
/*...................................................................*/  
  typedef struct{
    bool bVtk;           /*escrita do arquivo binario*/
    bool fItPlotRes; 
    bool fItPlot;
    bool fCell;
    bool fNode;        
    bool gradPres;       
    bool gradVel;        
    bool gradEnergy; 
    bool graduD1; 
    bool graduT1;
    bool uD1;
    bool uT1;
    bool vel;            
    bool pres;
    bool presTotal;           
    bool energy;         
    bool eddyViscosity;  
    bool densityFluid;   
    bool specificHeat;   
    bool dViscosity;     
    bool tConductivity; 
    bool densityD1;
    bool coefDiffD1;
    bool vorticity;
    bool wallParameters;
    bool stress; 
    bool kinetic;
    bool stressR;  
    bool cDynamic; 
    bool Qcriterion; 
    bool kTurb; 
    bool bconditions;    /*insere as condicoes de contorno nos valores nodais*/
    bool cc;             /*centro geomentrico da celula*/       
    bool pKelvin;        /*plot em kelvin*/  
    short stepPlot[2];
    FILE *fileItPlot[7];
    
  }FileOpt;

  void  iota(INT,char*);
  FILE* openFile(char *,char *);
  FILE* openFileBuffer(char *,char *,bool);
  void  fName(char*,INT,int,int,char*);
  void  readMacro(FILE*,char*,bool);
  void  clearLine(char *);
  int   rl(FILE *,char *);
  int   getNumProp(FILE *);
/*...*/  
  char macros[MAX_LINE][WORD_SIZE];/*todas as macros lidas no arquivo*/
  int  nmacro;
/*....................................................................*/  
#endif /*_FILE_H*/
