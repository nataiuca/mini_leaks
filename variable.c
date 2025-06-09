/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:32:10 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/26 22:32:47 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quote_state(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '\"'))
	{
		*quote = c;
		return (1);
	}
	if (*quote && c == *quote)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}

static char	*expand_named_var(char **s, t_minishell *minishell)
{
	int		len;
	char	*key;
	char	*val;
	char	*dup;

	len = 0;
	while ((*s)[len] && (ft_isalnum((*s)[len]) || (*s)[len] == '_'))
		len++;
	key = ft_substr(*s, 0, len);
	val = getenv_minishell(minishell, key);
	dup = strdup_safe(val);
	free(key);
	*s += len;
	return (dup);
}

static char *expand_chunk(char **s, char quote, t_minishell *minishell)
{
    char    *var;
    char    tmp[2];

    // Si encontramos una barra invertida, manejamos el escape
    if (**s == '\\')
    {
        // Si la barra invertida está seguida de un espacio, simplemente la eliminamos
        if (*(*s + 1) == ' ')
        {
            (*s)++;  // Saltamos el \ y el espacio
            return (strdup_safe(" "));  // Retornamos el espacio en lugar de la barra invertida
        }
        // Si la barra invertida está seguida de un tabulador, también la eliminamos
        else if (*(*s + 1) == '\t')
        {
            (*s)++;  // Saltamos el \ y el tabulador
            return (strdup_safe("\t"));  // Retornamos el tabulador
        }
        // Si la barra invertida está seguida de un carácter especial, lo dejamos como está
        else if (*(*s + 1) == '$' || *(*s + 1) == '"' || *(*s + 1) == '\'' || *(*s + 1) == '\\')
        {
            tmp[0] = **s;
            tmp[1] = '\0';
            (*s)++;  // Avanzamos el puntero
            return (strdup_safe(tmp));  // Retornamos el \ seguido del siguiente carácter
        }
        else
        {
            // Si la barra invertida está seguida de un carácter normal (no especial),
            // no hace falta escapar y simplemente la mantenemos
            tmp[0] = **s;
            tmp[1] = '\0';
            (*s)++;
            return (strdup_safe(tmp));
        }
    }

    // Expansión de variables si no estamos dentro de comillas simples
    if (**s == '$' && quote != '\'')
    {
        if (*s + 1 == NULL || !ft_isalnum(*(*s + 1)) && *(*s + 1) != '_' && *(*s + 1) != '?')
        {
            (*s)++;
            return (strdup_safe("$"));
        }
        (*s)++;
        if (**s == '?')
        {
            var = ft_itoa(minishell->status);  // Expande el $? a el status de la shell
            (*s)++;
        }
        else if (**s == '_' || ft_isalpha(**s))
            var = expand_named_var(s, minishell);  // Expande una variable con nombre
        else
            return (NULL);
    }
    else
    {
        tmp[0] = **s;
        tmp[1] = '\0';
        (*s)++;
        var = strdup_safe(tmp);  // Copia el carácter sin cambios
    }

    return (var);
}


static char	*strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

char	*expand_variables(char *arg, t_minishell *minishell)
{
	char	*s;
	char	*result;
	char	*chunk;
	char	quote;

	s = arg;
	result = strdup_safe("");
	quote = 0;
	while (*s)
	{
		if (handle_quote_state(*s, &quote))
		{
			s++;
			continue ;
		}
		chunk = expand_chunk(&s, quote, minishell);
		if (!chunk)
			continue ;
		result = strjoin_free(result, chunk);
	}
	return (result);
}
