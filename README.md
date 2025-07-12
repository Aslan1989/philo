# philo
    My project is an implementation of the classic Dining Philosophers problem using POSIX threads and mutexes in C.
    The goal is to simulate a set of philosophers sitting around a table, where each philosopher alternates between thinking, eating, and sleeping, while sharing forks (represented as mutexes) in a way that avoids deadlocks and race conditions.

🔧 Initialization

At the start, I:

    Parse the command-line arguments (number of philosophers, time to die, time to eat, time to sleep, and optionally number of meals),

    Allocate memory for forks and philosophers,

    Initialize all mutexes (one per fork and one global state lock),

    Assign forks and IDs to each philosopher.

🧵 Philosopher Threads

Each philosopher runs in its own thread, executing the following loop:

    Take forks (mutex locking in a specific order to avoid deadlock),

    Eat (update meal count and last meal time),

    Put down forks (unlock mutexes),

    Sleep and think (using custom sleep function for precision).

🕵️ Monitor Thread

I also launch a separate monitor thread, which constantly:

    Checks if any philosopher has not eaten within the allowed time (and declares them dead),

    Checks if all philosophers have finished the required number of meals (if specified),

    Signals the simulation to stop by setting a global someone_died flag.

⏱️ Utility Functions

The project includes utility functions:

    To get the current timestamp in milliseconds,

    To simulate accurate sleeping,

    To print philosopher actions in a thread-safe way (with mutex protection).

💡 Safety & Synchronization

All shared data is protected with mutexes to prevent race conditions.
Forks are always picked in a consistent order to prevent deadlocks.
The program also handles edge cases like only one philosopher.
