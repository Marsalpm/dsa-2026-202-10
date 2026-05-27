# Report

## Runtime complexity analysis of initializing the intersections map in Big-O. / Quant costa en temps crear el hashmap de interseccions.
El mapa d'interseccions s'inicialitza recorrent tots els segments del carrer i posan-lo dins del hasmap d'interseccions.
Li direm 'n' al numeros de segments del carrer (troç de intersecció a intersecció).

### Best case
El millor cas de la funció hash serà quan es posin les interseccons correctament entre els buckets del hashmap. Cada cop que inserim el cost es de O(1) i com inserim 'n' vegades (una vegada cada carrer), el cost total és: O(n).

### Average case
Això passa quan el hasmap té poque col·lisions i les insercions poden continuar tenint un cost constant. Per tant aquest cas és com l'anterior i la complexitat total conitua sent: O(n)

### Worst case
El pitjor cas es produirà quan moltes de les interceccions xoquen al mateix bucket. Això fa que cada inserció pugui arribar a costar O(n). I per tant com hi ha n insercions, la seva complexitat total és: O(n^2).