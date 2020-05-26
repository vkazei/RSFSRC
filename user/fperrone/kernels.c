#include <rsf.h>
#include "prep_utils.h"
#include "bench_utils.h"

void velupd2d(wfl_struct_t* wfl, mod_struct_t const* mod, acq_struct_t const * acq, adj_t adjflag)
/*< particle velocity time step in 2d >*/
{

  long const n1 = wfl->simN1;
  long const n2 = wfl->simN2;

  float * const v1c = wfl->v1c;
  float * const v2c = wfl->v2c;
  float const * v1p = wfl->v1p;
  float const * v2p = wfl->v2p;

  float * const v1a = wfl->v1a;
  float * const v2a = wfl->v2a;

  float const * pp = wfl->pp;
  float * const pa = wfl->pa;

  float const * tap1 = wfl->tap1;
  float const * tap2 = wfl->tap2;

  float const * buoy = mod->buoy;
  float const * incomp = mod->incomp;
  float const dt = acq->dt;
  float const d1 = mod->d1;
  float const d2 = mod->d2;

  float const dtd1 = dt/d1;
  float const dtd2 = dt/d2;

  float const d11=C1*dtd1;
  float const d12=C2*dtd1;
  float const d13=C3*dtd1;

  float const d21=C1*dtd2;
  float const d22=C2*dtd2;
  float const d23=C3*dtd2;

  long const i2start=NOP;
  long const i2end = n2-NOP;
  long const i1start=NOP;
  long const i1end = n1-NOP;

  switch (adjflag){
  case FWD:
  {
    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const b =buoy[idx];
        v1a[idx] = b*(d11*(pp[idx+1] - pp[idx  ])+
                      d12*(pp[idx+2] - pp[idx-1])+
                      d13*(pp[idx+3] - pp[idx-2]));
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const b =buoy[idx];
        v2a[idx] = b*(d21*(pp[idx+1*n1] - pp[idx     ])+
                      d22*(pp[idx+2*n1] - pp[idx-1*n1])+
                      d23*(pp[idx+3*n1] - pp[idx-2*n1]));
      }
    }

    // 1-component
    // 2-component
    for (long i2=i2start; i2<i2end; i2++){
      float const spox = tap2[i2];
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const spo = spox*tap1[i1];
        v1c[idx] = spo*(v1p[idx] - v1a[idx]);
        v2c[idx] = spo*(v2p[idx] - v2a[idx]);
      }
    }

    break;
  }
  case ADJ:
  {
    // ===============================================================
    // 2nd order in time
    for (int i2=i2start; i2<i2end; i2++){
      float const spo2 = tap2[i2];
      for (int i1=i1start, idx=IDX2D(i1,i2  ); i1<i1end; i1++,idx++){
        float const spo = tap1[i1]* spo2;
        float const k = incomp[idx];
        pa[idx] = spo*k*pp[idx];
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start, idx=IDX2D(i1,i2  ); i1<i1end; i1++,idx++){
        v1a[idx] = (d11*(pa[idx+1] - pa[idx  ])+
                    d12*(pa[idx+2] - pa[idx-1])+
                    d13*(pa[idx+3] - pa[idx-2]));
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start, idx=IDX2D(i1,i2  ); i1<i1end; i1++,idx++){
        v2a[idx] = (d21*(pa[idx+1*n1] - pa[idx     ])+
                    d22*(pa[idx+2*n1] - pa[idx-1*n1])+
                    d23*(pa[idx+3*n1] - pa[idx-2*n1]));
      }
    }

    // 1-component
    // 2-component
    for (long i2=i2start; i2<i2end; i2++){
      float const spox = tap2[i2];
      for (long i1=i1start, idx=IDX2D(i1,i2  ); i1<i1end; i1++,idx++){
        float const spo = spox*tap1[i1];
        v1c[idx] = spo*v1p[idx] - v1a[idx];
        v2c[idx] = spo*v2p[idx] - v2a[idx];
      }
    }

    break;
  }
  }

}

void velupd3d(wfl_struct_t* wfl, mod_struct_t const* mod, acq_struct_t const * acq, adj_t adjflag)
/*< particle velocity time step in 3d >*/
{

  long const n1 = wfl->simN1;
  long const n2 = wfl->simN2;
  long const n3 = wfl->simN3;
  long const n12 = n1*n2;

  float * const v1c = wfl->v1c;
  float * const v2c = wfl->v2c;
  float * const v3c = wfl->v3c;

  float const * v1p = wfl->v1p;
  float const * v2p = wfl->v2p;
  float const * v3p = wfl->v3p;

  float * const v1a = wfl->v1a;
  float * const v2a = wfl->v2a;
  float * const v3a = wfl->v3a;

  float const * pp = wfl->pp;
  float * const pa = wfl->pa;

  float const * tap1 = wfl->tap1;
  float const * tap2 = wfl->tap2;
  float const * tap3 = wfl->tap3;

  float const * buoy = mod->buoy;
  float const * incomp = mod->incomp;
  float const dt = acq->dt;
  float const d1 = mod->d1;
  float const d2 = mod->d2;
  float const d3 = mod->d3;

  float const dtd1 = dt/d1;
  float const dtd2 = dt/d2;
  float const dtd3 = dt/d3;

  float const d11 = C1*dtd1;
  float const d12 = C2*dtd1;
  float const d13 = C3*dtd1;

  float const d21 = C1*dtd2;
  float const d22 = C2*dtd2;
  float const d23 = C3*dtd2;

  float const d31 = C1*dtd3;
  float const d32 = C2*dtd3;
  float const d33 = C3*dtd3;

  long const i3start=NOP;
  long const i3end = n3-NOP;
  long const i2start=NOP;
  long const i2end = n2-NOP;
  long const i1start=NOP;
  long const i1end = n1-NOP;

  switch (adjflag){
  case FWD:
  {
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){

          v1a[idx] = (d11*(pp[idx+1] - pp[idx  ])+
                      d12*(pp[idx+2] - pp[idx-1])+
                      d13*(pp[idx+3] - pp[idx-2]));
        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){

          v2a[idx] = (d21*(pp[idx+  n1] - pp[idx     ])+
                      d22*(pp[idx+2*n1] - pp[idx-1*n1])+
                      d23*(pp[idx+3*n1] - pp[idx-2*n1]));

        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){

          v3a[idx] = (d31*(pp[idx+  n12] - pp[idx      ])+
                      d32*(pp[idx+2*n12] - pp[idx-1*n12])+
                      d33*(pp[idx+3*n12] - pp[idx-2*n12]));

        }
      }
    }

    // 1-component
    // 2-component
    // 3-component
    for (long i3=i3start; i3<i3end; i3++){
      float const spoy = tap3[i3];
      for (long i2=i2start; i2<i2end; i2++){
        float const spoxy = spoy*tap2[i2];
        for (long i1=i1start, idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          float const spo = spoxy*tap1[i1];
          float const b =buoy[idx];
          v1c[idx] = spo*(v1p[idx] - b*v1a[idx]);
          v2c[idx] = spo*(v2p[idx] - b*v2a[idx]);
          v3c[idx] = spo*(v3p[idx] - b*v3a[idx]);
        }
      }
    }

    break;
  }
  case ADJ:
  {
    // ===============================================================
    // 2nd order in time
    for (long i3=i3start; i3<i3end; i3++){
      float const spoy = tap3[i3];
      for (int i2=i2start; i2<i2end; i2++){
        float const spoxy = spoy*tap2[i2];
        for (int i1=i1start, idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          float const spo = tap1[i1]* spoxy;
          float const k = incomp[idx];
          pa[idx] = spo*k*pp[idx];
        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v1a[idx] = (d11*(pa[idx+1] - pa[idx  ])+
                      d12*(pa[idx+2] - pa[idx-1])+
                      d13*(pa[idx+3] - pa[idx-2]));
        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v2a[idx] = (d21*(pa[idx+  n1] - pa[idx     ])+
                      d22*(pa[idx+2*n1] - pa[idx-1*n1])+
                      d23*(pa[idx+3*n1] - pa[idx-2*n1]));

        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v3a[idx] = (d31*(pa[idx+  n12] - pa[idx      ])+
                      d32*(pa[idx+2*n12] - pa[idx-1*n12])+
                      d33*(pa[idx+3*n12] - pa[idx-2*n12]));

        }
      }
    }

    // 1-component
    // 2-component
    // 3-component
    for (long i3=i3start; i3<i3end; i3++){
      float const spoy = tap3[i3];
      for (long i2=i2start; i2<i2end; i2++){
        float const spoxy = spoy*tap2[i2];
        for (long i1=i1start, idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          float const spo = spoxy*tap1[i1];
          v1c[idx] = spo*v1p[idx] - v1a[idx];
          v2c[idx] = spo*v2p[idx] - v2a[idx];
          v3c[idx] = spo*v3p[idx] - v3a[idx];
        }
      }
    }

    break;
  }

  }

}

void presupd2d(wfl_struct_t* wfl, mod_struct_t const* mod, acq_struct_t const* acq, adj_t adjflag)
/*< pressure time step in 2d >*/
{

  long const n1 = wfl->simN1;
  long const n2 = wfl->simN2;

  float * const pc = wfl->pc;
  float const * pp = wfl->pp;

  float const *v1c = wfl->v1c;
  float const *v2c = wfl->v2c;
  float * const v1p = wfl->v1p;
  float * const v2p = wfl->v2p;

  float * const v1a = wfl->v1a;
  float * const v2a = wfl->v2a;

  float const * tap1 = wfl->tap1;
  float const * tap2 = wfl->tap2;

  float const *buoy   = mod->buoy;
  float const *incomp = mod->incomp;
  float const d1 = mod->d1;
  float const d2 = mod->d2;
  float const dt = acq->dt;

  float const dtd1 = dt/d1;
  float const dtd2 = dt/d2;

  float const d11=C1*dtd1;
  float const d12=C2*dtd1;
  float const d13=C3*dtd1;

  float const d21=C1*dtd2;
  float const d22=C2*dtd2;
  float const d23=C3*dtd2;

  long const i2start=NOP;
  long const i2end = n2-NOP;
  long const i1start=NOP;
  long const i1end = n1-NOP;

  switch (adjflag){
  case FWD:
  {
    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const k = incomp[idx];
        v1a[idx] = k*(d11*(-v1c[idx  ] + v1c[idx-1])+
                      d12*(-v1c[idx+1] + v1c[idx-2])+
                      d13*(-v1c[idx+2] + v1c[idx-3]));
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const k = incomp[idx];
        v2a[idx] =  k*( d21*(-v2c[idx     ] + v2c[idx-1*n1])+
                        d22*(-v2c[idx+1*n1] + v2c[idx-2*n1])+
                        d23*(-v2c[idx+2*n1] + v2c[idx-3*n1]));
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      float const spox = tap2[i2];
      for (long i1=i2start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const spo = spox*tap1[i1];
        pc[idx] = spo*(pp[idx] + (v1a[idx]+v2a[idx]));
      }
    }

    break;
  }
  case ADJ:
  {
    // ===============================================================
    // 2nd order in time
    for (long i2=i2start; i2<i2end; i2++){
      float const spo2 = tap2[i2];
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const spo = spo2*tap1[i1];
        float const irho = buoy[idx];
        v2p[idx] = spo*irho*v2c[idx];
        v1p[idx] = spo*irho*v1c[idx];
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        v1a[idx] = (d11*(-v1p[idx  ] + v1p[idx-1])+
                    d12*(-v1p[idx+1] + v1p[idx-2])+
                    d13*(-v1p[idx+2] + v1p[idx-3]));
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        v2a[idx] = (d21*(-v2p[idx     ] + v2p[idx-1*n1])+
                    d22*(-v2p[idx+1*n1] + v2p[idx-2*n1])+
                    d23*(-v2p[idx+2*n1] + v2p[idx-3*n1]));
      }
    }

    for (long i2=i2start; i2<i2end; i2++){
      float const spox = tap2[i2];
      for (long i1=i1start,idx=IDX2D(i1,i2); i1<i1end; i1++,idx++){
        float const spo = spox*tap1[i1];
        pc[idx] = spo*pp[idx] + (v1a[idx]+v2a[idx]);
      }
    }

    break;
  }

  }

}

void presupd3d(wfl_struct_t* wfl, mod_struct_t const* mod, acq_struct_t const* acq, adj_t adjflag)
/*< pressure time step in 3d >*/
{

  long const n1 = wfl->simN1;
  long const n2 = wfl->simN2;
  long const n3 = wfl->simN3;
  long const n12= n1*n2;

  float * const pc = wfl->pc;
  float const * pp = wfl->pp;

  float const *v1c = wfl->v1c;
  float const *v2c = wfl->v2c;
  float const *v3c = wfl->v3c;
  float * const v1p = wfl->v1p;
  float * const v2p = wfl->v2p;
  float * const v3p = wfl->v3p;

  float * const v1a = wfl->v1a;
  float * const v2a = wfl->v2a;
  float * const v3a = wfl->v3a;

  float const * tap1 = wfl->tap1;
  float const * tap2 = wfl->tap2;
  float const * tap3 = wfl->tap3;

  float const *buoy   = mod->buoy;
  float const *incomp = mod->incomp;
  float const d1 = mod->d1;
  float const d2 = mod->d2;
  float const d3 = mod->d3;
  float const dt = acq->dt;

  float const dtd1 = dt/d1;
  float const dtd2 = dt/d2;
  float const dtd3 = dt/d3;

  float const d11 = C1*dtd1;
  float const d12 = C2*dtd1;
  float const d13 = C3*dtd1;

  float const d21 = C1*dtd2;
  float const d22 = C2*dtd2;
  float const d23 = C3*dtd2;

  float const d31 = C1*dtd3;
  float const d32 = C2*dtd3;
  float const d33 = C3*dtd3;

  long const i3start=NOP;
  long const i3end = n3-NOP;
  long const i2start=NOP;
  long const i2end = n2-NOP;
  long const i1start=NOP;
  long const i1end = n1-NOP;

  switch (adjflag){
  case FWD:
  {
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v1a[idx] = (d11*(-v1c[idx  ] + v1c[idx-1])+
                      d12*(-v1c[idx+1] + v1c[idx-2])+
                      d13*(-v1c[idx+2] + v1c[idx-3]));
        }
      }
    }
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v2a[idx] =  (d21*(-v2c[idx     ] + v2c[idx-1*n1])+
                       d22*(-v2c[idx+1*n1] + v2c[idx-2*n1])+
                       d23*(-v2c[idx+2*n1] + v2c[idx-3*n1]));
        }
      }
    }
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v3a[idx] =  (d31*(-v3c[idx      ] + v3c[idx-1*n12])+
                       d32*(-v3c[idx+1*n12] + v3c[idx-2*n12])+
                       d33*(-v3c[idx+2*n12] + v3c[idx-3*n12]));
        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      float const spoy = tap3[i3];
      for (long i2=i2start; i2<i2end; i2++){
        float const spoxy = spoy*tap2[i2];
        for (long i1=i2start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          float const spo = spoxy*tap1[i1];
          float const k = incomp[idx];
          pc[idx] = spo*(pp[idx] + k*(v1a[idx]+v2a[idx]+v3a[idx]));
        }
      }
    }

    break;
  }
  case ADJ:
  {
    // ===============================================================
    // 2nd order in time
    for (long i3=i3start; i3<i3end; i3++){
      float const spoy = tap3[i3];
      for (long i2=i2start; i2<i2end; i2++){
        float const spoxy = spoy*tap2[i2];
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          float const spo = spoxy*tap1[i1];
          float irho = buoy[idx];
          v3p[idx] = spo*irho*v3c[idx];
          v2p[idx] = spo*irho*v2c[idx];
          v1p[idx] = spo*irho*v1c[idx];
        }
      }
    }
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v1a[idx] = (d11*(-v1p[idx  ] + v1p[idx-1])+
                      d12*(-v1p[idx+1] + v1p[idx-2])+
                      d13*(-v1p[idx+2] + v1p[idx-3]));
        }
      }
    }
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v2a[idx] =  (d21*(-v2p[idx     ] + v2p[idx-1*n1])+
                       d22*(-v2p[idx+1*n1] + v2p[idx-2*n1])+
                       d23*(-v2p[idx+2*n1] + v2p[idx-3*n1]));
        }
      }
    }
    for (long i3=i3start; i3<i3end; i3++){
      for (long i2=i2start; i2<i2end; i2++){
        for (long i1=i1start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          v3a[idx] =  (d31*(-v3p[idx      ] + v3p[idx-1*n12])+
                       d32*(-v3p[idx+1*n12] + v3p[idx-2*n12])+
                       d33*(-v3p[idx+2*n12] + v3p[idx-3*n12]));
        }
      }
    }

    for (long i3=i3start; i3<i3end; i3++){
      float const spoy = tap3[i3];
      for (long i2=i2start; i2<i2end; i2++){
        float const spoxy = spoy*tap2[i2];
        for (long i1=i2start,idx=IDX3D(i1,i2,i3); i1<i1end; i1++,idx++){
          float const spo = spoxy*tap1[i1];
          pc[idx] = spo*pp[idx] + (v1a[idx]+v2a[idx]+v3a[idx]);
        }
      }
    }

    break;
  }
  }

}


void injectPsource2d(wfl_struct_t* wfl,
                          mod_struct_t const * mod,
                          acq_struct_t const * acq,
                          long it)
/*< Injection of the source signature in 2d >*/
{

  long nsou = acq->ns;

  long N1 = wfl->simN1;

  float modD1 = mod->d1;
  float modD2 = mod->d2;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;

  float dt = acq->dt;
  float scale = dt/(modD1*modD2);

  float * wf = wfl->pc;

  for (long isou=0; isou<nsou; isou++){
    float xs = acq->scoord[isou*2];
    float zs = acq->scoord[isou*2+1];

    int ixs = (xs-o2)/modD2;
    int izs = (zs-o1)/modD1;
    float force = acq->wav[isou + nsou*it]*scale;
    long idx = izs + N1*ixs;
    const float K = mod->incomp[idx];

    for (int j=-3,jh=0; j<=4; j++,jh++){
      const float hicks2 = acq->hicksSou2[jh+isou*8];
      for (int i=-3,ih=0; i<=4; i++,ih++){
        const float hc = acq->hicksSou1[ih+isou*8]*hicks2;
        wf[idx + i + N1*j] += K*hc*force;
      }
    }

  }

}

static void injectPsource3d(wfl_struct_t* wfl,
                            mod_struct_t const * mod,
                            acq_struct_t const * acq,
                            long it){

  long nsou = acq->ns;

  long N1 = wfl->simN1;
  long N2 = wfl->simN2;

  float modD1 = mod->d1;
  float modD2 = mod->d2;
  float modD3 = mod->d3;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;
  float o3 = wfl->simO3;

  float dt = acq->dt;
  float scale = dt/(modD1*modD2);

  float * wf = wfl->pc;

  for (long isou=0; isou<nsou; isou++){
    float xs = acq->scoord[isou*3  ];
    float ys = acq->scoord[isou*3+1];
    float zs = acq->scoord[isou*3+2];

    int iys = (ys-o3)/modD3;
    int ixs = (xs-o2)/modD2;
    int izs = (zs-o1)/modD1;
    float force = acq->wav[isou + nsou*it]*scale;
    long idx = izs + N1*(ixs + N2*iys);

    for (int k=-3,kh=0; k<=4; k++,kh++){
      const float hicks3 = acq->hicksSou3[kh+isou*8];
      for (int j=-3,jh=0; j<=4; j++,jh++){
        const float hicks2 = acq->hicksSou2[jh+isou*8];
        for (int i=-3,ih=0; i<=4; i++,ih++){
          const float hc = acq->hicksSou1[ih+isou*8]*hicks2*hicks3;
          wf[idx + i + N1*(j + k*N2)] += hc*force;
        }
      }
    }

  }

}

void injectPdata2d(wfl_struct_t* wfl, mod_struct_t const * mod, acq_struct_t const * acq, long it)
/*< inject the recorded data in 2d >*/
{
  long nrec = acq->nr;

  long N1 = wfl->simN1;

  float modD1 = mod->d1;
  float modD2 = mod->d2;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;

  float *sp = sf_floatalloc(64);

  for (long irec=0; irec<nrec; irec++){
    float xr = acq->rcoord[irec*2];
    float zr = acq->rcoord[irec*2+1];

    int ixr = (xr-o2)/modD2;
    int izr = (zr-o1)/modD1;
    long idx = izr + N1*ixr;

    float force = acq->dat[irec + nrec*it];

    for (int j=-3,jh=0; j<=4; j++,jh++){
      const float hicks2 = acq->hicksRcv2[jh+irec*8];
      for (int i=-3,ih=0; i<=4; i++,ih++){
        const float hc = acq->hicksRcv1[ih+irec*8]*hicks2;
        sp[ih+8*jh] = hc*force;
      }
    }

    for (int j=-3,jh=0; j<=4; j++,jh++){
      for (int i=-3,ih=0; i<=4; i++,ih++){
        wfl->pc[idx + i + N1*j] += sp[ih+8*jh];
      }
    }

  }

  free(sp);

}

void injectPdata3d(wfl_struct_t* wfl, mod_struct_t const * mod, acq_struct_t const * acq, long it)
/*< inject the recorded data in 2d >*/
{
  long nrec = acq->nr;

  long N1 = wfl->simN1;
  long N2 = wfl->simN2;

  float modD1 = mod->d1;
  float modD2 = mod->d2;
  float modD3 = mod->d3;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;
  float o3 = wfl->simO3;

  float *sp = sf_floatalloc(8*8*8);

  for (long irec=0; irec<nrec; irec++){
    float xr = acq->rcoord[irec*3  ];
    float yr = acq->rcoord[irec*3+1];
    float zr = acq->rcoord[irec*3+2];

    int ixr = (xr-o2)/modD2;
    int iyr = (yr-o3)/modD3;
    int izr = (zr-o1)/modD1;
    long idx = izr + N1*(ixr+N2*iyr);

    float force = acq->dat[irec + nrec*it];

    for (int k=-3,kh=0; k<=4; k++,kh++){
      const float hicks3 = acq->hicksRcv3[kh+irec*8];
      for (int j=-3,jh=0; j<=4; j++,jh++){
        const float hicks2 = acq->hicksRcv2[jh+irec*8];
        for (int i=-3,ih=0; i<=4; i++,ih++){
          const float hc = acq->hicksRcv1[ih+irec*8]*hicks2*hicks3;
          sp[ih+8*(jh+8*kh)] = hc*force;
        }
      }
    }

    for (int k=-3,kh=0; k<=4; k++,kh++){
      for (int j=-3,jh=0; j<=4; j++,jh++){
        for (int i=-3,ih=0; i<=4; i++,ih++){
          wfl->pc[idx + i + N1*(j+ k*N2)] += sp[ih+8*(jh+8*kh)];
        }
      }
    }

  }

  free(sp);

}

static void injectBornVelocitySource2d(wfl_struct_t * const wfl, mod_struct_t const *mod, acq_struct_t const * acq, long it){
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long n12 = modN1*modN2;
  long nb = wfl->nabc;

  float dt = acq->dt;

  fread(wfl->v1a,sizeof(float),n12,wfl->Fprgrd1);
  fread(wfl->v2a,sizeof(float),n12,wfl->Fprgrd2);

  for (long i2=0; i2<modN2; i2++){
    for (long i1=0; i1<modN1; i1++){
      long simIdx = (i1+nb) + (i2+nb)*wfl->simN1;
      long modIdx = i1 + i2*modN1;

      float dr = mod->denpert[modIdx];
      float rh = mod->dmod[modIdx];
      float rp = dr*dt/(rh*rh);

      wfl->v1c[simIdx] += rp*wfl->v1a[modIdx];
      wfl->v2c[simIdx] += rp*wfl->v2a[modIdx];
    }
  }

}

static void injectBornVelocitySource3d(wfl_struct_t * const wfl, mod_struct_t const *mod, acq_struct_t const * acq, long it){
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long modN3 = wfl->modN3;
  long nelem = modN1*modN2*modN3;
  long nb = wfl->nabc;

  float dt = acq->dt;

  fread(wfl->v1a,sizeof(float),nelem,wfl->Fprgrd);
  fread(wfl->v2a,sizeof(float),nelem,wfl->Fprgrd);
  fread(wfl->v3a,sizeof(float),nelem,wfl->Fprgrd);

  for (long i3=0; i3<modN3; i3++){
    for (long i2=0; i2<modN2; i2++){
      for (long i1=0; i1<modN1; i1++){
        long simIdx = (i1+nb) + wfl->simN1*((i2+nb)+wfl->simN2*(i3+nb));
        long modIdx = i1 + modN1*(i2+i3*modN2);

        float dr = mod->denpert[modIdx];
        float rh = mod->dmod[modIdx];
        float rp = dr*dt/(rh*rh);

        wfl->v1c[simIdx] += rp*wfl->v1a[modIdx];
        wfl->v2c[simIdx] += rp*wfl->v2a[modIdx];
        wfl->v3c[simIdx] += rp*wfl->v3a[modIdx];
      }
    }
  }

}

void injectBornPressureSource2d(wfl_struct_t * const wfl, mod_struct_t const *mod, acq_struct_t const * acq, long it)
/*< injection of a extended source over the whole volume in 2d >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long n12 = modN1*modN2;
  long nb = wfl->nabc;

  float dt = acq->dt;

  fread(wfl->bwfl,n12,sizeof(float),wfl->Fpvdiv);

  for (long i2=0; i2<modN2; i2++){
    for (long i1=0; i1<modN1; i1++){
      long simIdx = (i1+nb) + (i2+nb)*wfl->simN1;
      long modIdx = i1 + i2*modN1;

      float const dv = mod->velpert[modIdx];
      float const dr = mod->denpert[modIdx];
      float const vc = mod->vmod[modIdx];
      float const rh = mod->dmod[modIdx];
      float const vp= 2.f*vc*dv*rh;
      float const rp= vc*vc*dr;

      wfl->pc[simIdx] += (vp+rp)*dt*wfl->bwfl[modIdx];

    }
  }

}

void injectBornPressureSource3d(wfl_struct_t * const wfl, mod_struct_t const *mod, acq_struct_t const * acq, long it)
/*< injection of a extended source over the whole volume in 3d >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long modN3 = wfl->modN3;
  long nelem = modN1*modN2*modN3;
  long nb = wfl->nabc;

  float dt = acq->dt;

  fread(wfl->bwfl,nelem,sizeof(float),wfl->Fpvdiv);

  for (long i3=0; i3<modN3; i3++){
    for (long i2=0; i2<modN2; i2++){
      for (long i1=0; i1<modN1; i1++){
        long simIdx = (i1+nb) + wfl->simN1*((i2+nb)+wfl->simN2*(i3+nb));
        long modIdx = i1 + modN1*(i2 + i3*modN2);

        float const dv = mod->velpert[modIdx];
        float const dr = mod->denpert[modIdx];
        float const vc = mod->vmod[modIdx];
        float const rh = mod->dmod[modIdx];
        float const vp= 2.f*vc*dv*rh;
        float const rp= vc*vc*dr;

        wfl->pc[simIdx] += (vp+rp)*dt*wfl->bwfl[modIdx];

      }
    }
  }

}

void swapwfl2d(wfl_struct_t* wfl)
/*< wavefield pointers circulation 2d >*/
{

  float *tmp = wfl->pc;
  wfl->pc = wfl->pp;
  wfl->pp = tmp;

  tmp = wfl->v1c;
  wfl->v1c = wfl->v1p;
  wfl->v1p = tmp;

  tmp = wfl->v2c;
  wfl->v2c = wfl->v2p;
  wfl->v2p = tmp;

}

static void swapwfl3d(wfl_struct_t* wfl)
{

  float *tmp = wfl->pc;
  wfl->pc = wfl->pp;
  wfl->pp = tmp;

  tmp = wfl->v1c;
  wfl->v1c = wfl->v1p;
  wfl->v1p = tmp;

  tmp = wfl->v2c;
  wfl->v2c = wfl->v2p;
  wfl->v2p = tmp;

  tmp = wfl->v3c;
  wfl->v3c = wfl->v3p;
  wfl->v3p = tmp;

}


void extract_vel_wfl_2d(wfl_struct_t* wfl, int comp)
/*< Extracts the velocity components in 2d >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long nabc  = wfl->nabc;
  long simN1 = wfl->simN1;

  // copy the write chunk of wavefield
  switch (comp){
  case 1:
    for (long i2=0; i2<modN2; i2++)
      memcpy(wfl->bwfl+i2*modN1,wfl->v1c+(nabc+(i2+nabc)*simN1),modN1*sizeof(float));
    break;
  case 2:
    for (long i2=0; i2<modN2; i2++)
      memcpy(wfl->bwfl+i2*modN1,wfl->v2c+(nabc+(i2+nabc)*simN1),modN1*sizeof(float));
    break;
  }

}

void extract_vel_wfl_3d(wfl_struct_t* wfl, int comp)
/*< Extracts the velocity components in 2d >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long modN3 = wfl->modN3;
  long nabc  = wfl->nabc;
  long simN1 = wfl->simN1;
  long simN2 = wfl->simN2;

  // copy the write chunk of wavefield
  switch (comp){
  case 1:
    for (long i3=0; i3<modN3; i3++)
    for (long i2=0; i2<modN2; i2++)
      memcpy( wfl->bwfl+modN1*(i2+modN2*i3),
              wfl->v1c+(nabc+simN1*((i2+nabc)+simN2*(i3+nabc))),
              modN1*sizeof(float));
    break;
  case 2:
    for (long i3=0; i3<modN3; i3++)
    for (long i2=0; i2<modN2; i2++)
      memcpy( wfl->bwfl+modN1*(i2+modN2*i3),
              wfl->v2c+(nabc+simN1*((i2+nabc)+simN2*(i3+nabc))),
              modN1*sizeof(float));
    break;
  case 3:
    for (long i3=0; i3<modN3; i3++)
    for (long i2=0; i2<modN2; i2++)
      memcpy( wfl->bwfl+modN1*(i2+modN2*i3),
              wfl->v3c+(nabc+simN1*((i2+nabc)+simN2*(i3+nabc))),
              modN1*sizeof(float));
    break;
  }

}

void extract_pres_wfl_2d(wfl_struct_t * const wfl)
/*< extract the pressure wavefield 2d >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long nabc  = wfl->nabc;
  long simN1 = wfl->simN1;

  // copy the write chunk of wavefield
  for (long i2=0; i2<modN2; i2++){
    float * const pwflo = wfl->bwfl+i2*modN1;
    float * const pwfli = wfl->pc + (i2+nabc)*simN1;
    for(long i1=0,j1=nabc; i1<modN1; i1++,j1++){
      pwflo[i1] = pwfli[j1];
    }
  }

}

static void extract_pres_wfl_3d(wfl_struct_t * const wfl){
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long modN3 = wfl->modN3;
  long nabc  = wfl->nabc;
  long simN1 = wfl->simN1;
  long simN2 = wfl->simN2;

  // copy the write chunk of wavefield
  for (long i3=0; i3<modN3; i3++){
    float * const pwflo3 = wfl->bwfl+i3*modN1*modN2;
    float * const pwfli3 = wfl->pc + (i3+nabc)*simN1*simN2;
    for (long i2=0; i2<modN2; i2++){
      float * const pwflo32 = pwflo3 + i2*modN1;
      float * const pwfli32 = pwfli3 + (i2+nabc)*simN1;

      for(long i1=0,j1=nabc; i1<modN1; i1++,j1++){
        pwflo32[i1] = pwfli32[j1];
      }
    }
  }

}

static void extract_dat_2d(wfl_struct_t* wfl,acq_struct_t const * acq){

  long nr = acq->nr;
  long n1 = wfl->simN1;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;
  float d1 = wfl->d1;
  float d2 = wfl->d2;

  wfl->rdata = sf_floatalloc(nr);
  for (long ir=0; ir<nr; ir++){
    float xr = acq->rcoord[2*ir];
    float zr = acq->rcoord[2*ir+1];
    long ixr = (xr - o2)/d2;
    long izr = (zr - o1)/d1;
    long idx = izr + n1*ixr;
    float rv = 0.;
    for (int j=-3,jh=0; j<=4; j++,jh++){
      const float hicks2 = acq->hicksRcv2[jh+ir*8];
      for (int i=-3,ih=0; i<=4; i++,ih++){
        const float hc = acq->hicksRcv1[ih+ir*8]*hicks2;
        rv += hc*wfl->pc[idx + i +j*n1];
      }
    }
    wfl->rdata[ir] = rv;
  }

  sf_floatwrite(wfl->rdata,nr,wfl->Fdata);

  free(wfl->rdata);

}

static void extract_dat_3d(wfl_struct_t* wfl,acq_struct_t const * acq){
  long nr = acq->nr;
  long n1 = wfl->simN1;
  long n2 = wfl->simN2;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;
  float o3 = wfl->simO3;
  float d1 = wfl->d1;
  float d2 = wfl->d2;
  float d3 = wfl->d3;

  wfl->rdata = sf_floatalloc(nr);

  for (long ir=0; ir<nr; ir++){
    float xr = acq->rcoord[3*ir  ];
    float yr = acq->rcoord[3*ir+1];
    float zr = acq->rcoord[3*ir+2];
    long ixr = (xr - o2)/d2;
    long iyr = (yr - o3)/d3;
    long izr = (zr - o1)/d1;
    long idx = izr + n1*(ixr + iyr*n2);

    float rv = 0.;
    for (int k=-3,kh=0; k<=4; k++,kh++){
      const float hicks3 = acq->hicksRcv3[kh+ir*8];
      for (int j=-3,jh=0; j<=4; j++,jh++){
        const float hicks2 = acq->hicksRcv2[jh+ir*8];
        for (int i=-3,ih=0; i<=4; i++,ih++){
          const float hc = acq->hicksRcv1[ih+ir*8]*hicks2*hicks3;
          rv += hc*wfl->pc[idx + i + n1*(j+k*n2)];
        }
      }
    }
    wfl->rdata[ir] = rv;
  }
  sf_floatwrite(wfl->rdata,nr,wfl->Fdata);

  free(wfl->rdata);
}

static void extract_scat_dat_2d(wfl_struct_t * const wfl,acq_struct_t const *acq){

  long nr = acq->nr;
  long n1 = wfl->simN1;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;
  float d1 = wfl->d1;
  float d2 = wfl->d2;

  wfl->rdata = sf_floatalloc(nr);
  for (long ir=0; ir<nr; ir++){
    float xr = acq->rcoord[2*ir];
    float zr = acq->rcoord[2*ir+1];
    long ixr = (xr - o2)/d2;
    long izr = (zr - o1)/d1;
    long idx = izr + n1*ixr;
    float rv = 0.;
    for (int j=-3,jh=0; j<=4; j++,jh++){
      const float hicks2 = acq->hicksRcv2[jh+ir*8];
      for (int i=-3,ih=0; i<=4; i++,ih++){
        const float hc = acq->hicksRcv1[ih+ir*8]*hicks2;
        rv += hc*wfl->pc[idx + i +j*n1];
      }
    }
    wfl->rdata[ir] = rv;
  }

  sf_floatwrite(wfl->rdata,nr,wfl->Fsdata);

  free(wfl->rdata);

}

static void extract_scat_dat_3d(wfl_struct_t * const wfl,acq_struct_t const *acq){

  long nr = acq->nr;
  long n1 = wfl->simN1;
  long n2 = wfl->simN2;
  float o1 = wfl->simO1;
  float o2 = wfl->simO2;
  float o3 = wfl->simO3;
  float d1 = wfl->d1;
  float d2 = wfl->d2;
  float d3 = wfl->d3;

  wfl->rdata = sf_floatalloc(nr);

  for (long ir=0; ir<nr; ir++){
    float xr = acq->rcoord[3*ir  ];
    float yr = acq->rcoord[3*ir+1];
    float zr = acq->rcoord[3*ir+2];
    long ixr = (xr - o2)/d2;
    long iyr = (yr - o3)/d3;
    long izr = (zr - o1)/d1;
    long idx = izr + n1*(ixr + iyr*n2);

    float rv = 0.;
    for (int k=-3,kh=0; k<=4; k++,kh++){
      const float hicks3 = acq->hicksRcv3[kh+ir*8];
      for (int j=-3,jh=0; j<=4; j++,jh++){
        const float hicks2 = acq->hicksRcv2[jh+ir*8];
        for (int i=-3,ih=0; i<=4; i++,ih++){
          const float hc = acq->hicksRcv1[ih+ir*8]*hicks2*hicks3;
          rv += hc*wfl->pc[idx + i + n1*(j + k*n2)];
        }
      }
    }
    wfl->rdata[ir] = rv;
  }
  sf_floatwrite(wfl->rdata,nr,wfl->Fsdata);

  free(wfl->rdata);
}

void applyFreeSurfaceBC2d(wfl_struct_t *wfl)
/*< apply the free surface boundary condition at the top of the model in 2d >*/
{

  long const nb = wfl->nabc;
  long const n1 = wfl->simN1;
  long const n2 = wfl->simN2;

  for (long i2=0; i2<n2; i2++)
    for (long i1=0; i1<nb+1; i1++)
      wfl->pc[i1+i2*n1]=0.;

}

void applyFreeSurfaceBC3d(wfl_struct_t *wfl)
/*< apply the free surface boundary condition at the top of the model in 3d >*/
{

  long const nb = wfl->nabc;
  long const n1 = wfl->simN1;
  long const n2 = wfl->simN2;
  long const n3 = wfl->simN3;

  for (long i3=0; i3<n3; i3++)
    for (long i2=0; i2<n2; i2++)
      for (long i1=0; i1<nb+1; i1++)
        wfl->pc[i1+n1*(i2+n2*i3)]=0.;

}

void fwdextrap2d(wfl_struct_t * wfl, acq_struct_t const * acq, mod_struct_t const * mod)
/*< extrapolation kernel 2d - forward operator >*/
{
  sf_warning("FORWARD EXTRAPOLATION..");

  long modN1=wfl->modN1;
  long modN2=wfl->modN2;
  long nelem=modN1*modN2;
  long nt = acq->ntdat;

  // loop over time
  for (long it=0; it<nt; it++){
    bool save = ((wfl->snap) && !(it%wfl->jsnap));
    if ((it+1)%100==0)
      sf_warning("Time step %4d of %4d (%2.1f %%)",it, nt, ((100.*it)/nt));

    tic("velupd2d");
    velupd2d(wfl,mod,acq,FWD);
    toc("velupd2d");

    tic("presupd2d");
    presupd2d(wfl,mod,acq,FWD);
    toc("presupd2d");

    tic("injectPsource2d");
    injectPsource2d(wfl,mod,acq,it);
    toc("injectPsource2d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC2d");
      applyFreeSurfaceBC2d(wfl);
      toc("applyFreeSurfaceBC2d");
    }

    // write the wavefield out
    if (save){
      tic("extract_pres_wfl_2d");
      extract_pres_wfl_2d(wfl);
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fwfl);
      toc("extract_pres_wfl_2d");
    }
    // extract the data at the receiver locations
    tic("extract_dat_2d");
    extract_dat_2d(wfl,acq);
    toc("extract_dat_2d");

    swapwfl2d(wfl);
  }

}

void fwdextrap3d(wfl_struct_t * wfl, acq_struct_t const * acq, mod_struct_t const * mod)
/*< extrapolation kernel 2d - forward operator >*/
{
  sf_warning("FORWARD EXTRAPOLATION..");

  long modN1=wfl->modN1;
  long modN2=wfl->modN2;
  long modN3=wfl->modN3;
  long nelem=modN1*modN2*modN3;
  long nt = acq->ntdat;

  // loop over time
  for (long it=0; it<nt; it++){
    bool save = ((wfl->snap) && !(it%wfl->jsnap));
    if ((it+1)%100==0)
      sf_warning("Time step %4d of %4d (%2.1f %%)",it, nt, ((100.*it)/nt));

    tic("velupd3d");
    velupd3d(wfl,mod,acq,FWD);
    toc("velupd3d");

    tic("presupd3d");
    presupd3d(wfl,mod,acq,FWD);
    toc("presupd3d");

    tic("injectPsource3d");
    injectPsource3d(wfl,mod,acq,it);
    toc("injectPsource3d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC3d");
      applyFreeSurfaceBC3d(wfl);
      toc("applyFreeSurfaceBC3d");
    }
    // write the wavefield out
    if (save){
      tic("extract_pres_wfl_3d");
      extract_pres_wfl_3d(wfl);
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fwfl);
      toc("extract_pres_wfl_3d");
    }
    // extract the data at the receiver locations
    tic("extract_dat_3d");
    extract_dat_3d(wfl,acq);
    toc("extract_dat_3d");

    swapwfl3d(wfl);
  }

}

void bornbckwfl2d(wfl_struct_t * wfl, acq_struct_t const * acq,  mod_struct_t const * mod, born_setup_struct_t para)
/*< Born background wavefield extrapolation >*/
{
  long modN1 =wfl->modN1;
  long modN2 =wfl->modN2;
  int nt = acq->ntdat;
  long nelem = modN1*modN2;
  bool saveData= para.outputBackgroundData;

  // loop over time
  for (int it=0; it<nt; it++){

    tic("velupd2d");
    velupd2d(wfl,mod,acq,FWD);
    toc("velupd2d");

    // extract the velocity secondary sources
    if (para.inputDenPerturbation || para.outputDenPertImage){
      tic("born_velocity_sources_2d");
      born_velocity_sources_2d(wfl,mod,acq);
      toc("born_velocity_sources_2d");
    }

    tic("presupd2d");
    presupd2d(wfl,mod,acq,FWD);
    toc("presupd2d");

    tic("injectPsource2d");
    injectPsource2d(wfl,mod,acq,it);
    toc("injectPsource2d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC2d");
      applyFreeSurfaceBC2d(wfl);
      toc("applyFreeSurfaceBC2d");
    }

    // extract the pressure secondary sources
    tic("born_pressure_sources_2d");
    born_pressure_sources_2d(wfl,mod,acq);
    toc("born_pressure_sources_2d");

    // write the wavefield out
    tic("extract_pres_wfl_2d");
    extract_pres_wfl_2d(wfl);

    if (para.outputBackgroundWfl)
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fwfl);
    else
      fwrite(wfl->bwfl,sizeof(float),nelem,para.Fbwfl);

    toc("extract_pres_wfl_2d");

    // extract the data at the receiver locations
    if (saveData){
      tic("extract_dat_2d");
      extract_dat_2d(wfl,acq);
      toc("extract_dat_2d");
    }

    swapwfl2d(wfl);
  }

}

void bornbckwfl3d(wfl_struct_t * wfl, acq_struct_t const * acq,  mod_struct_t const * mod, born_setup_struct_t para)
/*< Born background wavefield extrapolation >*/
{
  long modN1 =wfl->modN1;
  long modN2 =wfl->modN2;
  long modN3 =wfl->modN3;
  int nt = acq->ntdat;
  long nelem = modN1*modN2*modN3;
  bool saveData= para.outputBackgroundData;

  // loop over time
  for (int it=0; it<nt; it++){

    tic("velupd3d");
    velupd3d(wfl,mod,acq,FWD);
    toc("velupd3d");

    // extract the velocity secondary sources
    if (para.inputDenPerturbation || para.outputDenPertImage){
      tic("born_velocity_sources_3d");
      born_velocity_sources_3d(wfl,mod,acq);
      toc("born_velocity_sources_3d");
    }

    tic("presupd3d");
    presupd3d(wfl,mod,acq,FWD);
    toc("presupd3d");

    tic("injectPsource3d");
    injectPsource3d(wfl,mod,acq,it);
    toc("injectPsource3d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC3d");
      applyFreeSurfaceBC3d(wfl);
      toc("applyFreeSurfaceBC3d");
    }

    // extract the pressure secondary sources
    tic("born_pressure_sources_3d");
    born_pressure_sources_3d(wfl,mod,acq);
    toc("born_pressure_sources_3d");

    // write the wavefield out
    tic("extract_pres_wfl_3d");
    extract_pres_wfl_3d(wfl);

    if (para.outputBackgroundWfl)
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fwfl);
    else
      fwrite(wfl->bwfl,sizeof(float),nelem,para.Fbwfl);

    toc("extract_pres_wfl_3d");

    // extract the data at the receiver locations
    if (saveData){
      tic("extract_dat_3d");
      extract_dat_3d(wfl,acq);
      toc("extract_dat_3d");
    }

    swapwfl3d(wfl);
  }

}

void bornfwdextrap2d(wfl_struct_t * wfl, acq_struct_t const * acq, mod_struct_t const * mod, born_setup_struct_t para)
/*< kernel for Born forward extrapolation >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long nelem = modN1*modN2;
  int nt = acq->ntdat;

  // loop over time
  for (int it=0; it<nt; it++){
    bool save = (wfl->Fswfl);

    tic("velupd2d");
    velupd2d(wfl,mod,acq,FWD);
    toc("velupd2d");

    if (para.inputDenPerturbation){
      tic("injectBornVelocitySource2d");
      injectBornVelocitySource2d(wfl,mod,acq,it);
      toc("injectBornVelocitySource2d");
    }

    tic("presupd2d");
    presupd2d(wfl,mod,acq,FWD);
    toc("presupd2d");

    if (para.inputVelPerturbation){
      tic("injectBornPressureSource2d");
      injectBornPressureSource2d(wfl,mod,acq,it);
      toc("injectBornPressureSource2d");
    }


    if (wfl->freesurf){
      tic("applyFreeSurfaceBC2d");
      applyFreeSurfaceBC2d(wfl);
      toc("applyFreeSurfaceBC2d");
    }

    // write the wavefield out
    if (save){
      tic("extract_pres_wfl_2d");
      extract_pres_wfl_2d(wfl);
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fswfl);
      toc("extract_pres_wfl_2d");
    }

    // extract the data at the receiver locations
    tic("extract_scat_dat_2d");
    extract_scat_dat_2d(wfl,acq);
    toc("extract_scat_dat_2d");

    swapwfl2d(wfl);
  }
}

void bornfwdextrap3d(wfl_struct_t * wfl, acq_struct_t const * acq, mod_struct_t const * mod, born_setup_struct_t para)
/*< kernel for Born forward extrapolation >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long modN3 = wfl->modN3;
  long nelem = modN1*modN2*modN3;
  int nt = acq->ntdat;

  // loop over time
  for (int it=0; it<nt; it++){
    bool save = (wfl->Fswfl);

    tic("velupd3d");
    velupd3d(wfl,mod,acq,FWD);
    toc("velupd3d");

    if (para.inputDenPerturbation){
      tic("injectBornVelocitySource3d");
      injectBornVelocitySource3d(wfl,mod,acq,it);
      toc("injectBornVelocitySource3d");
    }

    tic("presupd3d");
    presupd3d(wfl,mod,acq,FWD);
    toc("velupd3d");

    if (para.inputVelPerturbation){
      tic("injectBornPressureSource3d");
      injectBornPressureSource3d(wfl,mod,acq,it);
      toc("injectBornPressureSource3d");
    }

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC3d");
      applyFreeSurfaceBC3d(wfl);
      toc("applyFreeSurfaceBC3d");
    }

    // write the wavefield out
    if (save){
      tic("extract_pres_wfl_3d");
      extract_pres_wfl_3d(wfl);
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fswfl);
      toc("extract_pres_wfl_3d");
    }

    // extract the data at the receiver locations
    tic("extract_scat_dat_3d");
    extract_scat_dat_3d(wfl,acq);
    toc("extract_scat_dat_3d");

    swapwfl3d(wfl);
  }

}

void adjextrap2d(wfl_struct_t * wfl, acq_struct_t const * acq, mod_struct_t const * mod)
/*< extrapolation kernel 2d - adjoint operator  >*/
{
  sf_warning("ADJOINT EXTRAPOLATION..");

  long modN1=wfl->modN1;
  long modN2=wfl->modN2;
  long nelem=modN1*modN2;
  long nt = acq->ntdat;

  // loop over time
  for (long it=0; it<nt; it++){
    bool save = ((wfl->snap) && !(it%wfl->jsnap));
    if ((it+1)%100==0)
      sf_warning("Time step %4d of %4d (%2.1f %%)",it, nt, ((100.*it)/nt));

    tic("velupd2d");
    velupd2d(wfl,mod,acq,ADJ);
    toc("velupd2d");

    tic("presupd2d");
    presupd2d(wfl,mod,acq,ADJ);
    toc("presupd2d");

    tic("injectPsource2d");
    injectPsource2d(wfl,mod,acq,it);
    toc("injectPsource2d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC2d");
      applyFreeSurfaceBC2d(wfl);
      toc("applyFreeSurfaceBC2d");
    }

    // write the wavefield out
    if (save) {
      tic("extract_pres_wfl_2d");
      extract_pres_wfl_2d(wfl);
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fwfl);
      toc("extract_pres_wfl_2d");
    }

    swapwfl2d(wfl);
  }

}

void adjextrap3d(wfl_struct_t * wfl, acq_struct_t const * acq, mod_struct_t const * mod)
/*< extrapolation kernel 2d - adjoint operator  >*/
{
  sf_warning("ADJOINT EXTRAPOLATION..");

  long modN1=wfl->modN1;
  long modN2=wfl->modN2;
  long modN3=wfl->modN3;
  long nelem=modN1*modN2*modN3;
  long nt = acq->ntdat;

  // loop over time
  for (long it=0; it<nt; it++){
    bool save = ((wfl->snap) && !(it%wfl->jsnap));
    if ((it+1)%100==0)
      sf_warning("Time step %4d of %4d (%2.1f %%)",it, nt, ((100.*it)/nt));

    tic("velupd3d");
    velupd3d(wfl,mod,acq,ADJ);
    toc("velupd3d");

    tic("presupd3d");
    presupd3d(wfl,mod,acq,ADJ);
    toc("presupd3d");

    tic("injectPsource3d");
    injectPsource3d(wfl,mod,acq,it);
    toc("injectPsource3d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC3d");
      applyFreeSurfaceBC3d(wfl);
      toc("applyFreeSurfaceBC3d");
    }

    // write the wavefield out
    if (save) {
      tic("extract_pres_wfl_3d");
      extract_pres_wfl_3d(wfl);
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fwfl);
      toc("extract_pres_wfl_3d");
    }

    swapwfl3d(wfl);
  }

}

void bornadjextrap2d(wfl_struct_t * wfl,
                     acq_struct_t const * acq,
                     mod_struct_t const * mod,
                     born_setup_struct_t *para)
/*< kernel for Born forward extrapolation >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long nelem = modN1*modN2;
  int nt = acq->ntdat;
  bool save = para->outputScatteredWfl;

  // loop over time
  for (int it=0; it<nt; it++){

    tic("velupd2d");
    velupd2d(wfl,mod,acq,ADJ);
    toc("velupd2d");

    if (para->outputDenPertImage){
      tic("extract_vel_wfl_2d");
      extract_vel_wfl_2d(wfl, 1);
      toc("extract_vel_wfl_2d");
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fpv1);
      tic("extract_vel_wfl_2d");
      extract_vel_wfl_2d(wfl, 2);
      toc("extract_vel_wfl_2d");
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fpv2);
    }

    tic("presupd2d");
    presupd2d(wfl,mod,acq,ADJ);
    toc("presupd2d");

    tic("injectPdata2d");
    injectPdata2d(wfl,mod,acq,it);
    toc("injectPdata2d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC2d");
      applyFreeSurfaceBC2d(wfl);
      toc("applyFreeSurfaceBC2d");
    }

    // write the wavefield out
    tic("extract_pres_wfl_2d");
    extract_pres_wfl_2d(wfl);
    if (save)
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fswfl);
    else
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fswfl);
    toc("extract_pres_wfl_2d");

    swapwfl2d(wfl);
  }

}

void bornadjextrap3d(wfl_struct_t * wfl,
                     acq_struct_t const * acq,
                     mod_struct_t const * mod,
                     born_setup_struct_t *para)
/*< kernel for Born forward extrapolation >*/
{
  long modN1 = wfl->modN1;
  long modN2 = wfl->modN2;
  long modN3 = wfl->modN3;
  long nelem = modN1*modN2*modN3;
  int nt = acq->ntdat;
  bool save = para->outputScatteredWfl;

  // loop over time
  for (int it=0; it<nt; it++){

    tic("velupd3d");
    velupd3d(wfl,mod,acq,ADJ);
    toc("velupd3d");

    if (para->outputDenPertImage){
      tic("extract_vel_wfl_3d");
      extract_vel_wfl_3d(wfl, 1);
      toc("extract_vel_wfl_3d");
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fpv1);
      tic("extract_vel_wfl_3d");
      extract_vel_wfl_3d(wfl, 2);
      toc("extract_vel_wfl_3d");
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fpv2);
      tic("extract_vel_wfl_3d");
      extract_vel_wfl_3d(wfl, 3);
      toc("extract_vel_wfl_3d");
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fpv3);
    }

    tic("presupd3d");
    presupd3d(wfl,mod,acq,ADJ);
    toc("presupd3d");

    tic("injectPdata3d");
    injectPdata3d(wfl,mod,acq,it);
    toc("injectPdata3d");

    if (wfl->freesurf){
      tic("applyFreeSurfaceBC3d");
      applyFreeSurfaceBC3d(wfl);
      toc("applyFreeSurfaceBC3d");
    }

    // write the wavefield out
    tic("extract_pres_wfl_3d");
    extract_pres_wfl_3d(wfl);
    if (save)
      sf_floatwrite(wfl->bwfl,nelem,wfl->Fswfl);
    else
      fwrite(wfl->bwfl,sizeof(float),nelem,para->Fswfl);

    toc("extract_pres_wfl_3d");

    swapwfl3d(wfl);
  }

}

void setupABC_2d(wfl_struct_t* wfl)
/*< Setup of the coefficients for the absorbing boundary >*/
{

  float tapbase = 0.92;
  float alpha = sqrt(-log(tapbase));

  for (long i=0; i<wfl->simN1; i++)
    wfl->tap1[i] = 1.;
  for (long i=0; i<wfl->simN2; i++)
    wfl->tap2[i] = 1.;

  for (int i=0; i<NOP; i++){
    float tap = exp(-alpha*alpha);
    wfl->tap1[i] = tap;
    wfl->tap2[i] = tap;
    wfl->tap1[wfl->simN1-1-i] = tap;
    wfl->tap2[wfl->simN2-1-i] = tap;
  }

  float taplen = wfl->nabc-NOP;
  for (int i=0,j=NOP; i<taplen; i++,j++){
    float arg = alpha*(taplen-i)/taplen;
    float tap = exp(-arg*arg);
    wfl->tap1[j] = tap;
    wfl->tap2[j] = tap;
    wfl->tap1[wfl->simN1-1-j] = tap;
    wfl->tap2[wfl->simN2-1-j] = tap;
  }

}

void setupABC_3d(wfl_struct_t* wfl)
/*< Setup of the coefficients for the absorbing boundary >*/
{
  float tapbase = 0.92;
  float alpha = sqrt(-log(tapbase));

  for (long i=0; i<wfl->simN1; i++)
    wfl->tap1[i] = 1.;
  for (long i=0; i<wfl->simN2; i++)
    wfl->tap2[i] = 1.;
  for (long i=0; i<wfl->simN3; i++)
    wfl->tap3[i] = 1.;

  for (int i=0; i<NOP; i++){
    float tap = exp(-alpha*alpha);
    wfl->tap1[i] = tap;
    wfl->tap2[i] = tap;
    wfl->tap3[i] = tap;
    wfl->tap1[wfl->simN1-1-i] = tap;
    wfl->tap2[wfl->simN2-1-i] = tap;
    wfl->tap3[wfl->simN3-1-i] = tap;
  }

  float taplen = wfl->nabc-NOP;
  for (int i=0,j=NOP; i<taplen; i++,j++){
    float arg = alpha*(taplen-i)/taplen;
    float tap = exp(-arg*arg);
    wfl->tap1[j] = tap;
    wfl->tap2[j] = tap;
    wfl->tap3[j] = tap;
    wfl->tap1[wfl->simN1-1-j] = tap;
    wfl->tap2[wfl->simN2-1-j] = tap;
    wfl->tap3[wfl->simN3-1-j] = tap;
  }

}

void reset_wfl_2d(wfl_struct_t* wfl)
/*< reset the wavefields to zero >*/
{

  long n1=wfl->simN1;
  long n2=wfl->simN2;
  long nelem=n1*n2;
  memset(wfl->v1c,0,nelem*sizeof(float));
  memset(wfl->v2c,0,nelem*sizeof(float));
  memset(wfl->v1p,0,nelem*sizeof(float));
  memset(wfl->v2p,0,nelem*sizeof(float));

  memset(wfl->pc,0,nelem*sizeof(float));
  memset(wfl->pp,0,nelem*sizeof(float));

}

void reset_wfl_3d(wfl_struct_t* wfl)
/*< reset the wavefields to zero >*/
{

  long n1=wfl->simN1;
  long n2=wfl->simN2;
  long n3=wfl->simN3;
  long nelem=n1*n2*n3;
  memset(wfl->v1c,0,nelem*sizeof(float));
  memset(wfl->v2c,0,nelem*sizeof(float));
  memset(wfl->v3c,0,nelem*sizeof(float));
  memset(wfl->v1p,0,nelem*sizeof(float));
  memset(wfl->v2p,0,nelem*sizeof(float));
  memset(wfl->v3p,0,nelem*sizeof(float));

  memset(wfl->pc,0,nelem*sizeof(float));
  memset(wfl->pp,0,nelem*sizeof(float));

}

