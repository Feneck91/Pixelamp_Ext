# Pixelamp_Ext
Code Arduino pour la lampe Pixel créée par Heliox et par un inconnu dont je laisse le commentaire plus bas.

## Version (Utiliser les Tags)
**1.0** : Première version utilisant le nouveau moteur et les modes (Pacman / Fx / Lumières / etc).
**2.0** : (en préparation) : possibilité de plier le rubant en ignorant une led en haut et en bas (non éclairée et pas dans une case de la lampe).
Ceci permettra d'éviter de couper puis soufer le rubant (très long à faire), très complexe a mettre dans la lampe et provoque des cassures, des faux contacts.
En pliant rubant en haut et en bas, on évite ces soudures et les faux contacts (en tout cas c'est mon espoir) mais il faut laisser une led non utilisée à chaque fois pour que la led suivante soit correctement au milieu de la case.
Bref, après un échec pour finir la lampe, je commande un nouveau ruban et je modifie le code pour faire un nouvel essai.

## Généralités
Le code original est disponible ici : https://drive.google.com/file/d/1V1oVRu5Qm_7Lyr85kFy6UF3OYe4PQe78/view
Le code fourni par Heliox permet de générer des animations et de jolis effets.
Toutefois, j'ai voulu aller plus loin et proposer quelques trucs en plus. Le but étant d'avoir à minima ce qui était proposé par le code original.

Mais si on ajoute beaucoup de choses différentes, le potentiomètre de choix des animations devient limité. J'ai donc choisi de définir des modes.
Dans chaque mode on a tous les effets du mode en question (Pacman / Fire / etc...).

J'ai ajouté du texte défilant pour chaque mode.
Un setup permet de définir quelques réglages :
- Le centre de la lampe (au dessus du bouton de réglage de lumière) afin que les animations de texte et réglages soient centrées et correctement visible.
- Inversion su potentiomètre de choix de la luminosité.
- Inversion su potentiomètre de choix de l'animation.
Ces choix sont conservés dans l'EEPROM de l'Arduino.

Afin de développer sans avoir l'Arduino, j'ai écris un simulateur de la lampe. C'est pas super propre, et tous les fonctionalités de FastLed ne fonctioneront
certainement pas mais c'est bien pratique de tester sans l'Arduino.

## Changement de mode
Comme il n'est pas question d'ajouter des boutons, j'ai utilisé ceux existants, à savoir le bouton de choix de la luminosité.
droite / gauche / droite pour aller au mode suivant.
gauche / droite / gauche pour aller au mode précédent.
gauche / droite / gauche / droite / gauche (ou l'inverse, pas de suivant / précédent pour le Setup) pour aller au mode Setup, idem pour en ressortir.

## Mode setup
La luminosité est bloquée, le bouton de lumière permet de sélectionner la valeur, le potentiomère des animations permet de sélectionner quel réglage est à initialiser.
Il suffit de bouger la luminosité et le réglage s'active.
Après 4 secondes il est bloqué et clignote pendant 4 secondes avant son enregistrement. Si le bouton de luminosité est utilisé pendant laps de temps (pendant que le réglage clignote) le
réglage est abandoné et retourne à sa position initiale sinon il est savegardé.
Après encore 4 secondes, il retourne a l'affichage des valeurs enregistrées.

# Code
Les commentaires du code sont écris en anglais. J'ai utilisé des classes que j'avais développé auparavant.
- surveillance des fuites mémoires en mode simulation (il reste 3 memory leaks au moment où j'écris ces lignes, je ne trouve pas pour le moment).
- Tous les sprites sont écris en mémoire programme, ça a l'avantage de ne pas prendre sur la mémoire vive qui est limitée. De plus les couleurs sont stockées sur un seul octet pour une
  optimisation de la mémoire et les couleurs sont mises lors du chargement du sprite (dans le code).
- Des pointeurs partagés sont utilisés afin d'éviter les fuites mémoire.

## Compilation
Visual Studio est utilisé ainsi que le plugin vMicro qui me permet d'accéder à l'Arduino Méga.

## Commentaires
Les commentaires suivants ne sont plus complètement d'actualité. Par respect du travail effectué qui m'a servi de base et qui, pour pas mal de parties, est encore présent dans mon code,
je laisse les commentaires du développeur qui a créé ces superbes animations.

# Pixelamp
## Généralités
Code réalisé pour Heliox dans le cadre de la Lampe Pixel, nom de code "pixelamp" durant le dev.

Pour étendre un peu le code, il faut d'abord le comprendre. Voici quelques pistes.

Globalement, la fonction `loop` appelle en boucle une des fonctions qui font des dessins. A la charge de ces fonctions de faire évoluer l'affichage (décaler un sprite, modifier les couleurs) à chaque nouvel appel.

Par exemple, la plus simple est la fonction `xyTester`. Utilisée au départ pour vérifier le positionnement des pixels, elle a finalement été conservée...parce que c'était joli à regarder. Bref, voici son code:

```C
void xyTester() {
  static uint8_t x=0;
  static uint8_t y=0;
  static uint8_t hue = 0;
  
  leds[XY(x, y, false, false)] = CHSV(hue, 255, 255);
  FastLED.show();
  FastLED.delay(20);

  x++;
  if (x >= kMatrixWidth) {
    y++;
    x = 0;
    hue++;
  }
  if (y >= kMatrixHeight) {
    y = x = 0;
    wipeMatrices();
  }
}
```

Les paramètres internes de la fonction sont déclarés en `static`. Vu que la fonction est appelée en boucle, il faut bien conserver une trace des paramètres de l'appel précédent pour les faire évoluer!
Deuxième chose intéressante, l'appel à la fonction `XY`. C'est la fonction de base, vraiment la plus importante. Pourquoi? Parce que FastLED, pas défaut, travaille avec des rubans de LEDs, pas des matrices. Du coup, on utilise un tableau de `CRGB` qu'on déclare au tout début, et on utilise `XY` pour faire le lien entre les deux. `XY` reçoit en paramètres les coordonnées d'un pixel et calcule la position de ce pixel dans le tableau, en tenant compte:
- du fait que les pixels sont arrangés en zigzg ou pas (sur la plupart des matrices, par exemple, le premier pixel est en bas à gauche, le suivant au dessus, ainsi de suite. Arrivé en haut de la colonne, on repart d'en bas. Sur la pixelamp d'Axelle, l'arrangment est différent: arrivé en haut de la colonne on passe à celle d'à coté et on descend.)
- du fait que le premier pixel peut être en haut à gauche ou en bas à gauche.
Pour résumer, lisez bien le code de XY et comprenez-le, tout vient de là.

Pour en revenir à la fonction `xyTester`, à chaque appel on colore un pixel, puis on passe au suivant (`x++`). Si on est en haut d'une colonne, on passe à la suivante. Si on est au bout de la matrice, on efface tout et on recommence à zéro.

Si vous avez bien compris comment fonctionne cet effet, vous avez la base.

## Ajouter un effet.
Pour rajouter un effet, il vous faut suivre les étapes suivantes:
- créer une fonction qui dessine l'effet. Elle doit absolument avoir pour prototype `void leNomDeVotreFonction()`.
- bien évidemment, coller son prototype dans pixelamp.h
- ajouter son nom au tableau `void (*effects[])() = {`que vous trouverez en ligne ~52 dans `pixelamp.cpp`. C'est un tableau de pointeurs de fonctions qui liste tous les effets, dans l'ordre.

## Gestion des sprites
Un sprite est une image qu'on affiche. Pour info, les images de la pixelamp ont été dessinée par Axelle et moi même avec nos petits doigts grâce à l'éditeur online "piskel". Je vous le recommande chaudement, difficile de trouver mieux.

Chaque pixel répond au prototype suivant

```C++
template <uint32_t N> struct Sprite {
  uint8_t w, h;
  uint8_t x, y;
  int8_t dx, dy;
  uint32_t data[N];
};
```

- `w` et `h` sont la largeur et la hauteur du sprite
- `x` et `y` sont sa position initiale
- `dx` et `dy` sont les décalage à appliquer au sprite à chaque tour. Par exemple, si `dx` vaut 1, le sprite avancera à chaque tour d'un pixel vers la droite.
- le reste, ce sont les pixels du sprite.

Ah et aussi: le `N` dans le prototype est la taille du sprite lui même, c'est à dire `x * y`.

Pour comprendre comment déplacer un sprite, allez voir la fonction `animatePacChase`, c'est celle qui affiche le pacman qui course le fantôme. Pour résumer:
- on affiche les sprites
- on décale les sprites affichés avec `moveSprite`
- on recommence.

## Ressources supplémentaires
Dans le fichier `pixelamp.h` se trouvent des variantes de couleurs pour les fantômes ainsi que des sprites qu'on a décidé de ne pas utiliser.

## Paramètres à modifier dans `pixelamp.cpp`
- suivant la taille de votre  matrice, vous pouvez ajuster `kMatrixWidth` et `kMatrixHeight` 
- la valeur de `Pal`, qui peut être l'une des valeurs que vous trouverez [ici](http://fastled.io/docs/3.1/colorpalettes_8h_source.html)
- la valeurs de `FIRE_HEIGHT` dans `pixelamp.h`

## Le mot de la fin.
Des questions concernant le code ? Des idées ? De nouveaux effets ? Contactez Axelle, elle sait où me trouver. J'espère sincèrement que des gens réussiront à faire des trucs cools avec ce projet. Par contre, merci de citer la provenance du code si vous le réutilisez. Quand Axelle dit qu'on a bossé dessus des semaines durant, c'est vrai, et tout ça sur notre temps 'libre'. Alors merci de citer les auteurs !
