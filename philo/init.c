/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:22:00 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 12:58:45 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Parses input arguments and sets initial values in the data struct.
 *
 * @param data Pointer to shared data.
 * @param argc Number of input arguments.
 * @param argv Array of argument strings.
 *
 * Fields explained:
 * - num_philos: Total number of philosophers.
 * - time_to_die: Time (ms) a philosopher can survive without eating.
 * - time_to_eat: Time (ms) it takes to eat.
 * - time_to_sleep: Time (ms) to sleep after eating.
 * - meals_required: Optional number of meals each philosopher must eat.
 * - someone_died: 0 by default; used to signal end of simulation.
 * - start_time: Initial timestamp to calculate simulation timing.
 * - finished_philos: Counter for how many philosophers finished eating.
 */
static void	parse_args(t_data *data, int argc, char **argv)
{
	data->num_philos = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->meals_required = atoi(argv[5]);
	else
		data->meals_required = -1;
	data->someone_died = 0;
	data->start_time = get_time();
	data->finished_philos = 0;
}

/**
 * @brief Initializes mutexes for forks, logging, and state protection.
 *
 * @param data Pointer to shared data.
 * @return int Returns 0 on success, 1 on memory allocation failure.
 *
 * Explanation:
 * - forks: Each fork is protected by its own mutex.
 * - write_lock: Ensures that only one thread writes to stdout at a time.
 * - meals_lock / state_lock: Protect shared variables
 * (like meals_eaten, last_meal, someone_died).
 */
static int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->ph = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->ph)
		return (1);
	pthread_mutex_init(&data->state_lock, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (0);
}

/**
 * @brief Initializes each philosopher's structure and assigns forks.
 *
 * @param data Pointer to shared data.
 *
 * Explanation:
 * - id: Philosopher's number (1-based).
 * - meals_eaten: Number of meals eaten so far.
 * - last_meal: Timestamp of last meal.
 * - left_fork / right_fork: Assign forks using modular arithmetic.
 * - data: Each philosopher has a pointer to the global data.
 */
static void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->ph[i].id = i + 1;
		data->ph[i].meals_eaten = 0;
		data->ph[i].last_meal = get_time();
		data->ph[i].left_fork = &data->forks[i];
		data->ph[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->ph[i].data = data;
		i++;
	}
}

/**
 * @brief Initializes all required data and structures.
 *
 * @param data Pointer to the shared data structure.
 * @param argc Argument count from main().
 * @param argv Argument values from main().
 * @return int Returns 0 on success, 1 on failure.
 *
 * Steps:
 * 1. Parse command-line arguments and fill the data struct.
 * 2. Initialize all mutexes (forks, write, state).
 * 3. Initialize all philosopher structures and assign forks.
 */
int	init_all(t_data *data, int argc, char **argv)
{
	parse_args(data, argc, argv);
	if (init_mutexes(data))
		return (1);
	init_philos(data);
	return (0);
}
