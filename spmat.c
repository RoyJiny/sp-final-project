#include "spmat.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

typedef struct node
{
	char val;
	int index;
	struct node *next;
} node;

typedef struct list
{
	node **rows;
} list;

void print_list(node *head)
{
	if (head == NULL)
	{
		printf("NULL\n");
	}
	else
	{
		printf(" (%d)%d->", head->index, head->val);
		print_list(head->next);
	}
}

/*--------------------------------------------------------LIST_IMP----------------------------------------------------------*/
node *create_list(const char *row, int n)
{
	node *head = NULL, *tail = NULL;
	const char *p = row;
	char first = 1;
	int counter = 0;

	for (; p < row + n; p++)
	{
		if (*p != 0)
		{
			if (first)
			{
				alloc(head,node,1,"create_list","head");
				head->val = *p;
				head->index = counter;
				tail = head;
				first = 0;
			}
			else
			{
				alloc(tail->next,node,1,"create_list","tail->next");
				tail = tail->next;
				tail->val = *p;
				tail->index = counter;
			}
		}
		counter++;
	}
	if (tail != NULL)
	{
		tail->next = NULL;
	}
	return head;
}

void addRow_list(spmat *A, const char *row, int i)
{
	node **rows = ((list *)(A->handle))->rows;
	*(rows + i) = create_list(row, A->n);
}

void delete_list(node *l)
{
	node *next;
	while (l != NULL) {
		next = l->next;
		free(l);
		l = next;
	}
}

void free_list(spmat *A)
{
	node **l = ((list *)(A->handle))->rows;
	node **end = (((list *)(A->handle))->rows) + A->n;
	for (; l < end; l++)
	{
		delete_list(*l);
	}
	free(((list *)(A->handle))->rows);
	free(A->handle);
	free(A);
}

void mult_list(const spmat *A, const double *v, double *result, double *elements_per_g, group *g)
{
	double sum;
	int *g_members_rows, *g_members_cols;
	node **currRow = ((list *)(A->handle))->rows; /* current row*/
	node *currElem;  /* current element*/
	double *currRes = result;
	const double *v_runner;
	g_members_rows = g->members;
	while (g_members_rows < g->members+g->size)
	{
		sum = 0;
		g_members_cols = g->members;
		*elements_per_g = 0;
		v_runner = v;
		currElem = *(currRow + *(g_members_rows));
		while (currElem != NULL)
		{
			if (*g_members_cols == (currElem->index)) {
				sum += *v_runner; /*the value is always 1 so no need to multiply*/
				(*elements_per_g) ++;
				currElem = currElem->next;
				if (g_members_cols >= (g->members + g->size)) {
					break;
				}
				g_members_cols ++;
				v_runner++;
			}
			else if (currElem->index > *g_members_cols) 
			{
				if (g_members_cols >= (g->members + g->size)) {
					break;
				}
				g_members_cols ++;
				v_runner++;
			} else {
				currElem = currElem->next;
			}
		}
		*(currRes) = sum;
		currRes++;
		g_members_rows++;
		elements_per_g ++;
	}
}

double add_to_row_list(const spmat *A, int row_index, double *row, group *g)
{
	double sum = 0;
	int *g_members, counter = 0;
	node **rows;
	node *curr_row;
	g_members = g->members;
	rows = ((list *)(A->handle))->rows;
	curr_row = *(rows + row_index);
	while (curr_row != NULL && counter < g->size)
	{
		if (curr_row->index == *g_members)
		{
			*row += curr_row->val;
			sum += *row; /*calc the row sum*/
			row++;

			g_members++;
			counter++;
			curr_row = curr_row->next;
		}
		else if (curr_row->index < *g_members)
		{
			curr_row = curr_row->next;
		}
		else
		{
			g_members++;
			counter++;
			sum += *row; /*calc the row sum*/
			row++;
		}
	}
	return sum;
}

void print_matrix_list(const spmat *mat)
{
	int i;
	node **rows = ((list *)(mat->handle))->rows;
	for (i = 0; i < mat->n; i++)
	{
		if (i < 10)
		{
			printf(" ");
		}
		printf("%d| ", i);
		print_list(*rows);
		printf("\n");
		rows++;
	}
}

spmat *spmat_allocate_list(int n)
{
	list *l;
	spmat *spm;

	alloc(l,list,1,"spmat_allocate_list","l");
	alloc(l->rows,node*,n,"spmat_allocate_list","l->rows");
	alloc(spm,spmat,1,"spmat_allocate_list","spm");

	spm->n = n;
	spm->handle = l;

	spm->add_row = addRow_list;
	spm->free = free_list;
	spm->mult = mult_list;
	spm->print_matrix = print_matrix_list;
	spm->add_to_row = add_to_row_list;
	return spm;
}