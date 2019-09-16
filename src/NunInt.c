#include<NumInt.h>

/********************************************************************** 
 * Data de criacao    : 07/12/2017                                    *
 * Data de modificaco : 00/00/0000                                    *
 * ------------------------------------------------------------------ *
 * INTPONTO: pontos de intgracao para quadrilateros e hexaedros       * 
 * quadratura de gauss                                                * 
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * nint - numeros de pontos de integracao                             *
 * i    - ponto de integracao                                         *
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * alf- valor do ponto de integracao                                  * 
 * -------------------------------------------------------------------* 
 *  OBS:                                                              *
 * -------------------------------------------------------------------* 
 **********************************************************************/
double intponto(short const nint, short const i){
  double alf[10]; /* ponto de integra��o*/
    switch(nint) {
      case 1:
      alf[0]= 0.00000000000000;
      break;
    case 2:
      alf[0]=-0.577350269189626;
      alf[1]= 0.577350269189626;
      break;
    case 3:
      alf[0]=-0.774596669241483;
      alf[1]= 0.000000000000000;
      alf[2]= 0.774596669241483;
      break;
    case 4:
      alf[0]=-0.861136311594053;
      alf[1]=-0.339981043584856;
      alf[2]= 0.339981043584856;
      alf[3]= 0.861136311594053;
      break;
    case 5:
      alf[0]=-0.906179845938664;
      alf[1]=-0.538469310105683;
      alf[2]= 0.000000000000000;
      alf[3]= 0.538469310105683;
      alf[4]= 0.906179845938664;
      break;
    case 6:
      alf[0]=-0.932469514203152;
      alf[1]=-0.661209386466265;
      alf[2]=-0.238619186083197;
      alf[3]= 0.238619186083197;
      alf[4]= 0.661209386466265;
      alf[5]= 0.932469514203152;
      break;
    case 7:
      alf[0]=-0.949107912342759;
      alf[1]=-0.741531185599394;
      alf[2]=-0.405845151377397;
      alf[3]= 0.000000000000000;
      alf[4]= 0.405845151377397;
      alf[5]= 0.741531185599394;
      alf[6]= 0.949107912342759;
      break;
    case 8:
      alf[0]=-0.960289856497536;
      alf[1]=-0.796666477413627;
      alf[2]=-0.525532409916329;
      alf[3]=-0.183434642495650;
      alf[4]= 0.183434642495650;
      alf[5]= 0.525532409916329;
      alf[6]= 0.796666477413627;
      alf[7]= 0.960289856497536;
      break;
    case 9:
      alf[0]=-0.968160239507626;
      alf[1]=-0.836031107326636;
      alf[2]=-0.613371432700590;
      alf[3]=-0.324253423403809;
      alf[4]= 0.000000000000000;
      alf[5]= 0.324253423403809;
      alf[6]= 0.613371432700590;
      alf[7]= 0.836031107326636;
      alf[8]= 0.968160239507626;
      break;
    case 10:
      alf[0]=-0.973906528517172;
      alf[1]=-0.865063366688958;
      alf[2]=-0.679409568299024;
      alf[3]=-0.433395394129247;
      alf[4]=-0.148874338981631;
      alf[5]= 0.148874338981631;
      alf[6]= 0.433395394129247;
      alf[7]= 0.679409568299024;
      alf[8]= 0.973906528517172;
      alf[9]= 0.865063366688958;
      break;
    default:  
      printf("Numero de pontos de integra��o n�o especificado(ponto) !");
      exit(EXIT_FAILURE);
    }
  return alf[i];
  }
/********************************************************************** 
 * Data de criacao    : 07/12/2017                                    *
 * Data de modificaco : 00/00/0000                                    *
 * ------------------------------------------------------------------ *
 * NPINT: numero de pontos de integracao para o triangulo  grau(1...4)* 
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * igrau - grau do polinomio                                          *
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * npint - numero de pontos de integracao                             * 
 * -------------------------------------------------------------------* 
 *  OBS:                                                              *
 * -------------------------------------------------------------------* 
 **********************************************************************/
 short npint(short const igrau){
   
   short p[]={1,3,4,7};

   return p[igrau-1];

}
/********************************************************************** 
 * Data de criacao    : 07/12/2017                                    *
 * Data de modificaco : 00/00/0000                                    *
 * ------------------------------------------------------------------ *
 * PESP1: pontos de intgracao para triangulos coordenada eps1         * 
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * igrau - grua do polonomio                                          *
 * i     - ponto de integracao                                        *
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * alf- valor do ponto de integracao                                  * 
 * -------------------------------------------------------------------* 
 *  OBS:                                                              *
 * -------------------------------------------------------------------* 
 **********************************************************************/
double peps1(short const igrau, short const i){
    
    double alf[7];
    
    switch(igrau) {
      case 1:
        alf[0] = 0.33333333333333;
      break;
    case 2:
        alf[0]= 0.50000000000000;
        alf[1]= 0.00000000000000;
        alf[2]= 0.50000000000000;
      break;
    case 3:
        alf[0]= 0.333333333333333;
        alf[1]= 0.600000000000000;
        alf[2]= 0.200000000000000;
        alf[3]= 0.200000000000000;
      break;
    case 4:
        alf[0]= 0.333333333333333;
        alf[1]= 0.059715871789770;
        alf[2]= 0.470142064105115;
        alf[3]= 0.470142064105115;
        alf[4]= 0.797426985533087;
        alf[5]= 0.101286507323456;
        alf[6]= 0.101286507323456;
      break;
    default:  
      printf("Numero do grau do polinomio invalido %d!\n"
             "funcao peps1.\n",igrau);
      exit(EXIT_FAILURE);
    }
    return alf[i];
  }

/********************************************************************** 
 * Data de criacao    : 07/12/2017                                    *
 * Data de modificaco : 00/00/0000                                    *
 *------------------------------------------------------------------- * 
 * PEPS2: pontos de intgracao para triangulos coordenada eps2         * 
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * igrau - grau do polonomio                                          *
 * i     - ponto de integracao                                        *
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * alf- valor do ponto de integracao                                  * 
 * -------------------------------------------------------------------* 
 *  OBS:                                                              *
 * -------------------------------------------------------------------* 
 **********************************************************************/
double peps2(short const igrau, short const i){
    
    double alf[7];
    
    switch(igrau) {
      case 1:
        alf[0] = 0.33333333333333;
      break;
    case 2:
        alf[0]= 0.50000000000000;
        alf[1]= 0.50000000000000;
        alf[2]= 0.00000000000000;
      break;
    case 3:
        alf[0]= 0.333333333333333;
        alf[1]= 0.200000000000000;
        alf[2]= 0.600000000000000;
        alf[3]= 0.200000000000000;
      break;
    case 4:
      alf[0]= 0.333333333333333;
      alf[1]= 0.470142064105115;
      alf[2]= 0.059715871789770;
      alf[3]= 0.470142064105115;
      alf[4]= 0.101286507323456;
      alf[5]= 0.797426985533087;
      alf[6]= 0.101286507323456;
      break;
    default:  
      printf("Numero do grau do polinomio invalido!\n"
             "funcao peps2.\n");
      exit(EXIT_FAILURE);
    }
  return alf[i];
  }

/**********************************************************************
 * Data de criacao    : 07/12/2017                                    *
 * Data de modificaco : 00/00/0000                                    *
 *------------------------------------------------------------------- *  
 * pesot: peso para intgracao de triangulos                           * 
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * igrau - grua do polonomio                                          *
 * i     - ponto de integracao                                        *
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * alf- valor do ponto de integracao                                  * 
 * -------------------------------------------------------------------* 
 *  OBS:                                                              *
 * -------------------------------------------------------------------* 
 **********************************************************************/
double pesot(short const igrau, short const i){
  double alf[7]; /* pesos*/
     
    switch(igrau) {
      case 1:
        alf[0]= 1.000000000000000;
      break;
     case 2:
        alf[0]= 0.333333333333333;
        alf[1]= 0.333333333333333;
        alf[2]= 0.333333333333333;
       break;
     case 3:
        alf[0]= -0.562500000000000;
        alf[1]=  0.520833333333333;
        alf[2]=  0.520833333333333;
        alf[4]=  0.520833333333333;
      break;
     case 4:
        alf[0]=  0.225030003000000;
        alf[1]=  0.132394152788506;
        alf[2]=  0.132394152788506;
        alf[3]=  0.132394152788506;
        alf[4]=  0.125939180544827;
        alf[5]=  0.125939180544827;
        alf[6]=  0.125939180544827;
      break;
    default:  
      printf("Grau do polinomio invalido.\n!"
             "funcao pesot.\n");
      exit(EXIT_FAILURE);
  }
 return alf[i];
}


/**********************************************************************
 * Data de criacao    : 07/12/2017                                    *
 * Data de modificaco : 00/00/0000                                    *
 *------------------------------------------------------------------- * 
 * peso : peso para intrgracao quadrilateros e hexaedros              * 
 * ------------------------------------------------------------------ *
 * parametros de entrada:                                             * 
 * ------------------------------------------------------------------ *
 * nint  - numero de pontos de integracao                             *
 * i     - ponto de integracao                                        *
 * ------------------------------------------------------------------ *
 * parametros de saida  :                                             * 
 * alf- valor do ponto de integracao                                  * 
 * -------------------------------------------------------------------* 
 *  OBS:                                                              *
 * -------------------------------------------------------------------* 
 **********************************************************************/
double peso(short const nint, short const i){
  double alf[10]; /* pesos*/

    switch(nint) {
      case 1:
        alf[0]= 2.000000000000000;
      break;
      case 2:
        alf[0]= 1.000000000000000;
        alf[1]= 1.000000000000000;
      break;
      case 3:
        alf[0]= 0.555555555555556;
        alf[1]= 0.888888888888888;
        alf[2]= 0.555555555555556;
      break;
      case 4:
        alf[0]= 0.347854845137454;
        alf[1]= 0.652145154862546;
        alf[2]= 0.652145154862546;
        alf[3]= 0.347854845137454;
      break;
      case 5:
        alf[0]= 0.236926885056189;
        alf[1]= 0.478628670499366;
        alf[2]= 0.568888888888889;
        alf[3]= 0.478628670499366;
        alf[4]= 0.236926885056189;
      break;
      case 6:
        alf[0]= 0.171324492379170;
        alf[1]= 0.360761573048139;
        alf[2]= 0.467913934572691;
        alf[3]= 0.467913934572691;
        alf[4]= 0.360761573048139;
        alf[5]= 0.171324492379170;
      break;
      case 7:
        alf[0]= 0.129484966168870;
        alf[1]= 0.279705391489277;
        alf[2]= 0.381830050505119;
        alf[3]= 0.417959183673469;
        alf[4]= 0.381830050505119;
        alf[5]= 0.279705391489277;
        alf[6]= 0.129484966168870;
      break;
      case 8:
        alf[0]= 0.101228536290376;
        alf[1]= 0.222381034453374;
        alf[2]= 0.313706645877887;
        alf[3]= 0.362683783378362;
        alf[4]= 0.362683783378362;
        alf[5]= 0.313706645877887;
        alf[6]= 0.222381034453374;
        alf[7]= 0.101228536290376;
      break;
      case 9:
        alf[0]= 0.081274388361574;
        alf[1]= 0.180648160694857;
        alf[2]= 0.260610696402935;
        alf[3]= 0.312347077040003;
        alf[4]= 0.330239355001260;
        alf[5]= 0.312347077040003;
        alf[6]= 0.260610696402935;
        alf[7]= 0.180648160694857;
        alf[8]= 0.081274388361574;
      break;
      case 10:
        alf[0]= 0.066671344308688;
        alf[1]= 0.149451349150581;
        alf[2]= 0.219086362515982;
        alf[3]= 0.269266719309996;
        alf[4]= 0.295524224714753;
        alf[5]= 0.295524224714753;
        alf[6]= 0.269266719309996;
        alf[7]= 0.219086362515982;
        alf[8]= 0.149451349150581;
        alf[9]= 0.066671344308688;
      break;
      default:  
        printf("Numero de pontos de integra��o n�o especificado(peso)\n!");
        exit(EXIT_FAILURE);
    }
  return alf[i];
}