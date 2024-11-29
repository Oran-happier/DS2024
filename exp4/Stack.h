#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include "Vector.h"
#include <string.h>
#include <cstdint>
#include <cmath>
#define N_OPTR 9 //���������
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;//���������
//�ӡ������ˡ������˷����׳ˡ������š������š���ʼ������ֹ��

void append(char*& RPN, float tp);
float calcu(float p1, char op, float p2);
float calcu(char op, float p);
float evaluate(char* S, char* RPN);//����ʽ��ֵ���沨����RPN��
char priority(char tp, char s);
int opIndex(char op);
const char pri[N_OPTR][N_OPTR] =
{
	/*              |-------------------- �� ǰ �� �� �� --------------------| */
	/*              +      -      *      /      ^      !      (      )      \0 */
	/* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	/* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	/* ջ  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	/* ��  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	/* ��  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
	/* ��  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
	/* ��  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '~',   ' ',
	/* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
	/* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '~'
};

template <typename T> class Stack :public Vector<T>
{
public://����ʽ�ӿڣ�ֱ��ʹ��
	void push(T const& e) { Vector<T>::insert(Vector<T>::size(), e); }//��ջ������Ԫ����Ϊ������ĩԪ�ز���
	T pop() { return Vector<T>::remove(Vector<T>::size() - 1); }//��ջ
	T& top() { return (*this)[Vector<T>::size() - 1]; }//ȡ����ֱ�ӷ���������ĩԪ��

	friend void readNumber(char* S, Stack<float>& opnd);
};

void convert(Stack<char>& S, __int64 n, int base)
{
	//�½����µ���λ����
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n > 0)
	{
		int remainder = (int)(n % base); S.push(digit[remainder]);//��������ǰλ����ջ
		n /= base;//����Ϊȥλ��
	}
}//�����γ��µĽ��ƣ��Զ����±�����ջ��

bool paren(const char exp[], Rank lo, Rank hi)
{
	//����ʽ����ƥ����,�ɼ����������
	Stack<char>S;//ʹ��ջ��¼�ѷ��ֵ���δƥ���������
	for (Rank i = lo; i <= hi; i++)/* ��һ��鵱ǰ�ַ� */
		switch (exp[i])
		{//������ֱ�ӽ�ջ;����������ջ��ʧ��,�����ʽ�ز�ƥ��
		case '(': break;
		case '[': break;
		case '{': S.push(exp[i]); break;
		case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
		case ']': if ((S.empty()) || ('[' != S.pop())) return false; break;
		case '}': if ((S.empty()) || ('{' != S.pop())) return false; break;
		default: break;//�������ַ�һ�ɺ���
		}
	return S.empty();//����ջ��,���ҽ���ƥ��
}

float evaluate(char* S, char* RPN)
{	//��(���޳��׿ո��)����ʽs��ֵ,��ת��Ϊ�沨��ʽRPN
	bool br = false;
	Stack<float>opnd; Stack<char>optr;//������ջ�������ջ
	optr.push('\0');//β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
	while (!optr.empty() && !br) {//�������ջ�ǿ�֮ǰ,�����������ʽ�и��ַ�
		if (isdigit(*S)) {//����ǰ�ַ�Ϊ������,��
			readNumber(S, opnd);
			append(RPN, opnd.top());//���������,���������RPNĩβ
			S++;
		}
		else //����ǰ�ַ�Ϊ�����,��
			switch (priority(optr.top(), *S))
			{	//������ջ�������֮�����ȼ��ߵͷֱ���
			case'<'://ջ����������ȼ�����ʱ
				optr.push(*S); S++;//�����Ƴ�,��ǰ�������ջ
				break;
			case'>': {//ջ����������ȼ�����ʱ,��ʵʩ��Ӧ�ļ���,�������������ջ
				char op = optr.pop(); append(RPN, op);//ջ���������ջ��������RPNĩβ
				if ('!' == op)//������һԪ�����
					opnd.push(calcu(op, opnd.pop()));//��ȡһ��������,��������ջ
				else
				{//��������(��Ԫ)�����
					float opnd2 = opnd[opnd.size() - 1], opnd1 = opnd[opnd.size() - 2];//*1
					//float opnd2 = opnd.pop(), opnd1 = opnd.pop();//ȡ����ǰ������
					if (optr.size() < DEFAULT_CAPACITY)
					{
						//����VectorԤ�����5��С���������еĲ���������6��ʼ
						// ��5����ȡ��ʱ��˵��Ԥ��ѹ���'\0'�Ѿ�����,����opnd��ȡ���˿պ����ս��
						//*1��ѡȡ��ʱ��Ϊ������ʱ��������pop()�Ķ����ǻ��޸�opnd�����
						// Ϊ�˱������ñ�ɾ�������ݣ����Բ���[]��ȡ�ķ�ʽ
						//*2�����ȷʵ��Ҫ���㣬��ô�ͻ���������ɣ���ʱ����pop�޸�opnd
						br = true;
					}
					else
					{
						opnd2 = opnd.pop(), opnd1 = opnd.pop();//ȡ����ǰ������
						//std::cout << calcu(opnd1, op, opnd2) << std::endl;
						//std::cout << op << std::endl;
						opnd.push(calcu(opnd1, op, opnd2));
						//ʵʩ��Ԫ����,�����ջ
					}
						
				}
				break;
			}
			case'~'://ƥ��������(���Ż�\0')ʱ
				optr.pop(); if(*S!='\0') S++;//�����Ų�ת����һ�ַ�
				break;
			default:exit(-1);//���﷨����,��������ֱ���˳�
			}//switch
		//std::cout << "top:" << opnd[opnd.size()-1] << std::endl;
	}//while
	return opnd.pop();//�������������ļ�����
}

float calcu(float p1, char op, float p2)
{
	switch (op)
	{
	case '+':return p1 + p2;
	case '-':return p1 - p2;
	case '*':return p1 * p2;
	case '/':return p1 / p2;
	case '^':return pow(p1, p2);
	default:return 0.0;
	}
}

float calcu(char op, float p)
{
	float np = 1.0;
	while (p > 1.0) { np *= p--; }//��ʵ��С���Ľ׳˲��������ģ����ǿ��ǵ�������ʵ��.0����ʽ��������
	return np;
}

char priority(char tp, char s)
{

	return pri[opIndex(tp)][opIndex(s)];
}

int opIndex(char op)
{
	switch (op)
	{
	case '+':
		return ADD;
	case '-':
		return SUB;
	case '*':
		return MUL;
	case '/':
		return DIV;
	case '^':
		return POW;
	case '!':
		return FAC;
	case '(':
		return L_P;
	case ')':
		return R_P;
	case '\0':
		return EOE;
	default:
		break;
	}
	return 0;
}

void readNumber(char* S, Stack<float>& opnd)
{
	opnd.push(*S - '0');
}

void append(char*& RPN, float tp)
{
	//���������ֻ���������ַ�����20���ڵĴ�С����ʵ���ǲ���������ȫ��
	//����Ҫ��RPN�㹻�󣬲�Ȼ�����������
	char newtp[20];
	sprintf(newtp, "%.2f", tp);
	strcat(RPN, newtp);
}