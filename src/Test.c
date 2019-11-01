#include<Define.h>
#include<Mesh.h>
#include<math.h>

/*############### Teste do gradiente ####################*/
double func1(double x, double y, double z)
{
  double pi = PI,v1,v2,v3;
  v1 = cos(pi*x);
  v2 = sin(pi*y);
  v3 = z*z*z -.75*z;
  return v1+v2+v3;
}

void gradFunc1(double x, double y, double z,double *gradZ)
{
  double pi = PI;

  gradZ[0] = -pi * sin(pi*x);
  gradZ[1] =  pi * cos(pi*y);
  gradZ[2] = 3*z*z-.75*z;

}  


double func2(double x, double y, double z)
{
  double pi = PI,r;
  r = sqrt(x*x + y*y + z*z);
  return sin(pi*r) + 0.05*sin(8*pi*r);
}

void gradFunc2(double x, double y, double z,double *gradZ)
{
  double pi = PI;

  gradZ[0] = -pi * sin(pi*x);
  gradZ[1] =  pi * cos(pi*y);
  gradZ[2] = 3*z*z;

}  

void velSmithHutton(double x, double y, double z, double *v)
{
  double pi = PI;

  v[0] =  2.e0*y*(1.e0 - x * x);
  v[1] = -2.e0*x*(1.e0 - y * y);
  v[2] = 0.0;

}


void gradErro(Mesh *mesh)
{
  int i;
  double grad[3],x,y,z,err[3],errR=0.0, sumGrad=0.0;
  for (i = 0; i<mesh->numel; i++)
  {
    x = MAT2D(i, 0, mesh->elm.geom.cc, 3);
    y = MAT2D(i, 1, mesh->elm.geom.cc, 3);
    z = MAT2D(i, 2, mesh->elm.geom.cc, 3);
    
    gradFunc1(x,y,z,grad);

    err[0] = grad[0] - MAT2D(i, 0, mesh->elm.gradTemp, 3);
    err[1] = grad[1] - MAT2D(i, 1, mesh->elm.gradTemp, 3);
    err[2] = grad[2] - MAT2D(i, 2, mesh->elm.gradTemp, 3);
    errR   += err[0]*err[0]+ err[1]* err[1]+ err[2]*err[2];
    sumGrad = grad[0]* grad[0] + grad[1]*grad[1] + grad[2]*grad[2];
  }
  
  printf("Err A %e\n", sqrt(errR));
  printf("Err R %e\n", sqrt(errR)/ sqrt(sumGrad));

}

void grad(Mesh *mesh)
{
  int i;
  DOUBLE x, y, z;
  fprintf(fileLogDebug, "initialD1\n");
  for (i = 0; i<mesh->numel; i++)
  {
    x = MAT2D(i, 0, mesh->elm.geom.cc, 3);
    y = MAT2D(i, 1, mesh->elm.geom.cc, 3);
    z = MAT2D(i, 2, mesh->elm.geom.cc, 3);
    fprintf(fileLogDebug, "%9d %12.6lf\n", i + 1, func1(x, y, z));
  }
  fprintf(fileLogDebug, "endInitialD1\nreturn\n");
}

void vel(Mesh *mesh)
{
  int i;
  DOUBLE x, y, z,v[3];
  fprintf(fileLogDebug, "initialVel\n");
  for (i = 0; i<mesh->numel; i++)
  {
    x = MAT2D(i, 0, mesh->elm.geom.cc, 3);
    y = MAT2D(i, 1, mesh->elm.geom.cc, 3);
    z = MAT2D(i, 2, mesh->elm.geom.cc, 3);
    velSmithHutton(x, y, z,v);
    fprintf(fileLogDebug, "%9d %e %e %e\n", i + 1,v[0],v[1],v[2]);
  }
  fprintf(fileLogDebug, "endInitialVel\nreturn\n");
}


void initialZ(Mesh *mesh)
{
  int i;
  DOUBLE x, y, z;
  fprintf(fileLogDebug, "initialZ\n");
  for (i = 0; i<mesh->numel; i++)
  {
    x = MAT2D(i, 0, mesh->elm.geom.cc, 3);
    y = MAT2D(i, 1, mesh->elm.geom.cc, 3);
    z = MAT2D(i, 2, mesh->elm.geom.cc, 3);
    if(y>0.0)
      fprintf(fileLogDebug, "%9d %e %e %e %e\n", i + 1,0.2,0.0,0.0,0.0);
    else
      fprintf(fileLogDebug, "%9d %e %e %e %e\n", i + 1,0.0,0.2,0.0,0.0);
  }
  fprintf(fileLogDebug, "endInitialZ\nreturn\n");

  fprintf(fileLogDebug, "coordinates\n");
  for (i = 0; i<mesh->nnode; i++)
  {
    x = MAT2D(i, 0, mesh->node.x, 3);
    y = MAT2D(i, 1, mesh->node.x, 3); 
    z = MAT2D(i, 2, mesh->node.x, 3);
    fprintf(fileLogDebug, "%9d %e %e %e\n", i + 1,x/10,y/10,z/10);
  }
  fprintf(fileLogDebug, "endCoordinates\n");
  exit(0);
}


/*#######################################################*/