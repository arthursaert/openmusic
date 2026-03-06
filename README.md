# OpenMusic

OpenMusic é um player de música de código aberto feito para as pessoas ouvirem música de forma simplificada, ele possuí as funcionalidades de:

- Retroceder e avançar
- Selecionar múltiplas músicas

## Compilação
O OpenMusic já é pré-compilado por padrão para Linux, mas se quiser compilar, siga esses passos:

### 1
Clonar repositório Git na pasta que você quiser
```bash
mkdir openmusic && cd openmusic
```
```bash
git clone https://github.com/arthursaert/openmusic.git
```
### 2
Compilar OpenMusic
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```
```bash
make -j$(nproc)
```

> O OpenMusic é feito especialmente para Linux, pode ser preciso modificações para compilar para o Windows ou Mac. O OpenMusic está disponível sobre a licença Apache 2.0.

**Haverá futuras atualizações para esse leitor**, espero que goste!

> O OpenMusic está disponível sobre a licença Apache 2.0.
