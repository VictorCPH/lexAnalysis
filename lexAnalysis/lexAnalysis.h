#ifndef LEXANALYSIS_H
#define LEXANALYSIS_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;
//�ؼ���
string keywords[32] = { "auto","break","case","char","const","continue","default","do","double","else","enum","extern",
						"float","for","goto","if","int","long","register","return","short","signed","sizeof","static",
						"struct","switch","typedef","union","unsigned","void","volatile","while" };

//�����
string operators[40] = { "+","-","*","/","%","++","--",//���������(0-6)
						">","<","==",">=","<=","!=",//��ϵ�����(7-12)
						"!","&&","||",//�߼������(13-15)
						"<<",">>","~","|","^","&",//λ�����(16-21)
						"=","+=","-=","*=","/=","%=",">>=","<<=","&=","^=","|=",//��ֵ�����(22-32)
						"?",":",//���������(33-34)
						".","->",//���������(35-36)
						"[","]",//�±������(37-38)
						"\\" };//ת�������(39)

//�ָ��� 
string boundary[9] = { ",",";","'","\"","(",")","{","}","#" };

void initTab();//��ʼ������
void alpha(char ch);//�ַ����»��߿�ͷ���ַ���������ʶ����ؼ��֣�
void digit(char ch);//���ִ�����
void strCon(char ch);//�ַ�������������
void otherChar(char ch);//ע�ͣ�������ȴ���
void output(string lexeme, string token, int location);//���ݽ����
int findID(string id);//������ʶ�����ر�ʶ������λ�ã���û���ڱ�β����
int findNum(string digit);//�������ֱ������ֳ�������λ�ã���û���ڱ�β����
int findStrCon(string str);//�����ַ������������ַ�������λ�ã���û���ڱ�β����
int findOperator(string s);//�������������������ڱ���λ�ã������ڷ���-1
int findBoundary(string s);//�����ָ������طָ����ڱ���λ�ã������ڷ���-1
void error(string tip, int line); //���������
#endif