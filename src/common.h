#ifndef COMMON_H
#define COMMON_H

#define MAX_COMPONENTS 10
#define MAX_NODES 10

extern int debug_enabled; // Add this line

typedef enum { RESISTOR, VOLTAGE_SRC, CURRENT_SRC } ComponentType;

typedef struct {
    ComponentType type;
    int node1, node2;
    double value;
} Component;

extern Component components[MAX_COMPONENTS];
extern int num_components;
extern int num_nodes;
extern double fixed_voltage[MAX_NODES + 1];
extern int variable_indices[MAX_NODES + 1];
extern int num_vars;

// Error codes
#define ERROR_FILE_NOT_FOUND 1
#define ERROR_INVALID_COMPONENT 2
#define ERROR_INVALID_NODE 3
#define ERROR_ZERO_RESISTANCE 4
#define ERROR_SINGULAR_MATRIX 5
#define ERROR_FLOATING_NODE 6

#endif