/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:12:46 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 11:12:20 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*death_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->data->meal_check);
		if ((get_time() - philo->last_meal) > philo->data->time_to_die)
		{
			sem_wait(philo->data->write_lock);
			printf("%lld %d died\n", get_time() - philo->data->start_time,
				philo->id);
			sem_post(philo->data->write_lock);
			exit(1);
		}
		sem_post(philo->data->meal_check);
		usleep(1000);
	}
	return (NULL);
}

static void	take_forks_and_eat(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->data->meal_check);
	philo->last_meal = get_time();
	print_action(philo, "is eating");
	sem_post(philo->data->meal_check);
	ft_usleep(philo->data->time_to_eat);
	philo->meals_eaten++;
	if (philo->data->meals_required != -1 && \
		philo->meals_eaten >= philo->data->meals_required)
		sem_post(&philo->data->all_ate);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

static void	sleep_and_think(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	print_action(philo, "is thinking");
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t	death_thread;

	if (pthread_create(&death_thread, NULL, death_monitor, philo) != 0)
		exit(1);
	pthread_detach(death_thread);
	while (1)
	{
		take_forks_and_eat(philo);
		sleep_and_think(philo);
	}
}

int	create_philosophers(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid < 0)
			return (1);
		if (data->philos[i].pid == 0)
			philosopher_routine(&data->philos[i]);
		i++;
	}
	return (0);
}
