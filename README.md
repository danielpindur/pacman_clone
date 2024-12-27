# pacman_clone  
Definitely not PACMAN  
Daniel Pindur, 2020  
Project for the course *Introduction to Programming*  

---

## 1. Compilation  

The project is built using **CMake**. It requires the following libraries:  
- SDL2  
- SDL2_IMAGE  
- SDL2_TTF  
- SDL2_MIXER  
- Math library  

---

## 2. Program Functionality  

When the program starts, it:  
1. Loads the high score from a file (or creates one if it doesn't exist).  
2. Loads the map layout from the `map_data.bit` file.  
3. Launches the **Main Menu**.  

### Main Menu  

In the Main Menu, you can navigate using the `U` and `S` keys (or `Up` and `Down` arrow keys).  
- Hover over **Play** and press `ENTER` to start the game.  
- Navigate to the configuration options, where you can adjust settings using `A` and `D` keys (or `Right` and `Left` arrows):  

#### Configuration Options:  
- **Difficulty:**  
  - Choose between `Easy` and `Hard`.  
  - On `Easy`, ghosts move once every two frames, making the player twice as fast as the ghosts.  
  - On `Hard`, ghosts move at the same speed as the player.  
- **Number of Ghosts:**  
  - Select the number of enemies (minimum: 1, maximum: 4).  
- **Music:**  
  - Toggle the background music on or off.  

---

### Gameplay  

The player starts with:  
- **3 lives**  
- **0 points**  

- **Objective:**  
  Collect coins scattered throughout the map. Each coin adds **1 point** to the player's score.  

- **Gameplay Mechanics:**  
  - At the beginning, the player and ghosts remain stationary until the player makes the first move.  
  - Once the player moves, the ghosts activate and begin chasing the player using the **A* pathfinding algorithm**.  
  - The player can use **teleporters** located on the left and right edges of the map to instantly appear on the opposite side.  
  - The game ends when the player loses all 3 lives, i.e., gets caught by ghosts 3 times.  

- **Ghost Catching Mechanics:**  
  - When a ghost catches the player:  
    - The player loses one life.  
    - All positions on the map are reset.  
  - If the player collects all the coins on the map:  
    - All positions on the map are reset.  
    - The player does not lose a life.  

---

### High Score  

After the player loses all lives:  
- The program checks if the new score is higher than the current high score.  
- If it is, the high score is updated both in the file and on the screen.  

From the **Game Over** screen, pressing `ENTER` allows the player to return to the Main Menu, adjust settings, and restart the game.  

---

## 3. Memory Leaks  

After program termination, the address sanitizer reports a **memory leak of 17 bytes**. This appears to originate from within the **SDL library**.  

---  
