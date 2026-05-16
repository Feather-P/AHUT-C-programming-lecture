#include <math.h>
#include <stdio.h>

enum Operator {
    PLUS = '+',
    MINUS = '-',
    MUL = '*',
    DIV = '/',
    POW = '^'
};

int main() {
    double num_a;
    double num_b;
    char op_char;
    enum Operator operator;
    scanf("%lf%c%lf", &num_a, &op_char, &num_b);
    switch (op_char) {
        case '+': {
            operator= PLUS;
            break;
        }
        case '-': {
            operator= MINUS;
            break;
        }
        case '*': {
            operator= MUL;
            break;
        }
        case '/': {
            operator= DIV;
            break;
        }
        case '^': {
            operator= POW;
            break;
        }
        default: {
            printf("Invalid operator\n");
            return 1;
        }
    }
    double ans;
    switch (operator) {
        case PLUS: {
            ans = num_a + num_b;
            break;
        }
        case MINUS: {
            ans = num_a - num_b;
            break;
        }
        case MUL: {
            ans = num_a * num_b;
            break;
        }
        case DIV: {
            if (num_b == 0) {
                printf("Division by 0\n");
                return 2;
            }
            ans = num_a / num_b;
            break;
        }
        case POW: {
            ans = pow(num_a, num_b);
        }
    }
    printf("%lf\n", ans);
}