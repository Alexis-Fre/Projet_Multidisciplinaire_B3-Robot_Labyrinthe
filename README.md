# Projet_Multidisciplinaire_B3-Robot_Labyrinthe

<p align="center">
  <img src="Photo_projet/Photo_Robot_monter"/>
</p>

**Membres du Groupe :** Papa Mbaye Diop - Alexis Freville

**Nom du Groupe :** [À définir]

Liens pour des vidéos d'exemples : [Projet Pluridisciplinaire](https://www.youtube.com/results?search_query=Projet+pluridisciplinaire+unilasalle+amiens)

Drive d'informations : [Drive](https://drive.google.com/drive/folders/1vGQ9Tg1n6bP1PL6xV9Bm0Bw2ANZ7lBGQ?usp=drive_link)

## Cahier des charges et Objectifs
- **Objectif principal :** Parcourir un labyrinthe (taille standard, configuration dévoilée au départ) le plus rapidement possible.
- **Fonctionnement :** Robot 100% autonome après activation (le chrono démarre à l'activation). Aucune intervention humaine autorisée.
- **Matériel :** Utilisation stricte des composants du kit fourni. **Exception :** Les supports imprimés en 3D sont autorisés. Toutes les sources d'énergie doivent être embarquées.
- **Détection :** 3 capteurs lasers à disposition pour la détection des murs et l'évitement des obstacles.
- **Navigation :** Intégration d'algorithmes de navigation et potentiellement de cartographie en temps réel.
- **Classement :** Basé sur le temps de parcours (critères de départage en cas d'égalité).

## Livrables
- [ ] **Vidéo (en Anglais) :** Preuve de la performance du robot dans le labyrinthe (à rendre sur clé USB, YouTube, Dailymotion...).
- [ ] **Rapport (en Français).**

## Bibliothèques utilisées
- Servo : https://docs.arduino.cc/libraries/servo/

## À faire :
### Code :
- [X] Faire la schematic du montage du code `Avancer.ino`
- [X] Calibration matérielle et logicielle des Servomoteurs. (Code créer dans `Codes/Servos/Calibration_Servos`)
- [X] Calibration de la distance de détection des capteurs lasers.
- [ ] Développement de l'algorithme de navigation.
- [ ] Tests physiques dans le labyrinthe et ajustements.
- [X] Test de tous les capteurs de distance
- [X] Test de la communication I2C avec les capteurs de distance mis sur le même bus
- [ ] Schema electronique à faire pour le montage des 3 catpeurs en série sur l'I2C

### Vidéo / Rapport :
- [ ] Montage du Robot (penser à filmer les étapes pour la vidéo en anglais).

### Autres
- [ ] Modélisation / Impression 3D des supports des capteurs
