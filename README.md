
Donkey Kong Game Submission

Student IDs:
- 211671722
- 211762570

Bonus Features Implemented:
1. Game Resume Countdown:
   - A feature that pauses the game and, upon resuming, displays a countdown from 3 to 1 before gameplay continues.

2. Barrel Movement Based on Real-Time:
   - The barrels' behavior and movements are synchronized with real-time rather than being tied to iterations, creating a more dynamic gameplay experience.

3. Points System with Real-Time Animation:
   - Points are rewarded in real time and are dynamically animated above Mario whenever he performs a scoring action (e.g., killing an enemy or wining stage). This adds a visually engaging element to the gameplay and provides immediate feedback to the player.

4. Menu Screen Animation:
   - The menu screen includes an animation where the text slides in from the left side of the screen, resembling a curtain being drawn. This feature was added to enhance the visual presentation and provide a smoother user experience.


Game Logic Notes:
1. Mario Falls Horizontally While Barrels Fall Straight Down:
   - When Mario falls, his movement includes horizontal motion, whereas barrels fall directly downward, creating distinct physics for each character.

2. Intentional Vertical Fall When Pressing Down:
   - If the player presses "down" while Mario is falling, he transitions to a vertical fall. This behavior is intentionally designed to give players additional control during gameplay.

3. Stop Message Borders Positioned Below Characters:
   - The "Stop" message during pauses is intentionally printed below the characters, ensuring it does not obstruct game visuals or interfere with gameplay clarity.
  
4. Invalid File Handling:
   -We implemented a robust file validation system that checks for any inconsistencies or errors in the game board file. If an invalid file is detected, the program behaves like a compiler, providing detailed feedback to the user.
   -The system identifies specific issues, such as missing characters, improperly formatted lines, or mismatched dimensions, and prints clear instructions on how to correct the file. This ensures that the user can quickly resolve errors and reload the file.


