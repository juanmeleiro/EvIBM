  #include "functions.h"

int main()
{
  int i, j, k, l, number_species, type, deltat;
  int sizes[40];
  Population progenitors, offspring;
  Graph G;
  Parameters info;
  char nome_arq_s[80] = "";
  char nome_arq_ss[80] = "";
  char nome_arq_p[80] = "";
  char nome_arq_st[80] = "";
  FILE *position;
  FILE *nspecies;
  FILE *size;
  FILE *stats;
  //unsigned int sample;
  time_t t;
  clock_t start, end;
  double rho, cpu_time_used_sim, total_cpu_time = 0;
  double n, x, lim;

  info = Set_Parameters();

  printf("type:\n1 to run mu tests\n2 to run rho tests\n3 to run g tests\n4 to run B tests\n");
  scanf ("%d", &type);
  printf("time interval?\n");
  scanf ("%d", &deltat);

  switch (type) {
    case 1:
    n = 0.0001;
    lim = 0.001;
    x = 0.0001;
    break;

    case 2:
    n = 700;
    lim = 2000;
    x = 100; 
    break;

    case 3:
    n = 0.01;
    lim = 0.1;
    x = 0.01;
    break;

    case 4:
    n = 1500;
    lim = 150000;
    x = n*10 - n;
    break;
  }
  printf("Type %d:\nn = %f, lim = %f, x = %f\n", type, n, lim, x);

  for (n; n <= lim; n += x) {
    if (type == 1) {
      info->mutation = n;
      printf("mu = %f\n", info->mutation);
    }
    if (type == 2) {
      info->number_individuals = (int) n;
      info->population_size = info->number_individuals;
      info->individual_vector_size = (int)(info->number_individuals * 1.05);
      rho = 0.83*((double) info->number_individuals)/((double) (info->lattice_length * info->lattice_width));
      info->density = (int) ceil(3.1416*rho*info->radius*info->radius*0.6);
    }
    if (type == 3) {
      info->reproductive_distance = (int)(floor(n*info->genome_size));
      printf("g = %d\n", info->reproductive_distance);
    }
    if (type == 4) {
      info->genome_size = n;
      printf("B = %d\n", info->genome_size);
    } 

    for (l = 0; l < 50; l++) {
      time(&t);
      srand (t);
      start = clock();
      //GLOBAL_RNG = gsl_rng_alloc (gsl_rng_taus);

      if (info->genome_size > 1500) {
        info->number_generations = 1000;
        if (l > 20) return 0;
      }
      
      info->population_size = info->number_individuals;

      printf("*************BEG**************\n%s******************************\n", ctime(&t));
      
      if (type == 1) sprintf (nome_arq_s, "./data/tests/mu/%d/species/numsp_%02d.csv", ((int) (info->mutation*10000)), l);
      if (type == 2) sprintf (nome_arq_s, "./data/tests/rho/%d/species/numsp_%02d.csv", (info->number_individuals/100), l);
      if (type == 3) sprintf (nome_arq_s, "./data/tests/g/%d/species/numsp_%02d.csv", (int)(n*100), l);
      if (type == 4) sprintf (nome_arq_s, "./data/tests/B/%d/species/numsp_%02d.csv", info->genome_size, l);
      nspecies = fopen (nome_arq_s, "w");
      fprintf (nspecies, "gen;sp;sim\n");

      if (type == 1) sprintf (nome_arq_ss, "./data/tests/mu/%d/sizes/sizes_%02d.csv", ((int) (info->mutation*10000)), l);
      if (type == 2) sprintf (nome_arq_ss, "./data/tests/rho/%d/sizes/sizes_%02d.csv", (info->number_individuals/100), l);
      if (type == 3) sprintf (nome_arq_ss, "./data/tests/g/%d/sizes/sizes_%02d.csv", (int)(n*100), l);
      if (type == 4) sprintf (nome_arq_ss, "./data/tests/B/%d/sizes/sizes_%02d.csv", info->genome_size, l);
      size = fopen (nome_arq_ss, "w");
      fprintf (size, "sim;gen;sp;size;pop\n");

      if (l == 0) {
        if (type == 1) sprintf (nome_arq_p, "./data/tests/mu/%d/indloc_00.csv", ((int) (info->mutation*10000)));
        if (type == 2) sprintf (nome_arq_p, "./data/tests/rho/%d/indloc_00.csv", ((int) info->number_individuals/100));
        if (type == 3) sprintf (nome_arq_p, "./data/tests/g/%d/indloc_00.csv", (int)(n*100));
        if (type == 4) sprintf (nome_arq_p, "./data/tests/B/%d/indloc_00.csv", info->genome_size);
        position = fopen (nome_arq_p, "w");
        fprintf (position, "id;x;y;sp;gen\n");
      }

      progenitors = Alloc_Population (info);
      offspring = Alloc_Population (info);  

      Set_Initial_Values (progenitors, info);

      G = CreateGraph (info->individual_vector_size, info->number_individuals);

      printf("Sim \t Gen \t nsp \t pop\n");
      for (i = 0; i <= info->number_generations; i++) {
        Stablish_Distances (G, progenitors, info);
        number_species = Count_Species (G, progenitors);
        Reproduction  (G, progenitors, offspring, info);
        if (i%10 == 0) {
          fprintf (nspecies, "%d;%d;%d\n", i, number_species, l);
        }
        if (i % deltat == 0) {
          Count_Sizes (G, progenitors, number_species, info, sizes);
          for (j = 0; j < number_species; ++j) {
            fprintf (size, "%d;%d;%d;%d;%d\n", l, i, j, sizes[j], (G->U));
          }
          for (j = 0; l == 0 && j < (G->U); j++) {
            fprintf(position, "%d;%f;%f;%d;%d\n", j, progenitors[j]->x, progenitors[j]->y, progenitors[j]->species, i); 
          }
          printf(" %d \t %d \t  %d \t %d\n", l, i, number_species, G->U);
        }
        Swap_Generations (&progenitors, &offspring);
      }

      DestroyGraph(G);
      Free_Population (progenitors, info);
      Free_Population (offspring, info);
      if (l == 0) fclose (position);
      fclose (size);
      fclose (nspecies);
      //gsl_rng_free (GLOBAL_RNG);

      end = clock();
      cpu_time_used_sim = ((double) (end - start)) / CLOCKS_PER_SEC;
      total_cpu_time += cpu_time_used_sim;
    }

  time(&t);
  if (type == 1) sprintf (nome_arq_st, "./data/tests/mu/%d/stats.txt", ((int) (info->mutation*10000)));
  if (type == 2) sprintf (nome_arq_st, "./data/tests/rho/%d/stats.txt", (info->number_individuals/100));
  if (type == 3) sprintf (nome_arq_st, "./data/tests/g/%d/stats.txt", (int)(n*100));
  if (type == 4) sprintf (nome_arq_st, "./data/tests/B/%d/stats.txt", info->genome_size);
  stats = fopen (nome_arq_st, "w");
  fprintf(stats, "STATS:\nfinnish time: %stotal CPU: %f\nmean CPU: %f\nN = %d, G = %d, gen = %d\n", ctime(&t), total_cpu_time, total_cpu_time/l, info->number_individuals, info->genome_size, info->number_generations);
  fclose(stats);
  printf("*******************************\nSTATS:\nfinnish time = %stotal CPU = %f\nmean CPU = %f\nN = %d, G = %d, gen = %d\n", ctime(&t), total_cpu_time, total_cpu_time/l, info->number_individuals, info->genome_size, info->number_generations);
  }

  free (info);

  return 0;
}