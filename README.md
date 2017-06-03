# xFinite
![logo](img/logo.png?raw=true "Logo")

A spaceships game based on the finite element method.

## Developing
### Moduli
- Editor di navicelle
  - Crea, sperimenta navicelle e salvale


- Gioco Multiplayer
  - I giocatori giocano solo online con varie regole. Le variante possono basarsi su costante elastica, viscosità dello spazio, ambientazione e varie.

### Multiplayer
C'è un sever con gli account e i dati dei giocatori. (ENet)

Il client calcola FEM sulla gpu e invia i dati di -posizione -deformazione e -scorrimento al server. Queste vengono condivise con gli altri giocatori.
Altre strutture vengono calcolate dal client. Il server in questo modo non carica la cpu o al massimo calcola gli eventi come gli scontri.

### Traduzione
Traduzioni linguistiche (gettext)
### Grafica
OpenGL e FLWM con grafica pixel. C'è molto lavoro da fare.
