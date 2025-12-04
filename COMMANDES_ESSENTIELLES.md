# ⚡ Commandes Essentielles - Aide-Mémoire

## 🔧 Compilation

```bash
cd /workspace/build
CC=gcc CXX=g++ cmake ..
make
```

---

## 🧪 Tests

### Lancer tous les tests
```bash
cd /workspace/build
ctest
```

### Voir les détails
```bash
ctest --verbose
```

### Test spécifique
```bash
ctest -R simple_test
```

### Tests E2E uniquement
```bash
ctest -L e2e
```

### Tests de performance uniquement
```bash
ctest -L performance
```

---

## 📸 Images de référence

### Générer les références
```bash
cd /workspace/build
make generate_references
```

⚠️ **À faire UNE FOIS avec une version FONCTIONNELLE avant optimisation**

---

## 🎨 Lancer le raytracer

### Scène simple
```bash
cd /workspace/build
./raytracer ../tests/scenes/simple-test.json output.png
```

### Scène complexe
```bash
./raytracer ../tests/scenes/complex-test.json output.png
```

### Scène du projet
```bash
./raytracer ../scenes/all.json output.png
```

---

## 🔍 Comparaison manuelle d'images

```bash
cd /workspace/build
./image_compare reference.png test.png 1.0
```

---

## 🎯 Pour l'évaluation

### Commande magique (tout en un)
```bash
cd /workspace/build && \
CC=gcc CXX=g++ cmake .. && \
make -j$(nproc) && \
make generate_references && \
ctest --output-on-failure
```

### Démontrer un échec
```bash
# Sauvegarder une référence
mv /workspace/tests/references/simple_test.png /tmp/backup.png

# Le test échoue
ctest -R simple_test

# Restaurer
mv /tmp/backup.png /workspace/tests/references/simple_test.png

# Le test passe
ctest -R simple_test
```

---

## 📊 Mesurer les performances

### Avant optimisation
```bash
cd /workspace/build
./raytracer ../scenes/all.json before.png
# Noter : "Total time: X.XXX seconds"
```

### Après optimisation
```bash
make
./raytracer ../scenes/all.json after.png
# Noter : "Total time: Y.YYY seconds"
# Calcul : Speedup = X / Y
```

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| `EXPLICATION_SIMPLE.md` | Explication simple et concise |
| `GUIDE_TESTS.md` | Guide complet en français |
| `PRESENTATION_EVALUATION.md` | Pour préparer l'évaluation |
| `FICHIERS_AJOUTES.md` | Liste de tous les fichiers |
| `tests/README.md` | Documentation technique détaillée |
| `COMMANDES_ESSENTIELLES.md` | Ce document |

---

## ✅ Checklist rapide

- [ ] Compiler : `cd build && CC=gcc CXX=g++ cmake .. && make`
- [ ] Générer références : `make generate_references`
- [ ] Tester : `ctest`
- [ ] Tout est vert ? ✅ Prêt !

---

## 🆘 Problème ?

### Tests échouent
```bash
# Vérifier les références
ls -la /workspace/tests/references/

# Si manquantes
cd /workspace/build
make generate_references
```

### Erreur de compilation
```bash
# Nettoyer et recompiler
cd /workspace/build
rm -rf *
CC=gcc CXX=g++ cmake ..
make
```

---

## 🎓 Les 6 tests

1. ✅ `simple_test` - Cas régulier
2. ✅ `edge_case_empty` - Scène vide
3. ✅ `edge_case_single_sphere` - Sphère seule
4. ✅ `complex_test` - Scène complexe
5. ✅ `simple_performance` - Performance simple
6. ✅ `complex_performance` - Performance complexe

---

**Tout est prêt pour l'évaluation ! 🚀**
