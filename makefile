run : main
	./out
	rm *.o out
	cd ../ProjetoFinalJB; Rscript ./R/space.R; Rscript ./R/species.R

main :
	gcc -c -Wall main.c -g
	gcc -o out main.o