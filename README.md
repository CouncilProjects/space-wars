# Space Wars

**Space Wars** is a classic arcade-style space shooter game where you dodge enemy bullets, face off against periodic boss ships, and fight for the highest score! The game was developed in 2024 using **C** and **SDL2**.

## How to Play

- **Move**: Arrow keys or `WASD` keys
- **Fire**: `Space` key
- **Pause**: `Esc`  key
- **Objective**: Dodge the green bullets and be cautious of the periodic boss ships that come into play.

Sneak peaks
![image](https://github.com/user-attachments/assets/38199c66-1a59-4a8b-b53e-5e0bd540187b)
![image](https://github.com/user-attachments/assets/dd48f766-0005-4e5f-990b-9abed3e1ed79)
![image](https://github.com/user-attachments/assets/cce4a3f9-330d-4e5d-84ed-bedf4e5b19ff)
![image](https://github.com/user-attachments/assets/7e3ee030-aa78-4ee8-9e98-e7491ef7d559)




## Repository Structure

This repository contains the following folders:

- **`source`**: Contains all the necessary parts to compile the game yourself. This includes the C source code and all dependencies needed to create self-contained versions of the game for both Linux and Windows (so the end user does not need to have the SDL2 libraries pre-installed).

  Existing in Releases
- **`windows-release`**: Contains the pre-compiled version of the game for Windows, along with all required assets.
  
- **`linux-release`**: Contains the pre-compiled version of the game for Linux, along with all required assets.

## Quick Start

If you just want to start playing immediately:

1. **Windows Users**: Download windows-release.zip, extract it, and read the README.txt
2. **Linux Users**: Download linux-release.zip, extract it, and read the README.txt

## Building the Game

If you prefer to build the game from source:

1. Clone the repository:
   ```bash
   git clone https://github.com/CouncilProjects/space-wars.git
   cd space-wars/source
   And read the READTHIS.txt in there
