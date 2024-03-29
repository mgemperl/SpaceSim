# SpaceSim
Source code for my 2D space sim game

2014_06_16: 
- Implemented and somewhat optimized detections of projectile hits
- Worked on targeting

2014_06_22:
- Worked more on targeting; sectors now store ordered sets of their occupants instead of unordered lists
- Fixed bugs caused by my work on targeting
- Further optimized projectiles' collision detection algorithm

2014_06_23:
- Got targeting working for the player and the AI
- Implemented an ugly version of dust so that the player knows where he or she is moving

2014_06_26:
- Implemented firing arcs and aim assist for projectile weapons
- Started working to improve movement AI, but it got really screwy so I'm giving up for now

2014_06_30:
- Optimized how vectors are handled in the program so that it no longer does the expensive magnitude computation when unnecessary.
- Fixed aiming bug introduced while implementing firing arcs and aim assist for projectile weapons. I love that high you get after solving a tough problem.
- Improved AI tracking and prediction of vessel movement in aiming, though it still can't deal well with acceleration
- Improved how the AI maneuvers
- Fixed targeting bug that caused you to just switch between the first two enemy ships

2014_07_01:
- Improved AI tactics -- they shoot while breaking now
- Improved targeting for player and AI --   added commands to find best target and to untarget

2014_07_09:
- Worked on finding a way to make a good GUI in a reasonable length of time
- Began reorganizing the implementation of the ship AI
- Worked on using xml files to store definitions and game state

2014_08_02:
- Massive restructuring of the general implementation; decided to go for a more M&B style gameplay
- Added stars in the background

2014_08_03:
- Worked on improving the weapon slots on ships
- Turreted weapons are in!

2014_08_05:
- Finally got guided weapons working right
- Made some improvements in my "CONST-CORRECTNESS"

2015_02_08:
- Switched to Direct3D to take advantage of sprites and rebuilt much of the engine architecture for better modularity and for better input and graphics capability
- Finally got basic sprites to be drawn to the screen, and even had them slowly rotate around - the framerate is looking good
- Started working on porting my previous work to the new engine architecture
- Also decided to switch back to a gameplay style closer to Escape Velocity

2015_02_16
- Finished porting pretty much all code from my old engine (except guided projectiles, but that won't be too hard)
- Added the background for sectors
