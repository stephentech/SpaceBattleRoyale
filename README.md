# SpaceBattleRoyale

This is a hobby project. Built in C/C++ in the Visual studios environment. Written in the year 2014.

I developed this C++ game as an exercise to reinforce my understanding of key game development concepts: **Quadtree implementation** for **optimized physics**, **AI systems** and **tile-based pathfinding**.  

The core gameplay revolves around a dynamic galaxy warzone where randomly generated stars influence gameplay through their gravitational pull on spaceships and projectiles. The game begins with five ships (one player-controlled, four AI) and offers endless play with respawning ships.  

I designed the ship controller system with future network multiplayer in mind by mapping the controls to interface commands to the ship.  
Projectile deflection is based on the mass of the star and projectile's mass.
Projectiles are derivable to allow for diverse behaviors. I had plans for additional types.

For Physics, I used a quadtree as its a 2d project. The collision detection are computed for objects that fall in the same leaf node.
