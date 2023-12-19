# 💣MatrixProject-BombMania💣

This game it's a simpler version of the Bomberman game. This project represents a mini-game created during the Introduction to Robotics course, using Arduino. You will find more of my journey through this course [HERE](https://github.com/lemnaruamedeea/IntroductionToRobotics).

<details>
<summary><h2>Game Rules 🎮</h2></summary>

The game is very simple and intuitive to play: the objective is to explode 💥 all the walls from all the rooms in the shortest time possible. Each room represents a level and the number of walls increases with each level passed. In order to destroy the walls, you have to place bombs 💣 near them to explode them. Be carefull tho, if you are too close to the bomb, you will lose a life 😵.

</details>

<details>
<summary><h2>Main Menu 📑</h2></summary>
Here's the menu I created for my game. You can scroll on the LCD with the joystick through functionalities:

1. **Intro Message**: I programmed a greeting message that shows up briefly when the game powers up.

2. **Main Menu**:
   (a) **Start Game**: This option initiates the initial level of my game.
   (b) **Highscore**: Initially set to 0, it updates after the game ends. I saved the top 3+ scores with names in EEPROM.
   (c) **Settings**: I included:
       - **Enter Name**: Players can input their names if they achieve new highscoreand it will be displayed in the highscore.
       - **LCD Brightness Control**: Adjust LCD brightness and save it in EEPROM.
       - **Matrix Brightness Control**: Display something on the matrix while adjusting the matrix brightness, and save settings in EEPROM.
       - **Sounds On/Off**: Toggle for sound, saves in EEPROM.
       - **Reset Highscore**: Option to reset the highscore.
   (d) **About**: I added details about myself as the creator of the game: game name, author, and GitHub user.
   (e) **How to Play**: I included a short, informative description for players.

3. **During Gameplay**: Displayed relevant info: lives and time.

4. **Upon Game Ending**:
   (a) **Screen 1**: Shows a message: if you win: "You Won! Score: x", else "Game Over!". Switches to Screen 2 after a few moments.
   (b) **Screen 2**: Notified players if they beat the highscore and if they do, they have to introduce a 3 letter name. Otherwise, the message: "Press joystick to go to main menu" will be shown and wait for the player's command.

   </details>

<details>
<summary><h2>Setup 🔌🛠️</h2></summary>

### Components used:

   -Arduino Uno Board
   -Joystick
   -8x8 LED Matrix
   -LCD Display
   -MAX7219
   -Buzzer
   -2 Resistors
   -2 Capacitors
   -1 Mini-Breadboard
   -1 Mid-Breadboard
   -Looots of connecting wires

![DriverScheme](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/DriverScheme.png?raw=true)
![JoystickConnectionsTable](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/JoystickConnection.png?raw=true)
![LCDScheme](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/LCDScheme.png?raw=true)
![MatrixConnectionsTable](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixConnectionsTable.png?raw=true)

![SetupImg1](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/SetupImg2.jpeg?raw=true)

</details>

<details>
<summary><h2>Code Details 💻</h2></summary>

### [Code](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/BombMania.ino). 💻🖱️

### Libraries 🗂️
  - LiquidCrystal.h (for LCD control)
  - LedControl.h (for matrix control)
  - EEPROM.h (for memory)

### EEPROM Memory addresses 💾
![EEPROMAddresses](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/EEPROMAddresses.jpeg?raw=true)

### Matrix Animation ✨
   **Main Menu**
<p float="left">
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/MainMenu.png?raw=true" alt="Main Menu" width="100" />
</p>

   **Main Menu Options**
<p float="left">
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Info.png?raw=true" alt="Info" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Settings.png?raw=true" alt="Settings" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/HighScore.png?raw=true" alt="Highscore" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/About.png?raw=true" alt="About" width="100" />
</p>

   **Settings Options**
<p float="left">
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/MatrixLight.png?raw=true" alt="Matrix Light" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Sound.png?raw=true" alt="Sound" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/ResetHS.png?raw=true" alt="ResetHS" width="100" />
</p>

   **Rooms**
<p float="left">
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Room1.png?raw=true" alt="Room 1" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Room2.png?raw=true" alt="Room 2" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Room3.png?raw=true" alt="Room 3" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Room4.png?raw=true" alt="Room 4" width="100" />
</p>

   **Win/Lose**
<p float="left">
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Win1.png?raw=true" alt="Win1" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Win2.png?raw=true" alt="Win2" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/MatrixAnimations/Lose.png?raw=true" alt="Lose" width="100" />
</p>

### LCD Simbols 🈁
<p float="left">
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/LCDSimbols/Empty.png?raw=true" alt="Empty" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/LCDSimbols/Full.png?raw=true" alt="Full" width="100" />
  <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/LCDSimbols/Heart.png?raw=true" alt="Arrows" width="100" />
   <img src="https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/LCDSimbols/UpDownArrows.png?raw=true" alt="Image 4" width="100" />
</p>

</details>

## Demo video on [YouTube](https://youtube.com/watch?v=4m5PInlXqao&si=Bl820Xt7EAQ1age-).📹

![Game1](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/Game1.jpeg?raw=true)
![Game2](https://github.com/lemnaruamedeea/MatrixProject-BombMania/blob/main/BombMania/Game2.jpeg?raw=true)
