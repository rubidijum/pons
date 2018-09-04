# RG063-pons
Pons

Simple bridge building simulation program developed for computer graphics course on Faculty of Mathematics, University of Belgrade. Written in C with OpenGL libraries.

## How to run:
* Download the repository
* Type in terminal
  ```
  $ make
  $ ./pons
  ```

Use a mouse to draw a bridge beam by beam(left click and drag to desired position for each beam). You can only click on existing joints when drawing. You can choose which type of beam is active by pressing 't'('T') for road or 'y'('Y') for supporting beam. Car can only cross the bridge via road, support beams are just for the support of the bridge. When you are ready to test your creation, press 'v'('V') to change view and then start the animation on 'g'('G'). You can pause the animation by pressing 's'('S') and also restart the animation by pressing 'r'('R'). If you want to undo your last added beam, pres 'u'('U'). 

 
## Controls:
|**key**|**action**|
|:---|:---|
|`g` `G`|Start animation|
|`s` `S`|Stop animation|
|`v` `V`|Change view|
|`b` `B`|Enter bridge building mode|
|`r` `R`|Restart animation|
|`mouse`|Draw beams|
|`u` `U`|Undo last beam|
|`t` `T`|Toggle beam purpose to 'road'|
|`y` `Y`|Toggle beam purpose to 'beam'|

![gif](https://github.com/MATF-RG17/RG063-pons/blob/master/Gifs/first.gif)

![gif](https://github.com/MATF-RG17/RG063-pons/blob/master/Gifs/second.gif)
