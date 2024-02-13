# Pixelamp (Commentaires du développeur à l'origine du projet)
Attention, ceci est laissé par respect de l'auteur mais le code a vraiment beaucoup changé, il est fort possible que vous ne retrouviez que des brides de code dans ce qui est décrit ci-dessous.

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
