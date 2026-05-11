#include <stdio.h>
#include <math.h>

int main(void) {
    int n_lati;
    double lato;
    double perimetro, area;

    printf("Inserisci il numero di lati del poligono regolare: ");
    if (scanf("%d", &n_lati) != 1 || n_lati < 3) {
        printf("Errore: un poligono deve avere almeno 3 lati.\n");
        return 1;
    }

    printf("Inserisci la lunghezza del lato: ");
    if (scanf("%lf", &lato) != 1 || lato <= 0) {
        printf("Errore: la lunghezza del lato deve essere positiva.\n");
        return 1;
    }

    perimetro = n_lati * lato;
    area = (n_lati * lato * lato) / (4.0 * tan(M_PI / n_lati));

    printf("\nPerimetro = %.4f\n", perimetro);
    printf("Area      = %.4f\n", area);

    return 0;
}
