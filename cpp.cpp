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

int math_prepare(char* arr, int arr_length, double* math_arr)
{
    double number=0, float_part=0;
    int after_dot=1;
    bool is_float = false;
    cout << arr_length << endl;
    for (int i=0; i<arr_length; i++)
    {
        if (iswdigit(arr[i]))
        {
            cout << arr[i] << " - " << to_digit(arr[i]) << endl;
            if (is_float)
            {
                float_part+=to_digit(arr[i])*pow(0.1,after_dot);
                after_dot++;
                cout << endl << "float part " << float_part << endl;
                continue;
            } else
            {
                number*=10;
                number+=to_digit(arr[i]);
                cout << endl << "number part " << number << endl;
                continue;
            }

        } else
            if (arr[i]=='.')
                {
                    cout << arr[i] << endl;
                    is_float=true;
                    continue;
                }

        number+=float_part;

        cout << endl << "number " << number;

        number = 0;
        float_part = 0;
        is_float = false;

    }
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
    char math_prioretetes[3][max_symbols];
    int input_pos = 0, math_numbers_lenght, float_digits=0;
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
                   /* for (int y=0; y < input_pos; y++)
                        cout << main_string[y];
                    cout << "--------" << endl;*/
                    math_numbers_lenght=math_prepare(main_string, input_pos, math_numbers);
                   /* for (int y = 0; y < math_numbers_lenght; y++)
                        cout << endl << math_numbers[y];*/
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
