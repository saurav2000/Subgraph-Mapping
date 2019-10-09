#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

FILE *fileptr, *fileptr2;
int n, n_ ;
char *input_name, *output_name, *param_name;
bool **g, **g_;
vector<string> v;

void readParam()
{
	fileptr = fopen(param_name, "r");
	fscanf(fileptr, "%d %d", &n, &n_);
	fclose(fileptr);
}

void writeParam()
{
	fileptr = fopen(param_name, "w");
	fprintf(fileptr, "%d %d", n, n_);
	fclose(fileptr);
}

void readGraphInput()
{
	vector<int> tg, tg_;
	int x, y;
	int max1 = 0, max2 = 0;
	bool flag = true;

	fileptr = fopen(input_name, "r");
	while(true)
	{
		int chk = fscanf(fileptr, "%d %d", &x, &y);
		if(chk==EOF)
			break;
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
		// if(fin.eof())
		// 	break;
	}
	fclose(fileptr);

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

	for(int i=1;i<=n;++i)
	{
		for(int j=1;j<=n_;++j)
		{
			if(cnt_in_g[i]>cnt_in_g_[j] || cnt_out_g[i]>cnt_out_g_[j])
				v.push_back("-"+to_string(j+(i-1)*n_)+" 0\n");
		}
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
		v.push_back(s);
	}

	for(int i=1;i<=n_;++i)
	{
		for(int j=0;j<n-1;++j)
		{
			for(int k=j+1;k<n;++k)
				v.push_back("-" + to_string(i+j*n_) + " -" + to_string(i+k*n_) + " 0\n");
		}
	}

	//One-One function clauses
	for(int i=0;i<n;++i)
	{
		for(int j=1;j<n_;++j)
		{
			for(int k=j+1;k<=n_;++k)
				v.push_back("-" + to_string(j+i*n_) + " -" + to_string(k+i*n_) + " 0\n");
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
						v.push_back("-" + to_string(k*n_ + i) + " -" + to_string(l*n_ + j) + " 0\n");

				}
			}
		}
	}

	fileptr = fopen(output_name, "w");
	fprintf(fileptr, "%s %d %d\n", "p cnf", n*n_, (int)v.size());
	// fout<<"p cnf "<< n*n_ << " "<< v.size() <<"\n";
	for(int i=0;i<v.size();++i)
		fprintf(fileptr, "%s", v[i].c_str());

	fclose(fileptr);
}

void printMapping()
{
	fileptr = fopen(input_name, "r");
	fileptr2 = fopen(output_name, "w");

	char *s;
	fscanf(fileptr, "%s", s);
	if(s[0]=='U')
	{
		fprintf(fileptr2, "%s\n","0");
		fclose(fileptr);
		fclose(fileptr2);
		return;
	}

	while(true)
	{
		int x;
		fscanf(fileptr, "%d", &x);
		if(!x)
			break;
		--x;
		if(x>=0)
			fprintf(stderr, "%d %d\n", (x/n_ + 1), (x%n_ + 1));
	}

	fclose(fileptr);
	fclose(fileptr2);
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
	cout<<time_span.count()<<"\n";
	return 0;
}