#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

ifstream fin;
ofstream fout;
int n, n_ ;
char *input_name, *output_name, *param_name;
bool **g, **g_;

void readParam()
{
	fin.open(param_name);
	fin>>n>>n_;
	fin.close();
}

void writeParam()
{
	fout.open(param_name);
	fout<<n<<" "<<n_;
	fout.close();
}

void readGraphInput()
{
	vector<int> tg, tg_;
	int x, y;
	int max1 = 0, max2 = 0;
	bool flag = true;

	fin.open(input_name);
	while(fin>>x>>y)
	{
		if(x==0&&y==0)
		{
			flag = false;
			continue;
		}

		if(flag)
		{
			tg_.push_back(x);
			tg_.push_back(y);
			max2 = max(max(max2, x), y);
		}
		else
		{
			tg.push_back(x);
			tg.push_back(y);
			max1 = max(max(max1, x), y);
		}
		if(fin.eof())
			break;
	}
	fin.close();

	n = max1;
	n_ = max2;
	writeParam();

	g = new bool*[n+1];
	g_ = new bool*[n_+1];
	for(int i=0;i<=n;++i)
		g[i] = new bool[n+1]();
	for(int i=0;i<=n_;++i)
		g_[i] = new bool[n_+1]();

	for(int i=0;i<tg.size();i+=2)
		g[tg[i]][tg[i+1]] = true;
	for(int i=0;i<tg_.size();i+=2)
		g_[tg_[i]][tg_[i+1]] = true;
}

void createMapping()
{
	fout.open(output_name);

	

	fout.close();
}

void printMapping()
{
	fin.open(input_name);
	fout.open(output_name);

	while(true)
	{
		int x;
		fin>>x;
		if(!x)
			break;
		--x;
		if(x>=0)
			fout<<(x/n_ + 1)<<" "<<(x%n_ + 1)<<"\n";
	}

	fin.close();
	fout.close();
}

char* c_string(string s)
{
	char *ch = new char[s.size()+1];
	copy(s.begin(), s.end(), ch);
	ch[s.size()] = '\0';
	return ch;
}

int main(int argc, char** argv)
{
	string s(argv[2]);
	param_name = c_string(s+".param");
	if(argv[1][0]=='0')
	{
		input_name = c_string(s+".graphs");
		output_name = c_string(s+".satinput");
		readGraphInput();
		createMapping();
	}
	else
	{
		input_name = c_string(s+".satinput");
		output_name = c_string(s+".mapping");
		readParam();
		printMapping();
	}

	return 0;
}