# Milestone 1 game

There is a companion on the left and an enemy on the right. The player can click the fireball button on the bottom once to enable fireball-launching. They can then click again to fire the fireball in the direction of the cursor. There is a turn indicator to tell whether it is the player's turn or the enemy's turn. For now, the enemy will remain idle in their turn. When the player fires a fireball, the enemy has a random chance to spawn a wall that will reflect any fireballs that collide with it. The enemy will die and explode once its HP reaches 0.


# Features to meet Milestone 1 goals

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

# Audio References

1. https://freesound.org/people/tonsil5/sounds/416839/
2. https://mixkit.co/free-sound-effects/explosion/

# Image references

1. https://idlechampions.fandom.com/wiki/Magical_Barrier
2. https://pimen.itch.io/fire-spell
3. https://hajileee.itch.io/hajileees-fantasy-characters-pack-magic-set
4. https://unsplash.com/photos/j5kjDQFiEnM
5. https://www.coolgenerator.com/png-text-generator
