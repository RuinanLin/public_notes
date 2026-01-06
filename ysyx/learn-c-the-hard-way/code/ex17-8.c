#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

Stack* createStack(int capacity) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->data = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

bool isFull(Stack *stack) {
    return stack->top == stack->capacity - 1;
}

bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

void push(Stack *stack, int item) {
    if (isFull(stack)) {
        printf("栈溢出！无法压入 %d\n", item);
        return;
    }
    stack->data[++stack->top] = item;
    printf("已压入: %d\n", item);
}

int pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("栈为空！无法弹出\n");
        return -1; 
    }
    return stack->data[stack->top--];
}

int peek(Stack *stack) {
    if (isEmpty(stack)) return -1;
    return stack->data[stack->top];
}

void freeStack(Stack *stack) {
    free(stack->data);
    free(stack);
}

int main() {
    Stack *s = createStack(5);

    push(s, 10);
    push(s, 20);
    push(s, 30);

    printf("当前栈顶元素: %d\n", peek(s));
    printf("弹出元素: %d\n", pop(s));
    printf("当前栈顶元素: %d\n", peek(s));

    freeStack(s);
    return 0;
}