#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<iomanip>
#include<cmath>
using namespace std;

double N=10000;
struct word
{
	string str;                  //记录单词
	int fre;                     //记录词频
	vector<double> docD;            //单词位置表
	int dft;                   //出现词项t的文档数目
	vector<int> docDx;            //单词位置表
};
struct result
{
	vector<double> cos;
	vector<int> list;
};
void readtxt(vector<struct word>  &data);
void sortdic(vector<struct word>  &data);
void cal(vector<struct word>  &data,string quest);
void print(vector<struct word> &data);
int main()
{
	vector<struct word> data;
	string se;

	data.reserve(30000);                 //防止内存溢出
	readtxt(data);                       //调readtxt函数
	sortdic(data);                       //调sortdic函数
	print(data);
	
	while(getline(cin,se))
	{
		cout<<endl<<endl;
		if(se=="##") return 0;
		else	cal(data,se);                         //调cal函数                
	}
	return 0;
}
//读文件函数
void readtxt(vector<struct word>  &data)
{
	double Wfd;                                                                        
	string s,l;
	int i,num;
	char doc[10];

	for(num=1;num<=N;num++)
	{
		sprintf(doc,"%u",num);
		ifstream in(doc);                       //循环读入10个文件
		while (getline(in, s))
		{
			for (istringstream sin(s); sin >> l; )            
			{
				struct word temp;                            //中间变量存放单词数据
				vector<double> t(10001,0);
				for(i=0;i<l.size();i++)
				{
					if(l[i]>='A'&&l[i]<='Z')  l[i]+=32;            //大写转小写
				}
				temp.fre=0;                     //初始化词频为0
				temp.str = l;  
				temp.docD=t;
				temp.dft=0;
				for ( i = 0; i!=data.size(); i++)
				{
					if (l == data[i].str)
					{
						data[i].dft++;
						data[i].docD[num]++;
						break;
					}
				}
				if (i == data.size())						 //词条尚未出现
				{
					temp.docD[num]++;
					temp.dft++;
					data.push_back(temp);
				}
			}
		}
		cout<<num<<endl;
	}
	double idft;
	for(int i=0;i<data.size();i++)
	{
		for(int j=1;j<data[i].docD.size();j++)
		{
			if(0!=data[i].docD[j])  
			{
				Wfd=1+log10(data[i].docD[j]);
				idft=log10((double)N/data[i].dft);
				data[i].docD[j]=Wfd*idft;
			}
		}
	}
	cout<<"Completed!"<<endl;
}
//字典排序
void sortdic(vector<struct word>  &data)
{
	int min;
	struct word temp;
	for(int i=1;i<data.size();i++)
	{
		min=i-1;
		for(int j=min+1;j<data.size();j++)
		{
			if(data[j].str<data[min].str)  min=j;
		}
		temp=data[min];
		data[min]=data[i-1];
		data[i-1]=temp;
	}                                                 //选择排序
}

void print(vector<struct word>  &data)
{
	ofstream out("dict.txt");                   //输出文件
	for(unsigned int i=0;i<data.size();i++)
	{
		out<<data[i].str<<endl;
		//out<<"DocFreq："<<data[i].dft<<endl;
		for(unsigned int j=0;j<data[i].docD.size();j++)
		{
			if(data[i].docD[j]!=0)
			{
				out<<data[i].docD[j]<<'\t';
				data[i].docDx.push_back(j);
			}
		}
		out<<endl;
		for(unsigned int j=0;j<data[i].docDx.size();j++)
		{
			out<<data[i].docDx[j]<<'\t';
		}
		out<<endl;
	}
}
void cal(vector<struct word>  &data,string quest)
{
	vector <struct word> sen;
	vector <string> words;
	vector <int> num;
	string t;
	vector <struct result> last;
	int count=0;
	for(istringstream sin(quest); sin >> t;)
	{
		words.push_back(t);
	}

	for(int i=0;i<data.size();i++)
	{
		for(int j=0;j<words.size();j++)
		{
			if(data[i].str!=words[j])  continue;
			else
			{
				struct word temp;
				temp.str=words[j];
				temp.dft=data[i].dft;
				temp.docD.push_back((1+log10((double)data[i].dft))*log10((double)N/data[i].dft));
				num.push_back(i);
				sen.push_back(temp);
			}
			count++;
		}
		if(count==words.size()) break;
	}
	if(count==0)  return ;

	for(int i=0;i<num.size();i++)
	{
		result tran;
		vector <double> cos;
		for(int j=0;j<data[num[i]].docD.size();j++)
		{
			cos.push_back(sen[i].docD[0]*data[num[i]].docD[j]);
		}
		tran.cos=cos;
		tran.list=data[num[i]].docDx;
		last.push_back(tran);
	}
	double m;
	vector <double> cos;
	vector <int> loc;
	result END;
	int k;
	for(int i=0;i<last.size();i++)
	{
		for(int j=0;j<last[i].list.size();j++)
		{
			for(k=0;k<loc.size();k++)
			{
				if(loc[k]==last[i].list[j])
				{
					cos[k]+=last[i].cos[j];
					break;
				}
			}
			if(k==loc.size())
			{
				loc.push_back(last[i].list[j]);
				cos.push_back(last[i].cos[j]);
			}
		}
	}
	END.cos=cos;
	END.list=loc;
	int max;
	double T;
	int tn;
	for(int i=1;i<END.cos.size();i++)
	{
		max=i-1;
		for(int j=max+1;j<END.cos.size();j++)
		{
			if(END.cos[j]>END.cos[max])  max=j;
		}
		T=END.cos[max];
		tn=END.list[max];
		END.cos[max]=END.cos[i-1];
		END.list[max]=END.list[i-1];
		END.cos[i-1]=T;
		END.list[i-1]=tn;
	}                                                 //选择排序
	for(int i=0;i<num.size();i++)
	{
		cout<<data[num[i]].str<<" : "<<data[num[i]].dft<<"  ";
	}
	cout<<endl;
	char doc[10];
	for(int i=0;i<10;i++)
	{
		string str;
		cout<<END.list[i]<<'\t';
		cout<<END.cos[i]<<endl<<endl;
		sprintf(doc,"%u",END.list[i]);

		ifstream in(doc);
		while(getline(in,str))
		{
			cout<<str;
		}
		cout<<endl;
		in.close();
	}
}