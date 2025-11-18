#define DT_MAX 0.0005
#define Ldomain 8

#include "axi.h"
#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "navier-stokes/conserving.h"
#include "tension.h"
#include "log-conform-EVP-data.h"
#include "fene-p-EVP.h"
#include "distance.h"
#include "adapt_wavelet_limited.h"

#define LEVEL 8
#define MAXlevel 9

#define tsnap (0.005)
// Error tolerances
#define fErr (1e-3)                                 // error tolerance in f1 VOF
#define KErr (1e-4)                                 // error tolerance in f2 VOF
#define VelErr (1e-2)                               // error tolerances in velocity
#define OmegaErr (1e-3)                             // error tolerances in vorticity

scalar mupv[], lambdav[], tau0v[];

// boundary conditions
u.n[right] = neumann(0.);
p[right] = dirichlet(0.);

double J, Deb, Bond, Oh, B;
char nameOut[80], nameIn[80];

int  main(int argc, char const *argv[]) {
  sprintf(nameOut, "%s", argv[2]);
  sprintf(nameIn, "%s", argv[1]);
  L0 = Ldomain;
  origin (-L0/2., 0.);
  init_grid (1 << 6);
  // Values taken from the terminal
  B = atof(argv[3]);
  J = atof(argv[4]);
  Deb = atof(argv[5]);

  Bond = 0.001;
  Oh = 0.01;

  rho1 = 1., rho2 = 0.001;
  mu1 = 0.01*B, mu2 = 0.0002;
  f.sigma = 1.0;

  foreach() {

    mupv[] = (1. - B)*f[]*mu1/B;
    lambdav[] = Deb*f[];
    tau0v[] = J*f[];
  }
  boundary ({lambdav, mupv, tau0v});

  lambda=lambdav;
  mup=mupv;
  tau0=tau0v;

  fprintf(ferr, "Level %d, Oh %2.1e, J %4.3f, Deb %4.3f\n", MAXlevel, Oh, J, Deb);

  restore(file=nameIn);

    scalar D2[];
    face vector D2f[];
  foreach_face(x) {
  double D11 = 0.5*( (u.y[0,1] - u.y[0,-1] + u.y[-1,1] - u.y[-1,-1])/(2.*Delta) );
  double D22 = (u.y[] + u.y[-1, 0])/(2*max(y, 1e-20));
  double D33 = (u.x[] - u.x[-1,0])/Delta;
  double D13 = 0.5*( (u.y[] - u.y[-1, 0])/Delta + 0.5*( (u.x[0,1] - u.x[0,-1] + u.x[-1,1] - u.x[-1,-1])/(2.*Delta) ) );

  double D2temp = sqrt( sq(D11) + sq(D22) + sq(D33) + 2*sq(D13) );
  D2f.x[] = D2temp;
  }

foreach_face(y) {
  double D11 = (u.y[0,0] - u.y[0,-1])/Delta;
  double D22 = (u.y[0,0] + u.y[0,-1])/(2*max(y, 1e-20));
  double D33 = 0.5*( (u.x[1,0] - u.x[-1,0] + u.x[1,-1] - u.x[-1,-1])/(2.*Delta) );
  double D13 = 0.5*( (u.x[0,0] - u.x[0,-1])/Delta + 0.5*( (u.y[1,0] - u.y[-1,0] + u.y[1,-1] - u.y[-1,-1])/(2.*Delta) ) );

  double D2temp = sqrt( sq(D11) + sq(D22) + sq(D33) + 2*sq(D13) );
  D2f.y[] = D2temp;
}

foreach(){
  D2[] = (D2f.x[]+D2f.y[]+D2f.x[1,0]+D2f.y[0,1])/4.;
  if (D2[] > 0.){
    D2[] = log(D2[])/log(10);
  } else {
    D2[] = -10;
  }
}
boundary({D2,D2f});

  FILE *fp = fopen(nameOut,"w");
  output_field({f, u.x, u.y, g.x, g.y, D2, rhov, tau_p.x.x, tau_p.y.x, tau_p.y.y, tau_qq, mupv, lambdav, tau0v, trA, solidreg},fp,1024,linear=true,box = {{-4.,0.},{4.,8.}});

  fclose (fp);
}

