# WorldOfAnother
 Sandbox game based on random generation

# Initial idea
I'm a big fan of RimWorld, Kenshi, and all kinds of sandbox colony sims where you can recruit, control, and develop characters and build infinite stories.
So, I wanted to create a sandbox game in which the world is procedurally generated, from regions to empires, magic schools, religions...
So that every time the player starts a new game, they have something to discover and build upon.

# Core concepts
## Entity system
A complex relation system, everything from a kingdom to a simple individual would be an Entity.
Entities have relations with each other, and it works as a cascade system (eg. if Individual A hates Empire B, then it will affect its relation with any Individual related to Empire B).

## Harnessing randomness
For every world to be unique, we need a random generation. But it should still be plausible and fun every time. There's nothing more boring than a bland procedural generation.
So the question is: how can we put enough of a framework around the generation of our world to preserve its uniqueness while making sure it stays believable and fun?

## Write your story in the world
Will you be a simple merchant or become the lord of a town? Whatever you choose, the world will not wait for you. It will change around while allowing you to change it too.

# State of the project
- Voronoi-based world generation (regions)
![World generation example](/Resources/WGSC.png)
- Entity system base framework (no relations yet)
