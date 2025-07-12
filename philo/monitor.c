/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:22:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 14:07:48 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Checks if any philosopher has died from starvation.
 *
 * @param d Pointer to shared data.
 * @return 1 if a death occurred, 0 otherwise.
 *
 * For each philosopher, compare current time - last_meal.
 * If it's > time_to_die, set someone_died and print death message.
 */
static int	check_death(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->num_philos)
	{
		pthread_mutex_lock(&d->state_lock);
		if ((get_time() - d->ph[i].last_meal) > d->time_to_die)
		{
			d->someone_died = 1;
			printf("%lld %d died\n", get_time() - d->start_time,
				d->ph[i].id);
			return (1);
		}
		pthread_mutex_unlock(&d->state_lock);
		i++;
	}
	return (0);
}

/**
 * @brief Checks if all philosophers have finished eating.
 *
 * @param d Pointer to shared data.
 * @return 1 if all have eaten enough, 0 otherwise.
 *
 * If `meals_required` is set and everyone finished,
 * end simulation by setting `someone_died`.
 */
static int	check_all_ate(t_data *d)
{
	int	result;

	pthread_mutex_lock(&d->state_lock);
	if (d->meals_required != -1 && d->finished_philos == d->num_philos)
	{
		d->someone_died = 1;
		result = 1;
	}
	else
		result = 0;
	pthread_mutex_unlock(&d->state_lock);
	return (result);
}

/**
 * @brief Monitor thread function that checks philosopher states.
 *
 * @param arg Pointer to shared data.
 * @return Always NULL.
 *
 * In a loop:
 * 1. Check someone_died flag.
 * 2. Call check_death (check starvation).
 * 3. Call check_all_ate (if everyone ate enough).
 * 4. Sleep briefly to avoid busy looping.
 */
void	*monitor(void *arg)
{
	t_data	*d;
	int		died;

	d = (t_data *)arg;
	while (!d->someone_died)
	{
		pthread_mutex_lock(&d->state_lock);
		died = d->someone_died;
		pthread_mutex_unlock(&d->state_lock);
		if (died)
			break ;
		if (check_death(d))
			break ;
		if (check_all_ate(d))
			break ;
		usleep(1000);
	}
	return (NULL);
}
