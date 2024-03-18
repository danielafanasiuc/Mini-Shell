// SPDX-License-Identifier: BSD-3-Clause

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmd.h"
#include "utils.h"

#define READ		0
#define WRITE		1

static bool check_redirect(simple_command_t *command)
{
	word_t *in = command->in;
	char *file_in = get_word(in);

	if (in) {
		// redirect input
		int in_fd = open(file_in, O_RDONLY, 0777);

		if (in_fd == -1)
			return 1;

		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	word_t *out = command->out;
	char *file_out = get_word(out);

	if (out) {
		// redirect output
		int out_fd;

		if (command->io_flags == IO_OUT_APPEND)
			out_fd = open(file_out, O_WRONLY | O_CREAT | O_APPEND, 0777);
		else
			out_fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0777);

		if (out_fd == -1)
			return 1;

		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	word_t *err = command->err;
	char *file_err = get_word(err);

	if (err) {
		// redirect error
		if (out && strcmp(file_out, file_err) == 0) {
			dup2(STDOUT_FILENO, STDERR_FILENO);
		} else {
			int err_fd;

			if (command->io_flags == IO_ERR_APPEND)
				err_fd = open(file_err, O_WRONLY | O_CREAT | O_APPEND, 0777);
			else
				err_fd = open(file_err, O_WRONLY | O_CREAT | O_TRUNC, 0777);

			if (err_fd == -1)
				return 1;

			dup2(err_fd, STDERR_FILENO);
			close(err_fd);
		}
	}

	return 0;
}

static int shell_run_command(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1) {
			fprintf(stderr, "Execution failed for '%s'\n", params[0]);
			exit(1);
		}

		if (execv(params[0], params) == -1) {
			fprintf(stderr, "Execution failed for '%s'\n", params[0]);
			exit(1);
		}
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_sleep(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("sleep", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_grep(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("grep", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_ls(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("ls", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_tr(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("tr", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_cut(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("cut", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_sort(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("sort", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_uniq(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("uniq", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_wc(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("wc", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_cp(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("cp", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_echo(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		// if error in child, finish executing child
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("echo", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_gcc(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("gcc", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_cat(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("cat", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_whoami(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("whoami", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_uname(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("uname", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_mkdir(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("mkdir", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_rm(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1)
			exit(1);

		if (execvp("rm", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

static int shell_pwd(char **params, simple_command_t *command)
{
	int pid = fork();

	if (pid == -1)
		return 1;

	if (pid == 0) {
		if (check_redirect(command) == 1) {
			fprintf(stderr, "Cannot open redirection file\n");
			exit(1);
		}

		if (execvp("pwd", params) == -1)
			exit(1);
	} else {
		int status;

		if (wait(&status) == -1)
			return 1;

		if (WIFEXITED(status)) {
			const int exit_status = WEXITSTATUS(status);

			return exit_status;
		}
		return 0;
	}
	return 0;
}

/**
 * Internal change-directory command.
 */
static bool shell_cd(simple_command_t *command)
{
	int param_cnt = 0;
	word_t *params = command->params;

	while (params) {
		params = params->next_word;
		param_cnt++;
	}

	if (param_cnt == 0 || param_cnt > 1) {
		fprintf(stderr, "Invalid no of parameters\n");
		return 1;
	}

	char *path = get_word(command->params);

	// duplicate STDIN, STDOUT, STDERR
	int in_fd = dup(STDIN_FILENO);
	int out_fd = dup(STDOUT_FILENO);
	int err_fd = dup(STDERR_FILENO);

	if (check_redirect(command) == 1) {
		fprintf(stderr, "Cannot open redirection file\n");
		return 1;
	}

	int err = 0;

	if (chdir(path) == -1) {
		fprintf(stderr, "Cannot change directory\n");
		err = 1;
	}

	// closing the rerouted stdin/out/err file descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// getting back the streams
	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	dup2(err_fd, STDERR_FILENO);

	// closing the duplicated file descriptors
	close(in_fd);
	close(out_fd);
	close(err_fd);

	return err;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit(void)
{
	return SHELL_EXIT;
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	int err = 0;
	char *command = get_word(s->verb);

	if (strcmp(command, "cd") == 0) {
		err = shell_cd(s);
		return err;
	} else if (strcmp(command, "exit") == 0) {
		return shell_exit();
	} else if (strcmp(command, "quit") == 0) {
		return shell_exit();
	}


	if (strchr(command, '=')) {
		char *name;
		char *rest = command;

		name = strtok_r(rest, "=", &rest);
		char *token = name, *value;


		while (token) {
			value = token;
			token = strtok_r(rest, "=", &rest);
		}
		if (setenv(name, value, 1) == -1)
			return 1;
		return 0;
	}


	int params_size;
	char **params = get_argv(s, &params_size);

	if (strcmp(command, "mkdir") == 0)
		err = shell_mkdir(params, s);
	else if (strcmp(command, "cp") == 0)
		err = shell_cp(params, s);
	else if (strcmp(command, "echo") == 0)
		err = shell_echo(params, s);
	else if (strcmp(command, "gcc") == 0)
		err = shell_gcc(params, s);
	else if (strcmp(command, "cat") == 0)
		err = shell_cat(params, s);
	else if (strcmp(command, "rm") == 0)
		err = shell_rm(params, s);
	else if (strcmp(command, "pwd") == 0)
		err = shell_pwd(params, s);
	else if (strcmp(command, "whoami") == 0)
		err = shell_whoami(params, s);
	else if (strcmp(command, "uname") == 0)
		err = shell_uname(params, s);
	else if (strcmp(command, "true") == 0)
		err = 0;
	else if (strcmp(command, "false") == 0)
		err = 1;
	else if (strcmp(command, "ls") == 0)
		err = shell_ls(params, s);
	else if (strcmp(command, "tr") == 0)
		err = shell_tr(params, s);
	else if (strcmp(command, "cut") == 0)
		err = shell_cut(params, s);
	else if (strcmp(command, "sort") == 0)
		err = shell_sort(params, s);
	else if (strcmp(command, "uniq") == 0)
		err = shell_uniq(params, s);
	else if (strcmp(command, "wc") == 0)
		err = shell_wc(params, s);
	else if (strcmp(command, "grep") == 0)
		err = shell_grep(params, s);
	else if (strcmp(command, "sleep") == 0)
		err = shell_sleep(params, s);
	else
		err = shell_run_command(params, s);
	return err;
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool run_in_parallel(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	int pid1 = fork();

	if (pid1 == -1)
		return 1;

	if (pid1 == 0) {
		int err = parse_command(cmd1, level + 1, father);

		exit(err);
	}

	int pid2 = fork();

	if (pid2 == -1)
		return 1;

	if (pid2 == 0) {
		int err = parse_command(cmd2, level + 1, father);

		exit(err);
	}

	int status1, status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	return 0;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2).
 */
static int run_on_pipe(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	char fifo_name[100];

	sprintf(fifo_name, "fifo%d", level);
	while (mkfifo(fifo_name, 0777) == -1) {
		if (errno != EEXIST) {
			fprintf(stderr, "Could not create fifo\n");
			return 1;
		}
		// if fifo exists, that means & operator forked twice
		// and | operator is used with the same level (so same name)
		// so create new fifo
		strcat(fifo_name, "_");
	}

	int pid1 = fork();

	if (pid1 == -1)
		return 1;

	if (pid1 == 0) {
		int write_fd = open(fifo_name, O_WRONLY, 0777);

		if (write_fd == -1)
			exit(1);

		dup2(write_fd, STDOUT_FILENO);
		close(write_fd);

		int err = parse_command(cmd1, level + 1, father);

		close(STDOUT_FILENO);
		exit(err);
	} else {
		int pid2 = fork();

		if (pid2 == -1)
			return 1;

		if (pid2 == 0) {
			int read_fd = open(fifo_name, O_RDONLY, 0777);

			if (read_fd == -1)
				exit(1);

			dup2(read_fd, STDIN_FILENO);
			close(read_fd);

			int err = parse_command(cmd2, level + 1, father);

			close(STDIN_FILENO);
			exit(err);
		}

		int status1, status2;

		waitpid(pid1, &status1, 0);
		waitpid(pid2, &status2, 0);

		// delete fifo
		unlink(fifo_name);

		if (WIFEXITED(status2)) {
			const int exit_status2 = WEXITSTATUS(status2);

			return exit_status2;
		}
		return 0;
	}
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	if (c->op == OP_NONE) {
		int err = 0;

		err = parse_simple(c->scmd, level + 1, c);

		return err;
	}

	int err1 = 0, err2 = 0;

	switch (c->op) {
	case OP_SEQUENTIAL:
		err1 = 0;
		err2 = 0;
		err1 = parse_command(c->cmd1, level + 1, c);
		if (err1 == SHELL_EXIT)
			return err1;
		err2 = parse_command(c->cmd2, level + 1, c);
		if (err2 == SHELL_EXIT)
			return err2;

		return err1 + err2;

	case OP_PARALLEL:
		err1 =  run_in_parallel(c->cmd1, c->cmd2, level + 1, c);
		return err1;

	case OP_CONDITIONAL_NZERO:
		err1 = 0;
		err2 = 0;
		err1 = parse_command(c->cmd1, level + 1, c);
		if (err1 == SHELL_EXIT)
			return err1;
		if (err1) {
			err2 = parse_command(c->cmd2, level + 1, c);
			if (err2 == SHELL_EXIT)
				return err2;
		}
		return err1 + err2;

	case OP_CONDITIONAL_ZERO:
		err1 = 0;
		err2 = 0;
		err1 = parse_command(c->cmd1, level + 1, c);
		if (err1 == SHELL_EXIT)
			return err1;
		if (!err1) {
			err2 = parse_command(c->cmd2, level + 1, c);
			if (err2 == SHELL_EXIT)
				return err2;
		}
		return err1 + err2;

	case OP_PIPE:
		err1 =  run_on_pipe(c->cmd1, c->cmd2, level + 1, c);
		return err1;

	default:
		return SHELL_EXIT;
	}
}
