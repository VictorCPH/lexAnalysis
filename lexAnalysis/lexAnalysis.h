#ifndef LEXANALYSIS_H
#define LEXANALYSIS_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;
//关键字
string keywords[32] = { "auto","break","case","char","const","continue","default","do","double","else","enum","extern",
						"float","for","goto","if","int","long","register","return","short","signed","sizeof","static",
						"struct","switch","typedef","union","unsigned","void","volatile","while" };

//运算符
string operators[40] = { "+","-","*","/","%","++","--",//算术运算符(0-6)
						">","<","==",">=","<=","!=",//关系运算符(7-12)
						"!","&&","||",//逻辑运算符(13-15)
						"<<",">>","~","|","^","&",//位运算符(16-21)
						"=","+=","-=","*=","/=","%=",">>=","<<=","&=","^=","|=",//赋值运算符(22-32)
						"?",":",//条件运算符(33-34)
						".","->",//分量运算符(35-36)
						"[","]",//下标运算符(37-38)
						"\\" };//转义运算符(39)

//分隔符 
string boundary[9] = { ",",";","'","\"","(",")","{","}","#" };

void initTab();//初始化各表
void alpha(char ch);//字符，下划线开头的字符串处理（标识符或关键字）
void digit(char ch);//数字串处理
void strCon(char ch);//字符串字面量处理
void otherChar(char ch);//注释，运算符等处理
void output(string lexeme, string token, int location);//数据结果表
int findID(string id);//遍历标识符表返回标识符所在位置，若没有在表尾插入
int findNum(string digit);//遍历数字表返回数字常量所在位置，若没有在表尾插入
int findStrCon(string str);//遍历字符串常量表返回字符串所在位置，若没有在表尾插入
int findOperator(string s);//遍历运算符表返回运算符在表中位置，若不在返回-1
int findBoundary(string s);//遍历分隔符表返回分隔符在表中位置，若不在返回-1
void error(string tip, int line); //错误处理输出
#endif