#define DT_MAX 0.0005
#define Ldomain 8

# include "axi.h"
#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "navier-stokes/conserving.h"
#include "tension.h"
#include "log-conform-EVP.h"
#include "fene-p-EVP.h"
#include "distance.h"
#include "adapt_wavelet_limited.h"

#define tmax 4.5
#define LEVEL 8
#define MAXlevel 12

// Error tolerancs
#define fErr (1e-3)                                 // error tolerance in f1 VOF
#define KErr (1e-4)                                 // error tolerance in f2 VOF
#define VelErr (1e-2)                               // error tolerances in velocity
#define OmegaErr (1e-3)                             // error tolerances in vorticity

scalar mupv[], lambdav[], tau0v[];

u.n[right] = neumann(0.);
p[right] = dirichlet(0.);

double Bond,J,Deb,timew,B;
char nameOut[80], namepng[80], dumpFile[80], nameIn[80];

int  main(int argc, char const *argv[]) {
  sprintf(nameIn, "%s", argv[1]);
  timew = atof(argv[2]);
  L0 = Ldomain;
  origin (-L0/2., 0.);
  init_grid (1 << 8);
  Bond = 0.001;
  B = atof(argv[3]);
  J = atof(argv[4]);
  Deb = atof(argv[5]);

  rho1 = 1., mu1 = 0.01*B;
  rho2 = 0.001, mu2 = 0.0002, f.sigma = 1.0;

// fprintf(ferr, "J %4.1f De %4.1f \n", J, Deb);

  lambda=lambdav;
  mup=mupv;
  tau0=tau0v;

  restore(file=nameIn);
  
    double ke = 0.;
  foreach (reduction(+:ke)){
    ke += (2*pi*y)*(0.5*(f[])*(sq(u.x[]) + sq(u.y[])))*sq(Delta);}
  
    static FILE * fp;
    fp = fopen ("ke.txt", "a");
    fprintf (fp, "%g %g\n", timew, ke);
    fclose (fp);
}