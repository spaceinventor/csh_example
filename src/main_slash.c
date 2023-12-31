
#include <stdio.h>
#include <stdlib.h>
#include <csp/csp.h>
#include <slash/slash.h>
#include <slash/optparse.h>
#include <slash/dflopt.h>

void libinfo(void) {

	printf("This APM is an example of how to add slash and param functionality\n");
	printf("It also shows how to use Space Inventor's param implementation\n");
}

static int aping_cmd(struct slash *slash)
{

	unsigned int node = 16383;
	unsigned int timeout = 1000;
	unsigned int size = 0;

    optparse_t * parser = optparse_new("aping", "[node]");
    optparse_add_help(parser);
    optparse_add_unsigned(parser, 'n', "node", "NUM", 0, &node, "node (default = 16383, broadcast to all)");
    optparse_add_unsigned(parser, 't', "timeout", "NUM", 0, &timeout, "timeout [ms] (default = 1000)");
	optparse_add_unsigned(parser, 's', "size", "NUM", 0, &size, "size (default = 0)");

    int argi = optparse_parse(parser, slash->argc - 1, (const char **) slash->argv + 1);
    if (argi < 0) {
        optparse_del(parser);
	    return SLASH_EINVAL;
    }

	if (++argi < slash->argc) {
		node = atoi(slash->argv[argi]);
	}

	slash_printf(slash, "Ping node %u size %u timeout %u: ", node, size, timeout);

	int result = csp_ping(node, timeout, size, CSP_O_CRC32);

	if (result >= 0) {
		slash_printf(slash, "Reply in %d [ms]\n", result);
	} else {
		slash_printf(slash, "No reply\n");
	}

	return SLASH_SUCCESS;
}
slash_command(aping, aping_cmd, "", "aping APM test");
