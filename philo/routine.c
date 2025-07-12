/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:22:29 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 14:19:43 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Philosopher picks up both forks (mutex lock).
 *
 * @param philo Pointer to the philosopher.
 *
 * Details:
 * - Locks the two forks in address order (left < right or vice versa).
 * - Prevents deadlocks by always locking in the same order.
 * - After locking each fork, prints the corresponding action.
 */
static void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	pthread_mutex_lock(first);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_action(philo, "has taken a fork");
}

/**
 * @brief Simulates the eating action.
 *
 * @param philo Pointer to the philosopher.
 *
 * Steps:
 * 1. Print "is eating".
 * 2. Update last_meal timestamp (under lock).
 * 3. Sleep for eating time (simulate time to eat).
 * 4. Increment meals_eaten.
 * 5. If meals_eaten == meals_required → increment finished_philos.
 *
 * All shared variables (last_meal, meals_eaten, finished_philos)
 * are accessed under `state_lock`.
 */
static void	eat(t_philo *philo)
{
	t_data	*d;

	d = philo->data;
	print_action(philo, "is eating");
	pthread_mutex_lock(&d->state_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&d->state_lock);
	ft_usleep(d->time_to_eat);
	pthread_mutex_lock(&d->state_lock);
	philo->meals_eaten++;
	if (d->meals_required != -1 && philo->meals_eaten == d->meals_required)
	{
		d->finished_philos++;
	}
	pthread_mutex_unlock(&d->state_lock);
}

/**
 * @brief Releases both forks after eating.
 *
 * @param philo Pointer to philosopher.
 *
 * Simply unlocks both fork mutexes.
 */
static void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

/**
 * @brief Simulates sleeping and thinking phases.
 *
 * @param philo Pointer to philosopher.
 *
 * 1. Print "is sleeping", then sleep.
 * 2. Print "is thinking".
 * No mutex needed, because no shared state is modified.
 */
static void	sleep_and_think(t_philo *philo)
{
	t_data	*d;

	d = philo->data;
	print_action(philo, "is sleeping");
	ft_usleep(d->time_to_sleep);
	print_action(philo, "is thinking");
}

/**
 * @brief The function executed by each philosopher thread.
 *
 * @param arg A pointer to a `t_philo` structure (individual philosopher).
 * @return Always returns NULL.
 *
 * Step-by-step:
 * 1. Delay even-numbered philosophers slightly to prevent deadlock (optional).
 * 2. Loop infinitely, unless `someone_died` becomes true.
 * 3. In each loop:
 *    - Take both forks (mutex lock).
 *    - Eat (update last_meal and meals_eaten).
 *    - Put forks down (mutex unlock).
 *    - Sleep and then think.
 *
 * Note:
 * - The `someone_died` variable is checked under mutex to avoid race conditions.
 */
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (!philo->data->someone_died)
	{
		pthread_mutex_lock(&philo->data->state_lock);
		if (philo->data->someone_died)
			break ;
		pthread_mutex_unlock(&philo->data->state_lock);
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
