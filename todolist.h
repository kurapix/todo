#ifndef TODOLIST_H
#define TODOLIST_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct todo {
  char task[200];
  int task_number;
  struct todo *next_task;
} ToDo;

void add_task(ToDo **head);
void print_todo_list(ToDo *head);
void remove_task(ToDo **head);
void remove_all_tasks(ToDo **head);
void edit_task(ToDo **head);
void load_tasks_from_file(ToDo **head);
void save_tasks_to_file(ToDo *head);

#endif