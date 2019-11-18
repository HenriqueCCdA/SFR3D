#include<Transient.h>  


/********************************************************************* 
 * Data de criacao    : 25/09/2019                                   *
 * Data de modificaco : 00/00/0000                                   *
 *-------------------------------------------------------------------*
 * changeSchemaTrans : troca a tecnica efetiva da integracao temporal* 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * iStep -> passo de tempo                                           * 
 * type -> nao definido                                              * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * type -> modificado                                                * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              *
 * o BACKWARD utiliza a tecnica de EULER no primeiro passo de tempo  *
 *-------------------------------------------------------------------* 
 *********************************************************************/
void changeSchemeTemporal(Temporal *ddt)
{

  if(ddt->typeReal == BACKWARD)
    ddt->type = BACKWARD; 

}
/*********************************************************************/


/********************************************************************* 
 * Data de criacao    : 00/00/2016                                   *
 * Data de modificaco : 25/09/2019                                   *
 *-------------------------------------------------------------------*
 * SETTRANSIENTSCHEME : set a discretizacao temporal                 * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 * word -> str com a discretizacao                                   * 
 * type -> nao definido                                              * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 * type -> tipo de discretizacao                                     * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
void setTransientScheme(char *word,Temporal *ddt){

  if(!strcmp(word,"EULER"))
  {
    ddt->type     = EULER;
    ddt->typeReal = EULER;
  }
  else if(!strcmp(word,"BACKWARD"))
  {
    ddt->type     = EULER;
    ddt->typeReal = BACKWARD;
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 00/00/2016                                   *
 * Data de modificaco : 27/08/2016                                   *
 *-------------------------------------------------------------------*
 * CELLTRANSIENT : discretizacao temporal                            *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * volume    -> volume das celulas                                   *
 * id        -> numera das equacoes                                  *
 * u0        -> solucao (n-1)                                        *
 * u         -> solucao (n)                                          *
 * density   -> massa especifica no tempo   (n)                      *
 * f         -> vetor de forcas                                      *
 * ddt       -> delta t                                              *
 * numel     -> numero de elementos                                  *
 * ndf       -> graus de liberade                                    *
 * type      -> tipo de discretizacao temporal                       *
 * fAdd      -> acumula o valor em f                                 *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * f -> atualizado com a discreticao temporal ( fAdd = true)         *
 *      sobreescreve com a discreticao temporal (fAdd = false)       *
 *-------------------------------------------------------------------*
 * OBS:                                                              *
 *-------------------------------------------------------------------*
*********************************************************************/
void cellTransient(DOUBLE *RESTRICT volume ,INT *RESTRICT id
                  ,DOUBLE *RESTRICT u0     ,DOUBLE *RESTRICT u
                  ,LevelTime density       ,DOUBLE *RESTRICT f
                  ,Temporal const ddt      ,INT const numel
                  ,short const ndf         ,bool const fAdd)
{
  INT nel, lNeq;
  DOUBLE t0, t00, tmp1;
  DOUBLE dt = ddt.dt[0], dt0 = ddt.dt[1];
  short type = ddt.type;
  short j;

/*...*/
  switch (type) 
  {
/*... EULER de primeira ordem*/
    case EULER:
/*... acumula em f*/
      if (fAdd) 
      {
/*... ndf = 1*/
        if (ndf == 1) 
        {
          for (nel = 0; nel < numel; nel++) 
          {
            lNeq = id[nel] - 1;
            if (lNeq > -1) 
            {
              t0 = density.t0[nel]*u[nel];
              f[lNeq] += volume[nel]*t0/dt;
            }
          }
        }
/*...................................................................*/

/*... ndf > 1*/
        else 
        {
          for (nel = 0; nel < numel; nel++) 
          {
            for (j = 0; j< ndf; j++) 
            {
              lNeq = MAT2D(nel, j, id, ndf) - 1;
              if (lNeq > -1) 
              {
                t0 = density.t0[nel]*MAT2D(nel,j,u,ndf);
                MAT2D(lNeq, j, f, ndf) += volume[nel] * t0 / dt;
              }
            }
          }
        }
/*...................................................................*/
      }
/*...................................................................*/

/*... sobrecreve f*/
      else 
      {
/*... ndf = 1*/
        if (ndf == 1) 
        {
          for (nel = 0; nel < numel; nel++) 
          {
            lNeq = id[nel] - 1;
            if (lNeq > -1)
            {
              t0 = density.t0[nel]*u[nel];
              f[lNeq] = volume[nel]*t0/dt;
            }
          }
        }
/*...................................................................*/

/*... ndf > 1*/
        else 
        {
          for (nel = 0; nel < numel; nel++) 
          {
            for (j = 0; j< ndf; j++) 
            {
              lNeq = MAT2D(nel, j, id, ndf) - 1;
              if (lNeq > -1) 
              {
                t0 = density.t0[nel]*MAT2D(nel,j,u,ndf);
                MAT2D(lNeq,j,f,ndf) = volume[nel]*t0/dt;
              }
            }
          }
        }
/*...................................................................*/
      }
/*...................................................................*/
    break;
/*...................................................................*/

/*... BACKWARD de segunda ordem*/
  case BACKWARD:
/*... acumula em f*/
    if (fAdd) 
    {
/*... ndf = 1*/
      if (ndf == 1) 
      {
        for (nel = 0; nel < numel; nel++) 
        {
          lNeq = id[nel] - 1;
          if (lNeq > -1) 
          {
            tmp1 = dt + dt0;
/*...*/
            t0 = density.t0[nel]*u[nel];
            t0 *= (tmp1/(dt*dt0));
/*...................................................................*/

/*...*/
            t00= density.t00[nel]*u0[nel];
            t00*= (dt/(dt0*tmp1));
/*...................................................................*/
            f[lNeq] += volume[nel]*(t0-t00);
          }
        }
      }
/*...................................................................*/

/*... ndf > 1*/
      else 
      {
        for (nel = 0; nel < numel; nel++) 
        {
          for (j = 0; j< ndf; j++) 
          {
            lNeq = MAT2D(nel, j, id, ndf) - 1;
            if (lNeq > -1) 
            {
              tmp1 = dt + dt0;
/*...*/
              t0 = density.t0[nel]*MAT2D(nel, j, u, ndf);
              t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*...*/
              t00= density.t00[nel]*MAT2D(nel, j, u0, ndf);
              t00*= (dt / (dt0*tmp1));
/*...................................................................*/
              MAT2D(lNeq,j,f,ndf) += volume[nel] * (t0 - t00);
            }
          }
        }
      }
/*...................................................................*/
    }
/*...................................................................*/

/*... sobrecreve f*/
    else 
    {
/*... ndf = 1*/
      if (ndf == 1) 
      {
        for (nel = 0; nel < numel; nel++) 
        {
          lNeq = id[nel] - 1;
          if (lNeq > -1) 
          {
            tmp1 = dt + dt0;
/*...*/
            t0 = density.t0[nel]*u[nel];
            t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*...*/
            t00= density.t00[nel]*u0[nel];
            t00*= (dt / (dt0*tmp1));
/*...................................................................*/
            f[lNeq] = volume[nel] *(t0-t00);
          }
        }
      }
/*...................................................................*/

/*... ndf > 1*/
      else 
      {
        for (nel = 0; nel < numel; nel++) 
        {
          for (j = 0; j< ndf; j++) 
          {
            lNeq = MAT2D(nel, j, id, ndf) - 1;
            if (lNeq > -1) 
            {
              tmp1 = dt + dt0;
/*...*/
              t0 = density.t0[nel]*MAT2D(nel,j,u,ndf);
              t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*...*/
              t00= density.t00[nel]*MAT2D(nel,j,u0,ndf);
              t00*= (dt / (dt0*tmp1));
/*...................................................................*/
              MAT2D(lNeq,j,f,ndf) = volume[nel]*(t0-t00);
            }
          }
        }
      }
/*...................................................................*/
    }
/*...................................................................*/
    break;
  }
}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 00/00/2016                                   *
* Data de modificaco : 26/08/2017                                   *
*-------------------------------------------------------------------*
* CELLTRANSIENTEnergy : discretizacao temporal                      *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* volume    -> volume das celulas                                   *
* id        -> numera das equacoes                                  *
* u0        -> solucao (n-1)                                        *
* u         -> solucao (n)                                          *
* density   -> massa especifica no tempo   (n,n-1)                  *
* sHeat     -> calor especifico no tempo   (n,n-1)                  *
* f         -> vetor de forcas                                      *
* ddt       -> delta t                                              *
* numel     -> numero de elementos                                  *
* type      -> tipo de discretizacao temporal                       *
* fAdd      -> acumula o valor em f                                 *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
* f -> atualizado com a discreticao temporal ( fAdd = true)         *
*      sobreescreve com a discreticao temporal (fAdd = false)       *
*-------------------------------------------------------------------*
* OBS:                                                              *
*-------------------------------------------------------------------*
*********************************************************************/
void cellTransientEnergy(DOUBLE *RESTRICT volume ,INT *RESTRICT id
                        ,DOUBLE *RESTRICT u0     ,DOUBLE *RESTRICT u
                        ,LevelTime density       ,LevelTime sHeat
                        ,DOUBLE *RESTRICT f
                        ,Temporal const ddt      ,INT const numel
                        ,bool const fAdd)
{
  INT nel, lNeq;
  DOUBLE t0, t00, tmp1,density0,sHeat0;
  DOUBLE dt = ddt.dt[0], dt0 = ddt.dt[1];
  short type = ddt.type;

/*...*/
  switch (type) {
/*... EULER de primeira ordem*/
  case EULER:
/*... acumula em f*/
    if (fAdd) {
/*... ndf = 1*/
      for (nel = 0; nel < numel; nel++) {
        lNeq = id[nel] - 1;
        if (lNeq > -1) {
/*...*/
          density0 = density.t0[nel];
          sHeat0   = sHeat.t0[nel];
/*...................................................................*/
          f[lNeq] += volume[nel] * sHeat0*density0*u[nel] / dt;
        }
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/

/*... sobrecreve f*/
    else {
/*... */
      for (nel = 0; nel < numel; nel++) {
        lNeq = id[nel] - 1;
        if (lNeq > -1){
/*...*/
          density0 = density.t0[nel];
          sHeat0   = sHeat.t0[nel];
/*...................................................................*/
          f[lNeq] = volume[nel]*sHeat0*density0*u[nel] / dt;
        }
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/
    break;
/*...................................................................*/

/*... BACKWARD de segunda ordem*/
  case BACKWARD:
/*... acumula em f*/
    if (fAdd) {
      for (nel = 0; nel < numel; nel++) {
        lNeq = id[nel] - 1;
        if (lNeq > -1) {
          tmp1 = dt + dt0;
 /*... t(n-1)*/
          density0 = density.t0[nel];
          sHeat0   = sHeat.t0[nel];
 /*...................................................................*/

 /*...*/
          t0 = sHeat0*density0*u[nel];
          t0 *= (tmp1 / (dt*dt0));
 /*...................................................................*/

 /*...t(n-2)*/
          density0 = density.t00[nel];
          sHeat0   = sHeat.t00[nel];
 /*...................................................................*/

 /*...*/
          t00 = sHeat0*density0*u0[nel];
          t00 *= (dt / (dt0*tmp1));
 /*...................................................................*/
          f[lNeq] += volume[nel] * (t0 - t00);
        }
/*...................................................................*/
      }
/*...................................................................*/
    }
/*...................................................................*/

/*... sobrecreve f*/
    else {
      for (nel = 0; nel < numel; nel++) {
        lNeq = id[nel] - 1;
        if (lNeq > -1) {
          tmp1 = dt + dt0;
 /*... t(n-1)*/
          density0 = density.t0[nel];
          sHeat0   = sHeat.t0[nel];
/*...................................................................*/

/*... t(n-2)*/
          t0  = sHeat0*density0*u[nel];
          t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*...*/ 
          density0 = density.t00[nel];
          sHeat0   = sHeat.t00[nel];
/*...................................................................*/

/*...*/
          t00= sHeat0*density0*u0[nel];
          t00*= (dt / (dt0*tmp1));
/*...................................................................*/
          f[lNeq] = volume[nel] * (t0 - t00);
        }
/*...................................................................*/
      } 
/*...................................................................*/
    }
/*...................................................................*/

    break;
  }
}
/*********************************************************************/

/*********************************************************************
* Data de criacao    : 18/11/2019                                   *
* Data de modificaco : 00/00/0000                                   *
*-------------------------------------------------------------------*
* CELLTRANSIENTSIMPLEINC : discretizacao temporal                   *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* volume    -> volume das celulas                                   *
* id        -> numera das equacoes                                  *
* u0        -> solucao (n-1)                                        *
* u         -> solucao (n)                                          *
* f         -> vetor de forcas                                      *
* ddt       -> delta t                                              *
* nEq       -> numero de equacoes                                   *
* numel     -> numero de elementos                                  *
* ndf       -> graus de liberade                                    *
* fAdd      -> acumula o valor em f                                 *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
* f -> atualizado com a discreticao temporal ( fAdd = true)         *
*      sobreescreve com a discreticao temporal (fAdd = false)       *
*-------------------------------------------------------------------*
* OBS: f | bu(1) bu(2) ... bu(neq) |                                *
*        | bv(1) bv(2) ... bv(neq) |                                *
*        | bw(1) bw(2) ... bw(neq) |                                *
*-------------------------------------------------------------------*
*********************************************************************/
void cellTransientSimpleInc(DOUBLE *RESTRICT volume ,INT *RESTRICT id
                        ,DOUBLE *RESTRICT prop      ,short  *RESTRICT mat  
                        ,DOUBLE *RESTRICT u0        ,DOUBLE *RESTRICT u
                        ,DOUBLE *RESTRICT f
                        ,Temporal const ddt         ,INT const nEq
                        ,INT const numel            ,short const ndf
                        ,bool const fAdd)
{
  INT nel,lNeq;
  DOUBLE t0,t00,tmp1,den;
  DOUBLE dt=ddt.dt[0],dt0 = ddt.dt[1];
  short type = ddt.type;
  short j,lMat;

  /*...*/
  switch (type) {
/*... EULER de primeira ordem*/
    case EULER:
/*... acumula em f*/
      if (fAdd) {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lMat = mat[nel] - 1;
            den = MAT2D(lMat, DENSITY, prop, MAXPROP);
            lNeq = id[nel] - 1;
            if (lNeq > -1) {
              t0 = den*MAT2D(nel,j,u,ndf);
              MAT2D(j, lNeq, f, nEq) += volume[nel]*t0/dt;
            }
          }
        } 
/*...................................................................*/
      }
/*...................................................................*/

/*... sobrecreve f*/
      else {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lMat = mat[nel] - 1;
            den = MAT2D(lMat, DENSITY, prop, MAXPROP);
            lNeq = id[nel] - 1;
            if (lNeq > -1) {
              t0 = den*MAT2D(nel,j,u,ndf);
              MAT2D(j, lNeq, f, nEq) = volume[nel]*t0/dt;
            }
          }
        }
/*...................................................................*/
      }
/*...................................................................*/
    break;
/*...................................................................*/

/*... BACKWARD de segunda ordem*/
    case BACKWARD:
/*... acumula em f*/
      if (fAdd) {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lNeq = id[nel] - 1;
            lMat = mat[nel] - 1;
            den = MAT2D(lMat, DENSITY, prop, MAXPROP);
            if (lNeq > -1) {
              tmp1 = dt + dt0;
/*...*/
              t0 = den*MAT2D(nel,j,u,ndf);
              t0 *= (tmp1/(dt*dt0));
/*...................................................................*/

/*...*/
              t00 = den*MAT2D(nel,j,u0,ndf);
              t00*= (dt/(dt0*tmp1));
/*...................................................................*/
              MAT2D(j,lNeq,f,nEq) += volume[nel]*(t0-t00);
            }
          }
        }
      }
/*...................................................................*/

/*... sobrecreve f*/
      else {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lNeq = id[nel] - 1;
            lMat = mat[nel] - 1;
            den = MAT2D(lMat, DENSITY, prop, MAXPROP);
            if (lNeq > -1) {
              tmp1 = dt + dt0;
/*..*/
              t0 = den*MAT2D(nel, j, u, ndf);
              t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*..*/
              t00= den*MAT2D(nel, j, u0, ndf);
              t00*= (dt / (dt0*tmp1));
/*...................................................................*/
              MAT2D(j, lNeq, f, nEq) = volume[nel] * (t0 - t00);
            }
          }
        }
      }
/*...................................................................*/
    break;
  }
}
/*********************************************************************/


/*********************************************************************
* Data de criacao    : 00/00/2016                                   *
* Data de modificaco : 27/08/2016                                   *
*-------------------------------------------------------------------*
* CELLTRANSIENTSIMPLE : discretizacao temporal                      *
*-------------------------------------------------------------------*
* Parametros de entrada:                                            *
*-------------------------------------------------------------------*
* volume    -> volume das celulas                                   *
* id        -> numera das equacoes                                  *
* u0        -> solucao (n-1)                                        *
* u         -> solucao (n)                                          *
* density   -> massa especifica no tempo   (n)                      *
* f         -> vetor de forcas                                      *
* ddt       -> delta t                                              *
* nEq       -> numero de equacoes                                   *
* numel     -> numero de elementos                                  *
* ndf       -> graus de liberade                                    *
* fAdd      -> acumula o valor em f                                 *
*-------------------------------------------------------------------*
* Parametros de saida:                                              *
*-------------------------------------------------------------------*
* f -> atualizado com a discreticao temporal ( fAdd = true)         *
*      sobreescreve com a discreticao temporal (fAdd = false)       *
*-------------------------------------------------------------------*
* OBS: f | bu(1) bu(2) ... bu(neq) |                                *
*        | bv(1) bv(2) ... bv(neq) |                                *
*        | bw(1) bw(2) ... bw(neq) |                                *
*-------------------------------------------------------------------*
*********************************************************************/
void cellTransientSimple(DOUBLE *RESTRICT volume ,INT *RESTRICT id
                        ,DOUBLE *RESTRICT u0     ,DOUBLE *RESTRICT u
                        ,LevelTime density       ,DOUBLE *RESTRICT f
                        ,Temporal const ddt      ,INT const nEq
                        ,INT const numel         ,short const ndf
                        ,bool const fAdd)
{
  INT nel,lNeq;
  DOUBLE t0,t00,tmp1;
  DOUBLE dt=ddt.dt[0],dt0 = ddt.dt[1];
  short type = ddt.type;
  short j;

  /*...*/
  switch (type) {
/*... EULER de primeira ordem*/
    case EULER:
/*... acumula em f*/
      if (fAdd) {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lNeq = id[nel] - 1;
            if (lNeq > -1) {
              t0 = density.t0[nel]*MAT2D(nel,j,u,ndf);
              MAT2D(j, lNeq, f, nEq) += volume[nel]*t0/dt;
            }
          }
        } 
/*...................................................................*/
      }
/*...................................................................*/

/*... sobrecreve f*/
      else {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lNeq = id[nel] - 1;
            if (lNeq > -1) {
              t0 = density.t0[nel]*MAT2D(nel,j,u,ndf);
              MAT2D(j, lNeq, f, nEq) = volume[nel]*t0/dt;
            }
          }
        }
/*...................................................................*/
      }
/*...................................................................*/
    break;
/*...................................................................*/

/*... BACKWARD de segunda ordem*/
    case BACKWARD:
/*... acumula em f*/
      if (fAdd) {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lNeq = id[nel] - 1;
            if (lNeq > -1) {
              tmp1 = dt + dt0;
/*...*/
              t0 = density.t0[nel]*MAT2D(nel,j,u,ndf);
              t0 *= (tmp1/(dt*dt0));
/*...................................................................*/

/*...*/
              t00 = density.t00[nel]*MAT2D(nel,j,u0,ndf);
              t00*= (dt/(dt0*tmp1));
/*...................................................................*/
              MAT2D(j,lNeq,f,nEq) += volume[nel]*(t0-t00);
            }
          }
        }
      }
/*...................................................................*/

/*... sobrecreve f*/
      else {
        for (j = 0; j< ndf; j++) {
          for (nel = 0; nel < numel; nel++) {
            lNeq = id[nel] - 1;
            if (lNeq > -1) {
              tmp1 = dt + dt0;
/*..*/
              t0 = density.t0[nel]*MAT2D(nel, j, u, ndf);
              t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*..*/
              t00= density.t00[nel]*MAT2D(nel, j, u0, ndf);
              t00*= (dt / (dt0*tmp1));
/*...................................................................*/
              MAT2D(j, lNeq, f, nEq) = volume[nel] * (t0 - t00);
            }
          }
        }
      }
/*...................................................................*/
    break;
  }
}
/*********************************************************************/

/*********************************************************************
 * Data de criacao    : 16/09/2016                                   *
 * Data de modificaco : 02/09/2017                                   *
 *-------------------------------------------------------------------*
 * CELLTRANSIENTPRIME:discretizacao temporal                         *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 * volume    -> volume das celulas                                   *
 * u0        -> solucao (n-1)                                        *
 * u         -> solucao (n)                                          *
 * density   -> massa especifica no tempo   (n)                      *
 * f         -> vetor de forcas                                      *
 * ddt       -> delta t                                              *
 * nEq       -> numero de equacoes                                   *
 * numel     -> numero de elementos                                  *
 * ndf       -> graus de liberade                                    *
 * fAdd      -> acumula o valor em f                                 *
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 * f -> atualizado com a discreticao temporal ( fAdd = true)         *
 *      sobreescreve com a discreticao temporal (fAdd = false)       *
 *-------------------------------------------------------------------*
 * OBS: f | bu(1) bv(1) bw(1) |                                      *
 *        |       ...         |                                      *
 *        | bu(n) bv(n) bw(n) |                                      *
 *-------------------------------------------------------------------*
 *********************************************************************/
void cellTransientPrime(DOUBLE *RESTRICT volume 
                       ,DOUBLE *RESTRICT u0     ,DOUBLE *RESTRICT u
                       ,DOUBLE *RESTRICT density,DOUBLE *RESTRICT f
                       ,Temporal const ddt      
                       ,INT const numel         ,short const ndf
                       ,bool const fAdd) 
{
  INT nel;
  DOUBLE t0,t00,tmp1;
  DOUBLE dt=ddt.dt[0],dt0 = ddt.dt[1];
  short type = ddt.type;
  short j;

  /*...*/
  switch (type) {
/*... EULER de primeira ordem*/
    case EULER:
/*... acumula em f*/
      if (fAdd) {
        for (nel = 0; nel < numel; nel++) { 
          for (j = 0; j< ndf; j++) {
//          t0 = MAT2D(nel,0,density,nD)*MAT2D(nel,j,u,ndf);
            MAT2D(nel,j,f,ndf) += volume[nel]*t0/dt;
          }
        }
      } 
/*...................................................................*/

/*... sobrecreve f*/
      else {
       for (nel = 0; nel < numel; nel++) { 
          for (j = 0; j< ndf; j++) {
//          t0 = MAT2D(nel,0,density,nD)*MAT2D(nel,j,u,ndf);
            MAT2D(nel,j,f,ndf) = volume[nel]*t0/dt;
          }
        }
      }
/*...................................................................*/
    break;
/*...................................................................*/

/*... BACKWARD de segunda ordem*/
    case BACKWARD:
/*... acumula em f*/
      if (fAdd) {
        for (nel = 0; nel < numel; nel++) {
          for (j = 0; j< ndf; j++) {
            tmp1 = dt + dt0;
/*...*/
//          t0 = MAT2D(nel,0,density,nD)*MAT2D(nel,j,u,ndf);
//          t0 *= (tmp1/(dt*dt0));
/*...................................................................*/

/*...*/
 //         t00 = MAT2D(nel,1,density,nD)*MAT2D(nel,j,u0,ndf);
 //         t00*= (dt/(dt0*tmp1));
/*...................................................................*/
            MAT2D(nel,j,f,ndf) += volume[nel]*(t0-t00);
          }
/*...................................................................*/
        }
/*...................................................................*/
      }
/*...................................................................*/

/*... sobrecreve f*/
      else {
        for (nel = 0; nel < numel; nel++) {
          for (j = 0; j< ndf; j++) {
            tmp1 = dt + dt0;
/*..*/
//          t0 = MAT2D(nel, 0, density, nD)*MAT2D(nel, j, u, ndf);
//          t0 *= (tmp1 / (dt*dt0));
/*...................................................................*/

/*..*/
//          t00= MAT2D(nel, 1, density, nD)*MAT2D(nel, j, u0, ndf);
//          t00*= (dt / (dt0*tmp1));
/*...................................................................*/
            MAT2D(nel,j,f,ndf) = volume[nel]*(t0 - t00);
          }
/*...................................................................*/
        }
/*...................................................................*/
      }
/*...................................................................*/
    break;
  }
}
/*********************************************************************/


/*********************************************************************
 * Data de criacao    : 27/09/2019                                   *
 * Data de modificaco : 13/11/2019                                   *
 *-------------------------------------------------------------------*
 * updateTime : atualiza��o do tempo                                 *
 *-------------------------------------------------------------------*
 * Parametros de entrada:                                            *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 * Parametros de saida:                                              *
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 *-------------------------------------------------------------------*
 *********************************************************************/
void updateTime(Temporal *ddt, FileOpt *opt
               , Macros *mm  , short const myId )
{

/*...*/
  if (!ddt->timeStep) changeSchemeTemporal(ddt);
/*...................................................................*/

/*...*/
//    jLoop            = 0;
  ddt->t0        = ddt->t;
  ddt->t        += ddt->dt[TIME_N];
  ddt->timeStep ++; 
  gStep          = ddt->timeStep;
/*...................................................................*/

/*...*/
  opt->ta       += ddt->dt[TIME_N];
  opt->stepPlot[1]++;
/*...................................................................*/

/*...*/
  if(ddt->t > ddt->total + 0.1e0*ddt->dt[TIME_N])
    mm->flWord = false;  
/*    if(sc.ddt.t > sc.ddt.total)
        flWord = false;  */
/*...................................................................*/
      
/*...*/
  if(!myId )
  {
    printf("dt(n-2) = %.10lf\n",ddt->dt[TIME_N_MINUS_2]);
    printf("dt(n-1) = %.10lf\n",ddt->dt[TIME_N_MINUS_1]);
    printf("dt(n)   = %.10lf\n",ddt->dt[TIME_N ]);
    printf("t(s)    = %.10lf\n",ddt->t);
    printf("step    = %d\n"    ,ddt->timeStep);
  }
/*...................................................................*/

}
/*********************************************************************/

