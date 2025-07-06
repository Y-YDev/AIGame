# AIGame
This project is the repository for the AI Game course, where we had to implement an AI on a slightly modified Awalé game.

Our AI won every match during our AI Game course   
[Find the result here](https://github.com/Y-YDev/AIGame/blob/main/Score.xlsx)

![image](https://github.com/user-attachments/assets/623abb48-780b-4888-b9b6-762f1a685832)  
*P1/P2 indicate our player number during the match where score is written like P1 - P2. 'Win - erreur' tell that the opponent had errors*

## How to run
* To play against the AI execute [C_PLAYER_VS_AI.exe](https://github.com/Y-YDev/AIGame/blob/main/C_PLAYER_VS_AI.exe)
* To see match AI vs AI execute [C_AI_VS_AI.exe](https://github.com/Y-YDev/AIGame/blob/main/C_AI_VS_AI.exe)

*We have a java version of the AI but i did not wrote here how to run it. C++ version is the better one*

## Our AI Overview

This section describes the main characteristics and mechanisms of our AI player.

####  Evaluation Function
Our AI uses a **naive evaluation function** based simply on the current score difference between the players. Even when it finds a winning move, it tries to **maximize the point margin**.  
We experimented with more complex evaluation heuristics, but they provided no significant improvement and often led to worse results.

####  Dynamic Depth
- Default search depth: **9**.
- The AI adjusts depth dynamically:
  - If the previous move took **< 450 ms**, depth increases by 1 (or by 2 if **< 10 ms**).
  - If a move takes **~1.3 seconds**, depth decreases by 1 to ensure we stay under the **2-second limit**.
- When the board reduces in size (due to game rules), computation becomes much faster, so the AI can reach **maximum depth (22)** reliably in endgame.

#### Multi-Threading
- Basic parallel approach:
  - One thread per possible initial move (12 threads initially, 6 after board reduction).
  - Each thread runs standard **minimax with alpha-beta pruning**.
  - Threads write their evaluations into a shared array, and the best move is selected by the main thread.

####  Move Opening Strategy
- Empirical tests showed certain openings performed better at deep search.
- For Player 1, opening on position **6** is generally strongest.
- For Player 2, the response is calculated as:
  - If **P % 4 < 2**: play **(P + 9) % 24**
  - If **P % 4 ≥ 2**: play **(P + 19) % 24**

####  Compilation Optimization
- We used compiler optimizations for faster response times.
  ```bash
  g++ -fno-stack-protector -fno-pie -no-pie -fno-pic -O3 -static -Wall *.cpp -o program

For more info [Report of our AI](https://github.com/Y-YDev/AIGame/blob/main/Rapport%20AI%20GAME.docx) 


## Rules of Awalé :

Awalé is a traditional African "sowing and capturing" game, part of the Mancala family.

**Objective :** Capture as many seeds as possible from your opponent's side.

#### Board Setup
- 2 rows of 6 pits each.
- 4 seeds in every pit at the start.

#### On player turn
1. Choose one of your pits on your side.
2. Pick up all seeds from that pit.
3. Sow them one by one into consecutive pits **counter-clockwise** (including the opponent's row).

#### Capture Rule
- If your last seed lands in the opponent’s row, and that pit reaches **2 or 3 seeds after your drop**, you capture them.
- You may continue capturing backward in the opponent's row if those pits also hold 2 or 3 seeds.
- No capture occurs if the last seed lands in your own row or the opponent’s pit has another number of seeds.

#### End of Game
- When a player cannot move (all their pits are empty), the game ends.
- Players count their captured seeds.
- The player with the most seeds wins.

#### Ethical Rule
- You must always leave the opponent with a legal move. You cannot starve them intentionally.

## Variant: Awalé Long with Teacher Rule

- 24 pits (cells) at start, each with 4 seeds.
- Sowing is cyclic over all pits (wrap-around).
- Capture when last seed lands in opponent’s pit with 2 or 3 seeds (classic Awalé rule).
- Starvation rule enforced: opponent must always have a move.
- Teacher Rule: when total seeds ≤ 48, board shrinks to 12 pits by merging pairs.

This is a modern extended variant, often used in AI competitions and educational settings to teach strategic reduction and mid-game adaptation.

## Screenshots
![image](https://github.com/user-attachments/assets/de6b1700-d589-479f-8982-7cb15442c6b7)

