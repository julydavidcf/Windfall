# Milestone 2 game

The game is now a turn-based RPG, where the player can choose to use different abilities during the battle. To complete the game, the player has to go through a 2v1 followed by a 2v2 battle scenario. The enemy has AI and will react based on what the player chooses to do.


# Features to meet Milestone 1 goals (100 points total)

* Team organization (10): We highlighted the things to implement for M1 and split work in this Google Doc: https://docs.google.com/document/d/1QWKuOjX_pdWck4bl-a7f72ohOxoafkprBA8ZyGuS5p8/edit?usp=sharing
* Random/coded action (5): The enemy gets knocked back when hit by fireball
* Textured geometry (5): Companion and enemy are both textured meshes
* Observer pattern (10): WorldSystem acts as Subject and notifies observers (RenderSystem, PhysicsSystem) when a mouse click is detected
* Keyboard/mouse control (10): Pressing the icon to enable firing a fireball in the mouse direction
* Basic collision detection (10): Fireball collides with enemy and wall
* Collision Resolution (10): A moving fireball can collide with a moving wall, and the fireball will bounce off the wall
* Graphic assets (5): Companion and enemy meshes created from PNG sprites, fireball PNG sprite, wall PNG sprite, particle PNG sprite
* Lag mitigation (5): The game does not lag
* Crash free (5): The game has no crashes
* Delay-agnostic design (10): The game is turn-based and the player has as much time as needed to make their move. Currently, clicking the fireball button will allow them to aim a fireball, which will not be affected by delay since the enemy does not move while the player is aiming
* Adaptive resolution (5): The game and its assets will display properly on all reasonable resolutions
* Particle Systems (10): The enemy will explode into hundreds of particles when HP reaches 0


# Features to meet Milestone 2 goals (100 points total)

* Gameplay I (10): Added the swordsman character, added a few abilities, including Melee Attack, Taunt, Heal, Ice Shard, Rock. There are two levels for the player to get through, and they need to strategize based on the enemy AI.
* Sprite animation (10): There are idle, death, and casting ability animations for both of the characters.
* Graphics assets (5): Added swordsman sprite with spritesheet and mage's spritesheet. Added sprites for all the projectiles and ability effects.
* Help (5): Added tooltips for all the abilities that show up after hovering over the ability icon.
* Debugging Graphics (5): The user can press the "D" key to see precise collision boxes for all characters.
* Advanced fragment shader (10): Added balls of light on the top of the screen with shifting colours, added fog effect.
* State machine (10): Added states that correspond to different states in the animation system.
* Behaviour tree (10): Used a behaviour tree to represent the enemy's AI. They will respond based on the player's choice of ability usage.
* Audio feedback (5): Added background music and various sound effects.
* Camera Control (5): Added camera tracking for flying projectiles.
* Parallax scrolling background (10): The background will scroll when the camera is tracking flying projectiles.
* Lag mitigation (5): There is no noticeable lag.
* Crash free (5): The game does not crash.
* Bug and feature fixes (5): Fixed numerous bugs.


# Audio References

1. https://freesound.org/people/tonsil5/sounds/416839/
2. https://mixkit.co/free-sound-effects/explosion/
3. https://freesound.org/people/tonsil5/sounds/416838/
4. https://freesound.org/people/theuncertainman/sounds/402645/

# Image references

1. https://idlechampions.fandom.com/wiki/Magical_Barrier
2. https://pimen.itch.io/fire-spell
3. https://hajileee.itch.io/hajileees-fantasy-characters-pack-magic-set
4. https://unsplash.com/photos/j5kjDQFiEnM
5. https://www.coolgenerator.com/png-text-generator
6. https://free-game-assets.itch.io/free-cartoon-forest-2d-backgrounds
7. https://oco.itch.io/medieval-fantasy-character-pack-2
