#include<Properties.h>

/*******************   MISTURA GASOSA  ******************************/
static DOUBLE pol(DOUBLE *RESTRICT a, DOUBLE const x,short const n)
{
  int i; 
  DOUBLE tmp = a[0];

  for(i=1;i<n;i++)
    tmp += a[i]*pow(x,(double) i);

  return tmp;
}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * philn:                                                            *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * parametro omega                                                   *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *        The properies of gase and liquids (5�)                     *
 *        pag 95                                                     *
 * A Simple and Accurate Method for Calculating Viscosity of Gas     *
 * Mixtures                                                          *
 *-------------------------------------------------------------------*
 *********************************************************************/
static DOUBLE philn(DOUBLE const mWi, DOUBLE const nui
                   ,DOUBLE const mWj, DOUBLE const nuj)
{

  DOUBLE tmp1,tmp2,r8;

  r8   = 1.e0/sqrt(8.e0);
  tmp1 = 1.e0/sqrt(1.e0 + mWi/mWj);
  tmp2 = 1.e0 + sqrt(nui/nuj)*pow(mWj/mWi,0.25e0);

  return r8*tmp1*tmp2*tmp2;
}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 15/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * collisionIntegral:                                                *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em kelvin                                         *
 * ek  - e/k do especies desejada (Leornard-Jones parameters)        *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * parametro omega                                                   *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * fonte: Manual do FDS                                              *
 *        livro computional fluid dynamics in fire engineering       *
 *        pag 486                                                    *
 *        The properies of gase and liquids (5�)                     *
 *        pag 95                                                     *
 * 0.3 < T* < 100                                                    *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE collisionIntegral(DOUBLE const t,DOUBLE const ek) 
{
  DOUBLE ta,num;
  DOUBLE A,B,C,D,E,F;

  A =  1.16145e0;
  B =  0.14874e0;
  C =  0.52487e0;
  D =  0.77320e0;
  E =  2.16178e0;
  F =  2.43787e0;

  ta = t/ek;
  
  if( ta < 0.3 || ta > 100)
  {    
    printf("collision Integral out of range!!\n"
           "0.3 < T* < 100\n"
           "t*               = %lf\n",ta);
    exit(EXIT_FAILURE);
  }

  num = A*pow(ta,-B) + C*exp(-D*ta) + E*exp(-F*ta);

  return num;

}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 20/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * collisionIntegral:                                                *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em kelvin                                         *
 * ek  - e/k do especies desejada (Leornard-Jones parameters)        *
 * ei  - e/i do especies inerte   (Leornard-Jones parameters)        *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * parametro omega                                                   *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * fonte: Manual do FDS                                              *
 *        livro computional fluid dynamics in fire engineering       *
 *        pag 486                                                    *
 *        The properies of gase and liquids (5�)                     *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE diffusionCollisionIntegral(DOUBLE const t
                                 ,DOUBLE const ekl,DOUBLE const eki) 
{
  DOUBLE ekli,ta,num;
  DOUBLE A,B,C,D,E,F;

  A =  1.080794e0;
  B =  0.160330e0;
  C =  0.605009e0;
  D =  0.885240e0;
  E =  2.115672e0;
  F =  2.983080e0;

  ekli = sqrt(ekl*eki);

  ta = t/ekli;
  
  num = A*pow(ta,-B) + C*exp(-D*ta) + E*exp(-F*ta);

  return num;

}
/********************************************************************/

/*********************************************************************
 * Data de criacao    : 29/08/2017                                   *
 * Data de modificaco : 04/05/2019                                   *
 *-------------------------------------------------------------------*
 * airDensity: kg/(m^3)                                              *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em kelvin                                         *
 * presRef - pressao de referencia ou termomecanica                  *
 * p       - pressao ( pressao do modelo)                            *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE mixtureSpeciesDensity(PropPol *den        ,DOUBLE const malorMassMix
                            ,DOUBLE const t      ,DOUBLE const p
                            ,DOUBLE const presRef,bool const fKelvin)
{
  short i,n=den->nPol[0];
  DOUBLE tc,y,d;
//DOUBLE a[MAXPLODEG],tc,y,d;

/*for (i = 0; i < MAXPLODEG; i++)
    a[i] = 0.0e0;*/
  
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  
  
  switch (den->type)
  {
/*...*/
    case IDEALGAS:
      y = (malorMassMix*p)/(IDEALGASR*tc);
      d = 1.e+00; 
      break;
/*.....................................................................*/

/*...*/
    case INCIDEALGAS:
      y = (malorMassMix*presRef)/(IDEALGASR*tc);
      d = 1.e+00;
      break;
/*.....................................................................*/

/*...*/
    default:  
      ERRO_OP(__FILE__,__func__,den->type);
      break;
/*.....................................................................*/
  }

  return y*d;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 25/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * updateMixDensity:                                                 *
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
void updateMixDensity(PropPol *pDen         , Combustion *cModel
                 , DOUBLE *RESTRICT temp    , DOUBLE *RESTRICT pressure
                 , DOUBLE *RESTRICT density , DOUBLE *RESTRICT yComb
                 , DOUBLE const alpha       , bool const iKelvin    
                 , INT const nEl            , char  const iCod)

{
  short nD = DENSITY_LEVEL,ns = cModel->nOfSpecies;
  INT i;
  DOUBLE den,den0,molarMassMix,*y=NULL;
/*...*/
  switch (iCod){
    case PROP_UPDATE_NL_LOOP:
      for(i=0;i<nEl;i++)
      {
        y = &MAT2D(i,0,yComb,ns);
        molarMassMix =  mixtureMolarMass(cModel,y); 
        den0 =  MAT2D(i,TIME_N ,density ,nD);         
        den =  mixtureSpeciesDensity(pDen            ,molarMassMix
                                    ,temp[i]         ,pressure[i]
                                    ,thDynamic.pTh[2],iKelvin);
/*...*/           
        MAT2D(i,TIME_N ,density ,nD) =  alpha*den + (1.e0-alpha)*den0;
      }
/*..................................................................*/
    break;  

    case PROP_UPDATE_OLD_TIME:
      for(i=0;i<nEl;i++)
      {
/*...t(n-2) = t(n-1)*/
        MAT2D(i,TIME_N_MINUS_2 ,density ,nD) = MAT2D(i,1 ,density ,nD);
/*...t(n-1) = t(n)*/           
        MAT2D(i,TIME_N_MINUS_1 ,density ,nD) = MAT2D(i,2 ,density ,nD);
      }
/*..................................................................*/
    break;
  }
/*..................................................................*/

}
/*********************************************************************/ 

/*********************************************************************
 * Data de criacao    : 25/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * initMixtureSpeciesfiHeat:                                         *
 * ----------------------------------------------------------------- *
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initMixtureSpeciesfiHeat(PropPol *prop, char *s, FILE *file)
{
  FILE *fileOut;
  
  char word[WORD_SIZE],species[MAXSPECIES][WORD_SIZE];
  char nameAux[1000];
  short i,j,k,jk;
  int nSpecies;
  double x[MAXPLODEG],g;


  if (!strcmp(s, "polinomio")) 
  {
    prop->type = POL;
  
    fscanf(file, "%s", nameAux);
    fileOut = openFile(nameAux, "r");

    fscanf(fileOut, "%d", &nSpecies);    
/*...*/
    for(j=0;j<nSpecies;j++)
    {

      readMacro(fileOut,word,false);
      convStringLower(word);
      
      fscanf(fileOut, "%lf", &g);
/*...*/
      for(k=0;k<MAXSPECIES;k++)
      {
/*... fuel*/
        if(!strcmp(word,"fuel"))
        {
          jk = SP_FUEL;  
          strcpy(species[jk],"fuel");
          break;
        }
/*... n2*/
        else if(!strcmp(word,"n2"))
        {
          jk = SP_N2; 
          strcpy(species[jk],"n2"); 
          break;
        } 
/*... o2*/
        else if(!strcmp(word,"o2"))
        {
          jk = SP_O2;  
          strcpy(species[jk],"o2");
          break;
        } 
/*... co2*/
        else if(!strcmp(word,"co2"))
        {
          jk = SP_CO2;  
          strcpy(species[jk],"co2");
          break;
        }
/*... h2o*/
        else if(!strcmp(word,"h2o"))
        {
          jk = SP_H2O;
          strcpy(species[jk],"h2o");  
          break;
        }    
      }
/*.....................................................................*/

      prop->nPol[jk] = readFileLineSimple(x, fileOut);
      ERRO_POL_READ(prop->nPol[jk], MAXPLODEG, __FILE__, __func__, __LINE__);

      for (i = 0; i < prop->nPol[jk]; i++)
        MAT2D(jk,i,prop->a,MAXPLODEG) = x[i]/g;
    }

    fclose(fileOut);

    fprintf(fileLogExc, "%-25s: %s\n", "Type", s);

  }
/*.....................................................................*/
  
  printf("Write SpeciesfiHeat cp(T):\n");
  fileOut = openFile("species_cp.out", "w");
  for(i=0;i<nSpecies;i++)
  {
    fprintf(fileOut,"%s\n",species[i]);
    for (j = 0, g =200; j < 40; j++) 
    {
      fprintf(fileOut,"%lf %lf\n",g
              ,pol(&MAT2D(i,0,prop->a,10),g,prop->nPol[i]));
      g += 200;
    }
  }
  fclose(fileOut);

}
/********************************************************************/

/********************************************************************* 
 * Data de criacao    : 20/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * etEnergyForTempMix: obtem a entalpia sensivel apartir da temp     *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------*
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * yFrac    - fracao de massa da especies primitivas                 * 
 * temp   - temp                                                     *
 * energy - nao definido                                             * 
 * prop   - propriedades por material                                *
 * mat    - material da celula                                       *
 * nCell  - numero da celulas                                        *
 * nOfPrSp  - numero de especies primitivas                          *
 * fsHeat - variacao do calor especifico em funcao da Temperatura    *
 *          (true|false)                                             *
 * fKelnvin - temperatura em kelvin (true|false)                     *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * energy - entalpia sensivel                                        * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void getEnergyForTempMix(PropPol *sHeatPol    ,DOUBLE *RESTRICT yFrac 
                        ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                        ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                        ,INT const nCell      ,short const nOfPrSp
                        ,bool const fSheat    ,bool const fKelvin
                        ,bool const fOmp      ,short const nThreads ) 
{
  short lMat;
  INT i;  
  DOUBLE sHeatRef,*y;

/*...*/
  if (fOmp) 
  {
/*...*/ 
#pragma omp parallel  for default(none) num_threads(nThreads)\
    private(i,lMat,sHeatRef,y) shared(prop,mat,energy,temp,sHeatPol,nOfPrSp,yFrac)
    for (i = 0; i < nCell; i++) 
    {
      lMat  = mat[i] - 1;
      sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
      y = &MAT2D(i,0,yFrac,nOfPrSp);
      energy[i] = tempForSpecificEnthalpyMix(sHeatPol ,yFrac
                                            ,temp[i]  ,sHeatRef
                                            ,nOfPrSp
                                            ,fSheat  ,fKelvin);  
    }
/*...................................................................*/ 
  }
/*...................................................................*/ 

  else
  {
/*...*/ 
    for (i = 0; i < nCell; i++) 
    {
      lMat  = mat[i] - 1;
      sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
      y = &MAT2D(i,0,yFrac,nOfPrSp);
      energy[i] = tempForSpecificEnthalpyMix(sHeatPol ,yFrac
                                            ,temp[i]  ,sHeatRef
                                            ,nOfPrSp
                                            ,fSheat  ,fKelvin); 
    }
/*...................................................................*/ 
  }
/*...................................................................*/ 
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 20/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * getTempForEnergyMix : obtem a temperatura apartir da entalpia     *  
 * sensivel                                                          *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * yFrac    - fracao de massa da especies primitivas                 * 
 * temp   - nao definido                                             *
 * energy - entalpia sensivel                                        * 
 * prop   - propriedades por material                                *
 * mat    - material da celula                                       *
 * nCell  - numero da celulas                                        *
 * nOfPrSp  - numero de especies primitivas                          *
 * fTemp  - equaca da energia na forma de temperatura (true|false)   *
 * fsHeat - variacao do calor especifico em funcao da Temperatura    *
 *          (true|false)                                             *
 * fKelnvin - temperatura em kelvin (true|false)                     *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * temp   - temperatura (C ou kelvin)                                * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void getTempForEnergyMix(PropPol *sHeatPol    ,DOUBLE *RESTRICT yFrac
                        ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                        ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                        ,INT const nCell      ,short const nOfPrSp 
                        ,bool const fTemp     ,bool const fSheat    
                        ,bool const fKelvin
                        ,bool const fOmp      ,short const nThreads )
{
  
  short lMat;
  INT i;  
  DOUBLE sHeatRef,*y;

/*... resolucao da eq da energia na forma de temperatura*/ 
  if(fTemp)
    for (i = 0; i < nCell; i++)
      temp[i] = energy[i]; 
/*...................................................................*/ 

/*... resolucao da eq da energia na forma de entalpia sensivel*/  
  else{
    if(fOmp){
#pragma omp parallel  for default(none) num_threads(nThreads)\
      private(i,lMat,sHeatRef,y) shared(prop,mat,energy,temp,sHeatPol,nOfPrSp,yFrac)
      for (i = 0; i < nCell; i++) {
        lMat  = mat[i] - 1;
        sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
        y = &MAT2D(i,0,yFrac,nOfPrSp);
        temp[i] = specificEnthalpyForTempOfMix(sHeatPol
                                         , energy[i], y    
                                         , sHeatRef , nOfPrSp
                                         , fSheat   , fKelvin
                                         , i);
      }
/*...................................................................*/
    }
/*...................................................................*/

/*...*/
    else{
      for (i = 0; i < nCell; i++) {
        lMat  = mat[i] - 1;
        sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
        y = &MAT2D(i,0,yFrac,nOfPrSp);
        temp[i] = specificEnthalpyForTempOfMix(sHeatPol
                                         , energy[i], y    
                                         , sHeatRef , nOfPrSp
                                         , fSheat   , fKelvin
                                         , i);
      }
/*...................................................................*/
    }
/*...................................................................*/ 
  }
/*...................................................................*/ 

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 19/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * updateMixSpecificHeat:                                            *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * temp     - temperatura (�C/K)                                     *
 * yFrac    - fracao de massa da especies primitivas                 *
 * sHeat    - calor especifico por celula                            *
 * nOfPrSp  - numero de especies primitivas                          * 
 * fKelvin  - temperatura dada em kelvin                             *
 * nEl      - numero total de elmentos                               *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * sHeat    - calor especifico por celula atualizado                 *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateMixSpecificHeat(PropPol *sHeatPol
                         , DOUBLE *RESTRICT temp  , DOUBLE *RESTRICT yFrac  
                         , DOUBLE *RESTRICT sHeat , short const nOfPrSp
                         , bool const iKelvin
                         , INT const nEl          , char  const iCod)

{
  short nD = SHEAT_LEVEL;
  INT i;
  DOUBLE *y;  
  
/*...*/
  switch (iCod)
  {
    case PROP_UPDATE_NL_LOOP:
      for(i=0;i<nEl;i++)
      {
        y = &MAT2D(i,0,yFrac,nOfPrSp);
/*...*/           
        MAT2D(i,TIME_N ,sHeat ,nD) = 
        mixtureSpecifiHeat(sHeatPol ,y,temp[i]  ,nOfPrSp,iKelvin);
      }
/*..................................................................*/
    break;  

  case PROP_UPDATE_OLD_TIME:
    for(i=0;i<nEl;i++){
/*...*/
      MAT2D(i, TIME_N_MINUS_2, sHeat, nD) = MAT2D(i,1 ,sHeat ,nD);           
      MAT2D(i, TIME_N_MINUS_1, sHeat, nD) = MAT2D(i,2 ,sHeat ,nD);
    }
/*..................................................................*/
    break;
  }
/*..................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 29/08/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * mixtureSpecifiHeat: kJ/(kg.K)                                     *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * yFrac    - fracao de massa da especies primitivas                 * 
 * t - temperatura em Kelvin                                         *
 * nOfPrSp  - numero de especies primitivas                          *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE mixtureSpecifiHeat(PropPol *sHeat    , DOUBLE *yFrac
                         , DOUBLE const t    , short const mOfPrSp
                         , bool const fKelvin) 
{

  short i,k,n;  
  DOUBLE a[MAXPLODEG],cpk,cp,d;
  DOUBLE tc;

  a[0] = 0.0e0;
  
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  for(k=0,cp=0.e0;k<mOfPrSp;k++)
  {
    n = sHeat->nPol[k];
    for (i = 0; i < n; i++)
      a[i] = MAT2D(k,i,sHeat->a,MAXPLODEG);
  
/*... polinomio*/
    cpk = a[0];
    for (i = 1; i < n; i++)
      cpk += a[i]*pow(tc,i);
/*.....................................................................*/
    yFrac[k] = max(yFrac[k],0.0);
    if (cpk < 0.e0 || yFrac[k] < 0.e0)
    {
      printf("Calor especifico negativo!!\n"
             "Y                = %e\n"
             "Species          = %d\n"   
             "Calor especifico = %e\n"
             "Temperatura      = %lf\n!!",yFrac[k], k,cpk,tc);
      exit(EXIT_FAILURE);
    }

/*...*/
    cp += yFrac[k]*cpk;
/*.....................................................................*/
  }
/*.....................................................................*/

/*...*/
  d = 1.e0;
/*.....................................................................*/

 if (cp < 0.e0) {
    printf("Calor especifico negativo!!"
           "Calor especifico = %e\n"
           "Temperatura      = %lf\n!!",d*cp,tc);
    exit(EXIT_FAILURE);
  }

  return d*cp;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 06/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * specieSpecifiHeat: calor especifico da especie k kJ/(kg.K)        *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeat    - estrutra par o polinoimio do calor especifico          *
 * kSpecie  - fracao de massa da especies primitivas                 * 
 * t - temperatura em Kelvin                                         *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE specieSpecifiHeat(PropPol *sHeat     , short const kSpecie
                        , DOUBLE const t    , bool const fKelvin) 
{

  short i,n;
  DOUBLE a[MAXPLODEG],cp,d,tc;

  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  n=sHeat->nPol[kSpecie];

  for (i = 0; i < n; i++)
    a[i] = MAT2D(kSpecie,i,sHeat->a,MAXPLODEG);
  
/*... polinomio*/
  cp = a[0];
  for (i = 1; i < n; i++)
    cp += a[i]*pow(tc,i);
/*.....................................................................*/

  if (cp < 0.e0 )
  {
    printf("Calor especifico negativo!!"
           "Species          = %hd "   
           "Calor especifico = %e\n"
           "Temperatura      = %lf\n!!",kSpecie, cp, tc);
    exit(EXIT_FAILURE);
  }

/*...*/
  d = 1.e0;
/*.....................................................................*/

 if (cp < 0.e0) {
    printf("Calor especifico negativo!!"
           "Calor especifico = %e\n"
           "Temperatura      = %lf\n!!",d*cp,tc);
    exit(EXIT_FAILURE);
  }

  return d*cp;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * specieViscosity: viscosidade dinamica kg/(m.s)                    *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * dVisc     - estrutra par o polinoimio da viscosidade              *
 * cModel    - massa molar da especie                                *
 * yFrac     - fracao massica da mistura                             *
 * t         - temperatura                                           *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE mixtureDynamicViscosity(PropPol *dVisc      ,Combustion *cModel
                            ,DOUBLE *RESTRICT yFrac,DOUBLE const t 
                            ,bool const fKelvin)
{
  short i,j,ns;
  DOUBLE molarMassMix,tc,y,d,mWa,sum1,sum2,tmp1,sigmaA,ek,phi;
  DOUBLE nuA[MAXSPECIES],xA[MAXSPECIES];
  
 
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  
  
  
  switch (dVisc->type)
  {
/*...*/
    case FDSVISCOSITY:
      d  = 1.e0;
      ns = cModel->nOfSpecies;
      molarMassMix =  mixtureMolarMass(cModel,yFrac);
      for(i=0,sum1=0,sum2=0;i<ns;i++)
      { 
        mWa = cModel->mW[i];
        xA[0] = (mWa*yFrac[i])/molarMassMix;

        sigmaA = cModel->leornadJones[i][0];
        ek     = cModel->leornadJones[i][1];
        nuA[0] = specieViscosity(mWa,sigmaA,ek,tc); 

        tmp1 = xA[0]*sqrt(mWa);
        sum1 += nuA[0]*tmp1;
        sum2 += tmp1; 
      }
      y = sum1/sum2;
      break;
/*.....................................................................*/

/*...*/
    case WILKELAW:
      d  = 1.e0;
      ns = cModel->nOfSpecies;
      molarMassMix =  mixtureMolarMass(cModel,yFrac);

/*...*/
      for(i=0;i<ns;i++)
      { 
        sigmaA = cModel->leornadJones[i][0];
        ek     = cModel->leornadJones[i][1];
        mWa    = cModel->mW[i];
        nuA[i] = specieViscosity(mWa,sigmaA,ek,tc); 

        xA[i] = (mWa*yFrac[i])/molarMassMix;
      }  
/*...................................................................*/

/*...*/
      for(i=0,sum2=0.e0;i<ns;i++)
      {
        for(j=0,sum1=0.e0;j<ns;j++)
        {
          if(i==j)
            phi = 1.e0;
          else  
            phi = philn(cModel->mW[i],nuA[i]
                       ,cModel->mW[j],nuA[j]);
          sum1+= xA[j]*phi;
        }
        sum2 += xA[i]*nuA[i]/sum1;
      }
/*...................................................................*/

      y = sum2;
      break;
/*.....................................................................*/

/*...*/
    default:  
      ERRO_OP(__FILE__,__func__,dVisc->type);
      break;
/*.....................................................................*/
  }

  return y*d;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * specieViscosity: viscosidade dinamica kg/(m.s)                    *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * molarMass - massa molar da especie                                *
 * sigmaA    - parametro de Leornad-Jones                            *
 * ek        - parametro de Leornad-Jones                            *
 * t - temperatura em Kelvin                                         *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE specieViscosity(DOUBLE const molarMass
                      ,DOUBLE const sigmaA   ,DOUBLE const ek   
                      ,DOUBLE const t     ) 
{

  short i,n;
  DOUBLE omega,visc;

  omega     = collisionIntegral(t,ek);
  visc      = 26.69e-07*sqrt(molarMass*t)/(sigmaA*sigmaA*omega);

  return visc;
}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * updateDynamicViscosity:                                           *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * dVisc   -> intepolacao da viscosidade dinamica                    *
 * cModel  ->                                                        *
 * temp    -> temperatura                                            *
 * visc    -> vetor de viscosidades dianamica por celula             *
 * nOfPrSp  - numero de especies primitivas                          * 
 * iKelvin -> kelvin ou celsus                                       *
 * nEl     -> numero total de celulas                                *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateMixDynamicViscosity(PropPol *dVisc    ,Combustion *cModel
                          ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT yFrac
                          ,DOUBLE *RESTRICT visc ,short const nOfPrSp   
                          ,bool const iKelvin    ,INT const nEl)
{
  INT i;
  DOUBLE *y;

  for(i=0;i<nEl;i++)
  {
    y = &MAT2D(i,0,yFrac,nOfPrSp);         
    visc[i] = mixtureDynamicViscosity(dVisc    ,cModel
                                     ,yFrac    ,temp[i]  
                                     ,iKelvin);
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 18/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * updateMixDynamicThermalCond:                                      *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * propF     - proprieade variavel do fluido                         *
 * temp    -> temperatura                                            *
 * thc     -> vetor de condutividade termica por celula              *
 * nOfPrSp  - numero de especies primitivas                          * 
 * iKelvin -> kelvin ou celsus                                       *
 * nEl     -> numero total de celulas                                *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateMixDynamicThermalCond(PropVarFluid *propF,Combustion *cModel 
                          ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT yFrac
                          ,DOUBLE *RESTRICT thc  ,short const nOfPrSp   
                          ,bool const iKelvin    ,INT const nEl)
{
  INT i;
  DOUBLE *y;

  for(i=0;i<nEl;i++)
  {
    y = &MAT2D(i,0,yFrac,nOfPrSp);         
    thc[i] = mixtureThermalConductvity(propF    ,cModel
                                      ,yFrac    ,temp[i]  
                                      ,iKelvin);
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * mixtureThermalConductvity: condutividade termica [KW/m.K]         *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * propF     - proprieade variavel do fluido                         *
 * cModel    - massa molar da especie                                *
 * yFrac     - fracao massica da mistura                             *
 * t         - temperatura                                           *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE mixtureThermalConductvity(PropVarFluid *propF   ,Combustion *cModel 
                                ,DOUBLE *RESTRICT yFrac,DOUBLE const t 
                                ,bool const fKelvin)
{
  short i,j,ns;
  DOUBLE molarMassMix,tc,y,d,mWa,sum1,sum2,tmp1,sigmaA,ek,phi;
  DOUBLE nuA,cP,xA[MAXSPECIES],k[MAXSPECIES];
  DOUBLE Pr=0.7;
   
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  
  
  
  switch (propF->thCond.type)
  {
/*...*/
    case FDSTHERMALCOND:
      d  = 1.e0;
      ns = cModel->nOfSpecies;
      molarMassMix =  mixtureMolarMass(cModel,yFrac);
/*...*/
      for(i=0,sum1=sum2=0;i<ns;i++)
      {        
/*... viscosidade dinamica da especie i*/
        mWa    = cModel->mW[i];
        sigmaA = cModel->leornadJones[i][0];
        ek     = cModel->leornadJones[i][1];
        nuA    = specieViscosity(mWa,sigmaA,ek,tc); 
/*... color especifico da especie i*/
        if(propF->fSpecificHeat)
          cP = specieSpecifiHeat(&propF->sHeat,i,t,fKelvin); 
        else
          cP = propF->sHeatRef;
/*... condutividade termica*/
        k[0]   = nuA*cP/Pr;
/*... fracao molar*/
        xA[0] = (mWa*yFrac[i])/molarMassMix;
/*...*/
        tmp1 = xA[0]*sqrt(mWa);
        sum1 += k[0]*tmp1;
        sum2 += tmp1; 
      }
      y = sum1/sum2;
      break;
/*.....................................................................*/

/*...*/
    case WILKELAW:
      d  = 1.e0;
      ns = cModel->nOfSpecies;
      molarMassMix =  mixtureMolarMass(cModel,yFrac);

/*...*/
      for(i=0;i<ns;i++)
      { 
/*... viscosidade dinamica da especie i*/
        mWa    = cModel->mW[i];
        sigmaA = cModel->leornadJones[i][0];
        ek     = cModel->leornadJones[i][1];
        nuA    = specieViscosity(mWa,sigmaA,ek,tc); 
/*... color especifico da especie i*/
        if(propF->fSpecificHeat)
          cP = specieSpecifiHeat(&propF->sHeat,i,t,fKelvin); 
        else
          cP = propF->sHeatRef;
/*... condutividade termica*/
        k[i] = nuA*cP/Pr;
/*... fracao molar*/
        xA[i] = (mWa*yFrac[i])/molarMassMix;
      }  
/*...................................................................*/

/*...*/
      for(i=0,sum2=0.e0;i<ns;i++)
      {
        for(j=0,sum1=0.e0;j<ns;j++)
        {
          if(i==j)
            phi = 1.e0;
          else  
            phi = philn(cModel->mW[i],k[i]
                       ,cModel->mW[j],k[j]);
          sum1+= xA[j]*phi;
        }
        sum2 += xA[i]*k[i]/sum1;
      }
/*...................................................................*/

      y = sum2;
      break;
/*.....................................................................*/

/*...*/
    default:  
      ERRO_OP(__FILE__,__func__,propF->thCond.type);
      break;
/*.....................................................................*/
  }

  return y*d;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * mixtureDiffusion : coeficiente de difusacao da especie  A  [m2/s] *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * propF     - proprieade variavel do fluido                         *
 * cModel    - massa molar da especie                                *
 * yFrac     - fracao massica da mistura                             *
 * t         - temperatura                                           *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE mixtureDiffusion(PropVarFluid *propF   ,Combustion *cModel 
                       ,DOUBLE *RESTRICT yFrac,DOUBLE const t 
                       ,short const kSpecieA  ,short const kSpecieI 
                       ,bool const fKelvin)
{
  short j,ns=cModel->nOfSpecies;
  DOUBLE tc,mWa,sA,mWi,sI,ekA,ekI,y,mMassMix,xA,sum1,d=1.e0;
   
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  switch (propF->diff.type)
  {
/*...*/
    case FDSDIFF:
/*... especie inerte*/
      mWi    = cModel->mW[kSpecieI];
      sI     = cModel->leornadJones[kSpecieI][0];
      ekI    = cModel->leornadJones[kSpecieI][1];
/*....................................................................*/
  
/*... especie inerte desejada*/
      mWa    = cModel->mW[kSpecieA];
      sA     = cModel->leornadJones[kSpecieA][0];
      ekA    = cModel->leornadJones[kSpecieA][1];
/*....................................................................*/

      y  = specieDiffusionBinary( mWa, mWi, sA , sI, ekA, ekI, t);
    break;
/*.....................................................................*/

/*...*/
    case HIRSCHDIFF:

      mMassMix = mixtureMolarMass(cModel,yFrac);
/*... especie inerte desejada*/
      mWa    = cModel->mW[kSpecieA];
      sA     = cModel->leornadJones[kSpecieA][0];
      ekA    = cModel->leornadJones[kSpecieA][1];
/*....................................................................*/

/*...*/
      for(j=0,sum1=0;j<ns;j++)
      {
        mWi    = cModel->mW[j];
        sI     = cModel->leornadJones[j][0];
        ekI    = cModel->leornadJones[j][1];
        if (j != kSpecieA) {
          xA = (cModel->mW[j]*yFrac[j])/mMassMix;
          y  = specieDiffusionBinary( mWa, mWi, sA , sI, ekA, ekI, t);
          sum1 += xA/y;
        }
      }
/*....................................................................*/
     
/*...*/
      y = (1.e0 - yFrac[kSpecieA])/sum1;
/*....................................................................*/
    break;
/*.....................................................................*/

/*...*/
    default:  
      ERRO_OP(__FILE__,__func__,propF->diff.type);
      break;
/*.....................................................................*/
  } 
 
  return y*d;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 20/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * specieDiffusionBinary: coeficiente de diffusao da especie A na    *
 * especie B [m2/s]                                                  *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * molarA    - massa molar da especie                                *
 * molarA    - massa molar da especie                                *
 * sigmaA    - parametro de Leornad-Jones                            *
 * sigmaB    - parametro de Leornad-Jones                            *
 * ekA       - parametro de Leornad-Jones                            *
 * ekB       - parametro de Leornad-Jones                            *
 * t - temperatura em Kelvin                                         *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE specieDiffusionBinary(DOUBLE const mMassA,DOUBLE const mMassB
                            ,DOUBLE const sigmaA,DOUBLE const sigmaB  
                            ,DOUBLE const ekA   ,DOUBLE const ekB
                            ,DOUBLE const t     ) 
{

  short i,n;
  DOUBLE omega,diffAB,mMassAB,sigmaAB;
  mMassAB = 2.e0*mMassA*mMassB/(mMassA + mMassB);
  sigmaAB = sqrt(0.5e0*(sigmaA + sigmaB));
  omega   = diffusionCollisionIntegral(t,ekA,ekB);
  diffAB  = 2.66e-07*pow(t,1.5)/(sigmaA*sigmaA*omega);

  return diffAB;
}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 18/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * updateMixDynamicThermalCond:                                      *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * propF   -> proprieade variavel do fluido                          *
 * temp    -> temperatura                                            *
 * diff    -> vetor de difusao das especies                          *
 * nOfPrSp -> numero de especies primitivas                          * 
 * nComb   -> numero de especies explicitamente resolvidas           * 
 * iKelvin -> kelvin ou celsus                                       *
 * nEl     -> numero total de celulas                                *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateMixDiffusion(PropVarFluid *propF,Combustion *cModel 
                       ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT yFrac
                       ,DOUBLE *RESTRICT diff ,short const nOfPrSp 
                       ,short const nComb       
                       ,bool const iKelvin    ,INT const nEl)
{
  short j;
  INT i;
  DOUBLE *y;

  for(i=0;i<nEl;i++)
  {
    y = &MAT2D(i,0,yFrac,nOfPrSp);         
    for(j=0;j<nComb;j++)
      MAT2D(i,j,diff,nComb) = mixtureDiffusion(propF   ,cModel 
                                              ,yFrac   ,temp[i]
                                              ,j       ,SP_N2 
                                              ,iKelvin);
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 19/08/2018                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * SPECIFICENTHALPYFORTEMPMIX:  calcula a temperatura apartir da     *
 * entalpia especifica                                               * 
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * hs       - entalpia sensivel                                      *
 * yFrac    - fracao de massa da especies primitivas                 * 
 * sHeatRef - calor especifico de referencia constante com temp      *
 * nOfPrSp  - numero de especies primitivas                          *
 * fSheat   - calor especifico com variacao com a Temperatura        *
 * fKelvin  - temperatura dada em kelvin                             *
 * nel      - numero da celula  
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * temperatura (�C/Kelvin)                                           *
 *-------------------------------------------------------------------*
 * OBS:                                                              * 
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE specificEnthalpyForTempOfMix(PropPol *sHeatPol
                             , DOUBLE const hs        , DOUBLE *yFrac
                             , DOUBLE const sHeatRef  , short const nOfPrSp
                             , bool const fSheat      , bool const fKelvin
                             , INT const nel ) 
{
  unsigned short i;
  bool flag = false;
  DOUBLE f,fl,t,conv,tol=1e-04;
 
/*...*/
  if(fSheat)
  {
/*... chute inicial usando a massa espeficia constante*/

    t = 0.5e0*(TREF + ENTHALPY_FOR_TEMP(sHeatRef,hs,TREF))+tol;
    if(t > 4000.0) t = 4000.0;
 
/*...*/
    conv = tol*(hs-tempForSpecificEnthalpyMix(sHeatPol,yFrac
                                             ,t       ,sHeatRef
                                             ,nOfPrSp  
                                             ,fSheat  ,true));
    conv = fabs(conv);
/*... Newton-Raphson*/
    for(i=0;i<60000;i++)
    {
      f  = hs-tempForSpecificEnthalpyMix(sHeatPol,yFrac
                                        ,t       ,sHeatRef
                                        ,nOfPrSp 
                                        ,fSheat  ,true);
      if(fabs(f) < conv || fabs(f) == 0.e0) 
      {
        flag = true;
        break;
      }
    
      fl = mixtureSpecifiHeat(sHeatPol,yFrac
                             ,t       ,nOfPrSp
                             ,true);
      t += f/fl;   
    }
/*...................................................................*/

    if(!flag)
    {
      printf("%i %e %e %e\n",i,t,f,conv);
      ERRO_GERAL(__FILE__,__func__,__LINE__,
      "sEnthalpy->temperature:\n Newton-raphson did not converge !!");
    }
  }
/*...................................................................*/

/*...*/
  else
    t = ENTHALPY_FOR_TEMP(sHeatRef,hs,TREF);
/*...................................................................*/

  if(!fKelvin)
    t = KELVIN_FOR_CELSIUS(t);  

  return t;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 19/08/2018                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * TEMPFORSPECIFICENTHALPY: calcula a entalpia espeficia apartir da  *
 * temperatura                                                       *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeat    - estrutra par o polinoimio do calor especifico          *
 * yFrac    - fracao de massa da especies primitivas                 *
 * t        - temperatura (�C/K)                                     *
 * sHeatRef - calor especifico de referencia constante com temp      *
 * nOfPrSp  - numero de especies primitivas                          *
 * fSheat   - calor especifico com variacao com a Temperatura        *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * entalpia sensivel                                                 *
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE tempForSpecificEnthalpyMix(PropPol *sHeat    , DOUBLE *yFrac
                                , DOUBLE const t    , DOUBLE const sHeatRef
                                , short const nOfPrSp
                                , bool const fSheat , bool const fKelvin) 
{

  short i,k,n;
  DOUBLE a[MAXPLODEG],d,dt,hk,hs;
  DOUBLE tc,tRef= TREF ;

  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  hs = 0.e0;
  if(fSheat)
  {
    for(k = 0; k < nOfPrSp; k++)
    {
      n = sHeat->nPol[k];
      for (i = 0; i < n; i++)
        a[i] = MAT2D(k,i,sHeat->a,MAXPLODEG);
  
      for (i = 0, hk =0.e0; i < n; i++) 
      {
        d    = (double) (i + 1);
        dt   =pow(tc,d) - pow(tRef,d);
        hk += a[i]*dt/d;
      }

      hs += yFrac[k]*hk; 
    }
  }

  else 
    hs = TEMP_FOR_ENTHALPY(sHeatRef,tc,TREF);

  return hs;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 07/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * TEMPFORSPECIFICENTHALPYSPECIES: calcula a entalpia espeficia da   *
 * especie k partir da temperatura                                   *                   *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeat    - estrutra par o polinoimio do calor especifico          *
 * yFrac    - fracao de massa da especies primitivas                 *
 * t        - temperatura (�C/K)                                     *
 * nOfPrSp  - numero de especies primitivas                          *
 * fSheat   - calor especifico com variacao com a Temperatura        *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * entalpia sensivel                                                 *
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE tempForSpecificEnthalpySpecies(PropPol *sHeat, short const kSpecie
                               , DOUBLE const t     , DOUBLE const sHeatRef
                               , bool const fSheat  , bool const fKelvin) 
{

  short i,n;
  DOUBLE a[MAXPLODEG],d,dt,hk;
  DOUBLE tc,tRef= TREF;

  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  if(fSheat)
  {
    n=sHeat->nPol[kSpecie];
    for (i = 0; i < n; i++)
      a[i] = MAT2D(kSpecie,i,sHeat->a,MAXPLODEG);
  
    for (i = 0, hk =0.e0; i < n; i++) 
    {
      d    = (double) (i + 1);
      dt   =pow(tc,d) - pow(tRef,d);
      hk += a[i]*dt/d;
    }
  }

  else
    hk       = TEMP_FOR_ENTHALPY(sHeatRef,tc,TREF);
  
  return hk;

}
/**********************************************************************/

/********************************************************************* 
 * Data de criacao    : 20/08/2018                                   *
 * Data de modificaco : 20/05/2019                                   *
 *-------------------------------------------------------------------*
 * initPropTempMix: inicializao de propriedades com variacao temporal*
 * dependentes da temperatura                                        *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * propFluid -> estrutura de dados das propriedades varaiveis        *
 * prop      -> nao definido                                         * 
 * t         -> temperatura                                          *
 * pressure  -> pressao                                              *
 * yFrac    - fracao de massa da especies primitivas                 *
 * nOfPrSp  - numero de especies primitivas                          *
 * np        -> numero niveis de tempos                              * 
 * nCell     -> numero de celulas                                    * 
 * iKelvin   -> temperatura em kelvin (true|false)                   *
 * iProp     -> numero da propriedade                                * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * prop    -> propriedade iniciacializada                            * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
  *********************************************************************/
void initPropTempMix(PropVarFluid *propFluid, Combustion *cModel
                 ,DOUBLE *RESTRICT prop     ,DOUBLE *RESTRICT t       
                 ,DOUBLE *RESTRICT pressure ,DOUBLE *RESTRICT yFrac   
                 ,short const nOfPrSp       ,short const np  
                 ,INT    const nCell        ,bool const iKelvin 
                 ,short const iProp)
{    
  INT i;
  unsigned short j,k,lMat;
  DOUBLE *y,molarMassMix,v;
         
  for(i=0;i<nCell;i++){    

/*...*/
    y = &MAT2D(i,0,yFrac, nOfPrSp);
/*...................................................................*/

/*...*/
    if( iProp == DENSITY )
    {
      
      molarMassMix =  mixtureMolarMass(cModel,y); 
      v = mixtureSpeciesDensity(&propFluid->den,molarMassMix
                      ,t[i]                    , pressure[i]
                      ,thDynamic.pTh[2]        , iKelvin);
    }
/*...................................................................*/

/*...*/
    else if( iProp == SPECIFICHEATCAPACITYFLUID)
    {
      v = mixtureSpecifiHeat(&propFluid->sHeat, y
                           ,t[i]            , nOfPrSp
                           ,iKelvin);
    }
/*...................................................................*/

/*...*/
    else if( iProp == DYNAMICVISCOSITY)
    {
      v = mixtureDynamicViscosity(&propFluid->dVisc,cModel
                               ,y                ,t[i]
                               ,iKelvin);
    }
/*...................................................................*/

/*...*/
    else if( iProp == THERMALCONDUCTIVITY)
    {
      v = mixtureThermalConductvity(propFluid         ,cModel
                                ,y                  ,t[i]
                                ,iKelvin);
    }
/*...................................................................*/

/*...*/
    for(j=0;j<np;j++)      
      MAT2D(i,j,prop,np) = v; 
/*...................................................................*/  
  }
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 20/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * initDiffMix: inicializao do coeficiente de diffusao das especies  *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * propFluid -> estrutura de dados das propriedades varaiveis        *
 * diff      -> nao definido                                         * 
 * t         -> temperatura                                          *
 * pressure  -> pressao                                              *
 * yFrac    - fracao de massa da especies primitivas                 *  
 * propMat -> propriedade de referencia por material                 * 
 * mat     -> material por celula                                    * 
 * nOfPrSp  - numero de especies primitivas                          *
 * nComb     -> numero de especies resolvidas pela eq de transporte  * 
 * nCell     -> numero de celulas                                    * 
 * iKelvin   -> temperatura em kelvin (true|false)                   *
 * iProp     -> numero da propriedade                                * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * prop    -> propriedade iniciacializada                            * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
  *********************************************************************/
void initDiffMix(PropVarFluid *propF    , Combustion *cModel
                ,DOUBLE *RESTRICT diff     ,DOUBLE *RESTRICT t  
                ,DOUBLE *RESTRICT pressure ,DOUBLE *RESTRICT yFrac 
                ,DOUBLE *RESTRICT propMat  ,short *RESTRICT mat    
                ,short const nOfPrSp       ,short const nComb   
                ,INT    const nCell        ,bool const iKelvin)
{    
  INT i;
  unsigned short j,k,lMat;
  DOUBLE *y;

/*...*/
  if(propF->fDiffusion)
  {
    for(i=0;i<nCell;i++)
    {    

/*...*/
      y = &MAT2D(i,0,yFrac, nOfPrSp);
/*...................................................................*/

/*...*/
      for(j=0;j<nComb;j++)
        MAT2D(i,j,diff,nComb) = mixtureDiffusion(propF       ,cModel 
                                ,yFrac       ,t[i]
                                ,j           ,SP_N2
                                ,iKelvin);
/*...................................................................*/
    }
/*...................................................................*/
  }
/*...................................................................*/

/*...*/
  else
  {
    for(i=0;i<nCell;i++)
    {    
      lMat               = mat[i]-1;
      for(j=0;j<nComb;j++)
        MAT2D(i,j,diff,nComb) = 
        MAT2D(lMat,SPECIEDIFUSSIONFUEL+j,propMat,MAXPROP);     
    }
  }
/*...................................................................*/
}
/*********************************************************************/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++ Ar ++++++++++++++++++++++++++++++++++++*/

/*********************************************************************
 * Data de criacao    : 29/08/2017                                   *
 * Data de modificaco : 12/07/2018                                   *
 *-------------------------------------------------------------------*
 * airDensity: kg/(m^3)                                              *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em kelvin                                         *
 * presRef - pressao de referencia ou termomecanica                  *
 * p       - pressao ( pressao do modelo)                            *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE airDensity(PropPol *den
                 ,DOUBLE const t      ,DOUBLE const p
                 ,DOUBLE const presRef,bool const fKelvin) {
  short i,n=den->nPol[0];
  DOUBLE a[MAXPLODEG],tc,y,d;

  for (i = 0; i < MAXPLODEG; i++)
    a[i] = 0.0e0;
  
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  switch (den->type) {
/*... polinomio*/
    case POL:
      for (i = 0; i < n; i++)
        a[i] = den->a[i];

/*... polinomio*/
      y = a[0];
      for (i = 1; i < n; i++)
        y += a[i]*pow(tc,i);
/*.....................................................................*/

/*.....................................................................*/
      d = 1.e0; 
      break;
/*.....................................................................*/
 
/*...*/
    case IDEALGAS:
      y = (MMOLARAR*p)/(IDEALGASR*tc);
      d = 1.e0; 
      break;
/*.....................................................................*/

/*...*/
    case INCIDEALGAS:
      y = (MMOLARAR*presRef)/(IDEALGASR*tc);
      d = 1.e0; 
      break;
/*.....................................................................*/


/*...*/
    default:  
      ERRO_OP(__FILE__,__func__,den->type);
      break;
/*.....................................................................*/
  }

  return y*d;

}
/**********************************************************************/

/*********************************************************************
* Data de criacao    : 12/05/2018                                   *
* Data de modificaco : 07/05/2019                                   *
*-------------------------------------------------------------------*
* diffProp:                                                         *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* u - valor da propriedade                                          *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
* retorna a valor da massa especifica                               *
*-------------------------------------------------------------------*
* OBS:                                                              *
*-------------------------------------------------------------------*
*********************************************************************/
DOUBLE diffProp(PropPol *pol  , DOUBLE u) 
{
  short i, n = pol->nPol[0];
  DOUBLE a[MAXPLODEG], y;

  for (i = 0; i < MAXPLODEG; i++)
    a[i] = 0.0e0;

  switch (pol->type)
  {
/*... polinomio*/
    case POL:
      for (i = 0; i < n; i++)
        a[i] = pol->a[i];

/*... polinomio*/
      y = a[0];
      for (i = 1; i < n; i++)
        y += a[i] * pow(u, i);
/*.....................................................................*/

/*.....................................................................*/
      break;
/*.....................................................................*/


/*...*/
    default:
      ERRO_OP(__FILE__, __func__, pol->type);
      break;
/*.....................................................................*/
  }

  return y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 29/08/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * AIRSPECIFIHEAT: kJ/(kg.K)                                         *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em Kelvin                                         *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * http://www.engineeringtoolbox.com/air-properties-d_156.html   	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE airSpecifiHeat(PropPol *sHeat, DOUBLE const t
                     ,bool const fKelvin) {

  short i,n=sHeat->nPol[0];  
  DOUBLE a[MAXPLODEG],y,d;
  DOUBLE tc;

  a[0] = 0.0e0;
  
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  for (i = 0; i < n; i++)
    a[i] = sHeat->a[i];
  
/*... polinomio*/
  y = a[0];
  for (i = 1; i < n; i++)
    y += a[i]*pow(tc,i);
/*.....................................................................*/

/*...*/
  d = 1.e+0;
/*.....................................................................*/

 if (y < 0) {
    printf("Calor especifico negativo!!"
           "Calor especifico = %e\n"
           "Temperatura      = %lf\n!!",d*y,tc);
    exit(EXIT_FAILURE);
  }


  return d*y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 29/08/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * AIRDYNAMICVISCOSITY: kg/(m.s)                                     *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em Kelvin                                         *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * http://www.engineeringtoolbox.com/air-properties-d_156.html       *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE airDynamicViscosity(PropPol *dVisc,DOUBLE const t
                          ,bool const fKelvin) {

  short i,n=dVisc->nPol[0];
  DOUBLE a[MAXPLODEG],x[MAXPLODEG-1],y,d;
  DOUBLE tc;

  a[0] = 0.0e0;
  
  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  switch (dVisc->type) {
/*... polinomio*/
    case POL:
      for (i = 0; i < n; i++)
        a[i] = dVisc->a[i];
/*.....................................................................*/
  
/*... polinomio*/
      y = a[0];
      for (i = 1; i < n; i++)
        y += a[i]*pow(tc,i);
      d = 1.e-05;
/*.....................................................................*/
      break;
/*.....................................................................*/

/*... polinomio*/
    case SUTHERLAND:
      a[0] = dVisc->a[0]; /*viscosidade de referencia*/
      a[1] = dVisc->a[1]; /*temperatura de referencia*/ 
      a[2] = dVisc->a[2]; /*constante de Sutherland*/

      x[0] = a[1]+ a[2];
      x[1] = tc  + a[2];

      y = a[0]*(x[0]/x[1])*pow(tc/a[1],1.5);

      d = 1.e0;
/*.....................................................................*/
      break;
/*.....................................................................*/

/*...*/
    default:  
      ERRO_OP(__FILE__,__func__,dVisc->type);
      break;
/*.....................................................................*/
  }
/*.....................................................................*/
 
  if (y < 0) {
    printf("Viscosidade dinamica negativa!!\n"
           "Viscosidade dinamica = %e\n"
           "Temperatura          = %lf\n!!",d*y,tc);
    exit(EXIT_FAILURE);
  }

  return d*y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 28/08/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * AIRTHERMALCONDUCTITY: [KW/m.K]                                    *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em kelvin                                         *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * http://www.engineeringtoolbox.com/air-properties-d_156.html   	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE airThermalConductvity(PropPol *thCond, DOUBLE const t 
                            ,bool const fKelvin) {

  short i,n=thCond->nPol[0];  
  DOUBLE a[MAXPLODEG],y,d;
  DOUBLE tc;

  a[0] = 0.0e0;

  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  
  switch (thCond->type) {
/*... polinomio*/
    case POL:
      for (i = 0; i < n; i++)
        a[i] = thCond->a[i];
/*.....................................................................*/

/*... polinomio*/
      y = a[0];
      for (i = 1; i < n; i++)
        y += a[i]*pow(tc,i);
      d = 1.e-05;
/*.....................................................................*/
      break;
/*.....................................................................*/
 /*...*/
    default:  
      ERRO_OP(__FILE__,__func__,thCond->type);
      break;
/*.....................................................................*/
  }
/*.....................................................................*/

/*...*/ 
  if (y < 0) {
    printf("Condutividade termica negativa!!\n"
           "Condutividade termica = %e\n"
           "Temperatura           = %lf\n!!",d*y,tc);
    exit(EXIT_FAILURE);
  }
/*.....................................................................*/

  return d*y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 28/08/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * TEMPFORSPECIFICENTHALPY: calcula a entalpia espeficia apartir da  *
 * temperatura                                                       *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeat    - estrutra par o polinoimio do calor especifico          *
 * t        - temperatura (�C/K)                                     *
 * sHeatRef - calor especifico de referencia constante com temp      *
 * fSheat   - calor especifico com variacao com a Temperatura        *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * entalpia sensivel                                                 *
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE tempForSpecificEnthalpy(PropPol *sHeat
                             , DOUBLE const t   , DOUBLE const sHeatRef
                             , bool const fSheat, bool const fKelvin) {

  short i,n=sHeat->nPol[0];
  DOUBLE a[6],d,dt,tmp;
  DOUBLE tc,tRef= TREF ;

  if(fKelvin)
    tc = t;  
  else
    tc = CELSIUS_FOR_KELVIN(t);  

  if(fSheat){
    for (i = 0; i < n; i++)
      a[i] = sHeat->a[i];

    tmp = 0.0;
    for (i = 0; i < n; i++) {
      d    = (double) (i + 1);
      dt   =pow(tc,d) - pow(tRef,d);
      tmp += a[i]*dt/d;
    }
  }

  else 
    tmp = TEMP_FOR_ENTHALPY(sHeatRef,tc,TREF);

  return tmp;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 28/08/2017                                   *
 * Data de modificaco : 15/07/2018                                   *
 *-------------------------------------------------------------------*
 * SPECIFICENTHALPYFORTEMP:  calcula a temperatura apartir da        *
 * entalpia especifica                                               * 
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * hs       - entalpia sensivel                                      *
 * sHeatRef - calor especifico de referencia constante com temp      *
 * fSheat   - calor especifico com variacao com a Temperatura        *
 * fKelvin  - temperatura dada em kelvin                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * temperatura (�C/Kelvin)                                          *
 *-------------------------------------------------------------------*
 * OBS:                                                              * 
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE specificEnthalpyForTemp(PropPol *sHeatPol
                             , DOUBLE const hs  , DOUBLE const sHeatRef
                             , bool const fSheat, bool const fKelvin) 
{
  unsigned short i;
  bool flag = false;
  DOUBLE f,fl,t,conv,tol=1e-04;
 
/*...*/
  if(fSheat){
/*... chute inicial usando a massa espeficia constante*/
    t = ENTHALPY_FOR_TEMP(sHeatRef,hs,TREF);
/*...*/
    conv = (hs-tempForSpecificEnthalpy(sHeatPol,t,sHeatRef,fSheat,true))*tol;
    conv = fabs(conv);
/*... Newton-Raphson*/
    for(i=0;i<60000;i++){
      f  = hs-tempForSpecificEnthalpy(sHeatPol,t,sHeatRef,fSheat,true);
      if(fabs(f) < conv) {
        flag = true;
        break;
      }
    
      fl = airSpecifiHeat(sHeatPol,t,true);
      t += f/fl;   
    }
/*...................................................................*/

    if(!flag){
      printf("%i %e %e %e\n",i,t,f,conv);
      ERRO_GERAL(__FILE__,__func__,__LINE__,
      "sEnthalpy->temperature:\n Newton-raphson did not converge !!");
    }
  }
/*...................................................................*/

/*...*/
  else
    t = ENTHALPY_FOR_TEMP(sHeatRef,hs,TREF);
/*...................................................................*/

  if(!fKelvin)
    t = KELVIN_FOR_CELSIUS(t);  

  return t;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * WATERDESNITY: kg/(m^3)                                            *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em �C                                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE waterDensity(DOUBLE const t) {

  DOUBLE a,b,c,d;
  DOUBLE tmp1,tmp2,tmp3,tmp4;

  a = 288.9414e0;
  b = 508929.2e0;
  c = 68.12963e0;
  d = 3.9863e0;
  
  tmp1 = t + a;
  tmp2 = b*(t+c);
  tmp3 = (t-d)*(t-d);  

  tmp4 =  1.e0 - tmp1/tmp2*tmp3;
  
  if (tmp4 < 0) {
    printf("Massa especifica negativa\n!!");
    exit(EXIT_FAILURE);
  }

  return 1000.e0*tmp4;


}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * WATERSPECIFIHEAT: kJ/(kg.�C)                                      *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em �C                                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * www.engineeringtoolbox.com/water-thermal-properties-d_162.html	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 * range  T=]0,100[                                                  * 
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE waterSpecifiHeat(DOUBLE const t) {

  DOUBLE a[MAXPLODEG],x[MAXPLODEG-1],y,d;

  a[0] = 4.21470622853361e+00;
  a[1] =-2.87563217630463e-03;
  a[2] = 7.57667401959410e-05;
  a[3] =-7.93659500485961e-07;
  a[4] = 3.28899958033620e-09;

/*... t */  
  x[0] = t;
/*... t*t */
  x[1] = t*t;
/*... t*t*t */
  x[2] = t*x[1];
/*... t*t*t*t */
  x[3] = t*x[2];
  
/*... polinomio*/
  y = a[0] + a[1]*x[0] + a[2]*x[1] + a[3]*x[2] + a[4]*x[3];
/*.....................................................................*/

  if (y < 0) {
    printf("Calor especifico negativo\n!!");
    exit(EXIT_FAILURE);
  }

/*...*/
  y = d = 1.e0;
/*.....................................................................*/

  return d*y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * WATERDINAMICVICOSITY: Kg/(m.s)                                    *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em �C                                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * www.engineeringtoolbox.com/water-thermal-properties-d_162.html	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 * range  T=]0,100[                                                  * 
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE waterDynamicViscosity(DOUBLE const t) {

  DOUBLE a[MAXPLODEG],x[MAXPLODEG-1],y,d;

  a[0] = 1.75373877709050e+00;
  a[1] =-5.20548110460391e-02;
  a[2] = 8.73350059860241e-04;
  a[3] =-7.60339598706936e-06;
  a[4] = 2.96188559332536E-16;

/*... t */  
  x[0] = t;
/*... t*t */
  x[1] = t*t;
/*... t*t*t */
  x[2] = t*x[1];
/*... t*t*t*t */
  x[3] = t*x[2];
  
/*... polinomio*/
  y = a[0] + a[1]*x[0] + a[2]*x[1] + a[3]*x[2] + a[4]*x[3];
/*.....................................................................*/
  
   if (y < 0) {
    printf("Visosidade dinamica negativa\n!!");
    exit(EXIT_FAILURE);
  }

/*...*/
  d = 1.e-3;
/*.....................................................................*/

  return d*y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 28/08/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * WATERTHERMALCONDUCTITY: [W/m.K]                                   *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * t - temperatura em �C                                             *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 * www.engineeringtoolbox.com/water-thermal-properties-d_162.html	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 * range  T=]0,100[                                                  * 
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE waterThermalConductvity(DOUBLE const t) {

  DOUBLE a[MAXPLODEG],x[MAXPLODEG-1],y,d;

  a[0] = 0.5706e+00;
  a[1] = 1.756e-03;
  a[2] = 6.46e-06;

/*... t */  
  x[0] = t;
/*... t*t */
  x[1] = t*t;
  
/*... polinomio*/
  y = a[0] + a[1]*x[0] + a[2]*x[1];
/*.....................................................................*/
  
   if (y < 0) {
    printf("Condutividade termica negativa\n!!");
    exit(EXIT_FAILURE);
  }

/*...*/
  d = 1.e-3;
/*.....................................................................*/

  return d*y;

}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 12/07/2018                                   *
 *-------------------------------------------------------------------*
 * UPDATEDENSITY:                                                    *
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
void updateDensity(PropPol *pDen
                 , DOUBLE *RESTRICT temp    , DOUBLE *RESTRICT pressure
                 , DOUBLE *RESTRICT density                 
                 , DOUBLE const alpha       , bool const iKelvin    
                 , INT const nEl            , char  const iCod)

{
  short nD = DENSITY_LEVEL;
  INT i;
  DOUBLE den,den0;
/*...*/
  switch (iCod){
    case PROP_UPDATE_NL_LOOP:
      for(i=0;i<nEl;i++){
        den0 =  MAT2D(i,2 ,density ,nD);         
        den = airDensity(pDen,temp[i],pressure[i],thDynamic.pTh[2]
                        ,iKelvin);
/*...*/           
        MAT2D(i,TIME_N ,density ,nD) =  alpha*den + (1.e0-alpha)*den0;
      }
/*..................................................................*/
    break;  

  case PROP_UPDATE_OLD_TIME:
    for(i=0;i<nEl;i++){
/*...t(n-2) = t(n-1)*/
      MAT2D(i,TIME_N_MINUS_2 ,density ,nD) = MAT2D(i,1 ,density ,nD);
/*...t(n-1) = t(n)*/           
      MAT2D(i,TIME_N_MINUS_1 ,density ,nD) = MAT2D(i,2 ,density ,nD);
    }
/*..................................................................*/
    break;
  }
/*..................................................................*/

}
/*********************************************************************/ 

/*********************************************************************
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 15/07/2018                                   *
 *-------------------------------------------------------------------*
 * UPDATESPECIFICHEAT:                                               *
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
void updateSpecificHeat( PropPol *sHeatPol
                       , DOUBLE *RESTRICT temp, DOUBLE *RESTRICT sHeat
                       , bool const iKelvin 
                       ,INT const nEl        , char  const iCod)

{
  short nD = SHEAT_LEVEL;
  INT i;  
  
/*...*/
  switch (iCod)
  {
    case PROP_UPDATE_NL_LOOP:
      for(i=0;i<nEl;i++)
/*...*/           
        MAT2D(i,TIME_N ,sHeat ,nD) = airSpecifiHeat(sHeatPol,temp[i],iKelvin);
/*..................................................................*/
    break;  

  case PROP_UPDATE_OLD_TIME:
    for(i=0;i<nEl;i++){
/*...*/
      MAT2D(i, TIME_N_MINUS_2, sHeat, nD) = MAT2D(i,1 ,sHeat ,nD);           
      MAT2D(i, TIME_N_MINUS_1, sHeat, nD) = MAT2D(i,2 ,sHeat ,nD);
    }
/*..................................................................*/
    break;
  }
/*..................................................................*/
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 12/07/2018                                   *
 *-------------------------------------------------------------------*
 * UPDATESPECIFICHEAT:                                               *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * thC     -> intepolacao a condutividade termica                    *
 * t       -> temperatura                                            *
 * thCond  -> vetor de condutividade termica por celula              *
 * iKelvin -> kelvin ou celsus                                       *
 * nEl     -> numero total de celulas                                *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateThermalconductivity(PropPol *thC
                              ,DOUBLE *RESTRICT t,DOUBLE *RESTRICT thCond   
                              ,bool const iKelvin,INT const nEl)
{
  INT i;

  for(i=0;i<nEl;i++)         
    thCond[i] = airThermalConductvity(thC,t[i],iKelvin);

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 01/09/2017                                   *
 * Data de modificaco : 15/07/2018                                   *
 *-------------------------------------------------------------------*
 * updateDynamicViscosity:                                           *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * dVisc   -> intepolacao da viscosidade dinamica                    *
 * t       -> temperatura                                            *
 * visc    -> vetor de viscosidades dianamica por celula             *
 * iKelvin -> kelvin ou celsus                                       *
 * nEl     -> numero total de celulas                                *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateDynamicViscosity(PropPol *dVisc
                          ,DOUBLE *RESTRICT temp ,DOUBLE *RESTRICT visc    
                          ,bool const iKelvin    ,INT const nEl)
{
  INT i;

  for(i=0;i<nEl;i++)         
    visc[i] = airDynamicViscosity(dVisc,temp[i],iKelvin);

}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 13/05/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * updateProp:                                                       *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * pol     - polinomico de interpolacao com u                        *
 * u       - variavel                                                *
 * coef    - coeficiente a ser atualizado                            *
 * nEl     - numero de elementos                                     *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateProp(PropPol *pol         , DOUBLE *RESTRICT u
              , DOUBLE *RESTRICT coef, INT nEl)

{
  INT i;

  for (i = 0; i<nEl; i++)
    coef[i] = diffProp(pol, u[i]);

}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 12/05/2018                                   *
* Data de modificaco : 00/00/0000                                   *
*-------------------------------------------------------------------*
* UPDATESPECIFICHEAT:                                               *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* pol     - polinomico de interpolacao com u                        *
* u       - variavel                                                *
* density - massa especifica                                        *
* nEl     - numero de elementos                                     *
* iCod    - codigo                                                  *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
*-------------------------------------------------------------------*
* OBS:                                                              *
*-------------------------------------------------------------------*
*********************************************************************/
void updateDensityCD(PropPol *pol            , DOUBLE *RESTRICT u
                   , DOUBLE *RESTRICT density, INT nEl    
                   , char  iCod)

{
  short nD = SHEAT_LEVEL;
  INT i;

/*...*/
  switch (iCod) {
  case PROP_UPDATE_NL_LOOP:
    for (i = 0; i<nEl; i++)
/*...*/
      MAT2D(i, TIME_N, density, nD) = diffProp(pol,u[i]);
/*..................................................................*/
    break;

  case PROP_UPDATE_OLD_TIME:
    for (i = 0; i<nEl; i++) {
/*...*/
      MAT2D(i,TIME_N_MINUS_2,density, nD) = MAT2D(i,1,density, nD);
      MAT2D(i,TIME_N_MINUS_1,density, nD) = MAT2D(i,2,density, nD);
    }
/*..................................................................*/
    break;
  }
/*..................................................................*/
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 27/08/2017                                   *
 * Data de modificaco : 12/07/2018                                   *
 *-------------------------------------------------------------------*
 * INITPROPTEMP: inicializao de propriedades com variacao temporal   *
 * dependentes da temperatura                                        *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * propFluid -> estrutura de dados das propriedades varaiveis        *
 * prop      -> nao definido                                         * 
 * t         -> temperatura                                          *
 * pressure  -> pressao                                              *
 * propMat   -> propriedade de referencia por material               * 
 * mat       -> material por celula                                  * 
 * np        -> numero niveis de tempos                              * 
 * nCell     -> numero de celulas                                    * 
 * iKelvin   -> temperatura em kelvin (true|false)                   *
 * iProp     -> numero da propriedade                                * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * prop    -> propriedade iniciacializada                            * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
  *********************************************************************/
void initPropTemp(PropVarFluid *propFluid
                 ,DOUBLE *RESTRICT prop   ,DOUBLE *RESTRICT t 
                 ,DOUBLE *RESTRICT pressure,DOUBLE *RESTRICT propMat
                 ,short *RESTRICT mat
                 ,short const np          ,INT    const nCell 
                 ,bool const iKelvin      ,short const iProp)
{    
  INT i;
  unsigned short j,lMat;         
  for(i=0;i<nCell;i++){    

/*...*/
    lMat               = mat[i]-1;
/*...................................................................*/

/*...*/
    if( iProp == DENSITY )
      MAT2D(lMat, iProp, propMat, MAXPROP) 
        = airDensity(&propFluid->den,t[i], pressure[i]
                    ,thDynamic.pTh[2], iKelvin);
/*...................................................................*/

/*...*/
    else if( iProp == SPECIFICHEATCAPACITYFLUID)  
      MAT2D(lMat, iProp, propMat, MAXPROP) 
      = airSpecifiHeat(&propFluid->sHeat,t[i],iKelvin);
/*...................................................................*/

/*...*/
    else if( iProp == DYNAMICVISCOSITY)  
      MAT2D(lMat, iProp, propMat, MAXPROP) 
      = airDynamicViscosity(&propFluid->dVisc,t[i],iKelvin);
/*...................................................................*/

/*...*/
    else if( iProp == THERMALCONDUCTIVITY)  
      MAT2D(lMat,iProp,propMat,MAXPROP) 
      = airThermalConductvity(&propFluid->thCond,t[i],iKelvin);
/*...................................................................*/

/*...*/
    for(j=0;j<np;j++)      
      MAT2D(i,j,prop,np) = MAT2D(lMat,iProp,propMat,MAXPROP); 
/*...................................................................*/
  
  }
}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 12/05/2018                                   *
* Data de modificaco : 00/00/0000                                   *
*-------------------------------------------------------------------*
* initPropCD: inicializao de propriedades com variacao de           *
* temperatura                                                       *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* pol     -> polinomio de baixa ordem                               *
* prop    -> nao definido                                           *
* u       -> temperatura                                            *
* propMat -> propriedade de referencia por material                 *
* mat     -> material por celula                                    *
* np      -> numero niveis de tempos                                *
* nCell   -> numero de celulas                                      *
* iProp   -> numero da propriedade                                  *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
* prop    -> propriedade iniciacializada                            *
*-------------------------------------------------------------------*
* OBS:                                                              *
*-------------------------------------------------------------------*
*********************************************************************/
void initPropCD(PropPol *pol            , DOUBLE *RESTRICT prop   
              , DOUBLE *RESTRICT u      , DOUBLE *RESTRICT propMat
              , short *RESTRICT mat
              , short np                , INT    nCell
              , short iProp)
{
  INT i;
  unsigned short j, lMat;
/*...*/
  for (i = 0; i<nCell; i++) 
  {
/*...*/
    lMat = mat[i] - 1;
/*...................................................................*/

/*...*/
    MAT2D(lMat, iProp, propMat, MAXPROP) = diffProp(pol, u[i]);
/*...................................................................*/

/*...*/
    for (j = 0; j<np; j++)
      MAT2D(i, j, prop, np) = MAT2D(lMat, iProp, propMat, MAXPROP);
/*...................................................................*/
  }
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 04/09/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * INITSHEATPOL: inicializao a estrutura para o calculo do calor     *
 * especifico em funcao da temperatura via polinomio                 *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * http://www.engineeringtoolbox.com/air-properties-d_156.html  	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 *********************************************************************/
void initSheatPol(PropPol *prop, char *s, FILE *file) {

  FILE *fileOut;
  char nameAux[1000];
  short i;
  double x[MAXPLODEG];

  if (!strcmp(s, "polinomio")) {
    prop->type = POL;
    fscanf(file, "%s", nameAux);
    fileOut = openFile(nameAux, "r");

    prop->nPol[0] = readFileLineSimple(x, fileOut);
    ERRO_POL_READ(prop->nPol[0], MAXPLODEG, __FILE__, __func__, __LINE__);

    for (i = 0; i < prop->nPol[0]; i++)
      prop->a[i] = x[i];

    fclose(fileOut);

    fprintf(fileLogExc, "%-25s: %s\n", "Type", s);

  }

}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 16/09/2017                                   *
 * Data de modificaco : 16/05/2019                                   *
 *-------------------------------------------------------------------*
 * INITVISCOSITYPOL: inicializao a estrutura para o calculo da       *
 * viscosidade dinamica via polinomio                                *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * http://www.engineeringtoolbox.com/air-properties-d_156.html  	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 *********************************************************************/
void initDviscosityPol(PropPol *prop, char *s, FILE *file) {

  FILE *fileOut;
  char nameAux[1000];
  short i;
  double x[MAXPLODEG];

  if (!strcmp(s, "polinomio")) 
  {
    prop->type = POL;
    fscanf(file, "%s", nameAux);
    fileOut = openFile(nameAux, "r");

    prop->nPol[0] = readFileLineSimple(x, fileOut);
    ERRO_POL_READ(prop->nPol[0], MAXPLODEG, __FILE__, __func__, __LINE__);

    for (i = 0; i < prop->nPol[0]; i++)
      prop->a[i] = x[i];

    fclose(fileOut);

    fprintf(fileLogExc, "%-25s: %s\n", "Type", s);

  }

  else if(!strcmp(s,"sutherland"))
  {
    prop->type = SUTHERLAND;
    prop->a[0] = 1.789e-05; /*viscosidade de referencia*/
    prop->a[1] = 273.11e0;  /*temperatura de referencia*/
    prop->a[2] = 110.56e0;  /*constante de Sutherland*/
  }
  
  else if(!strcmp(s,"fdsviscosity"))
  {
    prop->type = FDSVISCOSITY;
  }

  else if(!strcmp(s,"wilkelaw"))
  {
    prop->type = WILKELAW;
  }

  else {
    ERRO_GERAL(__FILE__,__func__,__LINE__,s);
  }
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 05/11/2017                                   *
 * Data de modificaco : 07/05/2019                                   *
 *-------------------------------------------------------------------*
 * INITDENSITY: inicializao a estrutura para o calculo da            *
 * densidade via polinomio                                           *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * http://www.engineeringtoolbox.com/air-properties-d_156.html  	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 *********************************************************************/
void initDensityPol(PropPol *prop, char *s, FILE *file) {

  FILE *fileOut;
  char nameAux[1000];
  short i;
  double x[MAXPLODEG];

  if(!strcmp(s,"polinomio")){
    prop->type = POL;
    fscanf(file, "%s", nameAux);
    fileOut = openFile(nameAux, "r");

    prop->nPol[0] = readFileLineSimple(x, fileOut);
    ERRO_POL_READ(prop->nPol[0], MAXPLODEG, __FILE__, __func__, __LINE__);

    for (i = 0; i < prop->nPol[0]; i++)
      prop->a[i] = x[i];

    fclose(fileOut);

    fprintf(fileLogExc, "%-25s: %s\n", "Type", s);

  }
/*... ideal gas (p)*/
  else if(!strcmp(s,"idealgas")){
    prop->type = IDEALGAS;
    fprintf(fileLogExc,"%-25s: %s\n","Type",s);
  }
/*... ideal gas incompressivel (PRef)*/
  else if(!strcmp(s,"incidealgas")){
    prop->type = INCIDEALGAS;
    fprintf(fileLogExc,"%-25s: %s\n","Type",s);
  }
  else {
    ERRO_GERAL(__FILE__,__func__,__LINE__,s);
  }
}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 05/11/2017                                   *
* Data de modificaco : 07/05/2019                                   *
*-------------------------------------------------------------------*
* initCdPol: inicializao a estrutura para o calculo da              *
* propriedade via polinomio                                         *
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
void initCdPol(PropPol *prop,char *s,FILE *file)
{

  FILE *fileOut;  
  char nameAux[1000];
  short i;
  double x[MAXPLODEG];

  if (!strcmp(s, "polinomio")) 
  {
    prop->type = POL;
    fscanf(file, "%s", nameAux);
    fileOut = openFile(nameAux, "r");

    prop->nPol[0] = readFileLineSimple(x,fileOut);
    ERRO_POL_READ(prop->nPol[0], MAXPLODEG, __FILE__,__func__, __LINE__);

    for (i = 0; i < prop->nPol[0]; i++)
      prop->a[i] = x[i];

    fclose(fileOut);

    fprintf(fileLogExc, "%-25s: %s\n", "Type", s);
  }
  else 
  {
    ERRO_GERAL(__FILE__, __func__, __LINE__, s);
  }
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 16/09/2017                                   *
 * Data de modificaco : 18/05/2019                                   *
 *-------------------------------------------------------------------*
 * INITTHCONDPOL: inicializao a estrutura para o calculo da          *
 * condutividade termica via polinomio                               *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 * http://www.engineeringtoolbox.com/air-properties-d_156.html  	   *
 *                                                                   *
 * regressao com polinomio de ordem 5 obtido pelo excel              *
 *                                                                   *
 *********************************************************************/
void initThCondPol(PropPol *prop, char *s, FILE *file) {

  FILE *fileOut;
  char nameAux[1000];
  short i;
  double x[MAXPLODEG];

  if(!strcmp(s,"polinomio"))
  {

    prop->type = POL;
    fscanf(file, "%s", nameAux);
    fileOut = openFile(nameAux, "r");

    prop->nPol[0] = readFileLineSimple(x, fileOut);
    ERRO_POL_READ(prop->nPol[0], MAXPLODEG, __FILE__, __func__, __LINE__);

    for (i = 0; i < prop->nPol[0]; i++)
      prop->a[i] = x[i];

    fclose(fileOut);

    fprintf(fileLogExc, "%-25s: %s\n", "Type", s);
  }
  else if(!strcmp(s,"sutherland"))
  {
    prop->type = SUTHERLAND;
    prop->a[0] = 1.789e-05; /*viscosidade de referencia*/
    prop->a[1] = 273.11e0;  /*temperatura de referencia*/
    prop->a[2] = 110.56e0;  /*constante de Sutherland*/
  }
  else if(!strcmp(s,"fdsthermalcond"))
  {
    prop->a[0] = 0.7e0;     /*numero de Prandlt*/
    prop->type = FDSTHERMALCOND;
  }

  else if(!strcmp(s,"wilkelaw"))
  {
    prop->a[0] = 0.7e0;     /*numero de Prandlt*/
    prop->type = WILKELAW;
  }
  else {
    ERRO_GERAL(__FILE__,__func__,__LINE__,s);
  }
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 21/09/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * INITDIFFSP:                                                       *
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
void initDiffSp(PropPol *prop, char *s, FILE *file) {

  FILE *fileOut;
  char nameAux[1000];
  short i;
  double x[MAXPLODEG];

  if(!strcmp(s,"fdsdiff"))
  {
    prop->type = FDSDIFF;
  }
  else if(!strcmp(s,"hirsch"))
  {
    prop->type = HIRSCHDIFF;
  }
  else {
    ERRO_GERAL(__FILE__,__func__,__LINE__,s);
  }
}
/*********************************************************************/


/********************************************************************* 
 * Data de criacao    : 06/09/2017                                   *
 * Data de modificaco : 15/07/2018                                   *
 *-------------------------------------------------------------------*
 * GETTEMPFORENERGY: obtem a temperatura aprtir da entalpia sensivel *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * temp   - nao definido                                             *
 * energy - entalpia sensivel                                        * 
 * prop   - propriedades por material                                *
 * mat    - material da celula                                       *
 * nCell  - numero da celulas                                        *
 * fTemp  - equaca da energia na forma de temperatura (true|false)   *
 * fsHeat - variacao do calor especifico em funcao da Temperatura    *
 *          (true|false)                                             *
 * fKelnvin - temperatura em kelvin (true|false)                     *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * temp   - temperatura (C ou kelvin)                                * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void getTempForEnergy(PropPol *sHeatPol
                     ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                     ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                     ,INT const nCell      ,bool const fTemp
                     ,bool const fSheat    ,bool const fKelvin
                     ,bool const fOmp      ,short const nThreads ){
  
  short lMat;
  INT i;  
  DOUBLE sHeatRef;

/*... resolucao da eq da energia na forma de temperatura*/ 
  if(fTemp)
    for (i = 0; i < nCell; i++)
      temp[i] = energy[i]; 
/*...................................................................*/ 

/*... resolucao da eq da energia na forma de entalpia sensivel*/  
  else{
    if(fOmp){
#pragma omp parallel  for default(none) num_threads(nThreads)\
      private(i,lMat,sHeatRef) shared(prop,mat,energy,temp,sHeatPol)
      for (i = 0; i < nCell; i++) {
        lMat  = mat[i] - 1;
        sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
        temp[i] = specificEnthalpyForTemp(sHeatPol
                                         , energy[i], sHeatRef
                                         , fSheat  , fKelvin);
      }
/*...................................................................*/
    }
/*...................................................................*/

/*...*/
    else{
      for (i = 0; i < nCell; i++) {
        lMat  = mat[i] - 1;
        sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
        temp[i] = specificEnthalpyForTemp(sHeatPol
                                         , energy[i], sHeatRef
                                         , fSheat   , fKelvin);
      }
/*...................................................................*/
    }
/*...................................................................*/ 
  }
/*...................................................................*/ 

}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 20/08/2018                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * etEnergyForTemp   : obtem a entalpia sensivel apartir da temp     *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------*
 * sHeatPol - estrutra par o polinoimio do calor especifico          *
 * temp   - temp                                                     *
 * energy - nao definido                                             * 
 * prop   - propriedades por material                                *
 * mat    - material da celula                                       *
 * nCell  - numero da celulas                                        *
 * fTemp  - equaca da energia na forma de temperatura (true|false)   *
 * fsHeat - variacao do calor especifico em funcao da Temperatura    *
 *          (true|false)                                             *
 * fKelnvin - temperatura em kelvin (true|false)                     *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * energy - entalpia sensivel                                        * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void getEnergyForTemp(PropPol *sHeatPol
                     ,DOUBLE *RESTRICT temp,DOUBLE *RESTRICT energy
                     ,DOUBLE *RESTRICT prop,short  *RESTRICT mat 
                     ,INT const nCell     
                     ,bool const fSheat    ,bool const fKelvin
                     ,bool const fOmp      ,short const nThreads ) 
{
  short lMat;
  INT i;  
  DOUBLE sHeatRef;

/*...*/
  if (fOmp) 
  {
/*...*/ 
#pragma omp parallel  for default(none) num_threads(nThreads)\
    private(i,lMat,sHeatRef) shared(prop,mat,energy,temp,sHeatPol)
    for (i = 0; i < nCell; i++) 
    {
      lMat  = mat[i] - 1;
      sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
      energy[i] = tempForSpecificEnthalpy(sHeatPol
                                         ,temp[i]  ,sHeatRef
                                         ,fSheat  ,fKelvin);
    }
/*...................................................................*/ 
  }
/*...................................................................*/ 

  else
  {
/*...*/ 
    for (i = 0; i < nCell; i++) 
    {
      lMat  = mat[i] - 1;
      sHeatRef = MAT2D(lMat, SPECIFICHEATCAPACITYFLUID, prop, MAXPROP);
      energy[i] = tempForSpecificEnthalpy(sHeatPol
                                         ,temp[i] ,sHeatRef
                                         ,fSheat  ,fKelvin);
    }
/*...................................................................*/ 
  }
/*...................................................................*/ 
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 15/09/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * SPECIFICMASSREF : calcula a massa especifica de referencia        *
 * atraves da media do valores nas celulas                           * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * temp   - temp                                                     *
 * volume - volume das celulas                                       *
 * prop   - propriedades por material                                *
 * mat    - material da celula                                       *
 * nCell  - numero da celulas                                        *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * densidade de referencia - entalpia sensivel                       * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void specificMassRefOld(DOUBLE *RESTRICT density, DOUBLE *RESTRICT volume                  
                  , DOUBLE *RESTRICT prop    , short  *RESTRICT mat
                  , INT const nCell)
{
  short nD = DENSITY_LEVEL;
  INT i;  
  DOUBLE dm,vm;

  dm = vm = 0.e0;

  for (i = 0; i < nCell; i++) {
/*...*/   
   dm += MAT2D(i,2 ,density ,nD)*volume[i];
   vm += volume[i];
/*...................................................................*/ 
  }

  MAT2D(0,DENSITY,prop,MAXPROP) = dm/vm;  

  printf("densityRef :%e\n",MAT2D(0,DENSITY,prop,MAXPROP));

}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 21/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * SPECIFICMASSREF : calcula a massa especifica de referencia        *
 * atraves da media do valores nas celulas                           * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * temp   - temp                                                     *
 * volume - volume das celulas                                       *
 * nCell  - numero da celulas                                        *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * densidade de referencia                                           * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
DOUBLE specificMassRef(DOUBLE *RESTRICT density, DOUBLE *RESTRICT volume                  
                    , INT const nCell)
{
  short nD = DENSITY_LEVEL;
  INT i;  
  DOUBLE dm,vm;

  dm = vm = 0.e0;

  for (i = 0; i < nCell; i++) {
/*...*/   
   dm += MAT2D(i,2 ,density ,nD)*volume[i];
   vm += volume[i];
/*...................................................................*/ 
  }

  printf("densityRef :%e\n",dm/vm);

  return dm/vm;
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 15/09/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * PRESREF : calcula da pressao de referencia atualizada             *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * temp0  - temperatura no passo n                                   *
 * temp   - temperatura no passo n + 1                               *
 * volume - volume das celulas                                       *
 * pTh    - pressao termodinamica de referencia                      *
 * nCell  - numero da celulas                                        *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * pTh    - pressao termodinamica de referencia atualizada           *
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void presRef(DOUBLE *RESTRICT temp0         , DOUBLE *RESTRICT temp  
                   , DOUBLE *RESTRICT volume, DOUBLE *pTh                               
                   , INT const nCell        , bool const fKelvin)
{
  INT i;  
  DOUBLE dm,vm,t,t0;

  dm = vm = 0.e0;

  for (i = 0; i < nCell; i++) {
/*...*/
    if(fKelvin){
      t  = temp[i];
      t0 = temp0[i];
    }  
    else{
      t  = CELSIUS_FOR_KELVIN(temp[i]); 
      t0 = CELSIUS_FOR_KELVIN(temp0[i]);  
    }
/*...................................................................*/ 

/*...*/   
    dm += t/t0*volume[i];
    vm += volume[i];
/*...................................................................*/ 
  }

  pTh[2] = pTh[1]* dm/vm;
}
/*********************************************************************/

/********************************************************************* 
 * Data de criacao    : 15/09/2017                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * PRESREF : calcula da pressao de referencia atualizada             *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * temp0  - temperatura no passo n                                   *
 * temp   - temperatura no passo n + 1                               *
 * volume - volume das celulas                                       *
 * pTh    - pressao termodinamica de referencia                      *
 * nCell  - numero da celulas                                        *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * pTh    - pressao termodinamica de referencia atualizada           *
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void presRefMix(Combustion *cModel
              , DOUBLE *RESTRICT temp0 , DOUBLE *RESTRICT temp  
              , DOUBLE *RESTRICT yFrac0, DOUBLE *RESTRICT yFrac
              , DOUBLE *RESTRICT volume, DOUBLE *pTh                               
              , INT const nCell        , bool const fKelvin)
{
  short ns = cModel->nOfSpecies;
  INT i;  
  DOUBLE dm,vm,t,t0,mW,mW0,*y,*y0;

  dm = vm = 0.e0;

  for (i = 0; i < nCell; i++) {
/*...*/
    if(fKelvin){
      t  = temp[i];
      t0 = temp0[i];
    }  
    else{
      t  = CELSIUS_FOR_KELVIN(temp[i]); 
      t0 = CELSIUS_FOR_KELVIN(temp0[i]);  
    }
/*...................................................................*/ 

/*...*/
    y  = &MAT2D(i,0,yFrac,ns);
    y0 = &MAT2D(i,0,yFrac0,ns);
    mW0 = mixtureMolarMass(cModel,y0);
    mW  = mixtureMolarMass(cModel,y);
/*...................................................................*/
/*...*/   
    dm += ((t*mW0)/(t0*mW))*volume[i];
    vm += volume[i];
/*...................................................................*/ 
  }

  pTh[2] = pTh[1]* dm/vm;
}
/*********************************************************************/


/********************************************************************* 
 * Data de criacao    : 15/09/2017                                   *
 * Data de modificaco : 26/08/2018                                   *
 *-------------------------------------------------------------------*
 * INIPRESREF : incializa a pressao ref atrazes da massa especifica  * 
 * de referencia e temperatura media do dominio                      *
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * temp   - temperatura no passo n + 1                               *
 * volume - volume das celulas                                       *
 * pTh    - pressao termodinamica de referencia                      *
 * prop                                                              *
 * mat                                                               *
 * molarMass                                                         *
 * nCell  - numero da celulas                                        *
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------*
 * pTh    - pressao termodinamica de referencia atualizada           *
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 *-------------------------------------------------------------------*
 *********************************************************************/
void initPresRef(DOUBLE *RESTRICT temp  , DOUBLE *RESTRICT volume
               , DOUBLE *pTh            , DOUBLE *RESTRICT prop  
               , short  *RESTRICT mat   , DOUBLE const molarMass                  
               , INT const nCell        , bool const fKelvin)
{
  INT i;  
  DOUBLE dRef,tm,dm,vm;

  dm = vm = 0.e0;

  for (i = 0; i < nCell; i++) {

/*...*/   
    dm += temp[i]*volume[i];
    vm += volume[i];
/*...................................................................*/ 
  }
  
  dm = dm/vm;

/*...*/
  if(fKelvin)
    tm  = dm;
  else
    tm  = CELSIUS_FOR_KELVIN(dm); 
/*...................................................................*/ 

  dRef = MAT2D(0,DENSITY,prop,MAXPROP);  
  vm   = PRESREF(dRef, IDEALGASR, tm, molarMass);

  pTh[0] = pTh[1] = pTh[2] = vm;

}
/*********************************************************************/

/**********************************************************************
 * Data de criacao    : 12/05/2018                                    *
 * Data de modificaco : 00/00/0000                                    *
 *--------------------------------------------------------------------*
 * setReGrad:                                                         *
 *--------------------------------------------------------------------*
 * Parametros de entrada:                                             *
 *--------------------------------------------------------------------*
 * x       -> nao definido                                            *
 * file    -> arquivo de arquivo                                      *
 *--------------------------------------------------------------------*
 * Parametros de saida:                                               *
 *--------------------------------------------------------------------*
 * x       -> valores lidos                                           *
 * nTerms  -> retornas o numero de valores lidos                      *
 *--------------------------------------------------------------------*
 * OBS:                                                               *
 *--------------------------------------------------------------------*
 **********************************************************************/
int readFileLineSimple(DOUBLE *a, FILE *file)
{

  int i, nTerms;

  fscanf(file, "%d", &nTerms);

  for (i = 0; i<nTerms; i++)
    fscanf(file, "%lf", a + i);

  return nTerms;

}
/**********************************************************************/

/**********************************************************************
 * Data de criacao    : 02/06/2018                                    *
 * Data de modificaco : 00/00/0000                                    *
 *--------------------------------------------------------------------*
 * initPropCD : inicializacao                                         *
 *--------------------------------------------------------------------*
 * Parametros de entrada:                                             *
 *--------------------------------------------------------------------*
 * propVar -> nao definido                                            *
 * n       -> numero de termors                                       *
 *--------------------------------------------------------------------*
 * Parametros de saida:                                               *
 *--------------------------------------------------------------------*
 * propVar -> inicializado                                            *
 *--------------------------------------------------------------------*
 * OBS:                                                               *
 *--------------------------------------------------------------------*
 **********************************************************************/
void initPropStructCD(PropVarCD *propVar, short const n)
{
  short i;

  for(i=0;i<n;i++)
  {
    propVar[i].fDensity        = false;
    propVar[i].fCeofDiff       = false;
    propVar[i].ceofDiff.type   = -1;
    propVar[i].ceofDiff.nPol[0] = 0;
    propVar[i].den.nPol[0]     = 0;
    propVar[i].den.type        = -1;
  }

}
/**********************************************************************/

/**********************************************************************
 * Data de criacao    : 16/05/2019                                    *
 * Data de modificaco : 00/00/0000                                    *
 *--------------------------------------------------------------------*
 * initLeornadJones : inicializacao                                   *
 *--------------------------------------------------------------------*
 * Parametros de entrada:                                             *
 *--------------------------------------------------------------------*
 *--------------------------------------------------------------------*
 * Parametros de saida:                                               *
 *--------------------------------------------------------------------*
 *--------------------------------------------------------------------*
 * OBS:                                                               *
 *--------------------------------------------------------------------*
 **********************************************************************/
void initLeornadJones(Combustion *cModel)
{
/*... Fuel*/
  cModel->leornadJones[SP_FUEL][0] = 3.758e0;
  cModel->leornadJones[SP_FUEL][1] = 148.6e0;
/*... O2*/
  cModel->leornadJones[SP_O2][0] = 3.467e0;
  cModel->leornadJones[SP_O2][1] = 106.7e0;
/*... N2*/
  cModel->leornadJones[SP_N2][0] = 3.798e0;
  cModel->leornadJones[SP_N2][1] = 71.4e0;
/*... CO2*/
  cModel->leornadJones[SP_CO2][0] = 3.941e0;
  cModel->leornadJones[SP_CO2][1] = 195.2e0;
/*... H2O*/
  cModel->leornadJones[SP_H2O][0] = 2.641e0;
  cModel->leornadJones[SP_H2O][1] = 809.1e0;
}
/**********************************************************************/

/*********************************************************************
 * Data de criacao    : 18/05/2019                                   *
 * Data de modificaco : 00/00/0000                                   * 
 *-------------------------------------------------------------------*
 * INITPROPREF: inicializao de propriedades de referencia            * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * propF   -> estrutura de propriedades variaveis                    * 
 * propMat -> propriedade de referencia por material                 * 
 * lMat    -> matetial do Fluido                                     * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * propF   -> atualizado                                             * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void initPropRef(PropVarFluid *propF ,DOUBLE *RESTRICT propMat
                ,short const lMat)
{

  propF->sHeatRef               =   MAT2D(lMat,SPECIFICHEATCAPACITYFLUID
                                         ,propMat,MAXPROP);
  propF->dViscosityRef          =   MAT2D(lMat,DYNAMICVISCOSITY
                                          ,propMat,MAXPROP);
  propF->ThermalConductivityRef =   MAT2D(lMat,THERMALCONDUCTIVITY
                                         ,propMat,MAXPROP);

}
/**********************************************************************/