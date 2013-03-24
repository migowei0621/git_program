#include <cstring>
#include <iostream>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <cctype>  //判断是否为字母函数 isalpha（）
using namespace std;
const int MAXSIZE=100;

int Level(char oper)
{
    switch (oper)
  {
     case 0  : return 0;
     case '+': return 1;
     case '-': return 1;
     case '*': return 2;
     case '/': return 2;
     case '%': return 2;//add
     case '(': return 3;
     case ')': return 3;
     default : return 0;
   }
}


//单元运算
double Calculate (int a,int b,char c)
{
   switch (c) //c is  operator
   {
    case '*': return a*b;
    case '/': if(b==0){return -1;} 
              else  return (double)a/b;
    case '%': return a%b; //add
    case '+': return a+b;
    case '-': return a-b;
    default : return 0; 
   }   
} 
//将中缀表达式转为后缀表达式
void Transform(char *src,char *src2)
{  stack<char> st;//符号栈
   int mpriori=0; //开始时的优先级
   int j=0;       //src2[j]放后缀表达式
   for (int i = 0;src[i]!='\0'; i++)
   {    
        if (src[i]=='(')
      {
          mpriori=0;
          st.push(src[i]);
      }

        else if (src[i]==')')
      { 
          char x=st.top();
          while(x!='(')
        {  
          src2[j++]=st.top();
          st.pop();
          x=st.top();
        }
          st.pop();
         if(!st.empty()) mpriori=Level(st.top());
         else mpriori=0;
      }
        else if (src[i]!='('&&src[i]!=')'&& src[i]!='-'&&src[i]!='+'&&src[i]!='*'&& src[i]!='/' && src[i]!='%')
      {
          src2[j++]=src[i];
      }
        else if (src[i]=='+' || src[i]=='-'|| src[i]=='/'|| src[i]=='*' || src[i]=='%')
      {
          if (mpriori<Level(src[i]))
        {
             mpriori=Level(src[i]);
             st.push(src[i]);
        }
      
          else
        {

            while(Level(src[i])<=mpriori)
            {
              src2[j++]=st.top();
              if (st.empty())
              {
                break;
              }
              st.pop();
              // cout<<Level(st.top())<<endl;

              if (!st.empty())
              {
               mpriori=Level(st.top());
              }
              else
               mpriori=0;
            }

             // src2[j++] =st.top();                 
             // st.pop();
             st.push(src[i]);
             mpriori=Level(src[i]);
        }
      }

    }
 
    while (!st.empty())
   {
       src2[j++]=st.top();
       st.pop();
   }
 
}

//计算后缀表达式
 double evaluation(char *src2)
{  stack<double> sc2;
   double a,b,c;
   double temp=0;
   char x[10];
   memset(x,0,sizeof(x));
    
  for (int i = 0; src2[i]!='\0'; i++)
 {    
        if(src2[i]!='+'&& src2[i]!='-'&& src2[i]!='*' && src2[i]!='/' && src2[i]!='%')
     {  
        x[0]=src2[i];
        temp=atof(x);// char to double
        sc2.push(temp);
     }
    else
   {
     a=sc2.top();
     sc2.pop();
     b=sc2.top();
     sc2.pop();
     c=Calculate(b,a,src2[i]);
     sc2.push(c);
     //cout<<c<<endl;
     
     if (c==-1)
     {
       return -1;
     }
     
   }
     
  }
  return sc2.top();
}

  int main(int argc, char const *argv[])
{ 
  double result;
  char src1[MAXSIZE];//输入序列
  char src2[MAXSIZE];//后缀表达式
  memset(src1,0,sizeof(src1));
  memset(src2,0,sizeof(src2));
  cout<<"Input"<<endl;

  gets(src1);
  for (int i = 0; i < MAXSIZE; ++i)
  {
    if (isalpha(src1[i]))
    {
      cout<<"Invalid Input! NO.1!"<<endl;
      system("pause"); 
      return 0;
    }
  }
  // if (src1[0]=='0'&&strlen(src1)==1) break;
  //cout<<src1<<endl;
  Transform(src1,src2);
  result=evaluation(src2);
  
  if (result==-1)
  {
    cout<<"Invalid Input! NO.2!"<<endl;//除数为零
    return 0;
  }
  //cout<<src2<<endl;
  cout<<"Output"<<endl;
  // cout<<(double)result<<endl;
  printf("%.4f\n", result);

  cout<<"End"<<endl;
  system("pause"); 
  return 0;
}