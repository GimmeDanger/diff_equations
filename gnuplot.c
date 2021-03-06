#include <stdio.h>
#include <math.h>
#include "gnuplot.h"
#include "func.h"
#include "config.h"

char *plot_name (char *name, double tau, double h1, double h2, int t)
{
  //sprintf(name, "../plotdata/th%02d%02d/level%d.log", (int)(tau*100), (int)(h*100), t);
  sprintf (name, "t%02dh%02dh%02dl%d.log", (int) (tau * 100), (int) (h1 * 100), (int) (h2 * 100), t);
  //printf("%s", name);
  return name;
}


char *tex_name (char *name, double tau, double h1, double h2, int t)
{
  //sprintf(name, "../plotdata/th%02d%02d/level%d.tex", (int)(tau*100), (int)(h*100), t);
  sprintf (name, "t%02dh%02dh%02dl%d.tex", (int) (tau * 100), (int) (h1 * 100), (int) (h2 * 100), t);
  return name;
}

void print_plot (const char *file_name, double *X, double *Y,
                 double *G, double *V1, double *V2,
                 int size, double tt)
{
  FILE *fp;
  int i;

  if (!file_name)
    {
      printf ("Incorrect filename by time %lf\n", tt);
    }

  if (! (fp = fopen (file_name, "w+")))
    {
      printf ("Cannot create file %s\n", file_name);
      return;
    }

  for (i = 0; i < size; i++)
    {
      //printf("%i \n", i);
      fprintf (fp, "%f %f %f %f %f\n", Y[i], X[i], V2[i], V1[i], exp (G[i]));

      if ( i < size - 1 && fabs (Y[i] - Y[i + 1]) > 1e-6)
        {
          fprintf (fp, "\n");
        }
    }

  fclose (fp);
}

void print_data (const char *file_name, double *G, double *V1, double *V2, int size)
{
  FILE *fp;
  int i;

  if (!file_name)
    {
      printf ("Incorrect filename\n");
    }

  if (! (fp = fopen (file_name, "w")))
    {
      printf ("Cannot create file %s\n", file_name);
      return;
    }

  fprintf (fp, "%d \n", size);

  for (i = 0; i < size; i++)
    {
      fprintf (fp, "%lf ", G[i]);
    }

  fprintf (fp, "\n");

  for (i = 0; i < size; i++)
    {
      fprintf (fp, "%lf ", V1[i]);
    }

  fprintf (fp, "\n");

  for (i = 0; i < size; i++)
    {
      fprintf (fp, "%lf ", V2[i]);
    }

  fprintf (fp, "\n");

  fclose (fp);
}




int make_graph (const char *texname, const char *plotname, double h1, double h2, double tau, double t)
{
  FILE *fout;

  if ((fout = fopen (texname, "w")))
    {
      fprintf (fout, "\\begin{minipage}{\\linewidth}\n"
               "\\centering\n");

      if (h1 > 0.)
        fprintf (fout,
                 "$\\tau = %1.3f$, $h = \\left(%1.3f, %1.3f\\right)$, $t = %1.3f$\n\n\n",
                 tau, h1, h2, t);

      fprintf (fout,
               "\\begin{minipage}{0.49 \\linewidth}\n"
               "\\begin{figure}[H]\n"
               "\\begin{gnuplot}\n"
               "set terminal epslatex color size 7cm,7cm\n"
               "set size ratio 1\n"
               "set xrange [0:3]\n"
               "set yrange [0:3]\n"
               "set xlabel \"X\" font ',18'\n"
               "set ylabel \"Y\" offset 1,0 font ',18'\n"
               "unset key\n"
               "set colorbox\n"
               "load 'paired.plt'\n"
               "plot '%s' every 4:4 using 2:1:($4/(4*sqrt($4*$4+$3*$3))):"
               "($3/(4*sqrt($4*$4+$3*$3))):(sqrt($4*$4+$3*$3)) "
               "with vectors filled lc palette title ' '\n"
               "\\end{gnuplot}\n"
               "\\caption{Скорость}\n"
               "\\end{figure}\n"
               "\\end{minipage}\n"
               "\\hfill\n"
               "\\begin{minipage}{0.49\\linewidth}\n"
               "\\begin{figure}[H]\n"
               "\\begin{gnuplot}\n"
               "set terminal epslatex color size 9cm,9cm\n"
               "set size ratio 1\n"
               "set xrange [0:3]\n"
               "set yrange [0:3]\n"
               "set xlabel \"X\" font ',18'\n"
               "set ylabel \"Y\" offset 1,0 font ',18'\n"
               ///"unset colorbox\n"
               "set view equal xy\n"
               "set view 0, 0\n"
               "unset ztics\n"
               "splot '%s' using 2:1:5 with pm3d at b t ' ' \n"
               "\\end{gnuplot}\n"
               "\\caption{Плотность}\n"
               "\\end{figure}\n"
               "\\end{minipage}\n"
               "\\end{minipage}\n"
               "\\vspace{1cm}\n", plotname,
               plotname);
    }
  else
    {
      printf ("Can't open file %s\n", texname);
      return -1;
    }

  fclose (fout);

  if (calc_type == SMOOTH)
    {
      fout = fopen (OUTTEX_SMOOTH, "a+");
    }
  else
    {
      fout = fopen (OUTTEX_ABRUPT, "a+");
    }

  if (fout != NULL)
    {
      fprintf (fout, "\\input{%s} \n", texname);
    }
  else
    {
      printf ("Can't open OUTTEX file\n");
      return -1;
    }

  fclose (fout);

  return 0;
}


void printhead (FILE *fout)
{
  fprintf (fout, "\\documentclass[a4paper, 12pt]{article} \n");
  fprintf (fout, "\\usepackage[utf8]{inputenc} \n");
  fprintf (fout, "\\usepackage[left=1cm,right=1cm,top=2cm,bottom=2cm,"
           "bindingoffset=0cm]{geometry} \n");
  fprintf (fout, "\\usepackage[english,russian]{babel} \n");
  fprintf (fout, "\\usepackage{amsmath,amssymb,amsfonts,textcomp,latexsym,"
           "pb-diagram,amsopn} \n");
  fprintf (fout, "\\usepackage{cite,enumerate,float,indentfirst} \n");
  fprintf (fout, "\\usepackage{graphicx,xcolor} \n");
  fprintf (fout, "\\usepackage{gnuplottex} \n");

  fprintf (fout, "\\begin{document} \n");

}
void printtail (FILE *fout)
{
  fprintf (fout, "\\end{document} \n");
}
