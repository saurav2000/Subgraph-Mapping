#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ifstream fin;
ofstream fout;
int n, n_ ;
char *input_name, *output_name, *param_name;

void readParam()
{
	fin.open(param_name);
	fin>>n>>n_;
	fin.close();
}

void writeParam()
{
	fout.open(param_name);
	fout<<n<<n_;
	fout.close();
}

void createMapping()
{
	
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