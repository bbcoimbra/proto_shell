#include <config.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
void my_execute (char *cmd, char **args);
int execute_internal_cmd (char *cmd, char **args);
void bye (void);
void author (void);

int main (int argc, char **argv, char **env)
{
	char *command_line;

	while ((command_line = readline((char *)PROMPT)) != NULL)
	{
		char *cmd, *args;
		int s;
		s = parse (command_line, cmd, &args);
		if (s)
			fputs ("Couldn't parse command\n", stderr);
		else
			my_execute (cmd, &args);
	}
	bye();
	exit(EXIT_SUCCESS);
}

int parse(char *command_line, char *ret_cmd, char **ret_args)
{
	char *token, *cmdline;
	cmdline = strdup(command_line);
	token = strsep(&cmdline, (char*)" ");
	strcpy (ret_cmd, token);
	ret_args = NULL;
	return 0;
}

void my_execute(char *cmd, char **args)
{
	// pid_t child;
	if (!execute_internal_cmd(cmd, args))
		fputs("Not internal command\n", stderr);
	return;
}

int execute_internal_cmd(char *cmd, char **args)
{
	if(strcmp(cmd, "sair") == 0)
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