#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "tabtex.h"
#include "func.h"

void Setka (int *st, double *X, double *Y, int *M0L,
            int *M0R, P_she *p_s, P_dif *p_d);

void Sxema (double *G, double *V1, double *V2, int *st, double *X, double *Y,
            int *M0L, int *M0R, P_she *p_s, P_dif *p_d);

int main()
{
  P_dif p_d;
  param_dif (&p_d);

  int it_t, it_t_max, it_sp, it_sp_max, n_ver, it;
  it_t_max = 3;
  it_sp_max = 3;
  it_t = 0;
  it_sp = 0;

  n_ver = (it_t_max + 1) * (it_sp_max + 1);

  double *nc_g, *nl2_g, *nc_v1, *nc_v2, *nl2_v1, *nl2_v2;
  double *time, *tauit;
  nc_g = (double*) malloc ((n_ver) * sizeof(double));
  nc_v1 = (double*) malloc ((n_ver) * sizeof(double));
  nc_v2 = (double*) malloc ((n_ver) * sizeof(double));
  nl2_g = (double*) malloc ((n_ver) * sizeof(double));
  nl2_v1 = (double*) malloc ((n_ver) * sizeof(double));
  nl2_v2 = (double*) malloc ((n_ver) * sizeof(double));
  time = (double*) malloc ((n_ver) * sizeof(double));
  tauit = (double*) malloc ((it_t_max + 1) * sizeof(double));

  P_she p_s;

  int *st, *M0L, *M0R;
  double *X, *Y, *G, *V1, *V2;

  clock_t BegClock, EndClock;

  it = 0;
  for(it_t = 0; it_t <= it_t_max; it_t++)
    {

      for(it_sp = 0; it_sp <= it_sp_max; it_sp++)
        {
          BegClock = clock ();
          // define area params
          param_she_step (&p_s, &p_d, it_t, it_sp);
          printf ("N = %d, M1 = %d, M2 = %d, Dim = %d\n", p_s.N, p_s.M_x, p_s.M_y, p_s.Dim);

          if (it_sp == 0)
            tauit[it_t] = p_s.tau;

          X = (double*) malloc ((p_s.Dim) * sizeof(double));  // x-coord array of nodes
          Y = (double*) malloc ((p_s.Dim) * sizeof(double));  // y-coord array of nodes
          G = (double*) malloc ((p_s.Dim) * sizeof(double));  // press array of nodes
          V1 = (double*) malloc ((p_s.Dim) * sizeof(double)); // v1 array of nodes
          V2 = (double*) malloc ((p_s.Dim) * sizeof(double)); // v2 array of nodes
          st = (int*) malloc ((p_s.Dim) * sizeof(int));       // status of nodes
          M0L = (int*) malloc ((p_s.Dim) * sizeof(int));
          M0R = (int*) malloc ((p_s.Dim) * sizeof(int));

          // define properties of nodes
          Setka (st, X, Y, M0L, M0R, &p_s, &p_d);

          Sxema (G, V1, V2, st, X, Y, M0L, M0R, &p_s, &p_d);

          EndClock = clock();
          time[it] = (double)(EndClock - BegClock) / CLOCKS_PER_SEC;

          nc_g[it] = Norm_c (G, p_s.Dim, X, Y, p_d.Segm_T, gg);
          nl2_g[it] = Norm_l2 (G, p_s.Dim, X, Y, p_d.Segm_T, gg);
          nc_v1[it] = Norm_c (V1, p_s.Dim, X, Y, p_d.Segm_T, u1);
          nl2_v1[it] = Norm_l2 (V1, p_s.Dim, X, Y, p_d.Segm_T, u1);
          nc_v2[it] = Norm_c (V2, p_s.Dim, X, Y, p_d.Segm_T, u2);
          nl2_v2[it] = Norm_l2 (V2, p_s.Dim, X, Y, p_d.Segm_T, u2);

          // debug
          printf (" %lf %lf %lf \n", nl2_g[it], nl2_v1[it], nl2_v2[it]);

          it++;

          free (G);
          free (V1);
          free (V2);
          free (st);
          free (X);
          free (Y);
          free (M0L);
          free (M0R);
        }
    }

  tabtex_nc_g (it_t_max, it_sp_max, nc_g, tauit, p_d.p_ro, p_d.mu);
  tabtex_nc_v1 (it_t_max, it_sp_max, nc_v1, tauit, p_d.p_ro, p_d.mu);
  tabtex_nc_v2 (it_t_max, it_sp_max, nc_v2, tauit, p_d.p_ro, p_d.mu);
  tabtex_nl2_g (it_t_max, it_sp_max, nl2_g, tauit, p_d.p_ro, p_d.mu);
  tabtex_nl2_v1 (it_t_max, it_sp_max, nl2_v1, tauit, p_d.p_ro, p_d.mu);
  tabtex_nl2_v2 (it_t_max, it_sp_max, nl2_v2, tauit, p_d.p_ro, p_d.mu);
  tabtex_time (it_t_max, it_sp_max, time, tauit, p_d.p_ro, p_d.mu);
  return 0;
}

