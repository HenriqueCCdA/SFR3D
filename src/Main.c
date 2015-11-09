/********************* includes padroes ******************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*********************************************************************/

/*********************************************************************/
#include<CellLoop.h>
#include<Coo.h>
#include<Diffusion.h>
#include<File.h>
#include<Memoria.h>
#include<Mesh.h>
#include<WriteVtk.h>
#include<WriteCsv.h>
#include<WriteLog.h>
#include<PartMesh.h>
#include<ParallelMpi.h>
#include<Sisteq.h>
#include<Solv.h>
#include<ReadFile.h>
#include<Reord.h>
#include<Transient.h>
#include<Transport.h>
/*********************************************************************/

/*********************************************************************/
#ifdef _DEBUG_ 
  #include<Debug.h>
#endif
/*********************************************************************/

/*********************************************************************/


int main(int argc,char**argv){

/*...Memoria principal*/  
  Memoria m;
/*... estrutura da dados para a malha*/
  Mesh *mesh=NULL,*mesh0=NULL;
/*... Sistema de equacao*/
  SistEq *sistEqD1=NULL, *sistEqT1=NULL;
/*... solver*/
  Solv *solvD1=NULL,*solvT1=NULL;
  bool fSolvD1 = false, fSolvT1 = false;
/*... reordenacao da malha*/
  Reord  *reordMesh=NULL;

/*... particionamento*/
  PartMesh *pMesh = NULL;

/*...*/
  char loopWord[100][MAX_LINE];
  unsigned short kLoop,jLoop;
  bool flWord=false;

/*... Estrutura de dados*/
  char strIa[MNOMEPONTEIRO],strJa[MNOMEPONTEIRO];
  char strA[MNOMEPONTEIRO],strAd[MNOMEPONTEIRO];

/*... arquivo*/
  char *nameIn=NULL,*nameOut=NULL,*preName=NULL,*auxName=NULL;
  FILE *fileIn=NULL,*fileOut=NULL,*fileLog=NULL;
  char str1[100],str2[100],str3[100],str4[100],str5[100],str6[100];
  FileOpt opt;

/*...*/
  Scheme sc;
/*...................................................................*/
 
/*... loop nas celulas*/
/*Lib lib;*/
  
/* ... macro camandos de leitura*/
  bool macroFlag; 
  char word[WORD_SIZE],str[WORD_SIZE];
  char macro[][WORD_SIZE] = 
  {"mesh"      ,"stop"      ,"config"          /* 0, 1, 2*/
  ,"pgeo"      ,"pcoob"     ,"pcoo"            /* 3, 4, 5*/ 
  ,"presolvD1" ,"presolvT1" ,""                /* 6, 7, 8*/
  ,"solvD1"    ,""          ,"pD1"             /* 9,10,11*/
  ,"nlItD1"    ,"pD1CsvCell","pD1CsvNode"      /*12,13,14*/
  ,"solvT1"    ,""          ,"pT1"             /*15,16,17*/
  ,"nlItT1"    ,"pT1CsvCell","pT1CsvNode"      /*18,19,20*/
  ,"transient" ,"timeUpdate","partd"           /*21,22,23*/
  ,"advection" ,"edp"       ,""     };         /*24,25,26*/
/* ..................................................................*/

/*... Memoria principal(valor padrao - bytes)*/
  nmax = 200000;
/* ..................................................................*/

/*... definicao de variaveis globais*/
  oneDivTree = 1.e0/3.e0;
/* ..................................................................*/

/* ... opcoes de arquivos */                                           
  opt.bVtk       = false;
  opt.fItPlotRes = false;
  opt.fItPlot    = false;
/* ..................................................................*/

/*... Mpi*/
  mpiStart(&argc,argv);
/*...................................................................*/

/*... Time*/
  tm.adjcency          = 0.e0;
  tm.geom              = 0.e0;
  tm.leastSquareMatrix = 0.e0;
  tm.reord             = 0.e0;
/*...*/
  tm.solvD1            = 0.e0;
  tm.numeqD1           = 0.e0;
  tm.dataStructD1      = 0.e0;
  tm.CellPloadD1       = 0.e0;
  tm.CellTransientD1   = 0.e0;
  tm.systFormD1        = 0.e0;
  tm.rcGradD1          = 0.e0;
  tm.solvEdpD1         = 0.e0;
/*...*/
  tm.solvT1            = 0.e0;
  tm.numeqT1           = 0.e0;
  tm.dataStructT1      = 0.e0;
  tm.CellPloadT1       = 0.e0;
  tm.CellTransientT1   = 0.e0;
  tm.systFormT1        = 0.e0;
  tm.rcGradT1          = 0.e0;
  tm.solvEdpT1         = 0.e0;
/*... Blas*/
  tm.matVecOverHeadMpi = 0.e0;
  tm.matVecSparse      = 0.e0;
  tm.dot               = 0.e0;
  tm.dotOverHeadMpi    = 0.e0;
/*... Iterativos  */
  tm.pcg               = 0.e0;
  tm.pbicgstab         = 0.e0;
/*... particionamento*/
  tm.partdMesh         = 0.e0;
  tm.partdMeshCom      = 0.e0;
/*...*/
  tm.overHeadCelMpi    = 0.e0;
  tm.overHeadNodMpi    = 0.e0;
  tm.overHeadNeqMpi    = 0.e0;
  tm.overHeadGCelMpi   = 0.e0;
  tm.overHeadGNodMpi   = 0.e0;
  tm.overHeadTotalMpi  = 0.e0;

/*... precondicionador*/
  tm.precondDiag       = 0.e0;
  tm.total             = getTimeC();
/*...................................................................*/

/*... particionamento da malha*/
  pMesh = (PartMesh*) malloc(sizeof(PartMesh));
  ERRO_MALLOC(pMesh,"pMesh",__LINE__,__FILE__,__func__);
/*...*/
  pMesh->fPartMesh      = false;
  pMesh->fPrintMesh     = false;
  pMesh->fPrintMeshPart = false;
/*...................................................................*/

/*... estrutura de dados para malha*/
  mesh0 = (Mesh*) malloc(sizeof(Mesh));
  ERRO_MALLOC(mesh0,"mesh0",__LINE__,__FILE__,__func__);

/*... tecnica padrao de resconstrucao de gradiente*/
  sc.rcGrad = RCGRADGAUSSN;
/*... D1,T1 */ 
  sc.nlD1.maxIt   = sc.nlT1.maxIt =   100; 
  sc.nlD1.tol     = sc.nlT1.tol   = 1.e-6; 
/*...................................................................*/

/*...*/
  sc.advT1.base = FBASE;
  sc.advT1.iCod = VANLEERFACE;
/*...................................................................*/

/*...*/  
  sc.ddt.flag     = false;
  sc.ddt.dt       = 1.e0;
  sc.ddt.total    = 1.e0;
  sc.ddt.timeStep = 1;
/*...................................................................*/

/*...*/  
  reordMesh = (Reord*) malloc(sizeof(Reord));
  ERRO_MALLOC(reordMesh,"reordMesh",__LINE__,__FILE__,__func__);
  reordMesh->flag = false; 
/* ..................................................................*/
    
/*... abrindo ar quivo de entrada*/ 
  nameIn = (char *) malloc(sizeof(char)*MAX_STR_LEN_IN);
    
  if( argc > 1)
    strcpy(nameIn,argv[1]);
  else{
    if(!mpiVar.myId ) printf("Arquivo de dados:\n");
    if(!mpiVar.myId ) scanf("%s",nameIn);
  }

  fileIn = openFile(nameIn,"r");
/*...................................................................*/

/*... arquivos de saida*/
  preName = (char *) malloc(sizeof(char)*MAX_STR_LEN_SUFIXO);
  if(preName == NULL){
    printf("Erro ponteiro prename\n");
    exit(EXIT_FAILURE);
  }
  
  auxName = (char *) malloc(sizeof(char)*MAX_STR_LEN_SUFIXO);
  if(preName == NULL){
    printf("Erro ponteiro auxName\n");
    exit(EXIT_FAILURE);
  }
  
  nameOut = (char *) malloc(sizeof(char)*(SIZEMAX));
  if(nameOut == NULL){
    printf("Erro ponteiro nameout\n");
    exit(EXIT_FAILURE);
  }
    
  if( argc > 2)
    strcpy(preName,argv[2]);
  else{
    if(!mpiVar.myId ) printf("Prefixo do arquivo de saida:\n");
    if(!mpiVar.myId ) scanf("%s",preName);
  }
/*...................................................................*/
  
/*loop de execucao*/
  macroFlag = true;
  do{
/*... macros na marcro trasient*/
    if(flWord){
      if( jLoop > kLoop) { 
        ERRO_GERAL(__FILE__,__func__,__LINE__,
                   "Numero de comandos na string trasient execedido"); 
      }
      strcpy(word,loopWord[jLoop]);
      jLoop++;
    }
/*... leitura da macro*/
    else 
      readMacro(fileIn,word,false);
/*...................................................................*/

/*===================================================================*
 * macro: mesh - leitura da malha e inicializa das estruturas        *
 * de resolucao do problema                                          * 
 *===================================================================*/
    if((!strcmp(word,macro[0]))){
      if(!mpiVar.myId) printf("%s\n",DIF);
      if(!mpiVar.myId) printf("%s\n",word); 
      if(!mpiVar.myId) printf("%s\n",DIF);
/*...*/
      if(mpiVar.nPrcs>1 && !pMesh->fPartMesh){
        printf("Erro: Falta a macro partd!!\n");
        mpiStop();
        exit(EXIT_FAILURE);
      }
/*...................................................................*/

/*... sistema de memoria*/
      initMem(&m,nmax,false);
/*... leitura da malha*/
      if(!mpiVar.myId)
        readFileFvMesh(&m,mesh0,fileIn);
      mpiWait();
/*...................................................................*/

/*... calcula a vizinhaca do elementos*/
      if(!mpiVar.myId){
        tm.adjcency = getTimeC();
        viz(&m                 ,mesh0->elm.node ,mesh0->elm.adj.nelcon
           ,mesh0->elm.nen     ,mesh0->nnode    ,mesh0->numel     
           ,mesh0->maxNo       ,mesh0->maxViz   ,mesh0->ndm);
        tm.adjcency = getTimeC() - tm.adjcency;
      }
/*...................................................................*/
/*... particionamento da malha*/
      if(pMesh->fPartMesh && mpiVar.nPrcs > 1){
        tm.partdMesh = getTimeC() - tm.partdMesh;
        if(!mpiVar.myId){
          partMesh(&m         
                  ,mesh0->node.x  ,mesh0->elm.node
                  ,mesh0->elm.nen
                  ,mesh0->nnode   ,mesh0->numel
                  ,pMesh   
                  ,mesh0->ndm     ,mesh0->maxNo 
                  ,mpiVar.nPrcs);
/*... */
          if(pMesh->fPrintMesh){
            fName(preName,mpiVar.nPrcs,0,1,&nameOut);
            wPartVtk(&m            
                    ,mesh0->node.x      ,mesh0->elm.node              
                    ,mesh0->elm.nen     ,mesh0->elm.geomType
                    ,pMesh->np          ,pMesh->ep          
                    ,mesh0->nnode       ,mesh0->numel    
                    ,mesh0->ndm               
                    ,mesh0->maxNo       ,mesh0->maxViz
                    ,nameOut            ,opt.bVtk             
                    ,fileOut);
          }
/*...................................................................*/
        }
/*...*/
        mesh = (Mesh*) malloc(sizeof(Mesh));
        ERRO_MALLOC(mesh,"mesh",__LINE__,__FILE__,__func__);
/*...*/
        tm.partdMeshCom = getTimeC() - tm.partdMeshCom;
        comunicateMesh(&m
                      ,mesh0     ,mesh
                      ,pMesh
                      ,loadsD1   ,loadsT1);
        tm.partdMeshCom = getTimeC() - tm.partdMeshCom;
/*...................................................................*/

        if(pMesh->fPrintMeshPart){
          fName(preName,mpiVar.nPrcs,mpiVar.myId,2,&nameOut);
          wMeshPartVtk(&m            
                      ,mesh->node.x      ,mesh->elm.node              
                      ,mesh->elm.nen     ,mesh->elm.geomType
                      ,mesh->nnode       ,mesh->numel    
                      ,mesh->ndm               
                      ,mesh->maxNo       ,mesh->maxViz
                      ,nameOut           ,opt.bVtk             
                      ,fileOut);
          fName(preName,mpiVar.nPrcs,mpiVar.myId,18,&nameOut);
          printMap(*pMesh
                  ,mesh->nnode ,mesh->numel
                  ,mpiVar.myId ,nameOut
                  ,fileOut);
        }
/*...................................................................*/
        tm.partdMesh = getTimeC() - tm.partdMesh;
      }
/*...................................................................*/

/*... sequencial*/
      else{
        mesh = mesh0;
      }
/*...................................................................*/

/*... calculo de propriedades geometricas recorrentes*/
      tm.geom = getTimeC() - tm.geom;
      pGeomForm(mesh->node.x         ,mesh->elm.node
               ,mesh->elm.adj.nelcon ,mesh->elm.nen 
               ,mesh->elm.adj.nViz   ,mesh->elm.geomType
               ,mesh->elm.geom.cc    ,mesh->elm.geom.ksi 
               ,mesh->elm.geom.mksi  ,mesh->elm.geom.eta   
               ,mesh->elm.geom.fArea ,mesh->elm.geom.normal
               ,mesh->elm.geom.volume,mesh->elm.geom.xm   
               ,mesh->elm.geom.xmcc                 
               ,mesh->elm.geom.vSkew ,mesh->elm.geom.mvSkew  
               ,mesh->elm.geom.dcca
               ,mesh->maxNo          ,mesh->maxViz
               ,mesh->ndm            ,mesh->numelNov);
      tm.geom = getTimeC() - tm.geom;
/*...................................................................*/

/*... geom(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.geom.cc  ,mesh->elm.geom.cc
                ,mesh->numelNov 
                ,mesh->ndm           ,1);
/*...................................................................*/
      
/*... reconstrucao de gradiente least square*/
      if(sc.rcGrad ==  RCLSQUARE || sc.rcGrad ==  RCLSQUAREQR){
        if(!mpiVar.myId ){
          printf("%s\n",DIF);
          printf("Least Square ...\n");
        }
/*... wleastSqaure*/
        HccaAlloc(DOUBLE,&m,mesh->elm.leastSquare
                 ,mesh->numelNov*mesh->maxViz*mesh->ndm
                 ,"leastSquare",_AD_);
/*... leastSqaure QR*/
        if(sc.rcGrad ==  RCLSQUAREQR){
          if(mesh->ndm == 2){ 
            HccaAlloc(DOUBLE,&m,mesh->elm.leastSquareR            
                     ,mesh->numelNov*3
                     ,"lSquareR   ",_AD_);
          }
          else if(mesh->ndm == 3){ 
            HccaAlloc(DOUBLE,&m,mesh->elm.leastSquareR            
                     ,mesh->numelNov*6
                     ,"lSquareR   ",_AD_);
          }
        }
/*...*/
        tm.leastSquareMatrix = getTimeC() - tm.leastSquareMatrix;
        rcLeastSquare(mesh->elm.geom.ksi   ,mesh->elm.geom.mksi
                     ,mesh->elm.leastSquare,mesh->elm.leastSquareR
                     ,mesh->elm.adj.nViz       
                     ,mesh->numelNov       ,mesh->maxViz
                     ,sc.rcGrad            ,mesh->ndm);
        tm.leastSquareMatrix = getTimeC() - tm.leastSquareMatrix;
/*...................................................................*/
        if(!mpiVar.myId ){
          printf("Least Square.\n");
          printf("%s\n",DIF);
        }
      }
/*...................................................................*/

/*... qualidade da malha*/
      meshQuality(&mesh->mQuality
                 ,mesh->elm.adj.nViz     ,mesh->elm.geom.volume
                 ,mesh->elm.geom.ksi     ,mesh->elm.geom.normal
                 ,mesh->elm.geom.mvSkew 
                 ,mesh->maxViz           ,mesh->ndm
                 ,mesh->numelNov);
/*... qualidade da malha global*/
      if(mpiVar.nPrcs > 1){
        globalMeshQuality(&mesh->mQuality);
      }  
/*...................................................................*/
         
/*... reodenando as celulas para dimuincao da banda*/
      HccaAlloc(INT,&m,reordMesh->num,mesh->numel,"rNum" ,_AD_);
      printf("%s\n",DIF);
      if(!mpiVar.myId ) printf("Reordenando a malha ...\n");
        tm.reord = getTimeC() - tm.reord;
        reord(&m                ,reordMesh->num,mesh->elm.adj.nelcon
             ,mesh->elm.adj.nViz,mesh->maxViz  
             ,mesh->numel       ,mesh->numelNov
             ,reordMesh->flag   ,mpiVar.nPrcs);
        tm.reord = getTimeC() - tm.reord;
      if(!mpiVar.myId ){
        printf("Malha reordenada.\n");
        printf("%s\n",DIF);
      }
/*...................................................................*/
      
/*...*/
      if(!mpiVar.myId ){
        strcpy(str,"MB");
        memoriaTotal(str);
        usoMemoria(&m,str);
      }
/*...................................................................*/

    }   
/*===================================================================*/

/*===================================================================*
 * macro: stop : finalizacao do programa
 *===================================================================*/
    else if((!strcmp(word,macro[1]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word); 
        printf("%s\n\n",DIF);
      }
      tm.total = getTimeC() - tm.total;
/*... */
      fName(preName,mpiVar.nPrcs,mpiVar.myId,7,&nameOut);
      fileLog = openFile(nameOut,"w");
      writeLog(*mesh     ,sc
              ,solvD1    ,sistEqD1
              ,solvT1    ,sistEqT1
              ,tm       
              ,fSolvD1   ,fSolvT1     
              ,nameIn    ,fileLog);
      fclose(fileLog);
/*...................................................................*/

/*... fechando o arquivo log pcg D1*/
      if(fSolvD1 && solvD1->log && !mpiVar.myId)  
        fclose(solvD1->fileSolv);
/*... fechando o arquivo log pcg T1*/
      if(fSolvT1 && solvT1->log && !mpiVar.myId)  
        fclose(solvT1->fileSolv);
/*... fechando o arquivo log nao linear D1*/      
      if(fSolvD1 && opt.fItPlot && !mpiVar.myId)  
        fclose(opt.fileItPlot[FITPLOTD1]);
/*... fechando o arquivo log nao linear T1*/      
      if(fSolvT1 && opt.fItPlot && !mpiVar.myId)  
        fclose(opt.fileItPlot[FITPLOTT1]);
      finalizeMem(&m,false);
      macroFlag = false;
    }    
/*===================================================================*/

/*===================================================================*
 * macro: config : configuracao basica de excucao
 *===================================================================*/
    else if((!strcmp(word,macro[2]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
      
      config(&opt          ,reordMesh
            ,&sc.rcGrad ,fileIn);
      
      if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: pgeo : escreve a geometria com os carregamentos
 *===================================================================*/
    else if((!strcmp(word,macro[3]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
/*... geometrica completa*/
        fName(preName,0,0,6,&nameOut);
        wGeoVtk(&m                       ,mesh0->node.x   
               ,mesh0->elm.node          ,mesh0->elm.mat    
               ,mesh0->elm.nen           ,mesh0->elm.geomType
               ,mesh0->elm.material.prop ,mesh0->elm.material.type 
               ,mesh0->elm.faceRd1       ,mesh0->elm.faceLoadD1
               ,mesh0->elm.faceRt1       ,mesh0->elm.faceLoadT1
               ,mesh0->nnode             ,mesh0->numel    
               ,mesh0->ndm               
               ,mesh0->maxNo             ,mesh0->maxViz
               ,mesh0->numat             
               ,mesh0->ndfD              ,mesh0->ndfT 
               ,nameOut                  ,opt.bVtk             
               ,fileOut);  
/*... face com cargas*/
       fName(preName,0,0,17,&nameOut);
       wGeoFaceVtk(&m                    ,mesh0->node.x        
             ,mesh0->elm.node          ,mesh0->elm.nen      
             ,mesh0->elm.geomType
             ,mesh0->elm.faceRd1       ,mesh0->elm.faceLoadD1
             ,mesh0->elm.faceRt1       ,mesh0->elm.faceLoadT1
             ,mesh0->nnode             ,mesh0->numel    
             ,mesh0->ndm               
             ,mesh0->ndfD[0]           ,mesh0->ndfT[0]
             ,mesh0->maxViz            ,mesh0->maxNo
             ,nameOut                  ,opt.bVtk             
             ,fileOut);  
      }
    }   
/*===================================================================*/
   
/*===================================================================*
 * macro: pcoob : escreve a matriz de coeficientes no formato COO
 *===================================================================*/
    else if((!strcmp(word,macro[4]))){
      if(mpiVar.nPrcs == 1){
        printf("%s\n",DIF);
        printf("%s\n",word);
       
        fName(preName,0,0,13,&nameOut);
/*...*/
        writeCoo(&m,sistEqD1->ia,sistEqD1->ja,sistEqD1->neq
                ,sistEqD1->au   ,sistEqD1->ad,sistEqD1->al        
                ,sistEqD1->nad  ,sistEqD1->storage
                ,sistEqD1->unsym,true
                ,nameOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: pcoo : escreve a matriz de coeficientes no formato COO
 *===================================================================*/
    else if((!strcmp(word,macro[5]))){
      if( mpiVar.nPrcs == 1 ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      
        fName(preName,0,0,12,&nameOut);
/*... matriz A*/
        writeCoo(&m,sistEqD1->ia,sistEqD1->ja,sistEqD1->neq
                ,sistEqD1->au   ,sistEqD1->ad,sistEqD1->al        
                ,sistEqD1->nad  ,sistEqD1->storage
                ,sistEqD1->unsym,false 
                ,nameOut);
/*...................................................................*/

/*... vetor de forcas b*/      
        fName(preName,0,0,14,&nameOut);
        for(int i=0;i<sistEqD1->neq;i++)
          sistEqD1->b[i] /= sistEqD1->ad[i];

        writeCooB(sistEqD1->b,sistEqD1->neq,nameOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/


/*===================================================================*
 * macro: presolvd1 : problema de difusao pura
 *===================================================================*/
    else if((!strcmp(word,macro[6]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
      

/*... inicializando a estrutura de equacoes do problema*/
      solvD1 = (Solv*) malloc(sizeof(Solv));
      if(solvD1 == NULL){
        printf("Erro ponteiro solvD1\n");
        exit(EXIT_FAILURE);
      }
      fSolvD1          = true;
      solvD1->solver   = PCG;
      solvD1->tol      = smachn();
      solvD1->maxIt    = 50000;    
      solvD1->fileSolv = NULL;
      solvD1->log      = true;
      solvD1->flag     = true;
/*...................................................................*/

/*...*/
      if(solvD1->log && !mpiVar.myId){  
        strcpy(auxName,preName);
        strcat(auxName,"_pcg_D1");
        fName(auxName,mpiVar.nPrcs,0,11,&nameOut);
        solvD1->fileSolv = openFile(nameOut,"w");
      }
/*...................................................................*/

/*...*/
      if(opt.fItPlot && !mpiVar.myId){  
        strcpy(auxName,preName);
        strcat(auxName,"_D1");
        fName(auxName,mpiVar.nPrcs,0,10,&nameOut);
        opt.fileItPlot[FITPLOTD1] = openFile(nameOut,"w");
        fprintf(opt.fileItPlot[FITPLOTD1]
               ,"#D1\n#it ||b||/||b0|| ||b||\n");
      }
/*...................................................................*/

/*... inicializa a estrutura do solver*/
      sistEqD1 = (SistEq*) malloc(sizeof(SistEq));
      if(sistEqD1 == NULL){
        printf("Erro ponteiro sistEqD1\n");
        exit(EXIT_FAILURE);
      }
      sistEqD1->storage = CSRD;
//    sistEqD1->storage = ELLPACK;
      sistEqD1->unsym   = false; 
/*...................................................................*/

/*... config*/
      readMacro(fileIn,word,false);
      if(!strcmp(word,"config:")){
/*... solver*/        
        readMacro(fileIn,word,false);
        setSolver(word,&solvD1->solver); 
        
/*... DataStruct*/    
        readMacro(fileIn,word,false);
        setDataStruct(word,&sistEqD1->storage); 

/*... simetria*/        
        readMacro(fileIn,word,false);
        if(!strcmp(word,"sym"))
          sistEqD1->unsym   = false;    
        else if(!strcmp("unSym",word))
          sistEqD1->unsym   = true;    
        
/*... */        
        fscanf(fileIn,"%u" ,&solvD1->maxIt);
        fscanf(fileIn,"%lf",&solvD1->tol);

        if( solvD1->tol == 0.e0) 
          solvD1->tol = smachn();

        if(!mpiVar.myId ) printf("MaxIt     : %d\n",solvD1->maxIt);
        if(!mpiVar.myId ) printf("Tol       : %e\n",solvD1->tol);
      
        if(solvD1->solver == PCG && !mpiVar.myId ){     
          printf("Solver    : PCG\n");
        }

        if(sistEqD1->storage == CSRD && !mpiVar.myId ){     
          printf("DataStruct: CSRD\n");
        }
        else if(sistEqD1->storage == ELLPACK && !mpiVar.myId ){     
          printf("DataStruct: ELLPACK\n");
        }
        
        else if(sistEqD1->storage == CSRDCOO && !mpiVar.myId ){     
          printf("DataStruct: CSRDCOO\n");
        }
        
        if(sistEqD1->unsym && !mpiVar.myId ){
          printf("Matrix    : unsymetric\n");
        } 
        else if (!mpiVar.myId){
          printf("Matrix    : symetric\n");
        } 
      }
/*...................................................................*/

/*... numeracao das equacoes*/
      HccaAlloc(INT,&m,sistEqD1->id
               ,mesh->numel*mesh->ndfD[0]
               ,"sistD1id",_AD_);
      if(!mpiVar.myId){
        printf("%s\n",DIF);
        printf("Numerando as equacoes.\n");
      }
      tm.numeqD1 = getTimeC() - tm.numeqD1;
      sistEqD1->neq = numeq(sistEqD1->id       ,reordMesh->num
                           ,mesh->elm.faceRd1  ,mesh->elm.adj.nViz
                           ,mesh->numel        ,mesh->maxViz
                           ,mesh->ndfD[0]);
      tm.numeqD1 = getTimeC() - tm.numeqD1;
      if(!mpiVar.myId){
        printf("Equacoes numeradas.\n");
        printf("%s\n",DIF);
      }
/*...................................................................*/

/*...*/
      if( mpiVar.nPrcs > 1) {      
        tm.numeqD1 = getTimeC() - tm.numeqD1;
        sistEqD1->neqNov = countEq(reordMesh->num
                            ,mesh->elm.faceRd1  ,mesh->elm.adj.nViz
                            ,mesh->numelNov     ,mesh->maxViz
                            ,mesh->ndfD[0]);
        tm.numeqD1 = getTimeC() - tm.numeqD1;
      }
      else{
        sistEqD1->neqNov = sistEqD1->neq;
      }
/*...................................................................*/

/*...*/
      HccaAlloc(DOUBLE                   ,&m        ,sistEqD1->b0
               ,sistEqD1->neq                  ,"sistD1b0",_AD_);
      HccaAlloc(DOUBLE                   ,&m        ,sistEqD1->b 
               ,sistEqD1->neq                 ,"sistD1b ",_AD_);
      HccaAlloc(DOUBLE                   ,&m        ,sistEqD1->x 
               ,sistEqD1->neq            ,"sistD1x ",_AD_);
      zero(sistEqD1->b0,sistEqD1->neq    ,DOUBLEC);
      zero(sistEqD1->b ,sistEqD1->neq    ,DOUBLEC);
      zero(sistEqD1->x ,sistEqD1->neq    ,DOUBLEC);
/*...................................................................*/

/*... Estrutura de Dados*/
      strcpy(strIa,"iaD1");
      strcpy(strJa,"JaD1");
      strcpy(strAd,"aDD1");
      strcpy(strA ,"aD1");
      if(!mpiVar.myId) printf("Montagem da estrura de dados esparsa.\n");
      tm.dataStructD1 = getTimeC() - tm.dataStructD1 ;
      dataStruct(&m,sistEqD1->id   ,reordMesh->num,mesh->elm.adj.nelcon
                ,mesh->elm.adj.nViz,mesh->numelNov,mesh->maxViz
                ,mesh->ndfD[0]     ,strIa         ,strJa
                ,strAd             ,strA          ,sistEqD1);
      tm.dataStructD1 = getTimeC() - tm.dataStructD1 ;
      if(!mpiVar.myId) printf("Estrutuda montada.\n");
/*...................................................................*/


/*... mapa de equacoes para comunicacao*/
      if( mpiVar.nPrcs > 1) {    
        front(&m,pMesh,sistEqD1,mesh->ndfD[0]);  
      } 
/*...................................................................*/

/*... informacao da memoria total usada*/
      if(!mpiVar.myId  ) {
        strcpy(str,"MB");
        memoriaTotal(str);
        usoMemoria(&m,str);
      }
/*...................................................................*/
      if(!mpiVar.myId  ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: presolvt1 : problema de transporte  
 *===================================================================*/
    else if((!strcmp(word,macro[7]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
      

/*... inicializando a estrutura de equacoes do problema*/
      solvT1 = (Solv*) malloc(sizeof(Solv));
      if(solvT1 == NULL){
        printf("Erro ponteiro solvT1\n");
        exit(EXIT_FAILURE);
      }
      fSolvT1          = true;
      solvT1->solver   = PBICGSTAB;
      solvT1->tol      = smachn();
      solvT1->maxIt    = 50000;    
      solvT1->fileSolv = NULL;
      solvT1->log      = true;
      solvT1->flag     = true;
/*...................................................................*/

/*...*/
      if(solvT1->log && !mpiVar.myId){  
        strcpy(auxName,preName);
        strcat(auxName,"_pcg_T1");
        fName(auxName,mpiVar.nPrcs,0,11,&nameOut);
        solvT1->fileSolv = openFile(nameOut,"w");
      }
/*...................................................................*/

/*...*/
      if(opt.fItPlot && !mpiVar.myId){  
        strcpy(auxName,preName);
        strcat(auxName,"_T1");
        fName(auxName,mpiVar.nPrcs,0,10,&nameOut);
        opt.fileItPlot[FITPLOTT1] = openFile(nameOut,"w");
        fprintf(opt.fileItPlot[FITPLOTT1]
               ,"#T1\n#it ||b||/||b0|| ||b||\n");
      }
/*...................................................................*/

/*... inicializa a estrutura do solver*/
      sistEqT1 = (SistEq*) malloc(sizeof(SistEq));
      if(sistEqT1 == NULL){
        printf("Erro ponteiro sistEqT1\n");
        exit(EXIT_FAILURE);
      }
      sistEqT1->storage = CSRD;
      sistEqT1->unsym   = true; 
/*...................................................................*/

/*... config*/
      readMacro(fileIn,word,false);
      if(!strcmp(word,"config:")){
/*... solver*/        
        readMacro(fileIn,word,false);
        setSolver(word,&solvT1->solver); 
        
/*... DataStruct*/    
        readMacro(fileIn,word,false);
        setDataStruct(word,&sistEqT1->storage); 

/*... */        
        fscanf(fileIn,"%u" ,&solvT1->maxIt);
        fscanf(fileIn,"%lf",&solvT1->tol);

        if( solvT1->tol == 0.e0) 
          solvT1->tol = smachn();

        if(!mpiVar.myId ) printf("MaxIt     : %d\n",solvT1->maxIt);
        if(!mpiVar.myId ) printf("Tol       : %e\n",solvT1->tol);
      
        if(solvT1->solver == PBICGSTAB && !mpiVar.myId ){     
          printf("Solver    : PBICGSTAB\n");
        }

        if(sistEqT1->storage == CSRD && !mpiVar.myId ){     
          printf("DataStruct: CSRD\n");
        }
        else if(sistEqT1->storage == ELLPACK && !mpiVar.myId ){     
          printf("DataStruct: ELLPACK\n");
        }
        
        else if(sistEqT1->storage == CSRDCOO && !mpiVar.myId ){     
          printf("DataStruct: CSRDCOO\n");
        }
        
      }
/*...................................................................*/

/*... numeracao das equacoes*/
      HccaAlloc(INT,&m,sistEqT1->id
               ,mesh->numel*mesh->ndfT[0]
               ,"sistT1id",_AD_);
      if(!mpiVar.myId){
        printf("%s\n",DIF);
        printf("Numerando as equacoes.\n");
      }
      tm.numeqT1 = getTimeC() - tm.numeqT1;
      sistEqT1->neq = numeq(sistEqT1->id       ,reordMesh->num
                           ,mesh->elm.faceRt1  ,mesh->elm.adj.nViz
                           ,mesh->numel        ,mesh->maxViz
                           ,mesh->ndfT[0]);
      tm.numeqT1 = getTimeC() - tm.numeqT1;
      if(!mpiVar.myId){
        printf("Equacoes numeradas.\n");
        printf("%s\n",DIF);
      }
/*...................................................................*/

/*...*/
      if( mpiVar.nPrcs > 1) {      
        tm.numeqT1 = getTimeC() - tm.numeqT1;
        sistEqT1->neqNov = countEq(reordMesh->num
                            ,mesh->elm.faceRt1  ,mesh->elm.adj.nViz
                            ,mesh->numelNov     ,mesh->maxViz
                            ,mesh->ndfT[0]);
        tm.numeqT1 = getTimeC() - tm.numeqT1;
      }
      else{
        sistEqT1->neqNov = sistEqT1->neq;
      }
/*...................................................................*/

/*...*/
      HccaAlloc(DOUBLE                   ,&m        ,sistEqT1->b0
               ,sistEqT1->neq                  ,"sistT1b0",_AD_);
      HccaAlloc(DOUBLE                   ,&m        ,sistEqT1->b 
               ,sistEqT1->neq                 ,"sistT1b ",_AD_);
      HccaAlloc(DOUBLE                   ,&m        ,sistEqT1->x 
               ,sistEqT1->neq            ,"sistT1x ",_AD_);
      zero(sistEqT1->b0,sistEqT1->neq    ,DOUBLEC);
      zero(sistEqT1->b ,sistEqT1->neq    ,DOUBLEC);
      zero(sistEqT1->x ,sistEqT1->neq    ,DOUBLEC);
/*...................................................................*/

/*... Estrutura de Dados*/
      strcpy(strIa,"iaT1");
      strcpy(strJa,"jaT1");
      strcpy(strAd,"adT1");
      strcpy(strA ,"aT1");
      if(!mpiVar.myId) printf("Montagem da estrura de dados esparsa.\n");
      tm.dataStructT1 = getTimeC() - tm.dataStructT1 ;
      dataStruct(&m,sistEqT1->id   ,reordMesh->num,mesh->elm.adj.nelcon
                ,mesh->elm.adj.nViz,mesh->numelNov,mesh->maxViz
                ,mesh->ndfT[0]     ,strIa         ,strJa
                ,strAd             ,strA          ,sistEqT1);
      tm.dataStructT1 = getTimeC() - tm.dataStructT1 ;
      if(!mpiVar.myId) printf("Estrutuda montada.\n");
/*...................................................................*/


/*... mapa de equacoes para comunicacao*/
      if( mpiVar.nPrcs > 1) {    
        front(&m,pMesh,sistEqT1,mesh->ndfT[0]);  
      } 
/*...................................................................*/

/*... informacao da memoria total usada*/
      if(!mpiVar.myId  ) {
        strcpy(str,"MB");
        memoriaTotal(str);
        usoMemoria(&m,str);
      }
/*...................................................................*/
      if(!mpiVar.myId  ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: solvd1: problema de difusao pura
 *===================================================================*/
    else if((!strcmp(word,macro[9]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
        printf("%s\n",DIF);
      }
      mpiWait();
      tm.solvEdpD1    = getTimeC() - tm.solvEdpD1;
/*...*/
      if(solvD1 == NULL){
        printf("Estrutara de dados nao montada para o solvD1!!!\n");
        exit(EXIT_FAILURE);
      }
/*...................................................................*/
     
/*...*/
      diffusion(&m         ,loadsD1
               ,mesh0      ,mesh           ,sistEqD1
               ,solvD1     ,sc             ,pMesh
               ,opt        ,preName        ,nameOut
               ,fileOut);
/*...................................................................*/

/*...*/
     tm.solvEdpD1    = getTimeC() - tm.solvEdpD1;
/*...................................................................*/
     if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }
/*===================================================================*/

/*===================================================================*
 * macro: puD1 : escreve os arquivos dos resultados da uD1          
 *===================================================================*/
    else if((!strcmp(word,macro[11]))){
/*... reconstruindo do gradiente*/
      tm.rcGradD1 = getTimeC() - tm.rcGradD1;
      rcGradU(&m                      ,loadsD1
             ,mesh->elm.node          ,mesh->elm.adj.nelcon
             ,mesh->elm.geom.cc       ,mesh->node.x   
             ,mesh->elm.nen           ,mesh->elm.adj.nViz 
             ,mesh->elm.geomType      ,mesh->elm.material.prop 
             ,mesh->elm.mat 
             ,mesh->elm.leastSquare   ,mesh->elm.leastSquareR
             ,mesh->elm.geom.ksi      ,mesh->elm.geom.mksi  
             ,mesh->elm.geom.eta      ,mesh->elm.geom.fArea    
             ,mesh->elm.geom.normal   ,mesh->elm.geom.volume   
             ,mesh->elm.geom.vSkew     
             ,mesh->elm.geom.xm       ,mesh->elm.geom.xmcc    
             ,mesh->elm.geom.dcca
             ,mesh->elm.faceRd1       ,mesh->elm.faceLoadD1    
             ,mesh->elm.uD1           ,mesh->elm.gradUd1                 
             ,mesh->node.uD1          ,sc.rcGrad
             ,mesh->maxNo             ,mesh->maxViz
             ,mesh->ndfD[0]           ,mesh->ndm       
             ,&pMesh->iNo             ,&pMesh->iEl  
             ,mesh->numelNov          ,mesh->numel        
             ,mesh->nnodeNov          ,mesh->nnode);  
      tm.rcGradD1 = getTimeC() - tm.rcGradD1;
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (Grad)*/
      interCellNode(&m                 ,loadsD1
                     ,mesh->node.gradUd1 ,mesh->elm.gradUd1 
                     ,mesh->elm.node     ,mesh->elm.geomType            
                     ,mesh->elm.geom.cc  ,mesh->node.x  
                     ,mesh->elm.geom.xm
                     ,mesh->elm.nen      ,mesh->elm.adj.nViz
                     ,mesh->elm.faceRd1  ,mesh->elm.faceLoadD1  
                     ,&pMesh->iNo          
                     ,mesh->numelNov     ,mesh->numel        
                     ,mesh->nnodeNov     ,mesh->nnode 
                     ,mesh->maxNo        ,mesh->maxViz   
                     ,mesh->ndm          ,1
                     ,mesh->ndm      
                     ,false              ,2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (uD1)*/
      interCellNode(&m               ,loadsD1
                    ,mesh->node.uD1    ,mesh->elm.uD1 
                    ,mesh->elm.node    ,mesh->elm.geomType
                    ,mesh->elm.geom.cc ,mesh->node.x
                    ,mesh->elm.geom.xm                  
                    ,mesh->elm.nen     ,mesh->elm.adj.nViz
                    ,mesh->elm.faceRd1 ,mesh->elm.faceLoadD1   
                    ,&pMesh->iNo          
                    ,mesh->numelNov    ,mesh->numel        
                    ,mesh->nnodeNov    ,mesh->nnode 
                    ,mesh->maxNo       ,mesh->maxViz 
                    ,mesh->ndfD[0]     ,1 
                    ,mesh->ndm
                    ,true              ,2);
/*...................................................................*/

/*... globalizacao das variaveis*/
/*... uD1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.uD1    ,mesh->node.uD1     
                 ,mesh->ndfD[0]      ,1               );
          
/*... gradUd1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.gradUd1,mesh->node.gradUd1     
                 ,mesh->ndm          ,1               );
/*... uD1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.uD1      ,mesh->elm.uD1
                ,mesh->numelNov 
                ,mesh->ndfD[0]      ,1);
/*... gradUd1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.gradUd1  ,mesh->elm.gradUd1
                ,mesh->numelNov 
                ,mesh->ndm           ,1);
/*...................................................................*/

/*...*/
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
        fName(preName,sc.ddt.timeStep,0,8,&nameOut);

        strcpy(str1,"elD1");
        strcpy(str2,"noD1");
        strcpy(str3,"elGradD1");
        strcpy(str4,"noGradD1");
/*...*/
        wResVtkDif(&m                 ,mesh0->node.x      
                  ,mesh0->elm.node    ,mesh0->elm.mat    
                  ,mesh0->elm.nen     ,mesh0->elm.geomType
                  ,mesh0->elm.uD1     ,mesh0->node.uD1 
                  ,mesh0->elm.gradUd1 ,mesh0->node.gradUd1  
                  ,mesh0->nnode       ,mesh0->numel  
                  ,mesh0->ndm         ,mesh0->maxNo 
                  ,mesh0->numat       ,mesh0->ndfD[0]
                  ,str1               ,str2         
                  ,str3               ,str4         
                  ,nameOut            ,opt.bVtk    
                  ,sc.ddt             ,fileOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: nlItD1: configura das iteracoes nao lineares             
 *===================================================================*/
    else if((!strcmp(word,macro[12]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
      fscanf(fileIn,"%d",&sc.nlD1.maxIt);
      fscanf(fileIn,"%lf",&sc.nlD1.tol);
      if(!mpiVar.myId ){
        printf("MaxIt: %d\n",sc.nlD1.maxIt);
        printf("Tol  : %e\n",sc.nlD1.tol);
      }
      readMacro(fileIn,word,false);
/*...................................................................*/
      if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: pD1CellCsv:imprime os resultados no formato csv                  
 *===================================================================*/
    else if((!strcmp(word,macro[13]))){
/*... globalizacao das variaveis*/
/*... uD1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.uD1      ,mesh->elm.uD1
                ,mesh->numelNov 
                ,mesh->ndfD[0]      ,1);
/*... gradUd1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.gradUd1  ,mesh->elm.gradUd1
                ,mesh->numelNov 
                ,mesh->ndm           ,1);
        
/*...*/
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      
/*...*/
        strcpy(auxName,preName);
        strcat(auxName,"_D1_cell_");
        fName(auxName,sc.ddt.timeStep,0,16,&nameOut);
        fileOut = openFile(nameOut,"w");
/*...*/
        writeCsvCell(mesh0->elm.uD1    ,mesh0->elm.gradUd1
                    ,mesh0->elm.geom.cc                  
                    ,mesh0->numel      ,mesh0->ndfD[0]
                    ,mesh0->ndm        ,fileOut);
/*...*/
        fclose(fileOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: pD1CsvNode:imprime os resultados no formato csv                  
 *===================================================================*/
    else if((!strcmp(word,macro[14]))){
/*... globalizacao das variaveis*/
/*... uD1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.uD1    ,mesh->node.uD1     
                 ,mesh->ndfD[0]      ,1               );
          
/*... gradUd1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.gradUd1,mesh->node.gradUd1     
                 ,mesh->ndm          ,1               );

/*...................................................................*/

/*...*/
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
/*...*/
        strcpy(auxName,preName);
        strcat(auxName,"_D1_node_");
        fName(auxName,sc.ddt.timeStep,0,16,&nameOut);
        fileOut = openFile(nameOut,"w");
/*...*/
        writeCsvNode(mesh0->node.uD1    ,mesh0->node.gradUd1
                  ,mesh0->node.x                  
                  ,mesh0->nnode         ,mesh0->ndfD[0]
                  ,mesh0->ndm           ,fileOut);
/*...*/
        fclose(fileOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: solvt1: problema de transporte  
 *===================================================================*/
    else if((!strcmp(word,macro[15]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
        printf("%s\n",DIF);
      }
      mpiWait();
      tm.solvEdpT1    = getTimeC() - tm.solvEdpT1;
/*...*/
      if(solvT1 == NULL){
        printf("Estrutara de dados nao montada para o solvT1!!!\n");
        exit(EXIT_FAILURE);
      }
/*...................................................................*/
     
/*...*/
      transport(&m         ,loadsT1
               ,mesh0      ,mesh           ,sistEqT1
               ,solvT1     ,sc             ,pMesh
               ,opt        ,preName        ,nameOut
               ,fileOut);
/*...................................................................*/

/*...*/
     tm.solvEdpT1    = getTimeC() - tm.solvEdpT1;
/*...................................................................*/
     if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }
/*===================================================================*/

/*===================================================================*
 * macro: puT1 : escreve os arquivos dos resultados da uT1          
 *===================================================================*/
    else if((!strcmp(word,macro[17]))){
/*... reconstruindo do gradiente*/
      tm.rcGradT1 = getTimeC() - tm.rcGradT1;
      rcGradU(&m                      ,loadsT1
             ,mesh->elm.node          ,mesh->elm.adj.nelcon
             ,mesh->elm.geom.cc       ,mesh->node.x   
             ,mesh->elm.nen           ,mesh->elm.adj.nViz 
             ,mesh->elm.geomType      ,mesh->elm.material.prop 
             ,mesh->elm.mat 
             ,mesh->elm.leastSquare   ,mesh->elm.leastSquareR
             ,mesh->elm.geom.ksi      ,mesh->elm.geom.mksi  
             ,mesh->elm.geom.eta      ,mesh->elm.geom.fArea    
             ,mesh->elm.geom.normal   ,mesh->elm.geom.volume   
             ,mesh->elm.geom.vSkew     
             ,mesh->elm.geom.xm       ,mesh->elm.geom.xmcc    
             ,mesh->elm.geom.dcca
             ,mesh->elm.faceRt1       ,mesh->elm.faceLoadT1    
             ,mesh->elm.uT1           ,mesh->elm.gradUt1                 
             ,mesh->node.uT1          ,sc.rcGrad
             ,mesh->maxNo             ,mesh->maxViz
             ,mesh->ndfT[0]           ,mesh->ndm       
             ,&pMesh->iNo             ,&pMesh->iEl  
             ,mesh->numelNov          ,mesh->numel        
             ,mesh->nnodeNov          ,mesh->nnode);  
      tm.rcGradT1 = getTimeC() - tm.rcGradT1;
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (Grad)*/
      interCellNode(&m                   ,loadsT1
                     ,mesh->node.gradUt1 ,mesh->elm.gradUt1 
                     ,mesh->elm.node     ,mesh->elm.geomType            
                     ,mesh->elm.geom.cc  ,mesh->node.x  
                     ,mesh->elm.geom.xm
                     ,mesh->elm.nen      ,mesh->elm.adj.nViz
                     ,mesh->elm.faceRt1  ,mesh->elm.faceLoadT1  
                     ,&pMesh->iNo          
                     ,mesh->numelNov     ,mesh->numel        
                     ,mesh->nnodeNov     ,mesh->nnode 
                     ,mesh->maxNo        ,mesh->maxViz   
                     ,mesh->ndm          ,1
                     ,mesh->ndm      
                     ,false              ,2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (vel)*/
      interCellNode(&m                   ,loadsT1
                   ,mesh->node.vel     ,mesh->elm.vel        
                   ,mesh->elm.node     ,mesh->elm.geomType            
                   ,mesh->elm.geom.cc  ,mesh->node.x  
                   ,mesh->elm.geom.xm
                   ,mesh->elm.nen      ,mesh->elm.adj.nViz
                   ,mesh->elm.faceRt1  ,mesh->elm.faceLoadT1  
                   ,&pMesh->iNo          
                   ,mesh->numelNov     ,mesh->numel        
                   ,mesh->nnodeNov     ,mesh->nnode 
                   ,mesh->maxNo        ,mesh->maxViz   
                   ,mesh->ndm          ,1
                   ,mesh->ndm      
                   ,false              ,2);
/*...................................................................*/

/*... interpolacao das variaveis da celulas para pos nos (uT1)*/
      interCellNode(&m                 ,loadsT1
                    ,mesh->node.uT1    ,mesh->elm.uT1 
                    ,mesh->elm.node    ,mesh->elm.geomType
                    ,mesh->elm.geom.cc ,mesh->node.x
                    ,mesh->elm.geom.xm                  
                    ,mesh->elm.nen     ,mesh->elm.adj.nViz
                    ,mesh->elm.faceRt1 ,mesh->elm.faceLoadT1   
                    ,&pMesh->iNo          
                    ,mesh->numelNov    ,mesh->numel        
                    ,mesh->nnodeNov    ,mesh->nnode 
                    ,mesh->maxNo       ,mesh->maxViz 
                    ,mesh->ndfT[0]     ,1 
                    ,mesh->ndm
                    ,true              ,2);
/*...................................................................*/

/*... globalizacao das variaveis*/
/*... uT1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.uT1    ,mesh->node.uT1     
                 ,mesh->ndfT[0]      ,1               );
          
/*... gradUt1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.gradUt1,mesh->node.gradUt1     
                 ,mesh->ndm          ,1               );
/*... vel(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.vel    ,mesh->node.vel         
                 ,mesh->ndm          ,1               );
/*... uT1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.uT1      ,mesh->elm.uT1
                ,mesh->numelNov 
                ,mesh->ndfT[0]      ,1);
/*... gradUt1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.gradUt1  ,mesh->elm.gradUt1
                ,mesh->numelNov 
                ,mesh->ndm           ,1);
/*... vel(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.vel      ,mesh->elm.vel       
                ,mesh->numelNov 
                ,mesh->ndm           ,1);
/*...................................................................*/

/*...*/
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
        fName(preName,sc.ddt.timeStep,0,20,&nameOut);

        strcpy(str1,"elT1");
        strcpy(str2,"noT1");
        strcpy(str3,"elGradT1");
        strcpy(str4,"noGradT1");
        strcpy(str5,"elVel");
        strcpy(str6,"noVel");
/*...*/
        wResVtkTrans(&m                 ,mesh0->node.x      
                    ,mesh0->elm.node    ,mesh0->elm.mat    
                    ,mesh0->elm.nen     ,mesh0->elm.geomType
                    ,mesh0->elm.uT1     ,mesh0->node.uT1 
                    ,mesh0->elm.gradUt1 ,mesh0->node.gradUt1  
                    ,mesh0->elm.vel     ,mesh0->node.vel      
                    ,mesh0->nnode       ,mesh0->numel  
                    ,mesh0->ndm         ,mesh0->maxNo 
                    ,mesh0->numat       ,mesh0->ndfT[0]
                    ,str1               ,str2         
                    ,str3               ,str4         
                    ,str5               ,str6         
                    ,nameOut            ,opt.bVtk    
                    ,sc.ddt             ,fileOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: nlItT1: configura das iteracoes nao lineares             
 *===================================================================*/
    else if((!strcmp(word,macro[18]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
      fscanf(fileIn,"%d",&sc.nlT1.maxIt);
      fscanf(fileIn,"%lf",&sc.nlT1.tol);
      if(!mpiVar.myId ){
        printf("MaxIt: %d\n",sc.nlT1.maxIt);
        printf("Tol  : %e\n",sc.nlT1.tol);
      }
      readMacro(fileIn,word,false);
/*...................................................................*/
      if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: pT1CellCsv:imprime os resultados no formato csv                  
 *===================================================================*/
    else if((!strcmp(word,macro[19]))){
/*... globalizacao das variaveis*/
/*... uT1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.uT1      ,mesh->elm.uT1
                ,mesh->numelNov 
                ,mesh->ndfT[0]      ,1);
/*... gradUt1(Cel)*/
      dGlobalCel(&m                  ,pMesh
                ,mesh0->elm.gradUt1  ,mesh->elm.gradUt1
                ,mesh->numelNov 
                ,mesh->ndm           ,1);
        
/*...*/
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      
/*...*/
        strcpy(auxName,preName);
        strcat(auxName,"_T1_cell_");
        fName(auxName,sc.ddt.timeStep,0,16,&nameOut);
        fileOut = openFile(nameOut,"w");
/*...*/
        writeCsvCell(mesh0->elm.uT1    ,mesh0->elm.gradUt1
                    ,mesh0->elm.geom.cc                  
                    ,mesh0->numel      ,mesh0->ndfT[0]
                    ,mesh0->ndm        ,fileOut);
/*...*/
        fclose(fileOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: pT1CsvNode:imprime os resultados no formato csv                  
 *===================================================================*/
    else if((!strcmp(word,macro[14]))){
/*... globalizacao das variaveis*/
/*... uT1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.uT1    ,mesh->node.uT1     
                 ,mesh->ndfT[0]      ,1               );
          
/*... gradUd1(Node)*/
      dGlobalNode(&m                 ,pMesh
                 ,mesh0->node.gradUd1,mesh->node.gradUd1     
                 ,mesh->ndm          ,1               );

/*...................................................................*/

/*...*/
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
/*...*/
        strcpy(auxName,preName);
        strcat(auxName,"_T1_node_");
        fName(auxName,sc.ddt.timeStep,0,16,&nameOut);
        fileOut = openFile(nameOut,"w");
/*...*/
        writeCsvNode(mesh0->node.uT1    ,mesh0->node.gradUt1
                  ,mesh0->node.x                  
                  ,mesh0->nnode         ,mesh0->ndfT[0]
                  ,mesh0->ndm           ,fileOut);
/*...*/
        fclose(fileOut);
/*...................................................................*/
        printf("%s\n\n",DIF);
      }
    }   
/*===================================================================*/

/*===================================================================*
 * macro: transient:configuracao da discretizacao temporal                 
 *===================================================================*/
    else if((!strcmp(word,macro[21]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
/*...*/
      sc.ddt.flag = true;
      readMacro(fileIn,word,false);
      if(!strcmp(word,"config:")){
/*... timer*/        
        readMacro(fileIn,word,false);
        setTransientScheme(word,&sc.ddt.type);
/*...*/ 
        fscanf(fileIn,"%lf",&sc.ddt.dt);
        fscanf(fileIn,"%lf",&sc.ddt.total);
/*...*/        
        if(!mpiVar.myId ) printf("dt(s)     : %lf\n",sc.ddt.dt);
        if(!mpiVar.myId ) printf("Total(s)  : %lf\n",sc.ddt.total);
      
        if(sc.ddt.type == EULER && !mpiVar.myId)     
          printf("ddtScheme : EULER\n");
        else if(sc.ddt.type == BACKWARD && !mpiVar.myId )     
          printf("ddtScheme : BACKWARD\n");

        sc.ddt.t        = 0.e0;
        sc.ddt.timeStep =    0;
      }
/*...................................................................*/

/*...*/
      flWord = true;
      kLoop  = 0;
      jLoop  = 0;
      do{
        readMacro(fileIn,word,false);
        strcpy(loopWord[kLoop],word);
        kLoop++;
        if(kLoop > 100){
          ERRO_GERAL(__FILE__,__func__,__LINE__,
                   "Numero de comandos na macro trasient execedido"); 
        }
      }while(strcmp(word,"endTransient"));
/*...................................................................*/
      if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: timeUpdate : macro de atualizaco do tempo                       
 *===================================================================*/
    else if((!strcmp(word,macro[22]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
/*...*/
      jLoop            = 0;
      sc.ddt.t        += sc.ddt.dt;
      sc.ddt.timeStep ++;
/*...................................................................*/
      
/*...*/
      if(sc.ddt.t > sc.ddt.total+ 0.5e0*sc.ddt.dt)
        flWord = false;
/*...................................................................*/
      
/*...*/
      else{
        if(!mpiVar.myId ) printf("t(s) = %lf\n",sc.ddt.t);
        if(!mpiVar.myId ) printf("step = %d\n" ,sc.ddt.timeStep);
      }
/*...................................................................*/
      if(!mpiVar.myId ) printf("%s\n\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: partd particionamento da malha                                   
 *===================================================================*/
    else if((!strcmp(word,macro[23]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
/*...*/
      pMesh->fPartMesh = true;
      readMacro(fileIn,word,false);
      if(!strcmp(word,"config:")){
/*... fPrintMesh*/        
        readMacro(fileIn,word,false);
        if(!strcmp(word,"true")){ 
          pMesh->fPrintMesh = true;
          if(!mpiVar.myId ) printf("fPrintMesh    : true\n");
        }

/*... fPrintMeshPart*/        
        readMacro(fileIn,word,false);
        if(!strcmp(word,"true")){ 
          pMesh->fPrintMeshPart = true;
          if(!mpiVar.myId ) printf("fPrintMeshPart: true\n");
        }
      }
/*...................................................................*/
       if(!mpiVar.myId ) printf("%s\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: advection tecnica aplicada no termo advectivo          
 *===================================================================*/
    else if((!strcmp(word,macro[24]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
/*... base na limataca de fluxo*/        
      readMacro(fileIn,word,false);
/*... limiado por face*/
      if(!strcmp(word,"fBase")){ 
        sc.advT1.base = FBASE;
        if(!mpiVar.myId ) printf("advT1 : fBase\n");
      }

/*... codigo da da funcao limitadora de fluxo*/        
      readMacro(fileIn,word,false);
      setFaceBase(word,&sc.advT1.iCod);
/*...................................................................*/
      if(!mpiVar.myId ) printf("%s\n",DIF);
    }   
/*===================================================================*/

/*===================================================================*
 * macro: edo equacoes diferencias resolvidas                    
 *===================================================================*/
    else if((!strcmp(word,macro[25]))){
      if(!mpiVar.myId ){
        printf("%s\n",DIF);
        printf("%s\n",word);
      }
      readEdo(mesh0,fileIn);
/*...................................................................*/
      if(!mpiVar.myId ) printf("%s\n",DIF);
    }   
/*===================================================================*/
  }while(macroFlag && (!feof(fileIn)));

  mpiStop();
  return EXIT_SUCCESS;
}    
/*********************************************************************/
 
 
