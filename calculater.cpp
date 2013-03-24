/**
 * 中缀表达式-->后缀表达式-->表达式求值
 *
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include <cctype>
#include <set>
#include <queue>
#include <stack>
#include <map>
#include <cmath>
using namespace std;

/**
 * 判断是否为操作符
 * 参数：
 *      ch : 待判断的字符
 * 返回值：
 *      是操作符，返回true；否则返回false；
 */
bool is_operator(char ch) {
    set<char> operator_set;
    operator_set.insert('+');
    operator_set.insert('-');
    operator_set.insert('*');
    operator_set.insert('/');
    operator_set.insert('%');
    operator_set.insert('^');
    return operator_set.find(ch) != operator_set.end();
}
/**
 * 比较两个操作符的优先级
 */
int compare_priority(char a, char b) {
    map<char, int> operator_priority;
    operator_priority.insert(make_pair('+', 1));
    operator_priority.insert(make_pair('-', 1));
    operator_priority.insert(make_pair('*', 2));
    operator_priority.insert(make_pair('/', 2));
    operator_priority.insert(make_pair('%', 2));
    operator_priority.insert(make_pair('^', 3));

    return operator_priority[a]-operator_priority[b];
}
/**
 * 判断是否有不匹配的括号
 */
bool is_bracket_valid(string infix) {
    stack<char> bracket;
    for(int i=0; i<infix.size(); i++) {
        if(infix[i]=='(') {
            bracket.push(infix[i]);
        } else if(infix[i]==')') {
            if(bracket.empty()) {
                cout<<"有右括号没有匹配"<<endl;
                return false;
            }
            bracket.pop();
        }
    }
    if(!bracket.empty()) {
        cout<<"有左括号没有匹配"<<endl;
        return false;
    }
    return true;
}
/**
 * 预处理
 * 参数：
 *      infix：预处理前的中序表达式
 * 返回值：
 *      处理后的中序表达式
 * 步骤：
 *      1. 去除空格
 *      2. 判断是否含有不能处理的字符
 *      3. 处理一元操作符 '+'和'-'
 */
string preprocess(const string infix) {
    string result = infix;
    //去除空格
    size_t index;
    while((index = result.find(string(" "))) != string::npos) {
        result.erase(index, 1);
    }
    cout<<"去除空格后  "<<result<<""<<endl;

    //初始化能够处理的字符集合
    set<char> valid_char_set;//能够处理的字符
    for(int i=0; i<=9; i++) {
        valid_char_set.insert(i+'0');
    }
    valid_char_set.insert('+');
    valid_char_set.insert('-');
    valid_char_set.insert('*');
    valid_char_set.insert('/');
    valid_char_set.insert('%');
    valid_char_set.insert('^');
    valid_char_set.insert('(');
    valid_char_set.insert(')');
    valid_char_set.insert('e');//'e'和'E'为科学计数法
    valid_char_set.insert('E');
    valid_char_set.insert('.');//小数点

    for(int i=0; i<result.size(); i++) {
        //如果字符不在valid_char_set，说明有不能处理的字符，结束程序
        if(valid_char_set.find(result[i]) == valid_char_set.end()) {
            cout<<"中缀表达式中有非法字符: "<<result[i]<<"  "<<i<<endl;
            exit(-1);
        }
    }

    //处理一元操作符
    for(int i=0; i<result.size(); i++) {
        char temp = result[i];
        if(temp != '+' && temp != '-') {
            continue;
        }
//        cout<<i<<"   "<<result<<"   "<<result[i]<<endl;
        if(i==0) { //表达式开始位置的 '-'
            result.insert(i, 1, 0+'0');
        } else if(i-1>=0 && !isdigit(result[i-1]) && result[i-1]!=')') { //一元+-，紧跟着其他符号后面
            result.insert(i, "(0");
            int j = i+3;
            int bracket_count=0;//如果有括号，应包含括号
            for(; j<result.size(); j++) {
                if(isdigit(result[j]) || result[j]=='.') {
                    continue;
                } else if(result[j]=='(') {
                    ++bracket_count;
                } else if(result[j]==')') {
//                    --left_bracket_count;
                    if((--bracket_count) == 0) {
                        break;
                    }
                } else if(bracket_count==0) {
                    break;
                }
            }
            i = j;
            result.insert(j, ")");
        }
    }
    return result;
}
/**
 * 中缀表达式-->后缀表达式
 */
queue<string> infix_to_post(string infix) {
    queue<string> postfix;//后缀表达式队列
    stack<char> operator_stack;//转换过程中，用来存储操作符的栈

    set<char> valid_operand_set;//操作数 中的字符
    for(int i=0; i<=9; i++) {
        valid_operand_set.insert(i+'0');
    }
    valid_operand_set.insert('.');
    valid_operand_set.insert('e');
    valid_operand_set.insert('E');

    for(int i=0; i<infix.size(); i++) {
        cout<<endl;
        char ch = infix[i];
        cout<<"序号："<<"  "<<i<<"  字符：  "<<ch<<endl;
        if(infix[i]=='(') {//左括号
            operator_stack.push(infix[i]);
        } else if(infix[i]==')') {//右括号
            while(!operator_stack.empty() && operator_stack.top()!='(') {
                postfix.push(string(1, operator_stack.top()));
                operator_stack.pop();
            }
            operator_stack.pop();//将"("出栈
        } else if(is_operator(infix[i]) == true) { //是操作符（不包含 左、右括号）
            if(operator_stack.empty()) { //操作符栈为空
                operator_stack.push(infix[i]);
                continue;
            }

            //操作符栈非空
            char top_stack = operator_stack.top();
            //将栈中“较高和相等”优先级的操作符放到 后缀表达式中。
            //终止条件为：当前操作符>栈顶操作符优先级，或优先级相等、但栈顶操作符的结合性是“从右向左”。
            while(compare_priority(infix[i], top_stack)<=0) {
                //优先级相等，但栈顶操作符结合性为从右向左，这里特指'^'
                if(compare_priority(infix[i], top_stack)==0 && infix[i]=='^') {   //因为'^'的结合性从右向左，所以单独处理
                    break;
                }
                //当前操作符<=栈顶操作符优先级，当前操作符结合性为从左到右
                postfix.push(string(1, top_stack));
                operator_stack.pop();
                if(!operator_stack.empty()) {
                    top_stack = operator_stack.top();
                } else {
                    break;
                }

            }
            //将当前操作符入栈
            operator_stack.push(infix[i]);
        } else {//操作数
            string current_operator;
            int j=i;
            while(valid_operand_set.find(infix[j]) != valid_operand_set.end()) {
                current_operator += infix[j];
                ++j;
            }
            postfix.push(current_operator);
            i=j-1;//因为for循环，每次都会执行i++
        }
        
        //打印处理过程
        cout<<"当前栈顶： "<<(operator_stack.empty() ?' ': operator_stack.top())<<endl;
        queue<string> temp_queue = postfix;
        cout<<"当前后缀表达式： ";
        while(temp_queue.size()>0) {
            cout<<temp_queue.front()<<"  ";
            temp_queue.front();
            temp_queue.pop();
        }
        cout<<endl;

    }
    //最后将栈中内容全部取出来
    while(!operator_stack.empty()) {
        postfix.push(string(1, operator_stack.top()));
        operator_stack.pop();
    }

    return postfix;
}
/**
 * 计算两个操作数
 */
double calculate_two(double a, double b, string operand) {
    double result;
    if(operand == "+") {
        result = a+b;
    } else if(operand == "-") {
        result = a-b;
    } else if(operand == "*") {
        result = a*b;
    } else if(operand == "/") {
        if(b==0) {
            cout<<"除数不能为0"<<endl;
            exit(-1);
        }
        result = a/b;
    } else if(operand == "%") {
        result = (static_cast<int>(a)) % (static_cast<int>(b));
    } else if(operand == "^") {
        result = pow(a, b);
    }
    return result;
}
//对后缀表达式，进行计算
double calculate_post(queue<string>& post) {
    stack<double> result_stack;
    while(!post.empty()) {
        string temp = post.front();
        post.pop();
        if(is_operator(temp[0])) { //是操作符
            if(result_stack.size()<2) {
                cout<<"表达式错误"<<endl;
                exit(-1);
            }
            //从栈中取出两个元素，计算并将结果压入栈中
            double operand2 = result_stack.top();
            result_stack.pop();
            double operand1 = result_stack.top();
            result_stack.pop();

            double m = calculate_two(operand1, operand2, temp);
            result_stack.push(m);
        } else { //操作数
            double temp_operand = atof(temp.c_str());
            result_stack.push(temp_operand);
        }
    }
    return result_stack.top();
}

int main(int argc, char **argv) {
    string infix;
//    infix = "10e2+3*4-5%2-2^-(4/2)+.2 + 2^1^3";//结果为：1012.95
    cout<<"请输入中缀表达式： "<<endl;
    getline(cin, infix);
    cout<<endl<<"原始前缀表达式： "<<infix<<endl;

    //1. 检测括号是否匹配
    bool valid = is_bracket_valid(infix);
    if(valid){
        cout<<endl<<"括号匹配"<<endl<<endl;
    }else{
        return 0;
    }
    
    //2.预处理中缀表达式
    string result_infix = preprocess(infix);
    cout<<"预处理以后："<<result_infix<<endl;

    //3. 中缀 转 后缀
    queue<string> result_post = infix_to_post(result_infix);
    //打印后缀表达式
    queue<string> temp = result_post;
    cout<<"后缀表达式： ";
    while(!temp.empty()) {
        cout<<temp.front()<<"  ";
        temp.pop();
    }
    cout<<endl;
    
    //4. 计算结果
    double result = calculate_post(result_post);
    cout<<endl<<"计算结果： "<<result<<endl;
    
    return 0;
}