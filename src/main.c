#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "parse.h"
#include "mna.h"
#include "solver.h"

int debug_enabled = 0; // Global debug flag (0=off)

int main(int argc, char *argv[]) {
    bool quiet_mode = false;
    const char *netlist_file = NULL;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quiet") == 0) {
            quiet_mode = true;
            debug_enabled = 0; // Disable debug output
        } else if (strcmp(argv[i], "--debug") == 0) {
            debug_enabled = 1; // Enable debug output
        } else {
            netlist_file = argv[i];
        }
    }

    if (!netlist_file) {
        if (!quiet_mode) fprintf(stderr, "Usage: %s <netlist_file> [--quiet]\n", argv[0]);
        return ERROR_FILE_NOT_FOUND;
    }

    parse_netlist(netlist_file);

    // Allocate matrix and vector
    double **G = (double **)malloc(num_vars * sizeof(double *));
    double *b = (double *)malloc(num_vars * sizeof(double));
    for (int i = 0; i < num_vars; i++) {
        G[i] = (double *)calloc(num_vars, sizeof(double));
    }

    build_MNA(G, b);
    solve(G, b, num_vars);

    // ALWAYS print node voltages (even in quiet mode)
    printf("\nNode Voltages:\n");
    for (int i = 1; i <= num_nodes; i++) {
        if (fixed_voltage[i] != 0) {
            printf("V%d = %.3f V (fixed)\n", i, fixed_voltage[i]);
        } else if (variable_indices[i] != -1) {
            printf("V%d = %.3f V\n", i, b[variable_indices[i]]);
        }
    }

    // Cleanup
    for (int i = 0; i < num_vars; i++) free(G[i]);
    free(G);
    free(b);

    return 0;
}