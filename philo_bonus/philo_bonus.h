/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:06:55 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/12 11:11:53 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>

# define SEM_FORKS "/philo_forks"
# define SEM_WRITE "/philo_write"
# define SEM_MEAL "/philo_meal"
# define SEM_DEAD "/philo_dead"

// Forward declaration
struct	s_data;

typedef struct s_philo
{
	pid_t			pid;
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		monitor_thread;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int			num_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			meals_required;
	long long	start_time;
	int			all_ate;

	sem_t		*forks;
	sem_t		*write_lock;
	sem_t		*meal_check;
	sem_t		*dead_check;

	t_philo		*philos;
}	t_data;

long long	get_time(void);
void		ft_usleep(long long time);
void		print_action(t_philo *philo, char *action);

int			init_all(t_data *data, int argc, char **argv);
void		cleanup_resources(t_data *data);
void		start_meals_monitor(t_data *data);

void		wait_for_termination(t_data *data);
int			create_philosophers(t_data *data);

#endif
