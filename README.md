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

Components used:

-Arduino Uno Board

-Joystick

-8x8 LED Matrix

-LCD Display

-MAX7219

-Buzzer

-1 Resistor

-2 Capacitors

-1 Mini-Breadboard

-1 Mid-Breadboard

-Looots of connecting wires



</details>