/*
 * stringStack.h
 *
 *  Created on: 2011-3-25
 *      Author: xujingcn
 */

#ifndef __STRINGSTACK_H__
#define __STRINGSTACK_H__

#include <stdlib.h>
#include <string.h>

struct stackNode{
	char *value;
	struct stackNode *next;
};

struct stringStack{
	struct stackNode *top;
	struct stackNode *button;
};

int initStack (struct stringStack  *s) {
	s->top = (struct stackNode *)malloc(sizeof(struct stackNode));
	if (s->top == NULL) {
		return -1;
	}
	s->button = s->top;
	return 1;
}

int pushStack(struct stringStack *s, char *str) {
	struct stackNode *p = (struct stackNode *)malloc(sizeof(struct stackNode));
	if (p == NULL) {
		return -1;
	}
	p->value = malloc(strlen(str)+1);
	*(p->value+strlen(str)) = '\0';
	strcpy(p->value, str);
	p->next = s->top;
	s->top = p;
	return 1;
}

char * popStack(struct stringStack *s) {
	struct stackNode *node;
	char *str;
	if (s->top == s->button) {
		return NULL;
	}

	node = s->top;
	s->top = node->next;
	str = node->value;
	free(node);
	return str;
}

char * getStackTop(struct stringStack *s) {
	if (s->top == s->button) {
		return NULL;
	}
	return s->top->value;
}

int isEmptyStack (struct stringStack *s) {
	return (s->top == s->button) ? 1 : 0;
}

int destroyStack (struct stringStack *s) {
	char *str;
	while ((str = popStack(s)) != NULL) {
		free(str);
	}
	if (s->top == s->button) {
		free(s->top);
	}
	return 1;
}

#endif /* STRINGSTACK_H_ */
