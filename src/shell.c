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


#define PROMPT "proto_shell> "

int parse (char *command_line, char *ret_cmd, char **ret_args);
void my_execute (char *cmd, char **args, char **env);
int execute_internal_cmd (char *cmd, char **args);
void bye (void);
void author (void);

int main (int argc, char **argv, char **env)
{
	char *command_line;

	while ((command_line = readline((char *)PROMPT)) != NULL)
	{
		char *cmd, *args[] = {NULL, NULL};
		int s;
		s = parse (command_line, cmd, args);
		if (!s)
			my_execute (cmd, args, env);
	}
	bye();
	exit(EXIT_SUCCESS);
}

int parse(char *command_line, char *ret_cmd, char **ret_args)
{
	char *token, *cmdline;
	cmdline = strdup(command_line);
	token = strsep(&cmdline, (char*)" ");
	while ((token != NULL) && strcmp(token, "") == 0)
		token = strsep(&cmdline, (char*)" ");
	if (token != NULL)
	{
		strcpy (ret_cmd, token);
		ret_args[0] = malloc(sizeof(char)*strlen(ret_cmd));
		strcpy(ret_args[0], ret_cmd);
		return 0;
	}
	return 1;
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
		fputs(cmd, stderr);
		execve(cmd, args, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	return;
}

int execute_internal_cmd(char *cmd, char **args)
{
	if (strcmp(cmd, "sair") == 0)
	{
		bye();
		exit(EXIT_SUCCESS);
	}
	if (strcmp(cmd, "autor") == 0)
	{
		author();
		return 1;
	}
	return 0;
}

void author (void)
{
	printf("\tBruno Coimbra\n");
}
void bye (void)
{
	puts("bye");
}