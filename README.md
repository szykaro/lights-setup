# lights-setup

## Przetwarzanie zdjęć ze studio

### Wymagania

1. Zainstaluj pip: 
  'apt install python3-pip'
2. Zinstaluj cmake:
  'apt install cmake'
3. Zainstaluj exiftool:
  'apt install exiftool'
5. Zupgraduj pip:
  'pip3 install --upgrade pip'
6. Zainstaluj zależności potrzebne do uruchomienia skryptu:
  'pip3 install -r requirements.txt'
7. Zainstaluj opencv ponownie, jeśli wyrzuca błędy:
  'apt-get install python3-opencv'
 
### Uruchomienie skryptu do przetwarzania

'python3 -m warsaw_preprocessing_2021_06 --in_dir input_dir_path --out_dir output_dir_path --workers num_of_workers'
