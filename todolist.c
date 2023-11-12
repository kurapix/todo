#include "todolist.h"

static void user_input_task(ToDo *node, ToDo **head, bool add_task_function)
{
    bool first_time = true;
    do {
        /* Se não for a primeira vez no loop devemos limpar os caracteres
         * que foram inseridos a mais no stdin */
        if (!first_time) {
            int discard;
            while ((discard = getchar()) != '\n' && discard != EOF) {
            }
        }
        printf("\nEscreve a tarefa: ");
        if (fgets(node->task, sizeof(node->task), stdin) == NULL) {
            puts("\nOcorreu um erro");
            if (add_task_function) {
                /* No caso da função add_task também precisamos de dar free ao
                 * node que ainda não está na lista */
                free(node);
            }
            remove_all_tasks(head); // Liberta a memória dinâmica
            exit(EXIT_FAILURE);
        }
        if (first_time) {
            first_time = false;
        }
        /* Se a condição for verdadeira a tarefa é demasiado grande */
    } while (strchr(node->task, '\n') == NULL);
}

static void creat_node(ToDo **node, ToDo **head)
{
    *node = (ToDo *)malloc(sizeof(ToDo));
    if (*node == NULL) {
        puts("\nA alocação de memória falhou");
        remove_all_tasks(head);
        exit(EXIT_FAILURE);
    }
}

void add_task(ToDo **head)
{
    ToDo *aux = *head;

    /* Se a lista estiver vazia */
    if (aux == NULL) {
        creat_node(&aux, head);

        user_input_task(aux, head, true);
        aux->task_number = 1;
        aux->next_task = NULL;
        *head = aux;
        return;
    }
    /* Se a lista já tiver nodes */
    int i = 2; // O counter começa na tarefa número 2

    /* Iteramos a linked list até chegar ao último node */
    while (aux->next_task != NULL) {
        aux = aux->next_task;
        ++i; // No fim do loop o counter fica com o número da nova tarefa
    }

    ToDo *new_task;
    creat_node(&new_task, head);

    user_input_task(new_task, head, true);
    new_task->task_number = i;
    aux->next_task = new_task;
    new_task->next_task = NULL;
}

void print_todo_list(ToDo *head)
{
    if (head == NULL) {
        puts("\nA lista de tarefas está vazia");
        return;
    }

    /* Se a lista não estiver vazia */
    puts(""); // <=> printf("\n")
    while (head != NULL) {
        printf("%d: %s", head->task_number, head->task);
        head = head->next_task;
    }
}

static void adjust_task_number(ToDo *head)
{
    int i = 1;
    while (head != NULL) {
        head->task_number = i;
        ++i;
        head = head->next_task;
    }
}

static int user_input_number(ToDo **head, bool remove_task_function)
{
    bool first_time = true;
    char buffer[10];
    do {
        /* Se não for a primeira vez no loop devemos limpar os restantes
         * caracteres que foram inseridos a mais no stdin */
        if (!first_time) {
            int discard;
            while ((discard = getchar()) != '\n' && discard != EOF) {
            }
        }
        if (remove_task_function) {
            printf("\nEscolhe o número da tarefa que queres remover: ");
        } else {
            printf("\nEscolhe o número da tarefa que queres editar: ");
        }
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            puts("\nOcorreu um erro");
            remove_all_tasks(head); // Liberta a memória dinâmica
            exit(EXIT_FAILURE);
        }
        if (first_time) {
            first_time = false;
        }
        /* Se a condição for verdadeira a string é demasiado grande */
    } while (strchr(buffer, '\n') == NULL);

    /* Conversão de string para long */
    char *endptr;
    long result = strtol(buffer, &endptr, 10);

    /* Usamos o endptr para verificar onde a função strtol parou, se ela não
     * parar em '\n' significa que não chegou até ao fim da string e retornamos
     * 0, que é um número inválido */
    if (*endptr != '\n') {
        return 0;
    }
    /* Se o número não puder ser representado num int retornamos 0 também */
    if (result < INT_MIN || result > INT_MAX) {
        return 0;
    }

    return (int)result;
}

void remove_task(ToDo **head)
{
    if (*head == NULL) {
        puts("\nA lista de tarefas já está vazia");
        return;
    }

    int task_to_remove = user_input_number(head, true);

    /* Se for para remover a primeira tarefa da lista */
    if (task_to_remove == 1) {
        ToDo *aux = *head;
        *head = (*head)->next_task;
        free(aux);
        adjust_task_number(*head);
        return;
    }

    /* Se for para remover uma tarefa que não seja a primeira procuramos pela
     * tarefa que queremos remover  */
    ToDo *aux1 = *head;
    while (aux1->next_task != NULL) {
        if (aux1->next_task->task_number == task_to_remove) {
            ToDo *aux2 = aux1->next_task;
            aux1->next_task = aux2->next_task;
            free(aux2);
            adjust_task_number(*head);
            return;
        }
        aux1 = aux1->next_task;
    }
    /* Se não encontrarmos a tarefa */
    puts("\nO número da tarefa é inválido, nada foi feito");
}

/* Se a lista estiver vazia a função não faz nada */
void remove_all_tasks(ToDo **head)
{
    while (*head != NULL) {
        ToDo *aux = *head;
        *head = (*head)->next_task;
        free(aux);
    }
}

void edit_task(ToDo **head)
{
    if (*head == NULL) {
        puts("\nA lista de tarefas está vazia");
        return;
    }

    int task_to_edit = user_input_number(head, false);

    /* Procuramos pela tarefa que queremos editar */
    ToDo *aux = *head;
    while (aux != NULL) {
        if (task_to_edit == aux->task_number) {
            user_input_task(aux, head, false);
            return;
        }
        aux = aux->next_task;
    }
    /* Se não encontrarmos a tarefa */
    puts("\nO número da tarefa é inválido, nada foi feito");
}

void load_tasks_from_file(ToDo **head)
{
    FILE *save_file = fopen("tasks.txt", "r");
    if (save_file == NULL) {
        puts("\n\
Não tens um save file ou não foi possível aceder às tarefas");
        return;
    }
    ToDo *aux = *head;
    int i = 2;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), save_file) != NULL) {
        /* Se a lista estiver vazia */
        if (aux == NULL) {
            creat_node(&aux, head);
            /* O limite de 198 caracteres certifica que nunca vamos modificar
             * memória que não devemos */
            sscanf(buffer, "%198[^\n]", aux->task);
            /* Mesmo depois do strcat o numéro máximo de caracteres possíveis é
             * 199 mais null char (200 bytes) */
            strcat(aux->task, "\n");
            aux->task_number = 1;
            aux->next_task = NULL;
            *head = aux;
        } else {
            /* Se a lista já tiver nodes */
            while (aux->next_task != NULL) {
                aux = aux->next_task;
            }
            ToDo *new_task;
            creat_node(&new_task, head);
            sscanf(buffer, "%198[^\n]", new_task->task);
            strcat(new_task->task, "\n");
            new_task->task_number = i;
            aux->next_task = new_task;
            new_task->next_task = NULL;
            ++i;
        }
    }
    fclose(save_file);
}

void save_tasks_to_file(ToDo *head)
{
    FILE *save_file = fopen("tasks.txt", "w");
    if (save_file == NULL) {
        puts("\nNão foi possível guardar as tarefas");
        return;
    }

    while (head != NULL) {
        fprintf(save_file, "%s", head->task);
        head = head->next_task;
    }

    fclose(save_file);
}
