/* ========== CAHIER DES CHARGES ============

    La simulation est une simulation de fourmiliere qui �volue dans son environnement.
    Elle comportera :
        - Des fourmies
        - Une fourmiliere de fourmies avec un stock de nourriture
        - Des sources de nourritures fini

    I) Les fourmies
        Chaque fourmies

*/


//KeyListenner, voir conio21 de Philippe Latu

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <windows.h>

#define MAX_FOURMIE 150
#define MAX_SOURCE 10

#define ET_AVANCER_ALEA 1
#define ET_SUIVRE_TRACE 2
#define ET_RENTRER_HOME 3

const int Y = 50;
const int X = 100;


using namespace std;

typedef struct{
    int x;
    int y;
}t_coord;

typedef struct{
    t_coord coord;
    int nourriture;
    int vie;
    bool drop;
    int direction;
    int idChemin;
    int etat;
}t_fourmie;

typedef t_coord t_liste_coord[X];
typedef struct{
    t_liste_coord coord_pheros;
    int nbCoord;
    int id;
}chemin_phero;

typedef t_fourmie t_fourmies[MAX_FOURMIE];
typedef struct{
    t_coord coord;
    int nourriture;
    t_fourmies fourmies;
    int nbFourmies;
}fourmiliere;

typedef struct{
    t_coord coord;
    int reste;
}source_nourriture;

typedef chemin_phero t_chemins[MAX_SOURCE];
typedef source_nourriture t_sources[MAX_SOURCE];
typedef char t_case[Y][X+1];
typedef struct{
    t_case maMap;
    fourmiliere maison;
    t_sources sources;
    int nbSources;
    t_chemins chemins;
    int nbChemins;
}t_simulation;

string to_string(int a){
    std::ostringstream oss;
    oss << a;
    return oss.str();
}

void spawnFourmie(fourmiliere&fourmil){
    if(fourmil.nourriture > 0 && fourmil.nbFourmies < MAX_FOURMIE){
        t_fourmie fourmie = {fourmil.coord, 0, rand()%10+95, false, rand()%4, false, -1};
        fourmil.fourmies[fourmil.nbFourmies++] = fourmie;
    }
}

void supprimerFourmie(fourmiliere&fourmil, int index){
    fourmil.fourmies[index] = fourmil.fourmies[fourmil.nbFourmies-1];
    fourmil.nbFourmies--;
}

void fatiguerFourmies(fourmiliere&fourmil){
    for(int i = 0; i < fourmil.nbFourmies; i++){
        fourmil.fourmies[i].vie -= rand()%3;
        if(fourmil.fourmies[i].vie < 1)
            supprimerFourmie(fourmil, i);
    }
}

bool coordEquals(t_coord coord1, t_coord coord2){
    return coord1.x == coord2.x && coord1.y == coord2.y;
}



/** \brief retourne l'id de la trace de pheromone aux coordonn�es coord. Retourne -1 si il n'y a pas de trace a ces coordonn�es
 *
 * \param simu t_simulation
 * \param coord t_coord
 * \return int
 *
 */
bool isOnPhero(t_simulation simu, t_coord coord, chemin_phero & chemin){
    for(int i = 0; i < simu.nbChemins; i++)
        for(int j = 0; j < simu.chemins[i].nbCoord; j++)
            if(coordEquals(coord, simu.chemins[i].coord_pheros[j])){
                chemin = simu.chemins[i];
                return true;
            }
    return false;
}

bool isOnHome(t_simulation simu, t_coord coord){
    return coordEquals(coord, simu.maison.coord);
}

bool isOnNourriture(t_simulation simu, t_coord coord){
    for(int i = 0; i < simu.nbSources; i++)
        if(coordEquals(simu.sources[i].coord, coord))
            return true;
    return
}

void evolutionEtat(t_simulation&simu, t_fourmie&fourmie){
    switch(fourmie.etat){

    case ET_AVANCER_ALEA :
        chemin_phero phero;
        if(isOnPhero(simu, fourmie.coord, phero)){

            fourmie.etat = ET_SUIVRE_TRACE;
        }else if(isOnHome(simu, fourmie.coord))


        break;
    }
}

void majMap(t_simulation&simu){
        for(int i = 0; i < Y; i++){
            for(int j = 0; j < X; j++)
                simu.maMap[i][j] = ' ';
            simu.maMap[i][X] = '\0';
    }

    for(int i = 0; i < simu.maison.nbFourmies; i ++){
        t_fourmie fourmi = simu.maison.fourmies[i];
        char fourm;
        if(fourmi.direction < 2)
            fourm = '|';
        else
            fourm = '_';
        simu.maMap[fourmi.coord.y][fourmi.coord.x] = fourm;
    }


    simu.maMap[simu.maison.coord.y-1][simu.maison.coord.x-2] = 30;
    simu.maMap[simu.maison.coord.y][simu.maison.coord.x-2] = 219;
    simu.maMap[simu.maison.coord.y][simu.maison.coord.x-1] = 219;
    simu.maMap[simu.maison.coord.y-1][simu.maison.coord.x] = 30;
    simu.maMap[simu.maison.coord.y][simu.maison.coord.x] = 219;
    simu.maMap[simu.maison.coord.y-1][simu.maison.coord.x] = 30;
    simu.maMap[simu.maison.coord.y][simu.maison.coord.x] = 219;
}

void deplacerAleaFourmie(t_fourmie&fourmie){
    if(rand()%5 == 0)
        switch(rand()%4){
        case 0 :    //la fourmie monte
                fourmie.direction = 0;
            break;

        case 1 :    //la fourmie descend
                fourmie.direction = 1;
            break;

        case 2 :    //la fourmie va a droite
                fourmie.direction = 2;
            break;

        case 3 :    //la fourmie va a gauche
                fourmie.direction = 3;
            break;
        }

    switch(fourmie.direction){
        case 0 :    //la fourmie monte
            if(fourmie.coord.y > 0)
                fourmie.coord.y--;
            else
                fourmie.direction = 1;
            break;

        case 1 :    //la fourmie descend
            if(fourmie.coord.y < Y-1)
                fourmie.coord.y++;
            else
                fourmie.direction = 0;
            break;

        case 2 :    //la fourmie va a droite
            if(fourmie.coord.x < X-1)
                fourmie.coord.x++;
            else
                fourmie.direction = 3;
            break;

        case 3 :    //la fourmie va a gauche
            if(fourmie.coord.x > 0)
                fourmie.coord.x--;
            else
                fourmie.direction = 2;
            break;
    }
}

int main()
{
    srand(time(0));
    string cmd = "mode " + to_string(X+1) + ", " + to_string(Y+1);
    system(cmd.c_str());

    fourmiliere fourmil;
    fourmil.nbFourmies = 2;
    fourmil.nourriture = 1;
    fourmil.coord = {50, 30};

    for(int i = 0; i < 2; i++)
        fourmil.fourmies[i] = {{2*i, 25}, 0, 200, false, i%4, -1, 0};

    t_simulation simu;
    simu.maison = fourmil;

    int nbTour = 0;
    int nbTotalFourmis = 0;
    while(1){
        nbTour++;
        for(int i = 0; i < simu.maison.nbFourmies; i++)
            deplacerAleaFourmie(simu.maison.fourmies[i]);

        if(rand()%4 == 1)
            spawnFourmie(simu.maison);

        fatiguerFourmies(simu.maison);

        majMap(simu);
        nbTotalFourmis += simu.maison.nbFourmies;
        //if(nbTour % 100 == 0){
            system("cls");
            for(int i = 0; i < Y; i++)
                cout << simu.maMap[i] << "\n";
            //cout << "nbFourmies : " << simu.maison.nbFourmies << "      nbTour : " << nbTour << "      nbMoy : " << nbTotalFourmis/nbTour;
            Sleep(100);
        //}
    }
    return 0;
}
