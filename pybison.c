#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"  // Make sure this path is correct based on your project setup

// Define a structure to hold the command and its arguments
typedef struct {
    char* command_name;    // To store the command name
    char** arguments;      // To store arguments as an array of strings
    int arg_count;         // To keep track of the number of arguments
} SimpleCommandData;

typedef struct {
    int flags;
    int line;
    char* op; // Operator as a string
} CondCommandData;

typedef struct {
    int flags;
    int line;
    char* name; // Function name as a string
} FunctionDefData;

typedef struct {
    int ignore;
} GroupCommandData;

typedef struct {
    int flags;
    int line;
} SubshellCommandData;

typedef struct {
    int flags;
    char* name; // Coprocess name as a string
} CoprocCommandData;

SimpleCommandData* extract_simple_command(SIMPLE_COM* simple_cmd) {
    if (simple_cmd == NULL) {
        return NULL;
    }

    SimpleCommandData* result = malloc(sizeof(SimpleCommandData));
    if (!result) {
        perror("Failed to allocate SimpleCommandData");
        return NULL;
    }

    WORD_LIST* wl = simple_cmd->words;
    result->arg_count = 0;
    result->arguments = NULL;

    if (wl) {
        result->command_name = strdup(wl->word->word); // Duplicate the first word as command name
        wl = wl->next; // Move to the first argument

        // Count the number of arguments
        for (WORD_LIST* arg = wl; arg != NULL; arg = arg->next) {
            result->arg_count++;
        }

        if (result->arg_count > 0) {
            // Allocate memory for arguments array
            result->arguments = malloc(sizeof(char*) * result->arg_count);
            if (!result->arguments) {
                perror("Failed to allocate arguments array");
                free(result->command_name);
                free(result);
                return NULL;
            }

            // Copy arguments into the array
            int i = 0;
            for (WORD_LIST* arg = wl; arg != NULL; arg = arg->next, i++) {
                result->arguments[i] = strdup(arg->word->word);
            }
        }
    }

    return result;
}

CondCommandData* extract_cond_com(COND_COM* cond_cmd) {
    if (cond_cmd == NULL) {
        return NULL;
    }

    CondCommandData* result = malloc(sizeof(CondCommandData));
    if (!result) {
        perror("Failed to allocate CondCommandData");
        return NULL;
    }

    result->flags = cond_cmd->flags;
    result->line = cond_cmd->line;
    result->op = cond_cmd->op ? strdup(cond_cmd->op->word) : NULL;

    return result;
}

FunctionDefData* extract_function_def(FUNCTION_DEF* func_def) {
    if (func_def == NULL) {
        return NULL;
    }

    FunctionDefData* result = malloc(sizeof(FunctionDefData));
    if (!result) {
        perror("Failed to allocate FunctionDefData");
        return NULL;
    }

    result->flags = func_def->flags;
    result->line = func_def->line;
    result->name = func_def->name ? strdup(func_def->name->word) : NULL;

    return result;
}

GroupCommandData* extract_group_com(GROUP_COM* group_cmd) {
    if (group_cmd == NULL) {
        return NULL;
    }

    GroupCommandData* result = malloc(sizeof(GroupCommandData));
    if (!result) {
        perror("Failed to allocate GroupCommandData");
        return NULL;
    }

    result->ignore = group_cmd->ignore;

    return result;
}

SubshellCommandData* extract_subshell_com(SUBSHELL_COM* subshell_cmd) {
    if (subshell_cmd == NULL) {
        return NULL;
    }

    SubshellCommandData* result = malloc(sizeof(SubshellCommandData));
    if (!result) {
        perror("Failed to allocate SubshellCommandData");
        return NULL;
    }

    result->flags = subshell_cmd->flags;
    result->line = subshell_cmd->line;

    return result;
}

CoprocCommandData* extract_coproc_com(COPROC_COM* coproc_cmd) {
    if (coproc_cmd == NULL) {
        return NULL;
    }

    CoprocCommandData* result = malloc(sizeof(CoprocCommandData));
    if (!result) {
        perror("Failed to allocate CoprocCommandData");
        return NULL;
    }

    result->flags = coproc_cmd->flags;
    result->name = coproc_cmd->name ? strdup(coproc_cmd->name) : NULL;

    return result;
}
