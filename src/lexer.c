/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msilva-c <msilva-c@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:20:27 by codespace         #+#    #+#             */
/*   Updated: 2024/09/20 18:35:42 by msilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

t_token *tokenize(char *str, int wdlen)
{
    t_token *a;

    char *substring = ft_substr(str, 0, wdlen);

    a = newtoken(substring);
    a->type = findtype(substring); // sort type
    return (a);
}

int add_node(t_token **lst_head, char *line, int i, int end)
{
    t_token *last;

    printf("--- adding node ---\n");
    printf("i = %d, end = %d\n", i, end);
    while (i < end)
    {
        printf("%c", line[i]);
        i++;
    }
    printf("$\nadded node\n");
    return (end);

    t_token *newnode = tokenize(&line[i], end);
    if (!*lst_head)
    {
        *lst_head = newnode;
        return (end);
    }
    last = ft_tknlast(*lst_head);
    last->next = newnode;
    newnode->prev = last;
    ft_tknadd_back(lst_head, newnode);
    return (end);
}

/* return value: nr of chars that later we'll copy
   example:
   cmdline = ola"adeus"ola
   we sent cmdline[4] as parameter, and the ft will stop at cmdline[10]
   ft_strchr will return 6 + 1, which is length of the word to tokenize*/

int strchr_wdlen(const char *s, int c)
{
	int		i;

	i = 1;
	while (s[i])
	{
		if ((unsigned char)c == (unsigned char)s[i])
			return (i + 1);
		i++;
	}
	return (0);
}

void split_cmds(char *line, int i, int space, t_token **lst_head)
{
	int test = i;
    printf("\n--- entered split_cmds ---\n");
	while (test < space)
		printf("%c", line[test++]);
	printf("$\n\n");
	return ;

    int flag = 0;
    int start = i;
    while (line[i] && i < space)
    {
    	if (line[i] == '|' || line[i] == '>' || line[i] == '<')
        {
            if (line[i + 1] && line[i + 1] == line[i])
            {
                add_node(lst_head, line, start, i);
                add_node(lst_head, line, i + 1, 2);
                i += 3;
                flag = 0;
                start = i;
            }
            else
            {
                printf("i is %d\nline[i] = %c\nstart is %d\n", i, line[i], start);
                add_node(lst_head, line, start, i + 1);
                add_node(lst_head, line, i, i + 1);
                i += 2;
                flag = 0;
                start = i;

            }
        }
        else
        {
            flag++;
            i++;
        }

    }
    if (flag)
        add_node(lst_head, line, start, i + 1);
   /*  else if (start != i)
        add_node(lst_head, line, start, i + 1); */
    return ;
}

void new_lexer(char *cmdline, t_token **lst_head)
{
    int i = 0;
    int start;

    while (cmdline[i])
    {
        if (cmdline[i] == 39 || cmdline[i] == 34)
        {
            i += strchr_wdlen(&cmdline[i], cmdline[i]);
            start = i;
        }
        while (ft_isspace(cmdline[i]) && cmdline[i])
            i++;
        start = i;
        while (!ft_isspace(cmdline[i]) && cmdline[i])
            i++;
        split_cmds(cmdline, start, i, lst_head);
        start = i;
    }
    return ;
}


int	quote_handler(char *cmdline, int i, t_token **lst_head)
{
	int		wdlen;

	printf("\n--- entered quote handler ---\n");
	wdlen = strchr_wdlen(&cmdline[i], cmdline[i]);
	printf("wdlen is: %d\n", wdlen);
	if (wdlen)
	{
		int test = i + wdlen;
		while (i < test)
			printf("%c", cmdline[i++]);
		printf("$\n\n");
		//add_node(lst_head, cmdline, i, wdlen);
		return (wdlen);
	}
	printf("skipped quote\n");
	return (1);
}


void	final_lexer(char *cmdline, t_token **lst_head)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	int start;
	while (cmdline[i])
	{
		if (cmdline[i] == 39 || cmdline[i] == 34)
		{
			i += quote_handler(cmdline, i, lst_head);
			printf("i is %d\n", i);
		}
		else if (!ft_isspace(cmdline[i]))
		{
			start = i;
			while (!ft_isspace(cmdline[i]) && cmdline[i])
			{
				if (cmdline[i] == 39 || cmdline[i] == 34)
				{
					if (i > start)
						split_cmds(cmdline, start, i, lst_head);
					i += quote_handler(cmdline, i, lst_head);
					start = i;
				}
				else
					i++;
			}
	        split_cmds(cmdline, start, i, lst_head);
		}
		while (ft_isspace(cmdline[i]) && cmdline[i])
            i++;
	}
	return ;
}
