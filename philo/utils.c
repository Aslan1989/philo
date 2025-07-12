/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:22:15 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 09:52:42 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
 "This code gets the current time with microsecond precision using gettimeofday.
 It then converts this time to milliseconds by multiplying seconds by 1000 and
 dividing microseconds by 1000. We use milliseconds because the simulation
 times like 'time to eat' and 'time to die' are given in milliseconds,
 so it’s easier to compare and work with this unit.
*/
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

/**
 * @brief Custom usleep function that sleeps for a given time in milliseconds.
 *
 * @param ms Number of milliseconds to sleep.
 *
 * @details
 * Uses a loop with short usleep calls to achieve more accurate sleeping,
 * preventing oversleeping caused by standard usleep inaccuracies.
 */
void	ft_usleep(long long time)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(100);
}

/**
 * @brief Safely prints an action to stdout.
 *
 * @param philo The philosopher performing the action.
 * @param action A string like "is eating", "has taken a fork", etc.
 *
 * Uses state_lock to avoid overlapping prints and
 * ensure no message is printed after death.
 */
void	print_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->data->state_lock);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->state_lock);
		return ;
	}
	printf("%lld %d %s\n", get_time() - philo->data->start_time,
		philo->id, action);
	pthread_mutex_unlock(&philo->data->state_lock);
}
