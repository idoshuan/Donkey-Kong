
Donkey Kong Game Submission

Student IDs:
- 211671722
- 211762570

Bonus Features Implemented:
1. File Selection Navigation
We added the ability to navigate through file selection pages. Users can easily select a file even if more than 9 are available.

2. Hammer
   - I implemented a dynamic hammer visualization that moves alongside the character in real-time, enhancing gameplay immersion and providing immediate feedback.
   - A hammer animation was introduced to appear every time Mario attacks, adding excitement and visual feedback to the gameplay.

3. Screen and Score Animations
Screen transitions now include smooth animations, and every time Mario earns points, an animated score is displayed above his character for better engagement.

Game Logic Notes:
1. Mario Falls Horizontally While Barrels Fall Straight Down:
   - When Mario falls, his movement includes horizontal motion, whereas barrels fall directly downward, creating distinct physics for each character.

2. Intentional Vertical Fall When Pressing Down:
   - If the player presses "down" while Mario falls, he transitions to a vertical fall. This behavior is intentionally designed to give players additional control during gameplay.

3. Stop Message Borders Positioned Below Characters:
   - The "Stop" message during pauses is intentionally printed below the characters, ensuring it does not obstruct game visuals or interfere with gameplay clarity.
  
4. Invalid File Handling:
   - We implemented a robust file validation system that checks for any inconsistencies or errors in the game board file. If an invalid file is detected, the program behaves like a compiler, providing detailed feedback to the user.
   - The system identifies specific issues, such as missing characters, improperly formatted lines, or mismatched dimensions, and prints clear instructions on how to correct the file. This ensures that the user can quickly resolve errors and reload the file.


