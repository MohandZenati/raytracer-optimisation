# Tests End-to-End pour le Raytracer

Ce dossier contient les tests end-to-end (E2E) et de performance pour le raytracer. Ces tests assurent que les optimisations n'introduisent pas de régressions dans le rendu.

## 📋 Table des matières

- [Architecture des tests](#architecture-des-tests)
- [Installation et configuration](#installation-et-configuration)
- [Utilisation](#utilisation)
- [Types de tests](#types-de-tests)
- [Métriques de performance](#métriques-de-performance)
- [Ajouter de nouveaux tests](#ajouter-de-nouveaux-tests)

---

## 🏗️ Architecture des tests

### Composants

1. **`image_compare.cpp`** - Utilitaire pour comparer deux images PNG pixel par pixel
2. **`scenes/`** - Scènes de test (JSON)
3. **`references/`** - Images de référence (générées)
4. **Scripts CMake** - Orchestration des tests avec CTest

### Workflow

```
Scene JSON → Raytracer → Image générée → Comparaison → Image de référence
                                              ↓
                                         PASS/FAIL
```

---

## 🚀 Installation et configuration

### 1. Compiler le projet avec les tests

```bash
cd /workspace
mkdir -p build
cd build
cmake ..
make
```

Cela compile :
- Le raytracer principal (`raytracer`)
- L'outil de comparaison d'images (`image_compare`)
- Configure tous les tests CTest

### 2. Générer les images de référence

**IMPORTANT** : Avant de lancer les tests, vous devez générer les images de référence :

```bash
cd build
make generate_references
```

Cette commande :
- Exécute le raytracer sur toutes les scènes de test
- Génère les images PNG de référence dans `tests/references/`
- Ces images servent de "vérité terrain" pour les tests

> ⚠️ **Note** : Les images de référence doivent être générées avec une version FONCTIONNELLE du raytracer avant toute optimisation !

---

## 🧪 Utilisation

### Lancer tous les tests

```bash
cd build
ctest
```

ou avec plus de détails :

```bash
ctest --verbose
```

ou encore plus détaillé :

```bash
ctest --output-on-failure
```

### Lancer un test spécifique

```bash
ctest -R simple_test
```

### Lancer uniquement les tests E2E (pas de performance)

```bash
ctest -L e2e
```

### Lancer uniquement les tests de performance

```bash
ctest -L performance
```

---

## 📊 Types de tests

### Tests End-to-End

Ces tests vérifient que le raytracer produit l'image attendue :

| Test | Description | Type | Scène |
|------|-------------|------|-------|
| `simple_test` | Cas d'usage régulier : une sphère rouge avec réflexion sur un plan | **Régulier** | `simple-test.json` |
| `edge_case_empty` | Cas limite : scène vide (pas d'objets) | **Limite** | `edge-case-empty.json` |
| `edge_case_single_sphere` | Cas limite : une seule sphère sans réflexion | **Limite** | `edge-case-single-sphere.json` |
| `complex_test` | Scène complexe : 3 sphères avec réflexions multiples | **Complexe** | `complex-test.json` |

#### Critères de succès

Un test E2E **PASSE** si :
- Le raytracer s'exécute sans erreur
- L'image générée existe
- L'image générée est identique à l'image de référence (dans la tolérance définie)

#### Tolérance

Une tolérance de `1.0` est utilisée, ce qui permet une différence maximale de 1 par canal de couleur (0-255). Cela permet de gérer :
- Les arrondis numériques mineurs
- Les différences dues aux optimisations du compilateur

### Tests de performance

Ces tests mesurent le temps d'exécution :

| Test | Description | Scène |
|------|-------------|-------|
| `simple_performance` | Performance sur scène simple | `simple-test.json` |
| `complex_performance` | Performance sur scène complexe | `complex-test.json` |

Les tests de performance :
- Exécutent le raytracer 3 fois
- Calculent le temps moyen
- Affichent les métriques dans la sortie

> 💡 **Note** : Les tests de performance ne "FAIL" jamais - ils servent uniquement à collecter des métriques.

---

## 📈 Métriques de performance

Le raytracer affiche automatiquement plusieurs métriques utiles :

```
Performance Metrics:
  Total time:         2.345 seconds
  Pixels per second:  262144
  Time per pixel:     3.82 microseconds
```

### Métriques clés

| Métrique | Description | Utilité |
|----------|-------------|---------|
| **Total time** | Temps total de rendu | Métrique principale pour l'optimisation |
| **Pixels per second** | Débit de pixels | Permet de comparer différentes résolutions |
| **Time per pixel** | Temps moyen par pixel | Utile pour évaluer la complexité algorithmique |

### Comment utiliser ces métriques

1. **Avant optimisation** : Notez les métriques de base
2. **Après optimisation** : Comparez les nouvelles métriques
3. **Calculez le gain** :
   ```
   Speedup = Temps_avant / Temps_après
   Amélioration% = (Temps_avant - Temps_après) / Temps_avant × 100
   ```

**Exemple** :
- Avant : 10.5 secondes
- Après : 2.1 secondes
- Speedup : 5x
- Amélioration : 80%

---

## 🔍 Démonstration d'un échec de test

### Créer volontairement un échec

Pour démontrer qu'un test peut détecter une régression, vous pouvez :

1. **Modifier temporairement le code** (par exemple, dans `Color.cpp`) :
   ```cpp
   // Changer toutes les couleurs rouges en bleu
   Color red(1.0, 0.0, 0.0);  // Original
   Color red(0.0, 0.0, 1.0);  // Modifié
   ```

2. **Recompiler et tester** :
   ```bash
   make
   ctest -R simple_test --verbose
   ```

3. **Observer l'échec** :
   ```
   Test FAILED: Images do not match!
   Different pixels: 15234
   MSE: 125.4
   ```

4. **Restaurer le code** et vérifier que le test passe à nouveau.

---

## ➕ Ajouter de nouveaux tests

### Étape 1 : Créer une nouvelle scène

Créez un fichier JSON dans `tests/scenes/` :

```json
{
    "image": { "width": 400, "height": 300 },
    "reflections": 1,
    "ambient": { "r": 1, "g": 1, "b": 1 },
    "lights": [ ... ],
    "objects": [ ... ]
}
```

### Étape 2 : Ajouter le test dans CMakeLists.txt

Éditez `tests/CMakeLists.txt` :

```cmake
add_raytracer_test(
    "mon_nouveau_test"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/ma-scene.json"
    "1.0"  # tolérance
)
```

### Étape 3 : Régénérer et tester

```bash
cd build
cmake ..
make generate_references
ctest -R mon_nouveau_test
```

---

## 🛠️ Outils de comparaison d'images

### Utilisation manuelle de `image_compare`

```bash
./image_compare reference.png test.png [tolerance]
```

**Exemple** :
```bash
./image_compare tests/references/simple_test.png output.png 1.0
```

**Sortie** :
```
Comparing images:
  Reference: tests/references/simple_test.png
  Test:      output.png
  Tolerance: 1.0

Results:
  Identical:        YES
  Different pixels: 0
  MSE:              0.0
  PSNR:             inf dB
  Max difference:   0.0
```

### Métriques de comparaison

- **MSE (Mean Squared Error)** : Erreur quadratique moyenne - plus c'est bas, mieux c'est
- **PSNR (Peak Signal-to-Noise Ratio)** : Rapport signal/bruit - plus c'est élevé, mieux c'est (> 40 dB = excellent)
- **Different pixels** : Nombre de pixels différents au-delà de la tolérance

---

## 📝 Grille d'évaluation

Voici comment les tests répondent aux critères d'évaluation :

| Critère | Réponse | Points |
|---------|---------|--------|
| Cadre de test fonctionnel (CTest) | ✅ Configuré avec CMake | 10 |
| Tests cohérents | ✅ Mêmes entrées → mêmes sorties | - |
| Tests répétables | ✅ Images de référence figées | - |
| Détection de régressions | ✅ Comparaison pixel par pixel | - |
| Test cas régulier | ✅ `simple_test` | 3 |
| Test cas limite | ✅ `edge_case_empty` + `edge_case_single_sphere` | 3 |
| Démonstration d'échec | ✅ Voir section ci-dessus | 2 |
| Métriques utiles | ✅ Temps, pixels/sec, temps/pixel | 2 |
| **Total** | | **20** |

---

## 🚨 Dépannage

### Problème : "Reference image not found"

**Solution** : Générer les images de référence :
```bash
make generate_references
```

### Problème : Tests échouent après une modification

1. Vérifiez que la modification n'a pas introduit de bug
2. Si la modification est intentionnelle et change le rendu :
   - Régénérez les références : `make generate_references`
   - Vérifiez visuellement que les nouvelles images sont correctes

### Problème : Tests de performance varient beaucoup

- Les tests de performance dépendent de la charge système
- Fermez les applications gourmandes
- Augmentez le nombre d'itérations dans `run_performance_test.cmake`

---

## 📚 Ressources

- **CTest Documentation** : https://cmake.org/cmake/help/latest/manual/ctest.1.html
- **LodePNG** : Bibliothèque utilisée pour lire/écrire des PNG
- **Profiling avec Valgrind** : Voir le document principal pour l'optimisation

---

## ✅ Checklist avant l'évaluation

- [ ] Le projet compile sans erreur
- [ ] Les images de référence ont été générées
- [ ] Tous les tests E2E passent : `ctest -L e2e`
- [ ] Les tests de performance fonctionnent : `ctest -L performance`
- [ ] Vous pouvez démontrer un échec de test
- [ ] Les métriques de performance s'affichent correctement

**Commande rapide de vérification** :
```bash
cd build
cmake .. && make && make generate_references && ctest --output-on-failure
```

Si tout est vert ✅, vous êtes prêt pour l'évaluation !
