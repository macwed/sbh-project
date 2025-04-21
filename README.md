# SBH Instance Generator

Narzędzie do generowania instancji problemu SBH (DNA sequencing).
## Budowa
```bash
mkdir build && cd build
cmake .. && ninja

./sbh_instance_generator --mode single --size 500 --lmer 10
./sbh_instance_generator --mode full
