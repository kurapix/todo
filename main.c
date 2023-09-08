#include "todolist.h"

static void program_guide(void)
{
  puts("\n\
A opção 'a' serve para adicionar uma tarefa\n\
A opção 'v' serve para ver as tuas tarefas\n\
A opção 'r' serve para remover uma tarefa\n\
A opção 'e' serve para editar uma tarefa\n\
A opção 'h' serve para ver este guia de utilização outra vez\n\
A opção 's' serve para sair do programa");
}

static void important_information(void)
{
  puts("\n\
Quando saíres do programa as tuas tarefas serão guardadas num \
save file, para quando voltares não perderes o teu progresso.\n\
Se o teu input for demasiado longo, o programa \
irá pedir para voltares a escrever!");
}

static int user_input_option(ToDo **head)
{
  bool first_time = true;
  int option;
  do {
    /* Se não for a primeira vez no loop devemos limpar os
     * caracteres que foram inseridos a mais no stdin */
    if (!first_time) {
      int discard;
      while ((discard = getchar()) != '\n' && discard != EOF) {
      }
    }

    printf("\nEscolhe uma opção: ");
    option = getchar();

    if (option == EOF) {
      puts("\nErro de EOF");
      remove_all_tasks(head); // Liberta a memória dinâmica
      exit(EXIT_FAILURE);
    }
    /* Caso particular em que a condição do loop não se aplica */
    if (option == '\n') {
      return '\n';
    }
    if (first_time) {
      first_time = false;
    }
    /* Se a condição é verdadeira foi escrito mais de 1 caracter */
  } while (getchar() != '\n');

  return option;
}

int main(void)
{
  program_guide();
  important_information();

  ToDo *head = NULL; // A linked list começa vazia

  load_tasks_from_file(&head);

  for (;;) {
    int option = user_input_option(&head);

    switch (option) {
    case 'a':
      add_task(&head);
      break;
    case 'v':
      print_todo_list(head);
      break;
    case 'r':
      remove_task(&head);
      break;
    case 'e':
      edit_task(&head);
      break;
    case 'h':
      program_guide();
      break;
    case 's':
      save_tasks_to_file(head);
      remove_all_tasks(&head); // Liberta a memória dinâmica
      puts("\nSaíste do programa");
      return 0;
    default:
      puts("\nOpção inválida");
    }
  }
}