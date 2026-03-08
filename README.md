# Chess Engine with Minimax AI
### Minimax Algorithm + Alpha-Beta Pruning | C++

A fully functional chess game with a built-in AI opponent. The engine uses the Minimax algorithm to evaluate positions and determine the best possible move, simulating how a chess player thinks several moves ahead.

---

## How It Works

**Board Representation**
The chessboard is represented as an 8x8 data structure with full rule implementation — legal move generation, check/checkmate detection, castling, en passant, and pawn promotion.

**Minimax Algorithm**
The AI uses Minimax to explore the game tree recursively — simulating all possible moves for both players up to a defined depth, then selecting the move that maximizes its advantage while minimizing the opponent's.

**Alpha-Beta Pruning**
To reduce computation time, Alpha-Beta Pruning is applied to cut off branches of the game tree that cannot influence the final decision — significantly increasing search depth without extra cost.

**Position Evaluation**
Each board state is scored using a heuristic evaluation function based on:
- Material count (piece values)
- Piece positioning (center control, king safety)
- Mobility (number of available moves)

---

## Tech Stack

- **Language:** C++
- **Algorithm:** Minimax with Alpha-Beta Pruning
- **Data Structures:** Custom board representation, move generation trees

---

## Key Concepts Demonstrated

- Recursive tree search and game theory
- Alpha-Beta pruning for performance optimization
- Heuristic evaluation function design
- Object-oriented design in C++
- Complex rule implementation from scratch

---

## Features

- Full chess rule enforcement (legal moves only)
- Adjustable AI difficulty via search depth
- Human vs AI gameplay
- Check, checkmate, and stalemate detection