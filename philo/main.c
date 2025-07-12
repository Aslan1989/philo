/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:21:50 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 12:59:15 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->state_lock);
	free(data->forks);
	free(data->ph);
}

int	handle_edge_cases(t_data *data)
{
	if (data->num_philos == 0)
		return (1);
	if (data->num_philos == 1)
	{
		printf("0 1 has taken a fork\n");
		ft_usleep(data->time_to_die);
		printf("%d 1 died\n", data->time_to_die);
		return (1);
	}
	return (0);
}

/**
 * @brief The main function — entry point of the philosopher simulation.
 *
 * @param argc Number of command-line arguments. Must be 5 or 6.
 * @param argv Argument values:
 *        argv[1] = number_of_philosophers
 *        argv[2] = time_to_die
 *        argv[3] = time_to_eat
 *        argv[4] = time_to_sleep
 *        argv[5] = (optional) number_of_times_each_philosopher_must_eat
 *
 * @return int Returns 0 on success, 1 on error.
 *
 * Step-by-step explanation:
 * 1. We check if the number of arguments is valid.
 * If not, we print usage and exit.
 * 2. We initialize all data structures (mutexes, philosophers, args).
 * 3. For each philosopher, we create a thread that runs the `routine` function.
 * 4. We also create a monitor thread
 * that constantly checks for death or eating status.
 * 5. We wait (`join`) for the monitor thread to finish.
 * 6. Then we wait for each philosopher to finish.
 * 7. We clean up all allocated memory and destroy mutexes.
 */
int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	mon;
	int			i;

	if (argc != 5 && argc != 6)
		return (printf("Usage: ./philo 5 800 200 200 [5]\n"), 1);
	if (init_all(&data, argc, argv))
		return (printf("Init error\n"), 1);
	if (handle_edge_cases(&data))
		return (0);
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_create(&data.ph[i].thread, NULL, routine, &data.ph[i]) != 0)
			return (printf("Error creating philosopher thread\n"), 1);
		i++;
	}
	if (pthread_create(&mon, NULL, monitor, &data) != 0)
		return (printf("Error creating monitor thread\n"), 1);
	pthread_join(mon, NULL);
	i = 0;
	while (i < data.num_philos)
		pthread_join(data.ph[i++].thread, NULL);
	cleanup(&data);
	return (0);
}
