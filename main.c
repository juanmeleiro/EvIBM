#include "include/model.h"

extern gsl_rng *GLOBAL_RNG;

int main (int argc, char* argv[])
{
  int i, j, l, k, number_species, type, deltat, p, genome;
  int sizes[1000];
  char nome_arq_s[100] = "", nome_arq_st[100] = "", nome_arq_ss[100] = "";
  time_t t;
  Population progenitors, offspring;
  Parameters info;
  FILE *nspecies, *performance, *size;

  /* @ main_rand */
  srand (time(&t));
  GLOBAL_RNG = gsl_rng_alloc (gsl_rng_taus);
  gsl_rng_set (GLOBAL_RNG, (int) time(NULL));
  /* @ end */

  info = Set_Parameters();

  l = atoi(argv[3]);

  info->density                = 0.1;
  info->lattice_length         = atof(argv[2]);
  info->lattice_width          = info->lattice_length;
  info->number_individuals     = (int) info->lattice_length*info->lattice_width*info->density;
  info->population_size        = info->number_individuals;
  info->child_population_size  = info->number_individuals;
  info->radius                 = atof(argv[4]);

  printf("*************BEG**************\n%sB = %d, N = %d, L = %.f, R = %.f\n******************************\n", ctime(&t), info->genome_size, info->number_individuals, info->lattice_length, info->radius);
  
  sprintf (nome_arq_s, "./data/sizes_tests/%.f/%.f/species/numsp_%02d.csv", info->radius, info->lattice_length, l);
  nspecies = fopen (nome_arq_s, "w");
  fputs ("gen;sp;pop;sim\n", nspecies);

  sprintf (nome_arq_ss, "./data/sizes_tests/%.f/%.f/sizes/sizes_%02d.csv", info->radius, info->lattice_length, l);
  size = fopen (nome_arq_ss, "w");
  fprintf (size, "sim;gen;sp;size;pop\n");

  /* @ main_alloc */
  progenitors = Alloc_Population (info);
  offspring = Alloc_Population (info);  
  Set_Initial_Position (progenitors, info);
  /* @ end */

  /* @ main_loop */
  printf("Sim \t Gen \t nsp\t pop\n");
  for (i = 0; i <= info->number_generations; i++) {
    Stablish_Distances (progenitors, info);
    if (i%1 == 0) {
      number_species = Count_Species (progenitors, info, sizes);
      fprintf (nspecies, "%d;%d;%d;%d\n", i, number_species, info->population_size, l);
    }
    Reproduction  (progenitors, offspring, info);
    if (i % 25 == 0) {
      //if (info->genome < 15000) FindSpecies (progenitors, info);
      printf(" %d \t %d \t  %d \t %d\n", l, i, number_species, info->population_size);
    }
    if (i % 20 == 0) {  
      for (j = 0; j < number_species; ++j) {
        fprintf (size, "%d;%d;%d;%d;%d\n", l, i, j, sizes[j], info->population_size);
      }
    }
    Swap_Generations (&progenitors, &offspring);
  }
  /* @ end */
  
  /* @ main_free */
  Free_Population (progenitors, info);
  Free_Population (offspring, info);
  fclose (nspecies);
  fclose (size);
  gsl_rng_free (GLOBAL_RNG);
  free (info);
  /* @ end */

  return 0;
}