# Pixelamp_Ext
Code Arduino pour la lampe Pixel créée par Heliox et par un inconnu dont je laisse le commentaire plus bas.

## Version (Utiliser les Tags)
- **1.0** : Première version utilisant le nouveau moteur et les modes (Pacman / Fx / Lumières / etc).
- **2.0** : (en préparation) : possibilité de plier le rubant en ignorant une led en haut et en bas (non éclairée et pas dans une case de la lampe).
Ceci permettra d'éviter de couper puis souder le rubant (très long à faire), très complexe a mettre dans la lampe et provoque des cassures, des faux contacts.
En pliant rubant en haut et en bas, on évite ces soudures et les faux contacts (en tout cas c'est mon espoir) mais il faut laisser une led non utilisée à chaque fois pour que la led suivante soit correctement au milieu de la case.
Bref, après un échec pour finir la lampe, je commande un nouveau ruban et je modifie le code pour faire un nouvel essai.

## Généralités
Le code original est disponible ici : https://drive.google.com/file/d/1V1oVRu5Qm_7Lyr85kFy6UF3OYe4PQe78/view
Le code fourni par Heliox permet de générer des animations et de jolis effets.
Toutefois, j'ai voulu aller plus loin et proposer quelques trucs en plus. Le but étant d'avoir à minima ce qui était proposé par le code original.

Mais si on ajoute beaucoup de choses différentes, le potentiomètre de choix des animations devient limité. J'ai donc choisi de définir des modes.
Dans chaque mode on a tous les effets du mode en question (Pacman / Fire / etc...).

J'ai ajouté du texte défilant pour chaque mode.

Afin de développer sans avoir l'Arduino, j'ai écris un simulateur de la lampe. C'est pas super propre, et tous les fonctionalités de FastLed ne fonctioneront
certainement pas mais c'est bien pratique de tester sans l'Arduino.

## Changement de mode d'affichage (Pacman / Feux / Fx / Lumières / Aléatoire)
Comme il n'est pas question d'ajouter des boutons, j'ai utilisé ceux existants, à savoir le bouton de choix de la luminosité :<br/>
droite / gauche / droite pour aller au mode suivant.
gauche / droite / gauche pour aller au mode précédent.

## Mode setup
Un setup permet de définir quelques réglages, il est accessible par le changement rapide du bouton de luminosité : gauche / droite / gauche / droite / gauche (ou l'inverse) pour aller au mode Setup, idem pour en ressortir.
Dans ce mode la luminosité est bloquée, le bouton de lumière permet de sélectionner la valeur, le potentiomère des animations permet de sélectionner quel réglage est à modifier.
Il suffit de bouger la luminosité et le réglage s'active.
Après 4 secondes il est bloqué et clignote pendant 4 secondes avant son enregistrement. Si le bouton de luminosité est utilisé pendant ce laps de temps (pendant que le réglage clignote) le
réglage est abandoné et retourne à sa position initiale sinon il est savegardé.
Après encore 4 secondes, il retourne a l'affichage des valeurs enregistrées.

<ins><b>Les réglages sont les suivants :</b></ins>
- Définir le centre de la lampe (à régler au dessus du bouton de réglage de lumière) afin que les animations de texte et réglages soient centrées et correctement visibles.
  Comme la version d'Heliox les animations n'avaient pas de centre il est possible de coller le ruban de led n'importe où (pas besoin de centrer la rangée 8 au centre du bouton de luminosité), or dans cette version la lampe affiche des messages et si ces messages ne sont pas centrés ça ne sera pas lisible (idem pour le Pacman lorsque le fantôme rentre dans sa maison), il est donc important de faire ce réglage.<br/>
  - <ins>Image de sélection du centre de la lampe, à positionner de préférence en face du bouton de luminosité :</ins><br/>
    ![Image du centre de la lampe](images/center_lamp.jpg)<br/>
- Inversion du potentiomètre de choix de la luminosité (si mal câblé, permet de corriger logiciellement).
  - <ins>Mode normal :</ins><br/>
    ![Potentiomètre de luminosité normal](images/light_normal.jpg)<br/>
  - <ins>Mode inversé :</ins><br/>
    ![Potentiomètre de luminosité inversé](images/light_inverted.jpg)<br/>
- Inversion du potentiomètre de choix de l'animation (si mal câblé, permet de corriger logiciellement).
  - <ins>Mode normal :</ins><br/>
    ![Potentiomètre d'animation normal](images/animation_normal.jpg)<br/>
  - <ins>Mode inversé :</ins><br/>
    ![Potentiomètre d'animation inversé](images/animation_inverted.jpg)<br/>

Ces choix sont conservés dans l'EEPROM de l'Arduino.

## Mode aléatoire
La liste des animations aléatoires est initialisée dans le code <i>pixelampext.ino</i>.
Dans ce mode, le potentiomètre de modification des animations n'est plus utilisé de la même manière puisque c'est le programme qui choisi aléatoirement les animations. Il est donc utilisé afin de sélectionner la durée de chaque animation. 

# Code
Les commentaires du code sont écris en anglais. J'ai utilisé des classes que j'avais développé auparavant.
- surveillance des fuites mémoires en mode simulation (il reste 3 memory leaks au moment où j'écris ces lignes, je ne trouve pas pour le moment).
- Tous les sprites sont écris en mémoire programme, ceci a l'avantage de ne pas prendre sur la mémoire vive qui est limitée. De plus les couleurs sont stockées sur un seul octet pour une
  optimisation de la mémoire et les couleurs sont mises lors du chargement du sprite (dans le code).
- Des pointeurs partagés sont utilisés afin d'éviter les fuites mémoire.

## Compilation
Visual Studio est utilisé ainsi que le plugin vMicro (version démo qui ne dure qu'un temps) qui me permet d'accéder à l'Arduino Méga.

## Constantes à modifier dans le code avant téléversement dans l'Arduino
Un certain nombre de constantes et de définitions permettent de modifier le comportement du programme.
- Mode aléatoire (<i>animationrandom.cpp</i>) :<br/>
Le potentiomètre de choix des animations est utilisé afin de sélectionner la durée de chaque animation. Cette durée est configurable entre RANDOM_MIN_TIME et RANDOM_MAX_TIME.
Ces valeurs sont actuellement renseignées avec les valeurs suivantes :<br/>
  - <b>RANDOM_MIN_TIME</b> = 10  (10 secondes)
  - <b>RANDOM_MAX_TIME</b> = 600 (10 minutes)
- Type de câblage (<i>enginedefines.h</i>) :<br/>
La configuration par défaut définie sur le câblage normal du projet d'Heliox. Or ce câblage est très complexe à réaliser : environ 90 soudures sur le ruban de leds, des soudures qui risquent de se casser et de faire des faux contacts, du collage à la colle chaude des rubans qui n'est pas du plus facile à faire (on se brule, on casse les soudures, ça se décolle, bref une galère).
Les constantes a définir sont les suivantes :
<b>NO_SOLDERING_LED</b> : Activer pour passer en mode sans soudure.
<b>NO_SOLDERING_FIRST_LED_IS_NOT_USED</b> : Ignorer la première LED, c'est la deuxième qui est dans la première case de la lampe. N'est actif que si <b>NO_SOLDERING_LED</b> est activé.
<b>NO_SOLDERING_LAST_LED_DOES_NOT_EXIST</b> : Ignorer la dernière LED, après la dernière LED, une led supplémentaire est laissée. N'est actif que si <b>NO_SOLDERING_LED</b> est activé.
Ce cablâge est expliqué plus en détails dans la documentation suivante :


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
