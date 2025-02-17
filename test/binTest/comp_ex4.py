#!/bin/python
import sys 
import math as m

def func1(x1,x2,k,l):
  u = k*(m.sin(x1) - x1*m.cos(l))
  return u

def dfunc1(x1,x2,k,l):
  u = k*(m.cos(x1) - m.cos(l))
  return (u,0.0)
  

def main(argv):
  
  progArg = (['fileIn csv'])

  nArgs = len(argv)
  if nArgs < 2:
    sys.stderr.write("Usage: %s "%argv[0])
    for arg in progArg:
      print arg 
    return 1



  fileMvf = open(sys.argv[1],"r")

  mvf     =[]
  gmvf    =[[]]
  x       =[[]]
  er      =[]
  gxer     =[]
  gyer     =[]
  exato   =[]
  gxexato =[]
  gyexato =[]
 
  fileMvf.readline()
  nCell = 0

  for f in fileMvf:
    s = f.split(',')
#...
    x.append([])
    x[nCell].append(float(s[1]))
    x[nCell].append(float(s[2]))
#...
    mvf.append(float(s[3]))
#...
    gmvf.append([])
    gmvf[nCell].append(float(s[4]))
    gmvf[nCell].append(float(s[5]))
#...
    nCell+=1


  for i in range(0,nCell):
    x1 = x[i][0] 
    x2 = x[i][1]
# ... solucao
    t = 0.25*x1*(10.0-0.5*x1)
    t = func1(x1,0.0,0.25,10.0)
    exato.append(t)

# ... derivada
    gx = dfunc1(x1,0.0,0.25,10.0)[0]
    gy = dfunc1(x1,0.0,0.25,10.0)[1]

    gxexato.append(gx)
    gyexato.append(gy)
    



  erC   = 0.e0      
  gxerC = 0.e0      
  gyerC = 0.e0      
  for i in range(0,nCell):
# ... solucao
    er.append((exato[i] - mvf[i])**2)
    erC += er[i]

# ... derivada
    gxer.append((gxexato[i] - gmvf[i][0])**2)
    gyer.append((gyexato[i] - gmvf[i][1])**2)
    gxerC += gxer[i]
    gyerC += gyer[i]

  exatoMax   = max(exato)     
  gxexatoMax = max(gxexato)     
  gyexatoMax = max(gyexato)     

  erMax = er[0]
  gxerMax = gxer[0]
  gyerMax = gyer[0]
  n=0
  nx=0
  ny=0
  for i in range(1,nCell):
    if erMax <= er[i]:
      erMax = er[i]
      n = i 
    if gxerMax <= gxer[i]:
      xgerMax = gxer[i]
      nx = i 
    if gyerMax <= gyer[i]:
      gyerMax = gyer[i]
      ny = i 

  erC   = erC/exatoMax**2 
  gxerC = gxerC/gxexatoMax**2
  gyerC = gyerC/gxexatoMax**2
  print "Solucao:"
  print "erro acumulado normalizado %e" %(erC)  
  print "erro Maximo     er[%d]=%e " %(n+1,erMax) 
  
  print "Derivada:" 
  print "x: erro acumulado normalizado %e" %(gxerC)  
  print "y: erro acumulado normalizado %e" %(gyerC)  
  print "x: erro Maximo     er[%d]=%e " %(nx+1,gxerMax) 
  print "y: erro Maximo     er[%d]=%e " %(ny+1,gyerMax) 
  
  if erC  > 1.e-4 :
    return 1 
  
  if gxerC  > 2.e-2 :
    return 1 
  
  if gyerC  > 1.e-3 :
    return 1 

if __name__ == '__main__':
  sys.exit(main(sys.argv))
