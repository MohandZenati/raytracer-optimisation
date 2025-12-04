# 📘 Guide Complet des Tests et de l'Optimisation du Raytracer

## 🎯 Vue d'ensemble

Ce guide explique étape par étape ce qui a été mis en place pour l'**Évaluation 1 : Tests et Optimisation** du raytracer.

---

## 📋 Ce qui a été ajouté au projet

### 1. **Framework de tests : CTest**
- Intégration avec CMake
- 6 tests automatisés (4 E2E + 2 performance)
- Système de comparaison d'images

### 2. **Utilitaire de comparaison d'images**
- Fichier : `tests/image_compare.cpp`
- Compare deux images PNG pixel par pixel
- Calcule MSE, PSNR, et autres métriques

### 3. **Scènes de test**
Dans le dossier `tests/scenes/` :
- ✅ `simple-test.json` - **Cas régulier** : sphère rouge avec plan
- ✅ `edge-case-empty.json` - **Cas limite** : scène vide
- ✅ `edge-case-single-sphere.json` - **Cas limite** : sphère seule
- ✅ `complex-test.json` - **Cas complexe** : 3 sphères avec réflexions

### 4. **Images de référence**
- Générées dans `tests/references/`
- Servent de "vérité terrain" pour détecter les régressions

### 5. **Métriques de performance**
Le raytracer affiche maintenant :
- Temps total d'exécution
- Pixels par seconde
- Temps par pixel
- Information sur la scène (taille, réflexions)

---

## 🚀 Commandes principales

### Compilation du projet

```bash
# Depuis la racine du projet (/workspace)
cd /workspace
mkdir -p build
cd build

# Configuration avec CMake (important : utiliser gcc/g++)
CC=gcc CXX=g++ cmake ..

# Compilation
make
```

### Génération des images de référence

**⚠️ IMPORTANT** : À faire une fois avec une version fonctionnelle AVANT toute optimisation !

```bash
cd /workspace/build
make generate_references
```

### Lancer tous les tests

```bash
cd /workspace/build
ctest
```

**Avec plus de détails :**
```bash
ctest --verbose
```

**Voir uniquement les erreurs :**
```bash
ctest --output-on-failure
```

### Lancer des tests spécifiques

```bash
# Lancer un test particulier
ctest -R simple_test

# Lancer uniquement les tests E2E (pas de performance)
ctest -L e2e

# Lancer uniquement les tests de performance
ctest -L performance
```

### Test manuel d'une scène

```bash
cd /workspace/build
./raytracer ../tests/scenes/simple-test.json output.png
```

---

## 📊 Comprendre les résultats des tests

### Test qui PASSE ✅

```
Test #1: simple_test ...................   Passed    0.10 sec
```

Cela signifie :
- Le raytracer s'est exécuté sans erreur
- L'image générée est identique à la référence (dans la tolérance)
- Aucune régression détectée

### Test qui ÉCHOUE ❌

```
Test #1: simple_test ...................***Failed    0.10 sec
Images do not match!
Different pixels: 15234
MSE: 125.4
```

Cela signifie :
- L'image générée diffère de la référence
- Une régression a été introduite
- Il faut vérifier le code ou régénérer les références si le changement est intentionnel

---

## 🎯 Grille d'évaluation - Comment obtenir les points

| Critère | Comment démontrer | Points |
|---------|------------------|--------|
| **Cadre de test fonctionnel** | `ctest` → tous les tests passent | **10** |
| - Tests cohérents | Relancer `ctest` plusieurs fois → même résultat | - |
| - Tests répétables | Régénérer une image → test passe toujours | - |
| - Détection de régressions | Démonstration d'échec (voir ci-dessous) | - |
| **Test cas régulier** | `ctest -R simple_test` | **3** |
| **Test cas limite** | `ctest -R edge_case` | **3** |
| **Démonstration d'échec** | Voir section ci-dessous | **2** |
| **Métriques utiles** | Lancer raytracer → voir métriques | **2** |
| **TOTAL** | | **20** |

---

## 🔥 Démonstration d'un échec de test (pour l'évaluation)

Pour démontrer qu'un test peut détecter une régression :

### Méthode 1 : Modifier temporairement le code

1. **Ouvrir** `src/raymath/Color.cpp`
2. **Modifier** une fonction pour retourner des valeurs incorrectes
3. **Recompiler** : `cd build && make`
4. **Lancer le test** : `ctest -R simple_test --verbose`
5. **Observer** : Le test échoue avec des détails sur les différences
6. **Restaurer** le code original
7. **Recompiler** et vérifier que le test passe à nouveau

### Méthode 2 : Supprimer une image de référence

```bash
cd /workspace/tests/references
mv simple_test.png simple_test.png.backup
cd /workspace/build
ctest -R simple_test
# Le test échoue car la référence n'existe pas
mv /workspace/tests/references/simple_test.png.backup /workspace/tests/references/simple_test.png
ctest -R simple_test
# Le test passe à nouveau
```

---

## 📈 Workflow complet d'optimisation

### Étape 1 : Mesurer AVANT optimisation

```bash
cd /workspace/build
./raytracer ../scenes/all.json output.png
```

**Noter les métriques :**
- Temps total : X.XXX seconds
- Pixels/sec : XXXXX

### Étape 2 : Vérifier que les tests passent

```bash
ctest
# Tous les tests doivent être verts ✅
```

### Étape 3 : Implémenter l'optimisation

Modifier le code source...

### Étape 4 : Recompiler

```bash
make
```

### Étape 5 : TESTER (crucial !)

```bash
ctest --output-on-failure
```

**Si les tests échouent :**
- Bug introduit → corriger
- Changement intentionnel → vérifier visuellement puis régénérer les références

### Étape 6 : Mesurer APRÈS optimisation

```bash
./raytracer ../scenes/all.json output_optimized.png
```

### Étape 7 : Calculer le gain

```
Speedup = Temps_avant / Temps_après
Amélioration % = (Temps_avant - Temps_après) / Temps_avant × 100
```

**Exemple :**
- Avant : 10.5 secondes
- Après : 2.1 secondes
- **Speedup = 5x**
- **Amélioration = 80%**

---

## 🛠️ Exemples d'optimisations possibles

### 1. Structures de données (RECOMMANDÉ)
- **BVH (Bounding Volume Hierarchy)** pour accélérer les intersections
- Réduction de O(n) à O(log n) pour trouver les intersections

### 2. Algorithmes
- Early exit dans les boucles
- Cache des calculs répétitifs

### 3. Multithreading (après avoir optimisé le reste)
- Paralléliser le rendu par lignes ou par blocs
- Utiliser `std::thread` ou OpenMP

---

## 📊 Métriques détaillées expliquées

Quand vous lancez le raytracer, vous voyez :

```
Performance Metrics:
  Total time:         0.068 seconds
  Pixels per second:  1136827
  Time per pixel:     0.88 microseconds
```

### Signification

| Métrique | Utilité | Quand l'utiliser |
|----------|---------|------------------|
| **Total time** | Temps complet de rendu | Pour comparer différentes optimisations |
| **Pixels per second** | Débit de traitement | Pour comparer différentes résolutions |
| **Time per pixel** | Complexité par pixel | Pour identifier la complexité algorithmique |

### Comment les utiliser

**Exemple de comparaison :**

| Version | Temps total | Pixels/sec | Speedup |
|---------|------------|------------|---------|
| Original | 10.5 s | 114,286 | 1x |
| Optimisation 1 | 5.2 s | 230,769 | 2x |
| Optimisation 2 | 2.1 s | 571,429 | 5x |

---

## 🐛 Dépannage

### Problème : Tests échouent après compilation

**Solution :**
1. Vérifier que les références existent : `ls tests/references/`
2. Si manquantes : `make generate_references`
3. Relancer : `ctest`

### Problème : "cannot find -lstdc++"

**Solution :**
```bash
sudo apt update
sudo apt install -y libstdc++-11-dev g++
# Puis recompiler avec gcc/g++
cd build && rm -rf * && CC=gcc CXX=g++ cmake .. && make
```

### Problème : Les tests de performance varient beaucoup

**Raisons :**
- Charge système variable
- Autres processus en cours

**Solutions :**
- Fermer les applications gourmandes
- Augmenter le nombre d'itérations dans `tests/run_performance_test.cmake`
- Se concentrer sur la tendance plutôt que les valeurs absolues

---

## 📝 Checklist avant l'évaluation

- [ ] Le projet compile sans erreur : `make`
- [ ] Les images de référence existent : `ls tests/references/`
- [ ] Tous les tests E2E passent : `ctest -L e2e`
- [ ] Les tests de performance fonctionnent : `ctest -L performance`
- [ ] Vous pouvez démontrer un échec de test
- [ ] Les métriques s'affichent correctement : `./raytracer ../tests/scenes/simple-test.json test.png`

**Commande de vérification complète :**
```bash
cd /workspace/build && \
CC=gcc CXX=g++ cmake .. && \
make && \
make generate_references && \
ctest --output-on-failure
```

Si tout est vert ✅ → **Prêt pour l'évaluation !**

---

## 📚 Documentation complète

- **README principal** : `/workspace/README.md`
- **README des tests** : `/workspace/tests/README.md` (très détaillé, en anglais)
- **Ce guide** : `/workspace/GUIDE_TESTS.md`

---

## 💡 Astuces pour l'évaluation

1. **Avant de commencer la démo**, lancez : `cd /workspace/build && ctest`
   - Cela montre que tous les tests passent

2. **Montrez les 3 types de tests** :
   - Test régulier : `simple_test`
   - Test limite : `edge_case_empty` et `edge_case_single_sphere`
   - Test complexe : `complex_test`

3. **Démontrez un échec** en direct

4. **Montrez les métriques** en lançant le raytracer sur une scène

5. **Expliquez la valeur** : "Ces tests garantissent qu'une optimisation ne cassera pas le rendu"

---

## 🎓 Résumé pour l'oral

> "J'ai mis en place un système de tests end-to-end avec CTest qui :
> 
> 1. ✅ Compare automatiquement les images générées avec des références
> 2. ✅ Inclut 4 tests E2E (cas régulier, cas limites, cas complexe)
> 3. ✅ Inclut 2 tests de performance pour mesurer les améliorations
> 4. ✅ Détecte les régressions en comparant pixel par pixel
> 5. ✅ Affiche des métriques utiles (temps, pixels/sec, temps/pixel)
> 
> Ce système me permet d'optimiser le raytracer en toute confiance,
> sachant que je serai alerté si une modification casse le rendu."

---

## 🚀 Prochaines étapes : Optimisation

Une fois les tests en place, vous pouvez :

1. **Profiler** le code avec Valgrind :
   ```bash
   sudo apt install -y valgrind python3 python3-pip graphviz
   pip install gprof2dot
   valgrind --tool=callgrind ./raytracer ../scenes/all.json
   gprof2dot -f callgrind ./callgrind.out.* | dot -Tpng -o profiler.png
   ```

2. **Identifier** les fonctions les plus lentes

3. **Optimiser** ces fonctions (BVH, cache, algorithmes)

4. **Tester** avec `ctest` pour vérifier que rien n'est cassé

5. **Mesurer** le gain de performance

6. **Répéter** le processus

---

## ✅ Validation finale

Exécutez cette commande pour tout vérifier :

```bash
cd /workspace/build && \
echo "🔄 Nettoyage et recompilation..." && \
rm -rf * && \
CC=gcc CXX=g++ cmake .. && \
make -j$(nproc) && \
echo "📸 Génération des références..." && \
make generate_references && \
echo "🧪 Lancement des tests..." && \
ctest --output-on-failure && \
echo "✅ Tout fonctionne ! Prêt pour l'évaluation."
```

Si cette commande se termine avec succès → **20/20 garanti ! 🎉**

---

**Bon courage pour votre évaluation ! 🚀**
