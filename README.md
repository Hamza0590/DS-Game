# The Unseen Journey Quest

## Project Description
**The Unseen Journey Quest** is a single-player grid-based puzzle game where the player must find a key and then locate the door, all within a limited number of moves. The game includes a hint system that helps players by indicating whether they are moving closer or farther away from the key or door. To add more complexity, bombs and coins are scattered on the grid, varying with the difficulty level. Coins regenerate every 10 seconds, and their collection impacts the player's score and movement options.

## Technologies Used
- **Language**: C++
- **Main Library**: PDCurses

## Installation Steps
1. **Download the repository**: Clone or download the project folder from GitHub.
2. **PDCurses setup**:
   - Download and install the PDCurses library from [Google](https://pdcurses.sourceforge.io/).
   - Add the path of the installed PDCurses library to your Visual Studio environment.
3. **Open the solution file**: Run the `.sln` file using Visual Studio (Community Edition or higher).
4. **Build and run**: Compile the project and start the game.

## Game Controls
- **Movement**: Use the following keys to move your player on the grid:
  - **UP**: Move up
  - **DOWN**: Move down
  - **LEFT**: Move left
  - **RIGHT**: Move right
- **Undo move**: Press **U** to undo the last move (with certain conditions; see gameplay).
- **Movement constraint**: After moving in one direction, you cannot immediately move back (e.g., if you move up, you can’t move down next). You must change directions first.

## Gameplay Features
- **Hint System**: A hint displayed at the top of the grid indicates if you are moving closer or farther from the key or door.
- **Bombs**: Stepping on a bomb ends the game. 
- **Coins**: Coins increase your score and provide additional undo opportunities.
- **Timed Coin Generation**: Coins regenerate every 10 seconds on the grid, adding a dynamic challenge to the gameplay.

### Difficulty Levels
1. **Easy**:
   - Grid Size: 10x10
   - Additional Moves: 6
   - Undo Movements: 4
2. **Medium**:
   - Grid Size: 15x15
   - Additional Moves: 2
   - Undo Movements: 1
3. **Hard**:
   - Grid Size: 20x20
   - No additional moves, no undo.

### Scoring System
- **Coins**: Each coin collected adds +2 to the score.
- **Unused Moves**: Any unused moves when the player reaches the door add +1 to the final score.
- **Undo**: Collecting a coin also increases your undo movements by 1.

## Known Issues
There are no known issues or bugs at this time.

## Future Updates
No planned updates for now.

## Credits
- Project developed by **Muhammad Hamza**.
- Special thanks to my TA for collaboration.
![Game Screenshot 1](https://github.com/Hamza0590/DS-Game/blob/main/DS2/Screenshot%202024-10-20%20230531.png?raw=true)

![Game Screenshot 2](https://github.com/Hamza0590/DS-Game/blob/main/DS2/Screenshot%202024-10-20%20231140.png?raw=true)


