# Rock-Paper-Scissors: Static vs Learning Markov Agent

A C simulation that opposes two Rock-Paper-Scissors agents against each other: a **static** player that follows a fixed Markov transition matrix, and a **learning** player that adapts its own transition matrix over time using a simple reward-based update rule. After 5000 rounds, the results are written to a file and plotted with Python/Matplotlib to visualize whether the learning agent improves against the static one. 

## How it works

Both agents choose their next move based on 3x3 transition matrix (`Map2D`) that gives the probability of playing Paper/Rock/Scissors conditioned on previous move:
- **Static player** - always uses a fixed transition matrix and it is never changed. 
- **Learning player** - starts from a copy of the same matrix, but after very round it adjusts the probability of the move it just made based on the reward it received (`+1` win, `0` tie, `-1` loss), then renormalizes the row so probabilities still sum up to 1. Its transitions are keyed on the *opponent's* last move rather than its own, so it is effectively trying to learn a counter-strategy. 

## Project structure

```
.
|- main.c           # Simulation: custom hash-map data structures, Markov agents, game loop
|- plot.py          # Reads results.txt and plots cumulative scores with Matplotlib
|- results.txt      # Generated at runtime (round-by-round scores)
|- basic-c.cbp      # Code::Blocks project file
|- basic-c.depend   # Code::Blocks dependency cache
|- basic-c.layout   # Code::Blocks editor layout
|- .gitignore
```

## Requirements

- A C compiler (Clange/GCC)
- Python 3 with `numpy` and `matplotlib` (only for the plotting step)

## Usage

Compile and run directly with GCC:

```bash
gcc main.c -o main -lm
./main
```
If only plotting is needed without rerunning the simulation:

```bash
python3 plot.py
```

## Example output

The plot shows two lines - cumulative score for the static player and the learning player - over 5000 rounds. If the learning update is working correctly, the learning player's cumulative score should trend upward relative to the static player's as it adapts to the static player's fixed pattern. 

## Notes

- The learning rate (`0.9`) and reward scheme are hardcoded in `learning_player()`
- Memory for the transition matrices is manually managed and freed at the end of `main()`.
- Number of simulated rounds is hardcoded to 5000 in `main()`.
