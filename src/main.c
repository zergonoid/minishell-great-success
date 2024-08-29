/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:01:56 by skioridi          #+#    #+#             */
/*   Updated: 2024/08/29 14:15:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

t_msh    *initsh(t_msh *msh)
{
    msh->line = (char *)malloc(sizeof(char));
    msh->exit = 0;
    msh->ret = 0;
    msh->lst_head = (t_token **)malloc(sizeof(t_token *));
    *(msh->lst_head) = NULL;
    return (msh);
}

void    freesh(t_msh *msh)
{
    if (msh->line)
    {
        free(msh->line);
    }
    if (msh->lst_head)
    {
        ft_tknclear(msh->lst_head);
    }
    return ;
}

void    handleline(t_msh *msh)
{
    if (msh->line)
    {
        free(msh->line);
        msh->line = (char *)NULL;
    }
    msh->line = readline("minishell$");
    if (msh->line && *msh->line)
        add_history(msh->line);
    if (msh->line && !ft_strncmp(msh->line, "exit", ft_strlen(msh->line)))
        msh->exit = 1;
    else if (msh->line && (!ft_strncmp(msh->line, "-1", 2)))
        msh->exit = 1;
    else if (!msh->line)
        msh->exit = 1;
    else
        lexer(msh->line, msh->lst_head);
    return ;
}

void    c_handler()
{
    ft_printf("\n"); // Move to a new line
    rl_on_new_line(); // Regenerate the prompt on a newline
    rl_replace_line("", 0); // Clear the previous text
    rl_redisplay();
    return ;
}

int main(int ac, char **av, char **envp)
{
    if ((ac != 1) || !envp[0] || !envp)
        ft_printf("Error: Exiting.\n");
    (void)av;
    (void)envp;
    t_msh msh;
    initsh(&msh);
    signal(2, c_handler); //ctrl-C SIGINT
    signal(3, SIG_IGN); //ctrl-\ SIGQUIT

    while (msh.exit == 0)
    {
        handleline(&msh);
    }
    freesh(&msh);
    return (msh.ret);
}