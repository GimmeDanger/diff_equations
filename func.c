#include <math.h>
#include "func.h"

int is_equal (double x1, double x2)
{
  return fabs (x1 - x2) < MINIMAL_FOR_COMPARE;
}

void param_dif (P_dif *p_d)
{
#if STAT_SOL_SRCH || EIG_FUNC_INIT
  p_d->Segm_T = 200;
#elif NO_SMOOTH
  p_d->Segm_T = 5;
#else
  p_d->Segm_T = 1;
#endif
  p_d->Segm_X = 3.;
  p_d->Segm_Y = 3.;
  p_d->Segm_X_0 = 2.;
  p_d->Segm_Y_0 = 1.;
  p_d->p_ro = 1.0;
  p_d->mu = 0.1;
}

void param_she_step (P_she *p_s, P_dif *p_d, int it_t, int it_sp)
{
  // main rectangle
  int i, k_sp, k_t;

  p_s->M_x = 60;
  p_s->M_y = 60;
#if STAT_SOL_SRCH || EIG_FUNC_INIT
  p_s->N   = 4000;
#elif NO_SMOOTH
  p_s->N   = 100;
#else
  p_s->N   = 20;
#endif

  // cut rectangle
  p_s->M_x_0 = 40;
  p_s->M_y_0 = 20;

  k_sp = 1;
  k_t = 1;

  if (it_sp > 0)
    {
      for (i = 1; i <= it_sp; i++)
        {
          k_sp *= 2;
        }
    }

  if (it_t > 0)
    {
      for (i = 1; i <= it_t; i++)
        {
          k_t *= 2;
        }
    }

  p_s->M_x *= k_sp;
  p_s->M_y *= k_sp;
  p_s->N   *= k_t;

  p_s->M_x_0 *= k_sp;
  p_s->M_y_0 *= k_sp;

  switch (grid_type)
    {
      case SQUARE:
        {
          p_s->Dim = (p_s->M_x + 1) * (p_s->M_y + 1);
          break;
        }

      case VOLODYA_9:
        {
          // cut rectangle
          p_s->M_x_0 = 40;
          p_s->M_y_0 = 20;
          p_s->M_x_0 *= k_sp;
          p_s->M_y_0 *= k_sp;
          p_s->Dim = (p_s->M_x + 1) * (p_s->M_y + 1) - (p_s->M_x_0 + 0) * (p_s->M_y_0 + 0);
          break;
        }

      case RAMZAN_10:
        {
          p_s->M_x_0 = 20;
          p_s->M_y_0 = 20;
          p_s->M_x_0 *= k_sp;
          p_s->M_y_0 *= k_sp;
          p_s->Dim = (p_s->M_x + 1) * (p_s->M_y + 1) - 3 * (p_s->M_x_0 + 0) * (p_s->M_y_0 + 0);
          break;
        }

  case NASTYA_11:
          {
            // main rectangle
            p_s->M_x = 60;
            p_s->M_y = 60;
          #if STAT_SOL_SRCH || EIG_FUNC_INIT
            p_s->N   = 2000;
          #elif NO_SMOOTH
            p_s->N   = 100;
          #else
            p_s->N   = 20;
          #endif

            // cut rectangle
            p_s->M_x_0 = 20;
            p_s->M_y_0 = 20;

            k_sp = 1;
            k_t = 1;

            if (it_sp > 0)
              {
                for (i = 1; i <= it_sp; i++)
                  {
                    k_sp *= 2;
                  }
              }

            if (it_t > 0)
              {
                for (i = 1; i <= it_t; i++)
                  {
                    k_t *= 2;
                  }
              }

            p_s->M_x *= k_sp;
            p_s->M_y *= k_sp;
            p_s->N   *= k_t;

            p_s->M_x_0 *= k_sp;
            p_s->M_y_0 *= k_sp;

            p_s->Dim = (p_s->M_x + 1) * (p_s->M_y + 1) - 4 * (p_s->M_x_0 + 0) * (p_s->M_y_0 + 0);
            break;
          }
    }

  p_s->h_x = p_d->Segm_X / p_s->M_x;
  p_s->h_y = p_d->Segm_Y / p_s->M_y;
  p_s->tau = p_d->Segm_T / p_s->N;
  p_s->eta = 0.;
}

//*******************************************************

double Norm_c (double *a, int Dim, double *X, double *Y, double t, double (*f) (double tt, double x1, double x2))
{
  int m;
  double norm = 0.;
  double tmp;

  for (m = 0; m < Dim; m++)
    {
      tmp = fabs (a[m] - (*f) (t, X[m], Y[m]));

      if (tmp > norm)
        {
          norm = tmp;
        }
    }

  return norm;
}

double Norm_l2 (double *a, int Dim, double *X, double *Y, double t, double (*f) (double tt, double x1, double x2))
{
  int m;
  double norm = 0.;
  double tmp;

  for (m = 0; m < Dim; m++)
    {
      tmp = a[m] - (*f) (t, X[m], Y[m]);
      norm += tmp * tmp;
    }

  return sqrt (norm / Dim);
}

inline double ro (double t, double x, double y)
{
  if (NO_SMOOTH)
    {
      // Left boundary
      if (is_equal (x, 0.))
        {
          return RHO_G;
        }

      // First step
      if (is_equal (t, 0.))
        {
          return RHO_0;
        }

      return (double) ((cos (M_PI * x) + 1.5) * (sin (M_PI * y) + 1.5) * exp (t));
    }
  else
    {
      if (LIGHT_G)
        {
          return (double) ((cos (M_PI * x) + 1.5) * (sin (M_PI * y) + 1.5));
        }
      else
        {
          return (double) ((cos (M_PI * x) + 1.5) * (sin (M_PI * y) + 1.5) * exp (t));
        }
    }
}

inline double gg (double t, double x, double y)
{
  return log (ro (t, x, y));
}

#define d_gg_dt 0.
#define d_gg_dx ((-M_PI) * sin (M_PI * x) / (cos (M_PI * x) + 1.5))
#define d_gg_dy ((+M_PI) * cos (M_PI * y) / (sin (M_PI * y) + 1.5))

inline double p (double t, double x, double y, double p_ro)
{
  return p_ro * ro (t, x, y);
}

inline double u1 (double t, double x, double y)
{
  if (NO_SMOOTH)
    {
      // Left boundary
      if (is_equal (x, 0.))
        {
          return W;
        }

      // First step
      if (is_equal (t, 0.))
        {
          return 0.;
        }

      return 0.;
    }
  else
    {
      if (LIGHT_U1)
        {
          return (double) (sin (M_PI * x));
        }
      else
        {
          return (double) (sin (M_PI * x) * sin (M_PI * y) * exp (t));
        }
    }
}

#define d_u1_dt 0.
#define d_u1_dx (M_PI * cos (M_PI * x))
#define d_u1_dy 0.
#define d_u1_dxdx (-M_PI * M_PI * sin (M_PI * x))
#define d_u1_dxdy 0.

inline double u2 (double t, double x, double y)
{
  if (NO_SMOOTH)
    {
      // Left boundary
      if (is_equal (x, 0.))
        {
          return 0.;
        }

      // First step
      if (is_equal (t, 0.))
        {
          return 0.;
        }

      return 0.;
    }
  else
    {
      if (LIGHT_U2)
        {
          return 0.;
        }
      else
        {
          return (double) (sin (M_PI * x) * sin (M_PI * y) * exp (-t));
        }
    }
}

#define d_u2_dt 0.
#define d_u2_dx 0.
#define d_u2_dy 0.
#define d_u2_dxdx 0.
#define d_u2_dydy 0.
#define d_u2_dxdy 0.

inline double Func_g (double t, double x, double y)
{
  if (NO_SMOOTH)
    {
      return 0.;
    }
  else
    {
      if (LIGHT_FUNCS)
        {
          return (double) (d_gg_dt + u1 (t, x, y) * d_gg_dx + u2 (t, x, y) * d_gg_dy + d_u1_dx + d_u2_dy);
        }
      else
        {
          return (double) (1
                           + u1 (t, x, y) * (-1 * M_PI * sin (M_PI * x)) / (cos (M_PI * x) + 1.5)
                           + u2 (t, x, y) * (+1 * M_PI * cos (M_PI * y)) / (sin (M_PI * y) + 1.5)
                           + M_PI * cos (M_PI * x) * sin (M_PI * y) * exp (t)
                           + M_PI * cos (M_PI * y) * sin (M_PI * x) * exp (-t));
        }
    }
}

inline double Func_v1 (double t, double x, double y, double p_ro, double mu)
{
  if (NO_SMOOTH)
    {
      return 0.;
    }
  else
    {
      if (LIGHT_FUNCS)
        {
          return (double) (d_u1_dt + u1 (t, x, y) * d_u1_dx + u2 (t, x, y) * d_u1_dy + p_ro * d_gg_dx
                           - mu * exp (-gg (t, x, y)) * ((4. / 3) * d_u1_dxdx));
        }
      else
        {
          return (double) (u1 (t, x, y)
                           + u1 (t, x, y) * (M_PI * cos (M_PI * x) * sin (M_PI * y) * exp (t))
                           + u2 (t, x, y) * (M_PI * sin (M_PI * x) * cos (M_PI * y) * exp (t))
                           + p_ro * (-1 * M_PI * sin (M_PI * x)) / (cos (M_PI *  x) + 1.5)
                           - mu * exp (-gg (t, x, y))
                           * ((4. / 3.) * (-1) * M_PI * M_PI * sin (M_PI * x) * sin (M_PI * y) * exp (t)
                              + (-1) * M_PI * M_PI * sin (M_PI * x) * sin (M_PI * y) * exp (t)
                              + (1. / 3.) * M_PI * M_PI * cos (M_PI * x) * cos (M_PI * y) * exp (-t)));
        }
    }
}

inline double Func_v2 (double t, double x, double y, double p_ro, double mu)
{
  if (NO_SMOOTH)
    {
      return 0.;
    }
  else
    {
      if (LIGHT_FUNCS)
        {
          return (double) (d_u2_dt + u1 (t, x, y) * d_u2_dx + u2 (t, x, y) * d_u2_dy + p_ro * d_gg_dy);
        }
      else
        {
          return (double) ((-1) * u2 (t, x, y)
                           + u2 (t, x, y) * (M_PI * cos (M_PI * y) * sin (M_PI * x) * exp (-t))
                           + u1 (t, x, y) * (M_PI * sin (M_PI * y) * cos (M_PI * x) * exp (-t))
                           + p_ro * ((M_PI * cos (M_PI * y)) / (sin (M_PI * y) + 1.5))
                           - mu * exp (-gg (t, x, y))
                           * ((4. / 3.) * (-1) * M_PI * M_PI * sin (M_PI * x) * sin (M_PI * y) * exp (-t)
                              + (-1) * M_PI * M_PI * sin (M_PI * x) * sin (M_PI * y) * exp (-t)
                              + (1. / 3.) * M_PI * M_PI * cos (M_PI * y) * cos (M_PI * x) * exp (t)));
        }
    }
}


double calc_sol_residual_norm (int Dim, const double *G, const double *V1,
                               const double *V2, const double *G_prev,
                               const double *V1_prev, const double *V2_prev)
{
  int m;
  double norm = 0.;
  double tmp_1, tmp_2, tmp_3;

  for (m = 0; m < Dim; m++)
    {
      tmp_1 = G[m] - G_prev[m];
      tmp_2 = V1[m] - V1_prev[m];
      tmp_3 = V2[m] - V2_prev[m];
      norm += tmp_1 * tmp_1 + tmp_2 * tmp_2 + tmp_3 * tmp_3;
    }

  return sqrt (norm / (3 * Dim));
}

void init_prev_with_curr (int Dim, double *G, double *V1, double *V2,
                          double *G_prev, double *V1_prev, double *V2_prev)
{
  int i;

  for (i = 0; i < Dim; i++)
    {
      G_prev[i]  = G[i];
      V1_prev[i] = V1[i];
      V2_prev[i] = V2[i];
    }

  return;
}
