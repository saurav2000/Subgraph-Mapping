#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

ifstream fin;
ofstream fout;
int n, n_ ;
char *input_name, *output_name, *param_name;
bool **g, **g_;
vector<string> v;

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

	vector<int> cnt_in_g(n+1, 0);
	vector<int> cnt_out_g(n+1, 0);
	vector<int> cnt_in_g_(n_+1, 0);
	vector<int> cnt_out_g_(n_+1, 0);

	g = new bool*[n+1];
	g_ = new bool*[n_+1];
	for(int i=0;i<=n;++i)
		g[i] = new bool[n+1]();
	for(int i=0;i<=n_;++i)
		g_[i] = new bool[n_+1]();

	for(int i=0;i<tg.size();i+=2)
	{
		g[tg[i]][tg[i+1]] = true;
		cnt_out_g[tg[i]]++;
		cnt_in_g[tg[i+1]]++;
	}
	for(int i=0;i<tg_.size();i+=2)
	{
		g_[tg_[i]][tg_[i+1]] = true;
		cnt_out_g_[tg_[i]]++;
		cnt_in_g_[tg_[i+1]]++;
	}
	// cout<<"Alloc done\n";
	int a = 0, b = 0;
	for(int i=1, j=1;i<=n || j<=n_;++i, ++j)
	{
		if(i<=n && cnt_in_g[i]==0 && cnt_out_g[i]==0)
			++a;
		if(j<=n_ && cnt_in_g_[j]==0 && cnt_out_g_[j]==0)
			++b;
	}

	if(a>b)
	{
		printf("p cnf 0 1\n");
		printf("0\n");
		exit(0);
	}

	long long clauses = n*n_ + n + n_*(n*(n-1))/2 + n*(n_*(n_-1))/2 + n*(n-1)*n_*n_/2;
	// fout.open(output_name);
	printf("p cnf %d %d\n", n*n_, clauses);//cout<<"p cnf "<< n*n_ << " "<< clauses <<"\n";

	for(int i=1;i<=n;++i)
	{
		bool flag = cnt_in_g[i]==0 && cnt_out_g[i]==0;
		string s = "";
		for(int j=1;j<=n_;++j)
		{
			if(flag)
			{
				if(cnt_in_g_[j]==0&&cnt_out_g_[j]==0)
					s+= (to_string((j+(i-1)*n_)) + " ");
			}

			else if(cnt_in_g[i]>cnt_in_g_[j] || cnt_out_g[i]>cnt_out_g_[j])
				printf("-%d 0\n", (j+(i-1)*n_));// cout<<("-"+to_string(j+(i-1)*n_)+" 0\n");
		}
		if(flag)
			printf("%s0\n", s.c_str());
	}
}

void createMapping()
{
	// v.reserve(n*n*n_*n_ + n + n_ + n*n_*n_);
	//At least one mapping
	for(int i=0;i<n;++i)
	{
		string s;
		for(int j=1;j<=n_;++j)
			s += to_string(j+i*n_)+" ";
		s+="0\n";
		printf(s.c_str());// cout<<(s);
	}

	for(int i=1;i<=n_;++i)
	{
		for(int j=0;j<n-1;++j)
		{
			for(int k=j+1;k<n;++k)
				printf("-%d -%d 0\n", (i+j*n_), (i+k*n_));// cout<<("-" + to_string(i+j*n_) + " -" + to_string(i+k*n_) + " 0\n");
		}
	}

	//One-One function clauses
	for(int i=0;i<n;++i)
	{
		for(int j=1;j<n_;++j)
		{
			for(int k=j+1;k<=n_;++k)
				printf("-%d -%d 0\n", (j+i*n_), (k+i*n_));// cout<<("-" + to_string(j+i*n_) + " -" + to_string(k+i*n_) + " 0\n");
		}
	}

	//Graph mapping clauses
	for(int i=1;i<n_;++i)
	{
		for(int j=i+1;j<=n_;++j)
		{
			for(int k=0;k<n;++k)
			{
				for(int l=0;l<n;++l)
				{
					if(k==l)
						continue;
					// cout<<i<<" "<<j<<" "<<k+1<<" "<<l+1<<"\n";
					// int x = k*n_ + i;
					// int y = l*n_ + j;
					bool a = g[k+1][l+1], b = g[l+1][k+1];
					bool x = g_[i][j], y = g_[j][i];
					// if(i==1&&k==1&&j==2&&l==0)
						// cout<<a<<b<<x<<y<<" "<<(! (((a&&x) || (!a&&!x)) && ((b&&y) || (!b&&!y))))<<"\n";
					if(! (((a&&x) || (!a&&!x)) && ((b&&y) || (!b&&!y))))
						printf("-%d -%d 0\n", (k*n_ + i), (j+l*n_));// cout<<("-" + to_string(k*n_ + i) + " -" + to_string(l*n_ + j) + " 0\n");

				}
			}
		}
	}

	
	// fout<<"p cnf "<< n*n_ << " "<< 100 <<"\n";
	// for(int i=0;i<v.size();++i)
	// 	fout<<v[i];

	// fout.close();
}

void printMapping()
{
	fin.open(input_name);
	fout.open(output_name);

	string s;
	fin>>s;
	if(s[0]=='U')
	{
		fout<<"0";
		fin.close();
		fout.close();
		return;
	}

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
	// ios::sync_with_stdio(0);
	// cout.tie(0);
	auto start = high_resolution_clock::now();
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
		input_name = c_string(s+".satoutput");
		output_name = c_string(s+".mapping");
		readParam();
		printMapping();
	}
	auto end = high_resolution_clock::now();
	auto time_span = duration_cast<duration<double>>(end - start);
	cerr<<time_span.count()<<"\n";
	return 0;
}