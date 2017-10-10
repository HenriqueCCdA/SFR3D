#include<Memoria.h>
/******************************************************************** 
 * INITMEM : inicia a estrutura basica da memoria                   * 
 * ---------------------------------------------------------------- * 
 * Parametro de entrada:                                            * 
 * ---------------------------------------------------------------- * 
 * m   -> estrutura de dados da memoria                             * 
 * iws -> saida extra de informacoes para o usuario                 * 
 * ---------------------------------------------------------------- * 
 * Parametros de saida :                                            * 
 * ---------------------------------------------------------------- * 
 * m->ia       - iniciado                                           * 
 * m->npont    - iniciado (= 0)                                     * 
 * m->iespont  - iniciado ( = 0)                                    * 
 * m->pont[][] - iniciado ( = 0)                                    * 
 * m->end[]    - iniciado (NULL)                                    * 
 * m->nome_pon - iniciado ( =" ")                                   * 
 * m->tempmem  - tempo  em s                                        * 
 * ---------------------------------------------------------------- * 
 ********************************************************************/
void initMem(Memoria *m, iptx nmax, bool iws)
{
   long int i;
   
// m->tempmem = getTimeC() - m->tempmem  ;
   
   if(iws)
     printf("inicializando a memoria...\n");
 
/* m->ia = (char *) malloc(nmax*sizeof(char));*/
   m->ia = (char *) calloc(nmax,sizeof(char));
   if(!m->ia){
     fprintf(stderr,"Erro memoria Insuficiente:\n"
                   "Memoria solicitada: %ld\n"
		   ,(long)(nmax*sizeof(char))); 
     exit(0);
   }		  
   m->npont   = 0;
   m->iespont = 0;
   for(i = 0; i < NPONTEIRO ; i++){
     m->pont[i][0]    = 0;
     m->pont[i][1]    = 0;
     m->end[i]        = NULL;
     cleanNamePoint(m,i);
   }
   
   if(iws)
     printf("Memoria inicializada.\n");
   
// m->tempmem = getTimeC() - m->tempmem;
     
}
/*********************************************************************/

/********************************************************************* 
 * FINALIZEMEN : finaliza o memoria                                  * 
 * ----------------------------------------------------------------- * 
 * Parametro de entrada:                                             * 
 * ----------------------------------------------------------------- * 
 * ----------------------------------------------------------------- * 
 * Parametros de saida :                                             * 
 * ----------------------------------------------------------------- * 
 *********************************************************************/
void finalizeMem(Memoria *m, bool iws)
{
  if(iws)
    printf("liberando memoria ...\n");
  free(m->ia);
}
/*********************************************************************/

/********************************************************************* 
 * ALLOC    : inicia a estrutura basica da memodira                  * 
 * ----------------------------------------------------------------- * 
 * Parametro de entrada:                                             * 
 * ----------------------------------------------------------------- * 
 * m     -> estrutura de dados da memoria                            * 
 * **end -> endereco do ponteiro allocado                            * 
 * comp  -> numero de posicoes                                       * 
 * *s    -> nome do vetor                                            * 
 * size  -> tamanho em bytes do tipo alocado                         * 
 * iws -> saida extra de informacoes para o usuario                  * 
 * ----------------------------------------------------------------- * 
 * Parametros de saida :                                             * 
 * ----------------------------------------------------------------- * 
 * m->npont    - atualizado                                          * 
 * m->iespont  - atualizado                                          * 
 * m->pont[][] - atualizado                                          * 
 * m->end[]    - atualizado                                          * 
 * m->nome_pon - atualizado                                          * 
 * m->tempmem  - acumulado tempo  em s                               * 
 * ----------------------------------------------------------------- * 
 *********************************************************************/
void* alloc(Memoria *m, TYPEADRESS **end
	         ,int comp,char *s,int size,bool iws)
{
	iptx livre,nec,necA,nv;
  int resto;

/*se o tamanho do vetor for zero transforma em 1 para nao haver
 problemas*/
  if(comp == 0)
    comp = 1;

//m->tempmem = getTimeC() - m->tempmem;

/*numero maximo de ponteiros*/  
  if( m->npont == NPONTEIRO){
    fprintf(stderr,"Numero maximo de ponterios para memoria excedido\n"
                  "mome :\"%s\"\n"
		  "MAX :%d     \n"
		  "solicitado :%d\n"
		  ,s,NPONTEIRO,(int)m->npont);
    return NULL;
  }
/*... espaco q sera alocado*/
  nec = comp * size;

/*... alinhamento como potencia de 64(cache-line)*/
  necA = nec;
#if ALIGN == 64     
  resto = nec%64;
  if(resto) 
    necA = (1 + (int) nec/64) * 64;
#elif ALIGN == 16     
  resto = nec%16;
  if(resto) 
    necA = (1 + (int) nec/16) * 16;
/*... alinhamento como potencia de 8*/
#elif ALIGN ==  8
  resto = nec%8 ;
  if(resto) 
    necA = (1 + (int) nec/8) * 8;
#endif
/*...espaco livre*/
  livre = nmax - m->iespont; 
/*...*/
  if(livre > necA){
/*...Set name pont*/  
    setNamePoint(m,s,iws);
/*...*/    
    m->pont[m->npont][0] = m->iespont; 
    m->pont[m->npont][1] = m->iespont + necA -1 ;
/*...................................................................*/
    nv                   = m->iespont;
    m->iespont           = necA + m->iespont;
/*... guardando endereco do ponteiro*/    
    m->end[m->npont]     = end;
/*...................................................................*/
/*...*/
    m->npont             = m->npont + 1;
/*...................................................................*/
    if(iws)
#ifdef _MSC_VER
     fprintf(stderr,"Memoria allocada %s %lld bytes\n"
              "Ponteiro retornado %p.\n"
	       ,s,necA,m->ia + nv);
#else
      fprintf(stderr,"Memoria allocada %s %ld bytes\n"
              "Ponteiro retornado %p.\n"
	       ,s,necA,m->ia + nv);
#endif

//  m->tempmem = getTimeC() - m->tempmem  ;
    return (m->ia + nv);
  }
/*...................................................................*/  

/*...*/  
  else{
#ifdef _MSC_VER
    fprintf(stderr,"Memoria insuficiente %s\n"
                  "Disponivel %lld bytes\n"
                  "necessario %lld bytes\n"
		              ,s,livre,necA);
#else
    fprintf(stderr,"Memoria insuficiente %s\n"
                  "Disponivel %ld bytes\n"
                  "necessario %ld bytes\n"
		              ,s,livre,necA);
#endif
     exit(EXIT_FAILURE); 
     return NULL;
  } 
/*...................................................................*/
}  
/*********************************************************************/

/********************************************************************* 
 * SETNAMEPOINT: guarda o nome do vetor e testa se ele ja foi alocado* 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m     -> estrutura de dados da memoria                            * 
 * *s    -> nome do vetor                                            * 
 * iws -> saida extra de informacoes para o usuario                  * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * m->nome_pon - atualizado                                          * 
 * ------------------------------------------------------------------* 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void setNamePoint(Memoria *m,char *s,bool iws){
  int i;
  i= (int) strlen(s);
  if(i>MNOMEPONTEIRO){
   fprintf(stderr,"Nome do arranjo \"%s\" maior que %d caracter.\n"
		 ,s,MNOMEPONTEIRO); 
   exit(0); 
  }
  if(locateNamePoint(m,s,iws)<0)
    strcpy(m->nome_ponteiro[m->npont],s);
  else{
   fprintf(stderr,"Ja ha um vetor chamado  \"%s\".\n"
		 ,s); 
   exit(0);
  }
}
/*********************************************************************/

/********************************************************************* 
 * LOCATENAMEPOINT : localiza o vetor pelo nome                      * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m   -  estrutura de dados da memoria                              * 
 * *s  -  nome do vetor                                              * 
 * iws -  saida extra de informacoes para o usuario                  * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * i-  retorna a localizacao                                         * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
int locateNamePoint(Memoria *m,char *s,bool iws){
  int i;

  for(i=0;i<NPONTEIRO;i++){
    if(!strcmp(m->nome_ponteiro[i],s)){
      if(iws)  
        fprintf(stderr,"Vetor \"%s\"  localizado.\n",s);
      return i;
    }  
  }
  if(iws)  
    fprintf(stderr,"Vetor \"%s\" nao localizado.\n",s);
  return -1;
}
/*********************************************************************/

/********************************************************************* 
 * DELLOC : libera memoria e reclacula as posicoes e ponteiros       * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m   -  estrutura de dados da memoria                              * 
 * *s  -  nome do vetor                                              * 
 * iws -  saida extra de informacoes para o usuario                  * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * m->npont    - atualizado                                          * 
 * m->iespont  - atualizado                                          * 
 * m->pont[][] - atualizado                                          * 
 * m->end[]    - atualizado                                          * 
 * m->nome_pon - atualizado                                          * 
 * m->tempmem  - acumulado tempo  em s                               * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void* dalloc(Memoria* m,char *s,bool iws)
{
	iptx nec;
  int  pt;
   
//m->tempmem = getTimeC() - m->tempmem;
/*localizar vetor*/
  if((pt=locateNamePoint(m,s,iws))<0)
    exit(EXIT_FAILURE);
  
  nec = m->pont[pt][1] - m->pont[pt][0] + 1 ;
  if(m->iespont > nec){
      m->iespont  = m->iespont - nec;
/*...*/      
      if(iws)
        mapVector(m);
/*...................................................................*/	
      cleanNamePoint(m,pt);
/*...*/      
      *(m->end[pt]) = NULL;
/*...................................................................*/
      relloc(m,pt,iws);
/*...*/      
      if(iws)
        mapVector(m);
/*...................................................................*/	
      m->npont -= 1;
      if(iws)
#ifdef _MSC_VER
        printf("Memoria liberada %s %lld bytes.\n",s,nec);
#else
        printf("Memoria liberada %s %ld bytes.\n",s,nec);
#endif
      
      return NULL;
    }  
  else{
#ifdef _MSC_VER
    fprintf(stderr,"Erro liberacao da memoria %s %lld bytes\n"
	           "possivel invazao de espaco de outra variavel.\n"
		  ,s,nec);
#else
    fprintf(stderr,"Erro liberacao da memoria %s %ld bytes\n"
	           "possivel invazao de espaco de outra variavel.\n"
		  ,s,nec);
#endif
    exit(EXIT_FAILURE);
  }  
  
//m->tempmem = getTimeC() - m->tempmem;
}
/*********************************************************************/

/********************************************************************* 
 * RELLOC : recalcula os posicoes e ponteiros                        * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m   -  estrutura de dados da memoria                              * 
 * pt  -  posicao do ponterio                                        * 
 * iws -  saida extra de informacoes para o usuario                  * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * m->pont[][] - atualizado                                          * 
 * m->end[]    - atualizado                                          * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void relloc(Memoria *m,int pt,bool iws){
  unsigned int i;
	iptx nec;

  for(i=pt;i<m->npont-1;i++){
    strcpy(m->nome_ponteiro[i],m->nome_ponteiro[i+1]);
    
    if(iws)
      fprintf(stderr,"Move vetor \"%s\".\n",m->nome_ponteiro[i+1]);
    moveVector(m,i,i+1);
    
    nec = m->pont[i+1][1]-m->pont[i+1][0];
    
    if(i == 0)
      m->pont[i][0] = 0;	      
    else 
      m->pont[i][0] = m->pont[i-1][1]+1;	      
    
    m->pont[i][1] = m->pont[i][0]+nec;
/*...Map de vetores ja alocados*/
/*...Endereco do ponteiro*/
    m->end[i] = m->end[i+1];
/*...Ponterio a potando para a novo lugar da memoria*/    
    *(m->end[i]) =(TYPEADRESS*) &m->ia[m->pont[i][0]];
/*...................................................................*/  
  }
  cleanNamePoint(m,i);
  m->pont[i][1] = m->pont[i][0] =0;
}
/*********************************************************************/

/********************************************************************* 
 * MOVEVECTOR : move um vetor na memoria principal                   * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m   -  estrutura de dados da memoria                              * 
 * v1  -  localizaca  do destine(referente ao ia)                    * 
 * v2  -  localizacao da oringem(referente ao ia)                    * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void moveVector(Memoria *m, int v1 , int v2){
	iptx sizev2;
	iptx iniv1,iniv2;
	iptx i,j;
/*... Tamanho do vetores em bytes*/
/*... caso de ser o primeiro vetor*/
  if(v1 == 0)
    iniv1 = 0;
  else  
    iniv1 = m->pont[v1-1][1]+1;
/*...................................................................*/
/**/
  iniv2 = m->pont[v2][0];
  sizev2 = m->pont[v2][1] - m->pont[v2][0] + 1;
/*...................................................................*/

/*fprintf(stderr,"iniv1 %ld\n",iniv1);
  fprintf(stderr,"iniv2 %ld sizev2 %ld\n",iniv2,sizev2);*/

/*...copia byte a byte*/
  for(i = iniv1 , j = 0; i < sizev2 + iniv1 ;j++,i++){
            m->ia[i] = m->ia[iniv2+j];
      m->ia[iniv2+j] = 0;
  }      
}
/*********************************************************************/

/********************************************************************* 
 * locate : localizao um vetor pelo nome e retorna seu ponteiro      * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m   -  estrutura de dados da memoria                              * 
 * name-  nome do vetor                                              * 
 * iws -  saida extra para o usuario                                 * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * ponterio para o vetor                                             * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void* locate(Memoria *m,char *name,bool iws){
	iptx ipont;
  int pt;

/*... lacalizando vetor*/
   if((pt=locateNamePoint(m,name,iws))<0)
      exit(0);
/*...................................................................*/      
   ipont = m->pont[pt][0];
   return &(m->ia[ipont]);

}
/*********************************************************************/

/*===================================================================* 
 * MAPVECTOR : map atual de vetores alocados                         * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m   -  estrutura de dados da memoria                              * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 * informacoes sobre a localizacao no ia, endereco dos ponterios e   * 
 * para onde apontam                                                 * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void mapVector(Memoria *m){
  
  unsigned int i;
  fprintf(stderr,"%s\n",DIF);
  fprintf(stderr,"|none        |posicao incial |posicao final  "
         "|endereco do ponteiros|ponteiro           |\n"  );
  for(i=0;i<m->npont;i++){
#ifdef _MSC_VER
   fprintf(stderr," \"%15s\"|%15lld|%15lld|pp = %16p|p = %16p|\n"
                  ,m->nome_ponteiro[i],m->pont[i][0],m->pont[i][1]
		  ,(void*)m->end[i],(void*)*(m->end[i]));
#else
    fprintf(stderr," \"%15s\"|%15ld|%15ld|pp = %16p|p = %16p|\n"
                  ,m->nome_ponteiro[i],m->pont[i][0],m->pont[i][1]
		  ,(void*)m->end[i],(void*)*(m->end[i]));
#endif
  }
  fprintf(stderr,"%s\n",DIF);
  
}
/*********************************************************************/

/********************************************************************* 
 * CLENAMEPOINT : limpa o nome do vetor                              * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m    - estrutura de dados da memoria                              * 
 * pont - localizacao                                                * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void cleanNamePoint(Memoria *m,int pont){
    strcpy(m->nome_ponteiro[pont]," ");
}
/*********************************************************************/

/********************************************************************* 
 * USOMEMORIA : Uso da memoria do ia                                 * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m    - estrutura de dados da memoria                              * 
 * s    - B BYTES                                                    * 
 *       KB KILOBYTES                                                * 
 *	 MB MEGA                                                         * 
 *	 GB GIGA                                                         * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 *===================================================================*/
iptx usoMemoria(Memoria *m,char *s){
  double conv;
  
  if(!strcmp(s,"B"))
   printf("Total de memoria usada: %20.4lf bytes\n"
         ,(double) m->iespont);
  else if(!strcmp(s,"KB")){
   conv = CONV_BYTES; 
   printf("Total de memoria usada: %20.4lf KB\n",m->iespont/conv);
  }  
  else if(!strcmp(s,"MB")){
   conv = CONV_BYTES*CONV_BYTES;
   printf("Total de memoria usada: %20.4lf MB\n",m->iespont/conv);
  } 
  else if(!strcmp(s,"GB")){
   conv = CONV_BYTES*CONV_BYTES*CONV_BYTES;
   printf("Total de memoria usada: %20.4lf GB\n",m->iespont/conv);
  }
  return m->iespont;
}
/*********************************************************************/

/********************************************************************* 
 * MEMORIATOTAL: Memoria total disponivel                            * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * m    - estrutura de dados da memoria                              * 
 * s    - B BYTES                                                    * 
 *       KB KILOBYTES                                                * 
 *	 MB MEGA                                                         * 
 *	 GB GIGA                                                         * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
double memoriaTotal(char *s)
{
   double conv;
   if(!strcmp(s,"B")){
     printf("Total disponivel : %25.4lf bytes\n",(double) nmax);
     return (double)nmax;
   } 
   else if(!strcmp(s,"KB")){
     conv = CONV_BYTES; 
     printf("Total disponivel : %25.4lf KB\n",nmax/conv);
     return nmax/conv;
   }  
   else if(!strcmp(s,"MB")){
     conv = CONV_BYTES*CONV_BYTES;
     printf("Total disponivel : %25.4lf MB\n",nmax/conv);
     return nmax/conv;
   } 
   else if(!strcmp(s,"GB")){
     conv = CONV_BYTES*CONV_BYTES*CONV_BYTES;
     printf("Total disponivel : %25.4lf GB\n",nmax/conv);
     return nmax/conv;
   }
   return -1.0;
}
/*********************************************************************/

/********************************************************************* 
 * MEMORIAVECTOR : Memoria de um determinado vetor                   * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------*  
 * m    - estrutura de dados da memoria                              *  
 * npont- nome do vetor                                              * 
 * s    - B BYTES                                                    * 
 *       KB KILOBYTES                                                * 
 *	 MB MEGA                                                         * 
 *	 GB GIGA                                                         * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
double memoriaVector(Memoria *m,char* s,char*npont,bool iws){
  
  int tp;
  double np,conv;
  if((tp=locateNamePoint(m,npont,iws)) < 0)
    exit(0);
  np =(double) (m->pont[tp][1]- m->pont[tp][0]) + 1.0;  
  if(!strcmp(s,"B")){
    printf("memoria usada pelo vetor \"%s\" :%20.4lf bytes\n"
                  ,npont,(double) np);
    return (double)np;
   } 
   else if(!strcmp(s,"KB")){
     conv = CONV_BYTES; 
     printf("memoria usada pelo vetor \"%s\": %20.4lf KB\n"
                   ,npont,np/conv);
     return np/conv;
   }  
   else if(!strcmp(s,"MB")){
     conv = CONV_BYTES*CONV_BYTES;
     printf("memoria usada pelo vetor \"%s\": %20.4lf MB\n"
                   ,npont,np/conv);
     return np/conv;
   } 
   else if(!strcmp(s,"GB")){
     conv = CONV_BYTES*CONV_BYTES*CONV_BYTES;
     printf("memoria usada pelo vetor \"%s\": %20.4lf GB\n"
                   ,npont,np/conv);
     return np/conv;
   }
   return -1.0;
}
/*********************************************************************/

/********************************************************************* 
 * VZERO: zera o vetor v                                             * 
 * ------------------------------------------------------------------* 
 * Parametro de entrada:                                             * 
 * ------------------------------------------------------------------* 
 * v    - vetor a ser zerado                                         * 
 * n    - compromento do vetor                                       * 
 * type - tipo de dados do vetor                                     * 
 *       short                                                       * 
 *	 int                                                             * 
 *	 long                                                            * 
 *	 double                                                          * 
 * ------------------------------------------------------------------* 
 * Parametros de saida :                                             * 
 * ------------------------------------------------------------------* 
 *********************************************************************/
void vzero(char *v,long n,char* type){

	iptx i,nby;
 
 /* fprintf(stderr,"endereco=%p tamanho=%ld\n",v,n);*/
  if(!strcmp(type,"int")){
    nby = sizeof(int);
    for(i=0;i<n*nby;i++)
      v[i] = 0;
  }
  else if(!strcmp(type,"char")){
    nby = sizeof(char);
    for(i=0;i<n*nby;i++)
      v[i] = 0;
  }
  else if(!strcmp(type,"bool")){
    nby = sizeof(bool);
    for(i=0;i<n*nby;i++)
      v[i] = 0;
  }
  else if(!strcmp(type,"short")){
    nby = sizeof(short);
    for(i=0;i<n*nby;i++)
      v[i] = 0;
  }
  else if(!strcmp(type,"long")){
    nby = sizeof(long);
    for(i=0;i<n*nby;i++)
      v[i] = 0;
  }
  else if(!strcmp(type,"double")){
    nby = sizeof(double);
    for(i=0;i<n*nby;i++)
      v[i] = 0;
  }
  else{
    printf("\n%s"
           "\nMEMORIA: funcao vzero"
           "\n***Tipo de dados na especificado***"
	   "\ntipo : %s\n"
	   "%s\n",DIF,type,DIF); 
    exit(0);
  }	   
}
/*===================================================================*/
