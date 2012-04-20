#include "lwes_event_type_db.h"
#include <getopt.h>

static const char help[] =
  "lwes-esf-validator [options]"                        "\n"
  ""                                                    "\n"
  "  where options are: "                               "\n"
  ""                                                    "\n"
  "    -f [one argument]"                               "\n"
  "       the ESF file location"                        "\n"
  ""                                                    "\n"
  "  returns 0 if the ESF file validates, 1 otherwise." "\n";

int main (int argc, char *argv[]) {

    char *filename = NULL;

    while (1) {
        char c = getopt (argc, argv, "f:h");

        if (c == -1) {
            break;
        }

        switch (c) {
            case 'f':
                filename = optarg;
                break;

            case 'h':
                fprintf (stderr, "%s", help);
                return 1;

            default:
                fprintf (stderr,
                        "error: unrecognized command line option -%c\n",+
                        optopt);

                return 1;
        }
    }

    if (filename == NULL) {
        printf("No filename specified!\n");
        return -1;
    }

    struct lwes_event_type_db *db = lwes_event_type_db_create(filename);
    if (db == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}
