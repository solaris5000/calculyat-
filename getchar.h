#include <iostream>
#include "getchar.h"
#include <cctype>
#include <cmath>

using namespace std;

int to_digit (char digit)
{
    switch (digit)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
    }
}

double math(double* numbers_arr, int n_a_length, int* operations_prior, int o_p_length)
{
    int operations_done = 0, current_prior=0, operation_pos=0;
    char curr_symbol;

    for (current_prior = 0; current_prior < o_p_length; current_prior++)
    {
        for (int i =0; i < o_p_length; i++) // looking for current operation symbol
        {
            if(operations_prior[i*2+1]==current_prior)
            {
                curr_symbol=char(operations_prior[i*2]);
                operation_pos = i-operations_done; // получаем позицию числа, которое
                if (operation_pos < 0)              // будет проводить операцию со следующим числом
                    operation_pos =0;
            }
        }

        switch (curr_symbol)
            {
                case '+':{numbers_arr[operation_pos]+=numbers_arr[operation_pos+1]; break;}
                case '-':{numbers_arr[operation_pos]-=numbers_arr[operation_pos+1]; break;}
                case '*':{numbers_arr[operation_pos]*=numbers_arr[operation_pos+1]; break;}
                case '/':{numbers_arr[operation_pos]/=numbers_arr[operation_pos+1]; break;}
            }
        operations_done++;

        for (int i = operation_pos+1; i < n_a_length; i++)
        {
            numbers_arr[i]=numbers_arr[i+1];
        }
    }

    return numbers_arr[0];
}

int math_prepare(char* arr, int arr_length, double* math_arr)
{
    int num_count=0;
    double number=0;
    int after_dot=1;
    bool is_float = false;
    for (int i=0; i<arr_length+1; i++)
    {

        if (iswdigit(arr[i]))
        {
            if (is_float)
            {
                number+=to_digit(arr[i])*pow(0.1,after_dot);
                after_dot++;
                continue;
            } else
            {
                number*=10;
                number+=to_digit(arr[i]);
                continue;
            }

        } else if (arr[i]=='.')
                {
                    is_float=true;
                    continue;
                }
        cout.precision(9);
        math_arr[num_count]=number;
        num_count++;
        number = 0;
        after_dot = 1;
        is_float = false;
    }

    return num_count;
}

int operations_prepare(char* arr, int arr_length, int* operations_arr)
{
    int operations_count=0;
    for (int i = 0; i < arr_length; i++)
    {
        switch (arr[i])
        {
            case '+':
            case '-':
            case '*':
            case '/': {operations_arr[operations_count*2]=arr[i]; break;}
            default : {continue; break;}
        }
        operations_count++;
    }

    int op_priority=0;
        cout << endl;

    for (int i = 0; i < operations_count; i++)
    {
        if (operations_arr[i*2]==int('*') || operations_arr[i*2]==int('/'))
            operations_arr[i*2+1]=op_priority++;
    }

    for (int i = 0; i < operations_count; i++)
    {
        if (operations_arr[i*2]=='+' || operations_arr[i*2]=='-')
            operations_arr[i*2+1]=op_priority++;
    }

    return operations_count;
}

void input_error_message(char* arr, int pos, bool& input_err)
{
    input_err=true;
    cout << "\033c";
    cout << "Input error" << endl;
        for (int i=0; i < pos; i++)
            cout << arr[i];
}


void size_error_message(char* arr, int pos, bool& input_err)
{
    input_err=true;
    cout << "\033c";
    cout << "Size error, max 50 symb" << endl;
        for (int i=0; i < pos; i++)
            cout << arr[i];
}

char input_check(char input_c)
{

    switch (input_c)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case '/':
        case '*':
        case '-':
        case '+': {return input_c; break;}
        case '=': {return 'r'; break;}
        case 10 : {cout << '='; return 'r'; break;}
        case 127 :{return 'b'; break;}
        default : {return 'e'; break;}

    }

}


int main()
{
    const int max_symbols = 150;
    char main_string[max_symbols];
    double math_numbers[max_symbols];
    int operations[max_symbols*2];
    int input_pos = 0, math_numbers_lenght, operations_length, float_digits=0;
    bool is_input = true;  // just for cycle and leaving it
    bool new_err = false;  // means that youve got a new error
    bool float_input = false; // for cheking that you printed .
    bool input_err = false; // for cheking that you pressed some that you shouldn't
    bool prev_symb_err = false; // for checking that prev symbol was math symbol and current symbol is math symbol
    bool prev_symb_znak = false; // for checking that prev symbol was math symbol
    bool curr_symb_znak = false; // for checking that current symbol is math symbol
    char c, input_result;

    cout << "input math expression:" << endl;

    while (is_input)
    {
        c = getch();
        if (input_pos==max_symbols && c!=127)
            {
                size_error_message(main_string, input_pos, input_err);
                continue;
            }
        else
            cout << c;

        input_result=input_check(c);

        if (input_err)
        {
            input_err=false;
            cout << "\033c";
            cout << "input math expression:" << endl;
                for (int i=0; i < input_pos; i++)
                    cout <<main_string[i];
            cout << c;
        }

        char prev_symb = main_string[input_pos-1];
        prev_symb_znak = (prev_symb == '+' || prev_symb == '-' || prev_symb == '/' || prev_symb == '*');
        curr_symb_znak = (c == '+' || c == '-' || c == '/' || c == '*');
        prev_symb_err = (prev_symb_znak && curr_symb_znak);


        switch (input_result)
        {
        case 'e':
            {
                input_error_message(main_string,input_pos,input_err);
                continue;
                break;
            }
        case 'r':
            {
                if (prev_symb_znak)
                {
                    input_error_message(main_string,input_pos,input_err);
                    continue;
                }
                else
                {
                    math_numbers_lenght=math_prepare(main_string, input_pos, math_numbers);
                    operations_length=operations_prepare(main_string, input_pos, operations);
                    cout << math(math_numbers, math_numbers_lenght, operations, operations_length);
                }
                break;
            }
        case 'b':
            {
                if (input_pos>0)
                {
                    if (main_string[input_pos-1]=='.')
                    float_input=false;
                    if (float_input)
                        float_digits--;
                    main_string[input_pos-1]='d';
                    input_pos--;
                }

                cout << "\033c";
                cout << "input math expression:" << endl;
                for (int i=0; i < input_pos; i++)
                    cout <<main_string[i];
                break;
            }
        default:
            {
                if (prev_symb_err)
                {
                    cout << "\b\b  \b\b" << c;
                    main_string[input_pos-1]=c;
                }
                else
                {
                    if (float_input)
                    {
                        if (c=='.' || float_digits >= 6 && !curr_symb_znak)
                        {
                            input_error_message(main_string,input_pos,input_err);
                            continue;
                        }
                        if (curr_symb_znak && c!='.')
                            float_input=false;
                        float_digits++;
                    }
                    if (c=='.')
                    {
                        float_digits = 0;
                        float_input=true;
                    }

                    main_string[input_pos]=c;
                    input_pos++;

                }
                break;
            }
        }
    }
    return 0;
}
