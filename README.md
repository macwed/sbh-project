# SBH Instance Generator

Narzędzie do generowania instancji problemu SBH (DNA sequencing).
## Budowa
```bash
mkdir build && cd build
cmake ..
make

./generator --mode single --size 500 --lmer 10
./generator --mode full
