# lights-setup

## Przetwarzanie zdjęć ze studio

### Wymagania

1. Zainstaluj pip: 
  apt-get install python3-pip
2. Zupgraduj pip:
  pip3 install --upgrade pip
3. Zainstaluj zależności potrzebne do uruchoemienia skryptu
  pip3 install -r requirements.txt
 
### Uruchomienie skryptu do przetwarzania

python3 -m photoaid.preprocessing.warsaw_preprocessing --in_dir input_dir_path \
        --out_dir output_dir_path --crop True
