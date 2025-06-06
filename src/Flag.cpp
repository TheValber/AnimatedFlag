/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code "squelette" pour prototypage avec libgfl.7e              =!*/
/*!=================================================================!*/

/* le seul #include nécessaire a priori
 * contient les libs C standards et OpenGl */
#include <gfl.h>

#include <vector>
#include <iostream>
#include <algorithm>

#include "../include/PMat.hpp"
#include "../include/Link.hpp"
#include "../include/SpatialHashGrid.hpp"

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540

/* -----------------------------------------------------------------------
 * ici, en général pas mal de variables GLOBALES
 * - les variables de données globales (points, vecteurs....)
 * - les FLAGS de dialogues
 * - les paramètres de dialogue
 * - ......
 * Pas trop le choix, puisque TOUT passe par des fonctions <void f(void)>
 * ----------------------------------------------------------------------- */

double Fe = 150;

// Flag size
int rows = 60;
int cols = 80;

int width = 8;
int height = 6;

std::vector<PMat> pmats;
std::vector<Link> links;
std::vector<Link> gravityLinks;
std::vector<Link> windLinks;
PMat nonePMat(0, Point(0, 0, 0), Vect(0, 0, 0), 0);

Vect wind(10, 0, 0);

SpatialHashGrid hashGrid(0.1f);
float minDist = 0.1f;
float repulsionStrength = 1000.0f;

bool show_links = false;
bool show_wind = false;

/* la fonction d'initialisation : appelée 1 seule fois, au début     */
static void init(void)
{
  /*!  Ici, pas de dessin, pas de calcul dynamique, rien que des initialisations
   *   -> allocation(s) de mémoire, init. des paramètres globaux,
   *      construction de modèles....
   *      ouverture de fichiers, pré-chargement de données....
   *
   *   Tout ce qu'il y a ici pourrait être écrit directement dans le main()
   *   juste après l'appel à gfl_InitWindow()
  !*/
  gfl_SetCameraSpheric(-0.5*PI, 0.5*PI, 64.);

  double y_size = (double) height / 2;
  double y_step = (double) height / rows;
  double x_size = (double) width / 2;
  double x_step = (double) width / cols;

  for (double y = - y_size; y < y_size; y += y_step) {
    pmats.push_back(PMat(1, Point(- x_size, y, 0), Vect(0, 0, 0), 0, 0, (y + y_size) / height));
    for (double x = - x_size + x_step; x < x_size; x += x_step) {
      double mass = 1.0 - x / (2 * x_size);
      pmats.push_back(PMat(mass, Point(x, y, 0), Vect(0, 0, 0), 2, (x + x_size) / width, (y + y_size) / height));
    }
  }

  int size = pmats.size();

  // Valeurs horizontales
  double k = 0.2f * Fe * Fe;
  double z = 0.005f * Fe;

  // Liens horizontaux
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols - 1; x++) {
      double x_pos = -width / 2.0 + x * (double)width / cols;
      double k_adjusted = k * (1.5 - (x_pos + width / 2.0) / width);
      links.push_back(Link(&pmats[y * cols + x], &pmats[y * cols + x + 1], 0, k_adjusted, z, 1, 0, 0));
    }
  }

  // Liens verticaux
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows - 1; y++) {
      links.push_back(Link(&pmats[y * cols + x], &pmats[(y + 1) * cols + x], 0, k, z, 1, 0, 0));
    }
  }

  // Valeurs diagonales
  double k2 = k;
  double z2 = z;

  // Liens diagonaux
  for (int y = 0; y < rows - 1; y++) {
    for (int x = 0; x < cols - 1; x++) {
      int idx = y * cols + x;
      links.push_back(Link(&pmats[idx], &pmats[idx + cols + 1], 0, k2, z2, 0, 0, 1));
      links.push_back(Link(&pmats[idx + 1], &pmats[idx + cols], 0, k2, z2, 0, 0, 1));
    }
  }

  // Valeurs ponts
  double k3 = k;
  double z3 = z;

  // Ponts horizontaux
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols - 2; x++) {
      links.push_back(Link(&pmats[y * cols + x], &pmats[y * cols + x + 2], 0, k3, z3, 0, 1, 0));
    }
  }

  // Ponts verticaux
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows - 2; y++) {
      links.push_back(Link(&pmats[y * cols + x], &pmats[(y + 2) * cols + x], 0, k3, z3, 0, 1, 0));
    }
  }

  // Gravité
  for (int i = 0; i < size; i++) {
    gravityLinks.push_back(Link(&pmats[i], &nonePMat, 1, 0, 0));
  }

  // Vent
  for (int i = 0; i < size; i++) {
    windLinks.push_back(Link(&pmats[i], &nonePMat, 2, 0, 0));
  }
}

static void reset() {
  // Efface tous les liens et les points et appelle init() pour les recréer
  links.clear();
  pmats.clear();
  gravityLinks.clear();
  windLinks.clear();
  // Réinitialise la grille de hachage
  hashGrid.clear();
  // Réinitialise le vent
  wind.setX(10);
  wind.setY(0);
  wind.setZ(0);
  // Réinitialise la fréquence d'échantillonnage
  Fe = 150;
  // Appelle init() pour recréer les points et les liens
  init();
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void ctrl(void)
{
  /*! Interface de dialogue (partie statique) : création des boutons, scrollbars....
   *  Tout ce qu'il y a ici pourrait être directement écrit dans la fonction init(),
   *  mais c'est plus 'propre' et plus pratique de séparer.
  !*/
  // gfl_CreateScrollv_d("y", pmats[1].getPosPtr()->getYPtr(), -4, 4, "Position y du premier point");
  // gfl_CreateScrollv_d("y", pmats[pmats.size() - 2].getPosPtr()->getYPtr(), -4, 4, "Position y du dernier point");
  gfl_CreateScrollh_d("Fe", &Fe, 140, 1000, "Fréquence d'échantillonnage");
  gfl_CreateSwitch("Show links", &show_links, "Afficher les liens");
  gfl_CreateSwitch("Show wind", &show_wind, "Afficher le vent");
  gfl_CreatePopUp("Reset", reset, "Réinitialiser le système");
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void evts(void)
{
  /*! Interface de dialogue (partie dynamique) : les touches clavier, la souris ....
   *  Tout ce qu'il y a ici pourrait être directement écrit dans la fonction draw(),
   *  mais c'est plus 'propre' et plus pratique de séparer.
  !*/
}

/* la fonction de dessin : appelée en boucle (indispensable) */
static void draw(void)
{
  /*! C'est la fonction de dessin principale : elle ne réalise que de l'affichage
   *  sa mise à jour est automatique :
   *  - si un paramètre de contrôle est modifié
   *  - si la fonction <anim()> (calcul) est activée
   *  ATTENTION : surtout pas d'alloc. mémoire ici !!!
  !*/
  // gfl_Axes(); /* spécifique 2D : affiche le repère principal (O,x,y) */
  // gfl_Repere(); // En 3d
  glDisable(GL_LIGHTING);
  glPointSize(5.0);
  if (show_links) {
    glPointSize(2.0);
    glLineWidth(1.0);
    for (auto& link : links) {
      link.draw();
    }
  }
  for (auto& pmat : pmats) {
    pmat.draw();
  }

  if (show_wind) {
    // Affichage de la direction et de la force du vent
    glLineWidth(10.0);
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(wind.getX(), wind.getY(), wind.getZ());
    glEnd();
}

  glEnable(GL_LIGHTING);
}

static void computeWind() {
  float windXChange = (rand() % 2001 - 1000) / 20000.0f;
  float windYChange = (rand() % 2001 - 1000) / 20000.0f;
  float windZChange = (rand() % 2001 - 1000) / 20000.0f;

  wind.setX(wind.getX() + windXChange);
  wind.setY(wind.getY() + windYChange);
  wind.setZ(wind.getZ() + windZChange);

  // std::cout << wind << std::endl;
}

static void computeCollisions() {
  hashGrid.clear();
  for (auto& p : pmats) hashGrid.insert(&p);

  for (auto& p : pmats) {
    auto voisins = hashGrid.queryNeighbors(&p, minDist);
    for (PMat* n : voisins) {
      float dist = (p.getPos() - n->getPos()).norm();
      float overlap = minDist - dist;
      if (overlap > 0) {
        Vect dir = (p.getPos() - n->getPos()).normalized();
        Vect force = dir * (repulsionStrength * overlap);
        p.addForce(force);
        n->addForce(-force);
      }
    }
  }
}

/* la fonction d'animation : appelée en boucle draw/anim/draw/anim... (facultatif) */
static void anim(void)
{
  /*! C'est la fonction de 'calcul' qui va modifier les 'objets' affichés
   *  par la fonction de dessin (déplacement des objets, calculs divers...)
   *  Si elle n'est pas définie, c'est qu'il n'y a pas d'animation.
   *  ATTENTION : surtout pas d'alloc. mémoire ici !!!
  !*/
  for (auto& pmat : pmats) {
    pmat.update(1.f / Fe);
  }

  for (auto& link : links) {
    link.update();
  }

  for (auto& link : gravityLinks) {
    link.update();
  }

  computeWind();

  for (auto& link : windLinks) {
    link.setWind(wind);
    link.update();
  }

  computeCollisions();
}

/* la fonction de sortie  (facultatif) */
static void quit(void)
{
  /*! Ici, les opérations à réaliser en sortie du programme
   *  - libération de la mémoire éventuellement alloueé dans <init()>
   *  - fermeture de fichiers ....
   *  - bilan et messages...
   *  Au final cette fonction est exécutée par un appel à <atexit()>
  !*/
}

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels)  */
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     par défaut (pas d'initialisation) la zone 'réelle'  *
   *     est [(0.,0.),(WWIDTH,WHEIGHT)]                      */
  // gfl_SetCenteredDrawZone(0.,0.,10.,0.);
  /* --- autre façon de faire ---
   *     ATTENTION : veiller à respecter les proportions
   *                 (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
   */
  //double wxmin = -10., wymin = -10.,
         //wxmax = +10., wymax = +10.;
  //gfl_SetWindowCoord(wxmin,wymin,wxmax,wymax);



  /* 3°) association des fonctions */
  gfl_SetInitFunction(init); /* fonction d'initialisation */
  gfl_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  gfl_SetEvtsFunction(evts); /* fonction d'événements     */
  gfl_SetDrawFunction(draw); /* fonction de dessin        */
  gfl_SetAnimFunction(anim); /* fonction d'animation      */
  gfl_SetExitFunction(quit); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return gfl_MainStart();
  /* RIEN APRES CA */
}
