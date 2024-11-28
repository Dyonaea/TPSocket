## But de l'application
L'objectif de cette application est de créer un programme client/serveur permettant de récupérer la moyenne d'un étudiant dans une matière donnée. Le client envoie une requête avec le nom, le prénom de l'étudiant et l'indice de la matière. Le serveur traite cette requête et renvoie la moyenne ou un message d'erreur si l'indice est invalide.

---

## Solution mise en œuvre

### Partie 1 : Description de la solution
Dans cette première partie, le serveur fonctionne en mode mono-client, ce qui signifie qu'il peut traiter une requête à la fois. Le client envoie une requête avec le nom, prénom de l'étudiant et l'indice de la matière pour laquelle il souhaite obtenir la moyenne. Le serveur répond avec la moyenne de l'étudiant pour la matière demandée, ou renvoie un message d'erreur si l'indice est invalide.

Le serveur utilise une socket TCP pour écouter les connexions des clients. Il reçoit la requête, vérifie la validité de l'indice de matière, puis répond au client avec la moyenne ou un message d'erreur.

### Partie 2 : Gestion des requêtes multiples (future amélioration)
Dans une version ultérieure, le serveur sera modifié pour gérer plusieurs clients simultanément, en utilisant des **threads** ou des **processus**. Cela permettra au serveur de répondre à plusieurs clients en parallèle sans attendre que le client précédent soit déconnecté.

---

## Choix des outils utilisés pour la solution

### 1. Définition de l'Étudiant
Un **étudiant** est défini par une structure contenant les informations suivantes :
- **Nom** et **Prénom** : chaînes de caractères.
- **Moyennes** : un tableau de 5 flottants représentant les moyennes dans 5 matières différentes.

#### Structure en C :
```c
typedef struct {
    char nom[50];
    char prenom[50];
    float moyennes[NB_MATIERE];
} Etudiant;
```
Le choix de cette structure est justifié par la simplicité et l'efficacité pour stocker et manipuler les informations d'un étudiant dans un programme de petite envergure.


