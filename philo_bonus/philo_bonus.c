/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:05:29 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 11:10:59 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	start_simulation(t_data *data)
{
	if (create_philosophers(data))
		return (1);
	if (data->meals_required != -1)
		start_meals_monitor(data);
	wait_for_termination(data);
	return (0);
}

/**
 * Entry point for the bonus version of the philosopher program.
 * Here we:
 * - Parse and validate arguments
 * - Initialize semaphores and structures
 * - Spawn philosopher processes
 * - Monitor termination conditions (death or all ate)
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo_bonus number_of_philosophers \
			time_to_die time_to_eat time_to_sleep \
			[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (init_all(&data, argc, argv))
	{
		printf("Initialization error\n");
		return (1);
	}
	if (start_simulation(&data))
	{
		printf("Simulation error\n");
		cleanup_resources(&data);
		return (1);
	}
	cleanup_resources(&data);
	return (0);
}
