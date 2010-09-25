/* shell.c -- implements simple shell prototype.
 *
 * licence: view LICENCE file on project root
 *
 * author: view AUTHORS file on project root
 *
 * changelog: please visit http://github.com/bbcoimbra/proto_shell
 *            or clone repository using command below:
 *            git clone git://github.com/bbcoimbra/proto_shell.git
 *
 */

#include <config.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef HAVE_LIBREADLINE
#  if defined(HAVE_READLINE_READLINE_H)
#    include <readline/readline.h>
#  elif defined(HAVE_READLINE_H)
#    include <readline.h>
#  else /* !defined(HAVE_READLINE_H) */
extern char *readline ();
#  endif /* !defined(HAVE_READLINE_H) */
char *cmdline = NULL;
#else /* !defined(HAVE_READLINE_READLINE_H) */
/* no readline */
#endif /* HAVE_LIBREADLINE */

#ifdef HAVE_READLINE_HISTORY
#  if defined(HAVE_READLINE_HISTORY_H)
#    include <readline/history.h>
#  elif defined(HAVE_HISTORY_H)
#    include <history.h>
#  else /* !defined(HAVE_HISTORY_H) */
extern void add_history ();
extern int write_history ();
extern int read_history ();
#  endif /* defined(HAVE_READLINE_HISTORY_H) */
  /* no history */
#endif /* HAVE_READLINE_HISTORY */


#define PROMPT "proto-shell> "

char** parse (char *);
void free_parse(char **);
void my_execute (char *cmd, char **args, char **env);
int execute_internal_cmd (char *cmd, char **args);
void bye (void);
void author (void);
void help (void);
void version (void);
int guess_word_num(const char *str);

int main (int argc, char **argv, char **env)
{
	char *command_line;

	while ((command_line = readline((char *)PROMPT)) != NULL)
	{
		char *cmd, **args;
		int s;
		args = parse (command_line);
		if(args)
		{
			cmd = (char *) malloc (sizeof(char) * strlen(*args));
			strcpy(cmd, *args);
			if (!s)
				my_execute (cmd, args, env);

			free(cmd);
			free_parse(args);
		}
	}
	bye();
	exit(EXIT_SUCCESS);
}

int guess_word_num(const char *str)
{
	char *aux;
	int count = 0;

	aux = (char *) str;
	while (*aux != '\0')
	{
		if (*aux != ' ')
			count++;
		while (*aux != '\0' && *aux != ' ')
			aux++;
		if (*aux == ' ')
			aux++;
	}
	return count;
}

char **parse(char *command_line)
{
	char *token, *cmdline, **aux, **ret_args;
	int str_count = guess_word_num (command_line);

  ret_args = (char **) malloc (sizeof (char *) * (str_count + 1));
	memset(ret_args, '\0', str_count + 1 );
	cmdline = strdup (command_line);
	aux = ret_args;
	while ((token = strsep (&cmdline, (char*) " ")) != NULL)
	{
		if (strcmp (token, (char *) "") != 0)
		{
			*aux = token;
			aux++;
		}
	}
	*aux = NULL;
	if (*ret_args && strlen (*ret_args) > 0)
		return ret_args;
	return NULL;
}

void free_parse(char **args)
{
		free(*args);
}
void my_execute(char *cmd, char **args, char **env)
{
	pid_t child_pid;
	if (execute_internal_cmd(cmd, args))
		return;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
	}
	if(child_pid != 0) /* Parent */
	{
		int status;
		waitpid(child_pid, &status, 0);
	}
	else /* Child */
	{
		execve(cmd, args, env);
		perror("proto_shell");
		exit(EXIT_FAILURE);
	}
	return;
}

int execute_internal_cmd(char *cmd, char **args)
{
	if (strcmp(cmd, "cd") == 0)
	{
		if(chdir(*(args+1)) == -1)
		{
			perror("cd");
			return 1;
		}
		return 1;
	}
	if (strcmp(cmd, "help") == 0)
	{
		help();
		return 1;
	}
	if (strcmp(cmd, "autor") == 0)
	{
		author();
		return 1;
	}
	if (strcmp(cmd, "version") == 0)
	{
		version();
		return 1;
	}
	if (strcmp(cmd, "sair") == 0)
	{
		bye();
		exit(EXIT_SUCCESS);
	}
	return 0;
}

void author (void)
{
	fprintf(stdout, "\tBruno Coimbra\n");
}
void bye (void)
{
	fprintf(stdout, "bye\n");
}

void version (void)
{
  fprintf (stdout, "%s\n", VERSION);
}

void help (void)
{
	fprintf(stdout, "proto_shell by bbcoimbra -- SO III -- 2s2010\n\n");
	fprintf(stdout, "\tAvaliable commands:\n\n");
	fprintf(stdout, "\tautor\t\tPrints author name on screen\n");
	fprintf(stdout, "\tcd <path>\tChange workdir to <path> \n");
	fprintf(stdout, "\thelp\t\tThis help message\n");
	fprintf(stdout, "\tsair\t\tExits shell\n");
	fprintf(stdout, "\tversion\t\tPrints %s version\n", PACKAGE);
	fprintf(stdout, "\n");
}
