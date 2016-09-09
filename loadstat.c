#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const char progname[] = "loadstat";
static const char version[] = "0.1.0";
static const char usage[] =
	"Usage: %s [-crahv]\n"
	"  -c  show number of cores\n"
	"  -r  show raw load average\n"
	"  -a  show load average adjusted to number of cores\n"
	"  -h  show this help\n"
	"  -v  show version\n";

static bool show(bool show_cores, bool show_raw, bool show_adj)
{
	int cores;
	double loads[3];

	cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (show_cores)
		printf("%d cores\n", cores);

	if (getloadavg(loads, 3) != 3)
		return false;

	if (show_raw)
		printf("%.2f %.2f %.2f raw\n", loads[0], loads[1], loads[2]);
	for (int i = 0; i < 3; i++)
		loads[i] = loads[i] / cores;
	if (show_adj)
		printf("%.2f %.2f %.2f adj\n", loads[0], loads[1], loads[2]);

	return true;
}

int main(int argc, char **argv)
{
	int opt;
	bool show_cores = false, show_raw = false, show_adj = false;

	while ((opt = getopt(argc, argv, "crahv")) != -1) {
		switch (opt) {
		case 'h':
			printf(usage, argv[0]);
			return 0;
		case 'v':
			printf("%s: version %s\n", progname, version);
			return 0;
		case 'c':
			show_cores = true;
			break;
		case 'r':
			show_raw = true;
			break;
		case 'a':
			show_adj = true;
			break;
		default:
			fprintf(stderr, usage, argv[0]);
			return 2;
		}
	}

	if (!show_cores && !show_raw && !show_adj)
		show_adj = true;

	return show(show_cores, show_raw, show_adj);
}
