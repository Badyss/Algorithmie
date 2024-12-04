#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/******************************************************************************************/
typedef struct element {
    int colonne;
    int valeur;
    struct element* suivant;
} element;


typedef element* liste_ligne;


typedef struct {
    liste_ligne* lignes;
    int Nlignes;
    int Ncolonnes;
} matrice_creuse;
/******************************************************************************************/

matrice_creuse* Creation_Matrice(int Nlignes, int Ncolonnes) {
    matrice_creuse* matrice = (matrice_creuse*)malloc(sizeof(matrice_creuse));
    if (!matrice) {
        printf("Erreur d'allocation mémoire pour la matrice.\n");
        exit(EXIT_FAILURE);
    }

    matrice->Nlignes = Nlignes;
    matrice->Ncolonnes = Ncolonnes;

    matrice->lignes = (liste_ligne*)malloc(Nlignes * sizeof(liste_ligne));
    if (!matrice->lignes) {
        printf("Erreur d'allocation mémoire pour les lignes.\n");
        free(matrice);
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < Nlignes; i++) {
        matrice->lignes[i] = NULL;
    }

    return matrice;
}
/******************************************************************************************/
void ajoute_Element(matrice_creuse* matrice, int ligne, int colonne, int valeur) {
    if (ligne < 0 || ligne >= matrice->Nlignes || colonne < 0 || colonne >= matrice->Ncolonnes) {
        printf("Indices hors limites.\n");
        return;
    }

    element* nouveau = (element*)malloc(sizeof(element));
    if (!nouveau) {
        printf("Erreur d'allocation mémoire pour un élément.\n");
        exit(EXIT_FAILURE);
    }
    nouveau->colonne = colonne;
    nouveau->valeur = valeur;
    nouveau->suivant = NULL;


    element** courant = &matrice->lignes[ligne];
    while (*courant && (*courant)->colonne < colonne) {
        courant = &(*courant)->suivant;
    }
    nouveau->suivant = *courant;
    *courant = nouveau;
}
/******************************************************************************************/

void remplir_Matrice(matrice_creuse* matrice) {
    int ligne, colonne, valeur;
    char choix;

    printf("Remplissage de la matrice (ajoutez uniquement les éléments non nuls).\n");
    do {
        printf("Entrez la ligne, la colonne et la valeur de l'élément (ex: 0 1 5): ");
        scanf("%d %d %d", &ligne, &colonne, &valeur);

        if (valeur != 0) {
            ajoute_Element(matrice, ligne, colonne, valeur);
        } else {
            printf("Valeur nulle ignorée.\n");
        }

        printf("Voulez-vous ajouter un autre élément ? (o/n) : ");
        scanf(" %c", &choix);
    } while (choix == 'o' || choix == 'O');
}

/******************************************************************************************/
void affiche_La_Matrice(matrice_creuse* matrice) {
    for (int i = 0; i < matrice->Nlignes; i++) {
        printf("Ligne %d: ", i);
        element* courant = matrice->lignes[i];
        while (courant) {
            printf("[Colonne %d: %d] ", courant->colonne, courant->valeur);
            courant = courant->suivant;
        }
        printf("\n");
    }
}

/******************************************************************************************/
void afficher_Matrice_De_Base(matrice_creuse* matrice) {
    printf("\nMatrice classique :\n");
    for (int i = 0; i < matrice->Nlignes; i++) {
        element* courant = matrice->lignes[i];
        for (int j = 0; j < matrice->Ncolonnes; j++) {
            if (courant && courant->colonne == j) {
                printf("%d ", courant->valeur);
                courant = courant->suivant;
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

/******************************************************************************************/
void liberer_Matrice(matrice_creuse* matrice) {
    for (int i = 0; i < matrice->Nlignes; i++) {
        element* courant = matrice->lignes[i];
        while (courant) {
            element* temp = courant;
            courant = courant->suivant;
            free(temp);
        }
    }
    free(matrice->lignes);
    free(matrice);
}
/******************************************************************************************/
int rechercher_valeur(matrice_creuse* M,int a,int b)
{
    element *pnt = M->lignes[a];
    while(pnt && pnt->colonne!=b)
        pnt = pnt->suivant;
    if (pnt)
        return pnt->valeur;
    else
        return 0;
}
/******************************************************************************************/
void somme_dans_premiere_matrice(matrice_creuse* mat1, matrice_creuse* mat2)
{
    if (mat1->Nlignes != mat2->Nlignes || mat1->Ncolonnes != mat2->Ncolonnes) {
        printf("Erreur : Les matrices n'ont pas les mêmes dimensions.\n");
        return;
    }

    for (int i = 0; i < mat1->Nlignes; i++) {
        element* courant1 = mat1->lignes[i];
        element* courant2 = mat2->lignes[i];
        element** precedent = &(mat1->lignes[i]);

        while (courant1 || courant2) {
            if (courant2 && (!courant1 || courant2->colonne < courant1->colonne)) {
                element* nouveau = (element*)malloc(sizeof(element));
                nouveau->colonne = courant2->colonne;
                nouveau->valeur = courant2->valeur;
                nouveau->suivant = courant1;
                *precedent = nouveau;

                courant2 = courant2->suivant;
                precedent = &(*precedent)->suivant;
            } else if (courant1 && (!courant2 || courant1->colonne < courant2->colonne)) {
                precedent = &courant1->suivant;
                courant1 = courant1->suivant;
            } else {
                courant1->valeur += courant2->valeur;

                if (courant1->valeur == 0) {
                    *precedent = courant1->suivant;
                    free(courant1);
                    courant1 = *precedent;
                } else {
                    precedent = &courant1->suivant;
                    courant1 = courant1->suivant;
                }

                courant2 = courant2->suivant;
            }
        }
    }

    printf("Les matrices ont été sommées avec succès. Le résultat est stocké dans la première matrice.\n");
}
/******************************************************************************************/

int taille_totale_matrice_creuse(matrice_creuse* matrice) {
    int taille_totale = sizeof(matrice_creuse);
    taille_totale += matrice->Nlignes * sizeof(element*);

    for (int i = 0; i < matrice->Nlignes; i++) {
        element* courant = matrice->lignes[i];
        while (courant) {
            taille_totale += sizeof(element);
            courant = courant->suivant;
        }
    }

    return matrice->Ncolonnes*matrice->Nlignes*4-taille_totale;
}
/******************************************************************************************/
int main() {
    int Nlignes, Ncolonnes;
    matrice_creuse *tab;
    printf("Entrez le nombre de lignes de la matrice : ");
    scanf("%d", &Nlignes);
    printf("Entrez le nombre de colonnes de la matrice : ");
    scanf("%d", &Ncolonnes);

    matrice_creuse* matrice = Creation_Matrice(Nlignes, Ncolonnes);


    printf("Entrez le nombre de lignes de la matrice : ");
    scanf("%d", &Nlignes);
    printf("Entrez le nombre de colonnes de la matrice : ");
    scanf("%d", &Ncolonnes);
    matrice_creuse* matrice2 = Creation_Matrice(Nlignes, Ncolonnes);
    remplir_Matrice(matrice);
    remplir_Matrice(matrice2);
    afficher_Matrice_De_Base(matrice);
    afficher_Matrice_De_Base(matrice2);
    somme_dans_premiere_matrice(matrice,matrice2);
    afficher_Matrice_De_Base(matrice);
    affiche_La_Matrice(matrice);
    printf("%d",taille_totale_matrice_creuse(matrice));

    return 0;
}
