  #include "functions.h"

int main()
{
  int i, j, l, number_species, singles;
  /* A vector for keeping all the individuals of the kth generation, and other for the
  /* (k+1)th generation */
  Population progenitors, offspring;
  Graph G, H;
  Parameters info;
  char nome_arq_p[80] = "";
  char nome_arq_s[80] = "";
  char linep[65] = "";
  char lines[65] = "";
  FILE *position;
  FILE *nspecies;
  unsigned int sample;
  time_t t;

  /* This loop is used when more simulations are needed */
  for (l = 0; l < 1; l++) {
    /* Using a fixed seed gives same results at every simulation. */
    //srand (time(NULL));
    srand(1);
    GLOBAL_RNG = gsl_rng_alloc(gsl_rng_taus);

    time(&t);
    printf("********************\n%s\n*********************\n", ctime(&t));

    sprintf (nome_arq_s, "./data/species/v1/numsp_vf_7_%02d.csv", l);
    nspecies = fopen (nome_arq_s, "w");
    fprintf (nspecies, "gen;sp;singles;sim\n");

    if (l == 0) {
      sprintf (nome_arq_p, "./data/position/indloc_vf_7.csv");
      position = fopen (nome_arq_p, "w");
      fprintf (position, "id;x;y;sp;gen\n");
    }

    info = Set_Parameters();

    progenitors = Alloc_Population (info);
    offspring = Alloc_Population (info);  

    Set_Initial_Values (progenitors, info);

    G = CreateGraph (info->individual_vector_size, info->number_individuals);

    printf("idensity = %d\n", info->density);

    printf("SIMULATION %d\n", l);
    for (i = 0; i <= info->number_generations; i++) {
      Stablish_Distances (G, progenitors, info);
      number_species = Count_Species (G, progenitors);
      singles = Count_Singletons (G, progenitors, number_species, info);
      //if (i > 0) CheckSpecies(G, progenitors, info);
  		Reproduction  (G, progenitors, offspring, info);
      if (1) {
        fprintf (nspecies, "%d;%d;%d;%d\n", i, number_species, singles, l);
        if (l == 0) {
          for (j = 0; j < (G->U); j++) {
            fprintf(position, "%d;%f;%f;%d;%d\n", j, progenitors[j]->x, progenitors[j]->y, progenitors[j]->species, i); 
          }
        }
      }
      if (i%5 == 0) {
        printf("GENERATION: %d\n", i);
        printf("pop size: %d\n", G->U);
        printf("NUMBER OF SPECIES = %d\n", number_species);
      }
      Swap_Generations (&progenitors, &offspring);
    }

    DestroyGraph(G);
    Free_Population (progenitors, info);
    Free_Population (offspring, info);
    free (info);
    if (l == 0) fclose (position);
    fclose (nspecies);
    gsl_rng_free (GLOBAL_RNG);
  }

return 0;
}