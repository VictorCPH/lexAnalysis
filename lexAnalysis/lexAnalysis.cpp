
#include "stdafx.h"
#include "lexAnalysis.h"

const int keywordNum = 32;
const int boundaryNum = 9;
const int operatorNum = 40;
ifstream infile;//Դ�����ļ�ָ��
int lines = 0;//������¼
int errors = 0;//�����¼
int tokenNum = 0;//��������¼
int chNum = 0;//�ַ�����¼

int main()
{
	initTab();//��ʼ���������

	string filename;                            //�������ļ�
	cout << "Please input the filename: ";
	cin >> filename;

	infile.open(filename.c_str(), ios::in);
	if (!infile)
	{
		cout << "Open the file error!" << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	cout << "���ڴ����ļ�......" << endl;        
	char ch;
	while (infile.get(ch))                      //�ʷ���������
	{
		++chNum;
		if (isalpha(ch) || ch == '_')			//�ַ����»��߿�ͷ���ַ�����������ʶ����ؼ��֣�
			alpha(ch);
		else if (isdigit(ch))					//���ִ�����
			digit(ch);
		else if (ch == ' ' || ch == '\t')		//�հ׷����Ʊ���������ֱ������
			--chNum;
		else if (ch == '\n' || ch == '\r')		//���з���������¼����
		{
			--chNum;
			++lines;
		}
		else if (ch == '"' || ch == '\'')      //�ַ���������
			strCon(ch);
		else									//��Ҫ��ǰ��������ʶ�������
			otherChar(ch);
	}

    if (errors)								  //����ͳ��
		cout << "A total of " << errors << " errors!" << endl;
	else
		cout << "No error��" << endl;
	cout << "�������: " << lines << endl;     //���ͳ��
	cout << "���ʸ���: " << tokenNum << endl;
	cout << "�ַ�����: " << chNum << endl;

	infile.close();
	
	system("pause");
	return 0;
}

void initTab()//��ʼ������
{
	ofstream outfile;

	//��ʼ���ؼ��ֱ�
	outfile.open("keywords.txt", ios::out);
	if (!outfile)
	{
		cout << "keywords.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	for (int i = 0; i < keywordNum; ++i)
		outfile << keywords[i] << endl;
	outfile.close();

	//��ʼ���ָ�����
	outfile.open("boundary.txt", ios::out);
	if (!outfile)
	{
		cout << "boundary.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	for (int i = 0; i < boundaryNum; ++i)
		outfile << boundary[i] << endl;
	outfile.close();

	//��ʼ���������
	outfile.open("operators.txt", ios::out);
	if (!outfile)
	{
		cout << "operators.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	for (int i = 0; i < operatorNum; ++i)
		outfile << operators[i] << "  ";
	outfile << endl;
	outfile.close();

	//��������ʼ������ļ�
	outfile.open("output.txt", ios::out);
	if (!outfile)
	{
		cout << "output.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	outfile.close();

	//��������ʼ����ʶ������ļ�
	outfile.open("id.txt", ios::out);
	if (!outfile)
	{
		cout << "id.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	outfile.close();
	//��������ʼ���ַ�����������ļ�
	outfile.open("strCon.txt", ios::out);
	if (!outfile)
	{
		cout << "strCon.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	outfile.close();

	//��������ʼ�����ֳ�������ļ�
	outfile.open("digit.txt", ios::out);
	if (!outfile)
	{
		cout << "digit.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	outfile.close();
}

void alpha(char ch)//�ַ����»��߿�ͷ���ַ�����������ʶ����ؼ��֣�
{
	string idOrKeyword;
	while (isalpha(ch) || isdigit(ch) || ch == '_')
	{
		idOrKeyword += ch;
		infile.get(ch);
		++chNum;
		if (infile.eof()) break;
	}

	int i;
	for (i = 0; i < keywordNum; ++i)
		if (idOrKeyword == keywords[i])//�ǹؼ���
		{
			output(keywords[i], "keyword", i + 1);
			break;
		}

	if (i == keywordNum)//���ǹؼ���
		output(idOrKeyword, "id", findID(idOrKeyword));
	infile.seekg(-1, ios::cur);
	--chNum;
}

void digit(char ch)//���ִ�����
{
	string digits;
	char nextChar = infile.get();	
	infile.seekg(-1, ios::cur);

	if (ch == '0' && nextChar != '.' 
		&& nextChar != 'e' && nextChar != 'E' 
		&& nextChar != 'x' && !isdigit(nextChar))  //����0
		digits += ch;
	else if (ch == '0')                    //0��ͷ��8���ƻ�16������
	{
		digits += ch;
		infile.get(ch);
		++chNum;
		if (ch == 'x' || ch == 'X')      //16���ƴ���
		{
			digits += ch;
			infile.get(ch);	
			++chNum;
			if (!isdigit(ch) && !(ch >= 'a' && ch <= 'f') && !(ch >= 'A' && ch <= 'F'))  //�Ƿ�0x �� 0X �ַ���
			{
				infile.seekg(-1, ios::cur);
				--chNum;
				error("Illegal hexadecimal!", lines + 1);
				return;
			}
			else                                                                         //�Ϸ���16������
			{
				while (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
				{
					digits += ch;
					infile.get(ch);
					++chNum;
					if (infile.eof()) break;		
				}
			}
		}
		else							//8���ƴ���
		{
			digits += ch;
			infile.get(ch);
			++chNum;
			
			if (ch <= '0' || ch >= '7')             //�Ƿ�8������
			{
				infile.seekg(-1, ios::cur);
				--chNum;
				error("Illegal octal!", lines + 1);
			}
			else                                    //�Ϸ�8������
			{
				while (ch >= '0' && ch <= '7')
				{
					digits += ch;
					infile.get(ch);
					++chNum;
					if (infile.eof()) break;
									
				}
			}
		}
	}
	else                                         //��������ʮ����
	{
		while (isdigit(ch))						
		{
			digits += ch;
			if (infile.eof()) break;
			infile.get(ch);
			++chNum;
		}
		if (ch == '.')						//����С������
		{
			digits += ch;
			infile.get(ch);
			++chNum;
			if (!isdigit(ch))
			{
				error("Illegal float!", lines + 1);
				return;
			}
			else
			{
				while (isdigit(ch))
				{
					digits += ch;
					if (infile.eof()) break;
					infile.get(ch);
					++chNum;
				}
				if (ch == 'e' || ch == 'E')
				{
					digits += ch;
					infile.get(ch);
					++chNum;
					if (ch == '+' || ch == '-')
					{
						digits += ch;
						infile.get(ch);
						++chNum;
						if (!isdigit(ch))
						{
							error("Illegal number!", lines + 1);
							return;
						}
						else
						{
							while (isdigit(ch))
							{
								digits += ch;
								if (infile.eof()) break;
								infile.get(ch);
								++chNum;
							}
						}
					}
					else if (isdigit(ch))
					{
						digits += ch;
						infile.get(ch);
						++chNum;
						while (isdigit(ch))
						{
							digits += ch;
							if (infile.eof()) break;
							infile.get(ch);
							++chNum;
						}
					}
					else
					{
						infile.seekg(-1, ios::cur);
						--chNum;
						error("Illegal number", lines + 1);
						return;
					}
				}
			}
		}		
		else
		{
			infile.seekg(-1, ios::cur);
			--chNum;
		}
	}
	output(digits, "num", findNum(digits));
}

void strCon(char ch)//�ַ�������������
{
	string strCon;
	int curLine = lines;//��¼�ַ�����ֵ�������кţ����ڴ�����
	char tmpc = ch; //��¼��һ�εõ����ǵ�����  ���� ˫���ţ�����ƥ��

	while (1)
	{
		infile.get(ch);
		++chNum;
		if (ch == '\\')
		{
			infile.get(ch);
			++chNum;
			if (ch == '\n' || ch == '\r')
			{
				--chNum;
				++lines;
			}
			else if (infile.eof())
			{
				error("Can not find matching string : \"\"\" or \"'\" !", curLine + 1);
				break;
			}
			else
				strCon += ch;
		}
		else if (ch == tmpc)
		{
			output(tmpc + strCon + tmpc, "strcon", findStrCon(tmpc + strCon + tmpc));
			break;
		}
		else if (ch == '\n' || ch == '\r')
		{
			--chNum;
			++lines;
		}
		else if (infile.eof())
		{
			error("Can not find matching string : \"\"\" or \"'\" !", curLine+1);
			break;
		}
		else
			strCon += ch;
	}
}

void otherChar(char ch)//ע�ͣ�������ȴ���
{
	int cur = infile.tellg();//��¼���ļ�ָ�뵱ǰλ��
	int curChNum = chNum;

	if (ch == '/')            //����ĸΪ'/'�Ĵ�
	{
		infile.get(ch);
		++chNum;
		if (ch == '/')  //ע�ͷ���Ϊ //
		{
			while (ch != '\n' && ch != '\r' && !infile.eof())
			{
				infile.get(ch);
				++chNum;
			}
			++lines;
		}
		else if (ch == '*')//ע�ͷ���Ϊ /*...*/
		{
			int curLine = lines;
			char charCur = infile.get();
			++chNum;
			char charNext = infile.get();
			++chNum;
			while (charCur != '*' || charNext != '/')
			{
				if (charCur == '\n' || charCur == '\r')
				{
					lines++;
					--chNum;
				}
				if (infile.eof())
				{
					error("Can not find matching string:\"*/\" !", curLine+1);
					break;
				}
				charCur = charNext;
				charNext = infile.get();
				++chNum;
			}
		}
		else//��������� /
		{
			infile.seekg(-1, ios::cur);
			--chNum;
			output("/", "operator", findOperator("/"));		
		}
	}
	else//���������
	{
		string str1 = "0";   //����Ϊ1���ַ�������¼��һ���ַ��������
		string str2 = "00";  //����Ϊ2���ַ�������¼�������ַ��������
		string str3 = "000"; //����Ϊ3���ַ�������¼�������ַ��������

		str3[0] = str2[0] = str1[0] = ch;
		str3[1] = str2[1] = infile.get();
		++chNum;
		str3[2] = infile.get();
		++chNum;

		if (findOperator(str3) != -1)    //���ȿ������ַ������
			output(str3, "operator", findOperator(str3));
		else
		{
			infile.seekg(-1, ios::cur);
			--chNum;
			if (findOperator(str2) != -1)  //�������ַ������
				output(str2, "operator", findOperator(str2));
			else
			{
				infile.seekg(cur);//�ļ�ָ�뷵�ص��ж�ǰ��λ��
				chNum = curChNum;
				if (findOperator(str1) != -1) //���ǵ��ַ������
					output(str1, "operator", findOperator(str1));
				else if (findBoundary(str1) != -1)  //���Ƿָ���
					output(str1, "boundary", findBoundary(str1));
				else								//�Ƿ��ַ�
					error("Illegal character " + str1 + "!", lines+1);
			}
		}
	}
}

void output(string lexeme, string token, int location)//���ݽ����
{
	ofstream outfile("output.txt", ios::app);
	if (!outfile)
	{
		cout << "output.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	outfile << lexeme << "\t\t\t\t" << "<" << token << "," << location << ">" << endl;
	++tokenNum;
	outfile.close();
}

int findID(string id)//������ʶ�������ر�ʶ������λ�ã���û���ڱ�β����
{
	string str;
	ifstream idIn("id.txt", ios::in);
	if (!idIn) 
	{
		cout << "id.txt open error!" << endl;
		system("pause");
		exit(-1);
	}

	int location = 0;
	while (idIn >> str)
	{
		++location;
		if (str == id)
		{
			idIn.close();
			return location;//��ʶ���Ѵ��ڱ���
		}
	}
	idIn.close();

	//��ʶ�����ڱ���
	ofstream idOut("id.txt", ios::app);
	if (!idOut)
	{
		cout << "id.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	idOut << id << endl;
	idOut.close();
	return location + 1;
}

int findNum(string digit)
{
	string str;
	ifstream numIn("digit.txt", ios::in);
	if (!numIn)
	{
		cout << "digit.txt open error!" << endl;
		system("pause");
		exit(-1);
	}

	int location = 0;
	while (numIn >> str)
	{
		++location;
		if (str == digit)
		{
			numIn.close();
			return location;//��ʶ���Ѵ��ڱ���
		}
	}
	numIn.close();

	//��ʶ�����ڱ���
	ofstream numOut("digit.txt", ios::app);
	if (!numOut)
	{
		cout << "digit.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	numOut << digit << endl;
	numOut.close();
	return location + 1;
}

int findStrCon(string strCon)//�����ַ��������������ַ�������λ�ã���û���ڱ�β����
{
	string str;
	ifstream strIn("strCon.txt", ios::in);
	if (!strIn)
	{
		cout << "strCon.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
//	cout << strCon << endl;
	int location = 0;
	while (getline(strIn, str))
	{
		++location;
		if (str == strCon)
		{
			strIn.close();
			return location;//��ʶ���Ѵ��ڱ���
		}
	}
	strIn.close();

	//��ʶ�����ڱ���
	ofstream strOut("strCon.txt", ios::app);
	if (!strOut)
	{
		cout << "strCon.txt open error!" << endl;
		system("pause");
		exit(-1);
	}
	strOut << strCon << endl;
	strOut.close();
	return location + 1;
}

int findOperator(string s)//���������������������ڱ���λ�ã������ڷ���-1
{
	for (int loc = 0; loc < operatorNum; ++loc)
		if (operators[loc] == s)
			return loc + 1;

	return -1;
}

int findBoundary(string s)//�����ָ��������طָ����ڱ���λ�ã������ڷ���-1
{
	for (int loc = 0; loc < boundaryNum; ++loc)
		if (boundary[loc] == s)
			return loc + 1;

	return -1;
}

void error(string tip, int line) //���������
{
	cout << "Error! No." << line << " line:" << tip << endl;
	errors++;
}