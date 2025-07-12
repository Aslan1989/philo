/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:08:46 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 11:06:50 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	data->all_ate = 0;
	data->start_time = get_time();
}

int	init_semaphores(t_data *data)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_DEAD);
	data->forks = sem_open(SEM_FORKS, O_CREAT, 0644, data->num_philos);
	data->write_lock = sem_open(SEM_WRITE, O_CREAT, 0644, 1);
	data->meal_check = sem_open(SEM_MEAL, O_CREAT, 0644, 1);
	data->dead_check = sem_open(SEM_DEAD, O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->write_lock == SEM_FAILED
		|| data->meal_check == SEM_FAILED || data->dead_check == SEM_FAILED)
		return (1);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = get_time();
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

int	init_all(t_data *data, int argc, char **argv)
{
	parse_args(data, argc, argv);
	if (init_semaphores(data))
		return (1);
	if (init_philosophers(data))
		return (1);
	return (0);
}

void	start_meals_monitor(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		sem_wait(&data->all_ate);
		i++;
	}
	sem_wait(data->write_lock);
	printf("All philosophers have eaten %d times\n", data->meals_required);
	sem_post(data->write_lock);
	i = 0;
	while (i < data->num_philos)
		kill(data->philos[i++].pid, SIGTERM);
}
