# 📓 Projet : Chaussures Clignotantes avec BMI160 et LEDs RGB

## Objectif

Créer un module à fixer sur une chaussure pour enfant, intégrant un accéléromètre BMI160 et un anneau de LEDs RGB adressables (Neopixel/WS2812).  
L’intensité et le type d’animation lumineuse dépendent du « choc » détecté (impact du pas, saut, etc).

---

## Concepts clés du BMI160

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

## Pourquoi régler les seuils et la fréquence d’échantillonnage ?

- **Fréquence d’échantillonnage** :  
  - Haute : mouvements rapides, meilleure réactivité, plus de conso/données.
  - Basse : mouvements lents, moins de conso, moins de données à gérer.
- **Seuils** :  
  - Filtrer le bruit/vibrations.
  - Déclencher les animations seulement pour des chocs pertinents (ex : > 1.5g).

---

## Logique de fonctionnement

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

## Extrait de pseudo-code

```python
ax, ay, az = lire_acceleration_bmi160()
magnitude = sqrt(ax**2 + ay**2 + az**2)

if magnitude > seuil_gros_choc:
    animation_flash()
elif magnitude > seuil_moyen_choc:
    animation_vif()
elif magnitude > seuil_petit_choc:
    animation_douce()
```

---

## Librairies utiles

- **Driver BMI160** : Ports MicroPython disponibles sur GitHub (ex : [robert-hh/BMI160-Micropython](https://github.com/robert-hh/BMI160-Micropython)), à adapter pour CircuitPython si besoin.
- **LEDs RGB** :  
  - CircuitPython : `adafruit_circuitpython_neopixel`
  - MicroPython : module `neopixel` intégré

---

## Conseils pratiques

- **Filtrage/détection** :  
  - Ajoute un antirebond logiciel (ex : pas plus d’une animation toutes les 0.2s).
- **Test sur table** :  
  - Régle les seuils avant montage sur la chaussure.
- **Boîtier 3D** :  
  - Prévoir l’accès au port USB et la solidité.

---

## Sauvegarde et partage de la conversation

- **Ce résumé** : à copier/coller dans un dépôt, un gist, ou un document de suivi.
- **Conversation complète** : sauvegarder régulièrement, car non synchronisée entre mobile et desktop GitHub Copilot.

---

## Besoins futurs ?

- Aide pour le code d’initialisation, la lecture des données, ou le mapping animations.
- Exemples d’animations LED.
- Conseils pour la conception du boîtier, l’alimentation, etc.

---

**N’hésite pas à reprendre contact en repartant de ce résumé !**  
Tu peux préciser : « Voir mon projet chaussures clignotantes, cf. résumé markdown » pour que je retrouve le contexte.

---

Si tu veux un formatage ou un découpage différent, dis-le-moi !