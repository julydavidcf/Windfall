# Milestone 4 game

Windfall is a turn-based RPG, where the player can choose to use different abilities during the battle. There is a tutorial at the beginning to teach the player how to play, and there are dialogues throughout the game to describe the background and lore of the game. After the tutorial, the player has to command his companions and fight through 3 levels to defeat the necromancer boss, who has 2 phases. The player can save the game and come back to it later. Between the battle levels, there are two free-roam levels where the player can move around and explore the environment. There are chests in these levels for them to grab stat boosts to aid them in their later battles. The game has a full beginning, story, and conclusion. There is also a custom mode called "Makeup Mode", where it becomes an arena-style game and the player can choose the characters they used and also the enemies they fight.


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

# Features to meet Milestone 3 goals (105 points total)

* Gameplay II (10): Added two more levels, added a tutorial, added 3 more enemy types and the necromancer boss for the final level. Added introduction and dialogue between levels to describe the story.
* Level loading (10): Added JSON files that can be loaded by the game as different levels, using a JSON loader.
* Save and reload (10): Added feature to save and load the game, so that the player can save the current state of the game. The player can close the game and still be able to re-load.
* External integration (10): Used nlohmann library to process JSON files.
* Mouse gestures (10): Integrated 3 types of mouse gesture abilities, the player can use them by holding down right-click and drawing the correct path. The correct path for the three abilities are straight-vertical, straight-horizontal, and circular.
* Advanced geometry shader (10): Added a shield ability that would deform upon collision with a projectile, added background objects that can also be deformed by projectiles.
* Tutorial (10): Added a textbox-based tutorial that introduces the on-screen objects and teaches the player how to use the abilities.
* Story (10): Added an introduction and dialogues in-between levels to inform the player of the background and lore.
* Graphics assets (5): Added many new sprites for abilities and enemy characters.
* Lag mitigation (5): The game does not lag.
* Crash free (5): The game does not crash.
* Memory management (5): Memory usage is fairly low and dynamically-allocated memory are released properly.
* Bug and feature fixes (5): Fixed numerous bugs such as the camera trail and the debugging boxes. Moreover removed the magenta border.

# Features to meet Milestone 4 goals (100 points total)

* Gameplay III (10): Added two free-roam levels where the player can explore and find rewards. Added an archer character to the battle system who is also the protagonist of the story. Added more dialogue between levels and a conclusion for the player to read.
* Simple path finding (10): Added an enemy-seeking arrow that calculates the shortest path using BFS to the enemy and hits them.
* Swarm behaviour (10): Added a swarm of fireflies in the second free-roam level that flies around as a group. It can be separated by the player shooting arrows, but will immediately start to re-group and travel as one swarm again.
* Complex prescribed motion (10): Added a flying dragon in the first free-roam level that flies in the sky. It has a non-linear motion path calculated using interpolation.
* Precise collision (10): Added a precise collision interaction between the player's arrows and the boulders in the first free-roam level, using meshes and a polygon collision detection method.
* Level Editor (10): Added a "Makeup Mode" that allows the player to edit the level by choosing the companions and enemies that exist in the level. They can choose up to a 4v4 battle with a variety of companions and enemies.
* Light (10): Added a light-up effect to the player's arrows that would illuminate the environment in the second free-roam level (Which is made to be dark using a fragment shader), the light increases its radius if it comes in contact with the firefly swarm.
* Game balance (5): Did an in-depth analysis on the health numbers of all characters, to determine how much health each character should have to give the player an optimal experience. In this case, the optimal experience implies that the game can last enough turns for them to try out all the abilities, but not too long that it becomes very boring. See the Balancing.pdf for details on the conducted analysis.

* Graphics assets (5): Added many new sprites for abilities and enemy characters.
* Lag mitigation (5): The game does not lag.
* Crash free (5): The game does not crash.
* Memory management (5): Memory usage is fairly low and dynamically-allocated memory are released properly.
* Bug and feature fixes (5): Fixed numerous bugs from the last milestone and new ones that appeared during the milestone 4 development to ensure there are no major bugs or crashes.

# Audio References

1. https://freesound.org/people/tonsil5/sounds/416839/
2. https://mixkit.co/free-sound-effects/explosion/
3. https://freesound.org/people/tonsil5/sounds/416838/
4. Background music: Darkest Dungeon OST
5. https://freesound.org/people/theuncertainman/sounds/402645/
6. https://freesound.org/people/wobesound/sounds/488382/
7. https://freesound.org/people/InspectorJ/sounds/416179/
8. https://freesound.org/people/alonsotm/sounds/396500/
9. https://freesound.org/people/18hiltc/sounds/186048/
10. https://freesound.org/people/MATRIXXX_/sounds/403297/
11. https://freesound.org/people/Breviceps/sounds/453391/
12. https://freesound.org/people/plasterbrain/sounds/423169/
13. https://freesound.org/people/SilverIllusionist/sounds/580814/
14. https://freesound.org/people/humanoide9000/sounds/329029/
15. https://freesound.org/people/Aleks41/sounds/406063/
16. https://downloads.khinsider.com/game-soundtracks/album/octopath-traveler-original-soundtrack-2018
17. https://downloads.khinsider.com/game-soundtracks/album/darkest-dungeon-ost
18. https://freesound.org/people/vixuxx/sounds/9874/

# Image references

1. https://idlechampions.fandom.com/wiki/Magical_Barrier
2. https://pimen.itch.io/fire-spell
3. https://hajileee.itch.io/hajileees-fantasy-characters-pack-magic-set
4. https://unsplash.com/photos/j5kjDQFiEnM
5. https://www.coolgenerator.com/png-text-generator
6. https://free-game-assets.itch.io/free-cartoon-forest-2d-backgrounds
7. https://oco.itch.io/medieval-fantasy-character-pack-2
8. https://www.pngwing.com/en/free-png-zxihk
9. https://www.pikpng.com/pngvi/JhRJw_anime-angry-face-png-anime-angry-vein-png/
10. https://dribbble.com/shots/4325775-Shard-of-ice
11. https://www.istockphoto.com/illustrations/fantasy-battle
12. https://www.artstation.com/artwork/Vd5444
13. https://sanctumpixel.itch.io/angel-of-death-pixel-art-character
14. https://samuellee.itch.io/reaper-animated-pixel-art
15. https://www.pngwing.com/en/free-png-bpuix/download
16. https://seliel-the-shaper.itch.io/treasure-chests
17. https://arludus.itch.io/2d-pixel-art-medieval-backgrounds-pack
18. https://ansimuz.itch.io/sideview-fantasy-patreon-collectio
