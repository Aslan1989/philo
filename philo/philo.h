/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:22:48 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/11 15:30:37 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}				t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t	*forks;
	t_philo			*ph;
	int				finished_philos;
	pthread_mutex_t	state_lock;
}				t_data;

long long	get_time(void);
void		ft_usleep(long long time);
void		print_action(t_philo *philo, char *action);

// init.c
int			init_all(t_data *data, int argc, char **argv);

// routine.c
void		*routine(void *arg);

// monitor.c
void		*monitor(void *arg);

#endif
