Game Challenge 6, Chuck Bolin, May 1, 2008
"Sinking of the Rising Sun"

Game Genre: Naval strategy from a simulated 3D perspective.
Goal of Game: Seek out and kill the enemy (all ships) avoid detection.

Graphics:  1024 x 768 

Game is playable. To play....
===============================
1) Run GC6_Chuck.exe.
2) Acknowledge Splash screen by left clicking and Select Submarine Operation.
3) Select Training Mission 1.
4) Read Patrol Orders and Accept.
5) View control panel. Sub is on heading of 0 degrees, speed is 5 knots,
   and depth is 25 feet.  
6) Sub may be controlled by clicking on icon below or to the right of the gauges.
7) Click on periscope (pipe). Scape raises.
8) Move mouse around scope. It may be rotated or lowered. Click on eyepiece.
9) From the scope view, user can rotate left or right. Placing the mouse on
   the inside of the rotate triangles if fine speed. Toward point of triangle
   is fast speed. Click above and below reticule to Zoom in and out.
10) Scope may be lowered quickly by clicking on Lower Scope.
11) Torpedos are fired from the scope.  There are an unlimited number of torpedos.
12) Click on Return to Conn icon.
13) The sonar, radar, torpedo, and damage icons are NOT programmed.
14) Click on Chart icon to see topdown view of ships.  This information is based
    is 'unrealistic' but useful for playing easily.
15) Player toggles between Conn, Scope, and Chart.

Three Ways to Lose
===================
1) Run out of time.
2) Collide on surface with a ship.
3) Sunk by depth charges.

One way to win
==============
1) Destroyed required number of ships within the prescribed time frame.

Judging Notes
=============
* All 7 missions are in data files and may be edited carefully.
* All sprites and graphics information is loaded by file (audio is not yet).
* Using Pyro Particle Effects API for first time (smoke and explosions cool
  but not realistic).
* Using FMOD for sound wrapped in Wrapper class.
* Physics done by self. That's why they are not realistic.
* No editor.
* AI is basic. Destroyers will hunt you down until convoy is 8000 yards away.
* All sources are listed in data\references.txt.
* Level files control time of day (minute number) and sea state (1 to 7).
* Ships are not animated as they sink. They simply disappear when their damage is 100%.









 



