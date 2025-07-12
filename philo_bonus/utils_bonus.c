/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:18:18 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 11:08:34 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(100);
}

void	print_action(t_philo *philo, char *action)
{
	sem_wait(philo->data->write_lock);
	printf("%lld %d %s\n", get_time() - philo->data->start_time,
		philo->id, action);
	sem_post(philo->data->write_lock);
}

void	cleanup_resources(t_data *data)
{
	sem_close(data->forks);
	sem_close(data->write_lock);
	sem_close(data->meal_check);
	sem_close(data->dead_check);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_DEAD);
	free(data->philos);
}

void	wait_for_termination(t_data *data)
{
	int	i;
	int	status;

	waitpid(-1, &status, 0);
	i = 0;
	while (i < data->num_philos)
		kill(data->philos[i++].pid, SIGTERM);
}
