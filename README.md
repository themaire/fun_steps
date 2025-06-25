# 📓 Projet : Chaussures Clignotantes avec BMI160 et LEDs RGB

## Présentation

Ce projet consiste à créer un module électronique à fixer sur une chaussure pour enfant (ou de grand enfant). Il utilise un accéléromètre BMI160 et un anneau de LEDs RGB adressables (Neopixel/WS2812).  
L’objectif est de détecter les mouvements (impact du pas, saut, etc.) et de déclencher des animations lumineuses dynamiques en fonction de l’intensité du choc.

---

## Ce que j’ai accompli

### Fonctionnalités principales
1. **Lecture des données du BMI160** :  
   - Communication via I2C.
   - Calcul de la magnitude des mouvements.
   - Détection des seuils pour différencier les chocs faibles, moyens et forts.

2. **Pilotage des LEDs WS2812** :  
   - Utilisation du PIO du Raspberry Pi Pico pour un contrôle précis.
   - Déclenchement d’animations dynamiques en fonction des chocs détectés.

3. **Structure modulaire du code** :  
   - Séparation des fonctionnalités dans des fichiers dédiés (`bmi160.c`, `led_manager.c`, etc.).
   - Utilisation de `CMake` pour une gestion efficace du projet.

---

## Informations techniques

### Matériel utilisé
1. **Microcontrôleur** : Raspberry Pi Pico  
   - Basé sur le RP2040 (dual-core ARM Cortex-M0+).
   - Utilisation du PIO pour piloter les LEDs WS2812.
2. **Capteur** : Bosch BMI160  
   - IMU (accéléromètre + gyroscope) avec communication I2C.
   - Plage d'accélération configurée à ±8g.
3. **LEDs** : Stick de 8 LEDs WS2812  
   - LEDs RGB adressables individuellement.
   - Contrôlées via PIO du Pico.

### Logiciel utilisé
1. **Langage** : C  
   - Code structuré en modules (`bmi160.c`, `led_manager.c`, etc.).
   - Utilisation de `CMake` pour la gestion du projet.
2. **SDK** : Raspberry Pi Pico SDK  
   - Fournit les bibliothèques pour le PIO, I2C, et autres fonctionnalités.
3. **Outils de développement** :  
   - **VS Code** avec l'extension Raspberry Pi Pico.
   - **CMake** pour la configuration.
   - **Ninja** ou `make` pour la compilation.

### Concepts clés du BMI160

- **Communication I2C/SPI** : I2C recommandé (adresse 0x68 ou 0x69 selon AD0).
- **Initialisation** :  
  - Sortir du mode veille/reset.
  - Configurer plages (±2g, ±4g, …) et fréquences (10Hz à 1600Hz selon besoin).
  - Activer accéléromètre/gyroscope.
- **Lecture** :  
  - Accélérations sur 3 axes, chaque axe sur 2 octets (16 bits, signed, little-endian).
- **Interruptions** :  
  - Possible d’utiliser INT1/INT2 pour détecter mouvement/tap/etc (optionnel pour ce projet).
- **WHO_AM_I** :  
  - Vérifier la présence du capteur (valeur attendue : 0xD1).

---

### Logique de fonctionnement

1. **Lecture de l’accélération**
2. **Calcul de la magnitude** :  
   `sqrt(ax**2 + ay**2 + az**2)`
3. **Comparaison avec les seuils**
4. **Déclenchement de l’animation LED adaptée**

### Exemple de mapping

| Magnitude du choc | Animation/couleur |
|-------------------|-------------------|
| Faible            | Bleu doux, 3 LEDs |
| Moyen             | Vert vif, 8 LEDs  |
| Fort              | Rouge/orange, tout l’anneau en flash |

---

### Idées pratiques

- **Filtrage/détection** :  
  - Ajoute un antirebond logiciel (ex : pas plus d’une animation toutes les 0.2s).
- **Test sur table** :  
  - Régle les seuils avant montage sur la chaussure.
- **Boîtier 3D** :  
  - Prévoir l’accès au port USB et la solidité.

---

## Besoins futurs

- Ajouter des animations LED supplémentaires.
- Optimiser la gestion des seuils pour des mouvements plus complexes.
- Tester en conditions réelles (montage sur une chaussure).
- Concevoir un boîtier 3D pour protéger le module.
