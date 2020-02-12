# Présentation

En quelques mots, mon but est d'explorer le plus loin possible tout ce qui touche aux nouvelles techniques de rendu en rapport avec le "Physically Based Rendering" (PBR). C'est un vaste sujets sur lequel j'ai réalistiquement 20 ans de retard.

De fil en aiguille, j'ai (mal)heureusement redécouverts d'autres sujets connexes tels le Ray-marching, le Path-tracing, les shaders, l'IBL (Image based lighting). Cela fait énormément de sujets intéressants à étudier en programmation

Tenir un journal me semble être le moyen le plus simple pour garder le cap dans mon projet et suivre mes progrès. 

J'y garde donc mes notes, mes motivations, mes objectifs, mes tâches en cours et tout ce qui pourra me servir à progresser. 
Avoir tout cela inscrit noir sur blanc me permet de m'y référer et de me rappeler régulièrement d'où je pars.

Je pense que cela sera un beau voyage.

***Note:** Pour des facilités d'usage, les notes les plus récentes sont en tête de journal (un peu comme un blog)*

# Objectifs & Tâches
## Court terme
 
 - Trouver une  référence de langage shader Qt5/OpenGL/GLSL
 - Comment gérer les UV dans le fragment shader
 - Comprendre les shaders?
 
 
## Moyen terme

 - Implémenter un rendu temps réel en OpenGL, avec déplacement de caméra et affichage d'objets utilisant des vertex shaders et des fragment shaders.
 - Utiliser quelques fragment shaders venant de ShaderToy
 - Lumières
 - Skybox
 - Matériaux PBR
 - importer des objets au format Wavefront (.OBJ)
 - Exporter au format .PBR et tester dans PBRt-v3
 
## Long terme

# Journal de développement

## 12/02/20 - Shadertoy


Très bonne avancée sur le développement de shaders et surtout sur les techniques liées au ray_marching et au description de scènes en "Signed Distance Field" (SDF)

Je suis plusieurs tutoriaus et réplique les techniques que j'ai pu analyser et comprendre.


## 06/02/20 - Premiers obstacles

J'ai compris que Qt5 avait amené pas mal de nouveauté dans  ce qu'on appelle "Modern OpenGL". Le résultat est que les tutoriaux de Developpez.com sont moins intéressants car ils s'appueint sur Qt4.

A mon grand regret, le blog de Trent Reed n'apporte plus de conseils simples pour progresser, une fois mon cube coloré affiché. 

il faudrait que je décortique ses code-sources pour progresser, ce qui risque d'être une solution fastideuse. Je me garde cette idée de côté au cas où je ne trouverai pas de tutoriaux aussi efficaces.

Sinon en parallèle, je regarde ce qui existe comme projets sur Shadertoy qui pourraient me servir de référence pour mieux comprendre les shaders. Mais attention, il est aisé de se disperser vu le nombre de domaines qui sont intéressant (ray marching, SDF, PBR, ...)

.../...

Finalement, j'ai pris le parti de continuer l'exploration des nouvelles technos par l'intermédiaire de Shadertoy. En effet, cela founit un accès facile à un moteur de rendu très performant, qui peret notamment de déjà explorer très facilement plusieurs technos.

Grace à Inigo Quilez et sa vidéo de deconstruction de Shader, j'ai déjà pu jouer à créer une visualisation par lancer de rayon, utilisant le principe de "Ray Marching" combiné à du "Sign Distance Field". Le rendu est pour l'instant à base de couleur diffuse, d'illumination directionnelle et du calcul d'ombres projetées.




## 05/02/20 - Shadertoy

J'ai voulu utiliser un shader simple de ShaderToy pour l'appliquer  en tant que fragment shader sur mon cube. 

Cela m'a permis de comprendre qu'en fait ShaderToy fournissait ses propres Uniforms. Il était nécessaire de faire moi-même le passage d'un Uniform pour le temps écoulé car ce n'était pas disponible par défaut. De même il faudrait que je fasse cela pour la souris.

J'ai vu aussi que ce n'était pas si simple d'avancer sans documentation sur le langage GLSL, donc il faut que je trouve un document de référence sur Qt5/OpenGL/GLSL

Le résultat de ma migration du shader a mis en évidence que le rendu était bien en coordonnées écran. La couleur ne semble donc pas mappée sur l'objet. Il faudrait donc que je transfère des informations d'UV pour  que cela me donne un rendu en perspective

## 04/02/20 - Premiers Tutoriels Qt-OpenGL

Au sein de tous les sites parcourus, trois sites m'ont particulièrement parus pertinents pour commencer l'étude d'OpenGL. Les deux premiers s'appuient sur Qt pour la gestion des entrées et des fenêtres:

 - [Le blog de Trent Reed](https://www.trentreed.net/topics/opengl/) et [ses sources](https://github.com/TReed0803/QtOpenGL/tree/tutorial-series)
 - [Le blog de Guillaume Belz](http://guillaume.belz.free.fr/doku.php?id=start#articles_opengl) et [ses sources](https://github.com/GuillaumeBelz/qt-opengl)
 - [Le site opengl-tutorial.org](http://www.opengl-tutorial.org/) et [ses sources](https://github.com/opengl-tutorials/ogl)
 
 C'est grâce aux tutos de Trent que j'ai pu commencer le développement, mais les autres tutoriaux sont très intéressants. Il est probable que j'ai d'abord à faire ces 3 tutoriaux avant de progresser dans ma quête.
 
 ### PBRt
 En ce qui concerne PBRt, j'ai trouvé ce site qui montre étape par étape les résultats qu'à obtenu Jan Walter avec son[Implémentation de PBRt en RUST](https://www.janwalter.org/jekyll/rendering/pbrt/rust/2017/05/08/pbrt-rust-v0_1_0.html). 
 
 C'est une bonnne idée que de progresser ainsi par étapes et je suivrai certainement un processus similaire. Mais pour le moment je garde cela dans un coin pour référence.

## Janvier 2020: Motivations originelles

Début 2020, je suis tombé par hasard sur ce livre disponible en accès libre depuis fin 2018: 
[Physically Based Rendering : From theory to implementation](http://www.pbr-book.org/3ed-2018/contents.html). 

Déjà ravi de trouver une telle manne de connaisance, c'était sans compter qu'en plus, ses généreux créateurs 
avaient mis à disposition [toutes les sources de leur "ray tracer"](https://github.com/mmp/pbrt-v3).

Bien évidemment, j'ai été totalement débordé par l'envie d'étudier le bousin et de profiter de cette manne pour
écrire mon propre RT. Comme d'hab', quoi! comme un gamin qui ouvre ses nouveaux jouets à Noël.

Mais c'est loin d'être simple, car en continuant à explorer le thème, j'ai découvert combien le PBR a influencé le
monde du rendu temps-réel, pas seulement celui du calcul en différé. Unreal ou d'autres moteurs de jeux comme "Star
Citizen" par exemples, s'appuient sur certains principes du PBR pour améliorer leur rendu.

J'ai aussi découvert que certains développeurs s'étaient même amusés a créer des shaders a base de ray-marching 
pour faire des rendus similaires à PBRt-v3, ou utilisant les formules du PBR pour le rendu des textures ou des 
surfaces ([Shadertoy - PBR](https://www.shadertoy.com/results?query=tag%3Dpbr)). 

A ce point là, je me suis demandé quelle serait la meilleure méthode pour progresser: Partir vers un Ray tracer, 
ou s'appuyer sur les shaders GLSL?

Mais en fait, pourquoi ne pas mixer les deux. 

J'ai donc réalisé que ce qui me satisferait serait de pouvoir obtenir un rendu temps-réel d'une scène, utilisant 
des matériaux PBR, que je pourrai ensuite utiliser dans un ray-tracer de mon cru.

OpenGL m'a paru être naturellement le meilleur candidat pour la partie temps réel, et faciliser l'utilisation 
des shaders GLSL

Continuant sur ce chemin, je suis alors tombé sur quantité de tutoriaux parlant de PBR, d'OpenGL, de Qt... Jusqu'à 
ce qu'un début de route se dessine, notamment grâce aux tutoriaux Qt/OpenGL de Trent Reed.

Voici donc les premières étapes que je me suis définies dans cette aventure:

 - Mettre en place une base d'outil de rendu temps réel, où il est possible de se déplacer autour d'un objet 3D
 - Supporter un format de données pour le chargement d'objets et de scène (par exemple: scènes .PBR, fichiers Wavefront .OBJ)
 - Définir des matériaux basiques et des lumières
 - Exporter une scène pour rendu ray-tracer, où la caméra sera positionnée comme dans l'outil
 - Faire un premier rendu basique de ray-tracing ou ray-marchine (à voir)
 
 J'ai notamment en tête deux images trouvées sur le net, que j'aimerai générer par moi même grâce à ces outils:
 
 *Ray tracing: Rendu d'une boîte de Cornell*
 
[Plusieurs exemples de rendus sur le site de Stanford](https://graphics.stanford.edu/~henrik/images/cbox.html)
 
<img src="https://graphics.stanford.edu/~henrik/images/imgs/cbox_pathtracing.jpg" width="300">
 
 *Temps-réel: Rendu multi matériaux par Unreal"

[Doc Unreal: Physically Bases Materials](https://docs.unrealengine.com/en-US/Engine/Rendering/Materials/PhysicallyBased/index.html)

<img src="https://docs.unrealengine.com/Images/Engine/Rendering/Materials/PhysicallyBased/measured_materials.png" width="300">
