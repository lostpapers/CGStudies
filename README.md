# Journal de développement


## Introduction

En quelques mots, mon but est d'explorer le plus loin possible tout ce qui touche aux nouvelles techniques de rendu en rapport avec le "Physically Based Rendering" (PBR)

C'est un vaste sujets sur lequel j'ai réalistiquement 20 ans de retard.

Tenir une sorte de journal me semble être le moyen le plus simple pour garder le cap dans mon projet et suivre mes progrès. 
J'y garde donc mes notes, mes motivations, mes objectifs, mes tâches en cours et tout ce qui pourra me servir à progresser. 
Avoir tout cela inscrit noir sur blanc me permet de m'y référer et de me rappeler régulièrement d'où je pars.

Je pense que cela sera un beau voyage.

***Note:** Pour des facilités d'usage, les notes les plus récentes sont en tête de journal (un peu comme un blog)*

## Objectifs & Tâches
### Court terme

 - Implémenter un rendu temps réel en OpenGL, avec déplacement de caméra et affichage d'objets utilisant des vertex shaders et des fragment shaders.
 - + Lumières
 - + Skybox
 - Utiliser quelques fragment shaders venant de ShaderToy
 - importer des objets au format Wavefront (.OBJ)

### Moyen terme
### Long terme
## Février 2020: Premiers Tutoriels Qt-OpenGL et PBRt

Au sein de tous les sites parcourus, deux sites m'ont particulièrement parus pertinents pour commencer l'étude d'OpenGL, en s'appuyant sur Qt pour la gestion des entrées et des fenêtres:

 - [Le blog de Trent Reed](https://www.trentreed.net/topics/opengl/) et [ses sources](https://github.com/TReed0803/QtOpenGL/tree/tutorial-series)
 - [Le blog de Guillaume Belz](http://guillaume.belz.free.fr/doku.php?id=start#articles_opengl) et [ses sources](https://github.com/GuillaumeBelz/qt-opengl)
 
 En ce qui concerne PBRt, j'ai trouvé ce site qui montre étape par étape les résultats qu'à obtenu Jan Walter avec son[ implémentation de PBRt en RUST](https://www.janwalter.org/jekyll/rendering/pbrt/rust/2017/05/08/pbrt-rust-v0_1_0.html). C'est une bonnne idée que de progresser ainsi par étapes

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
