#include <iostream>
#include <math.h>
#include <random>
#include <string.h>
#include <chrono>

using namespace std;

uint8_t dotMultiply(uint8_t * m1, uint8_t* m2, int n)
{
	uint8_t rlt=0;
	for(int i=0;i<n;++i)
	{
		rlt=rlt+(m1[i]*m2[n-1-i]);
	}
	return rlt;
}

uint8_t* mSequence(uint8_t* fbconnection,int n)
{
	unsigned N=pow(2,n)-1;
	uint8_t* newregister=new uint8_t[n];
	uint8_t* mseq=new uint8_t[N];
	memset(newregister,0,n);
	memset(mseq,0,N);
	uint8_t* reg = new uint8_t[n];
	memset(reg,0,n);
	reg[0]=1;
	mseq[0]=reg[n-1]=1;
	for(unsigned i=1;i<N;++i)
	{
		newregister[0]=dotMultiply(fbconnection,reg,n)%2;
		for(int j=1;j<n;++j)
		{
			newregister[j]=reg[j-1];
		}
		memcpy(reg,newregister,n);
		mseq[i]=reg[n-1];
	}
	delete[] newregister;
	delete[] reg;
	return mseq;
}

void output(uint8_t i)
{
	if(i==0)
		cout<<0;
	else if(i==1)
		cout<<1;
	else
		cout<<"wrong number";
}

uint8_t* get2Binary(qulonglong num)
{
	static uint8_t *buf=new uint8_t[64];

	memset(buf,0,64);
	for(int i=0;i<64;++i)
	{
		buf[i]=static_cast<uint8_t>(num>>i & 0x01);
	}
	return buf;
}

void generateRandomSeries(uint8_t* rtnseq,int length)
{
	unsigned seed1=chrono::system_clock::now().time_since_epoch().count;
	mt19937_64 g1(seed1);
	int rounds=length/64;
	int remainder=length%64;
	
	for(int i=0;i<rounds;++i)
	{
		memcpy(rtnseq+i*64,get2Binary(g1()),64);
	}
	uint8_t *buf;
	memcpy(rtnseq+rounds*64,buf=get2Binary(g1()),remainder);
	delete[] buf;
}

int generateMseq(uint8_t* rtnSeq,int length)
{
	int n=25;
	unsigned N=pow(2,n)-1;
	uint8_t fbconnection[] = {1,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,1};
	uint8_t* srcSeq=mSequence(fbconnection,n);
	unsigned whatevernum=chrono::system_clock::now().time_since_epoch().count;
	unsigned start=whatevernum%N;
	if((start+length)<=N)
	{
		memcpy(rtnSeq,srcSeq+start,length);
	}
	else
	{
		memcpy(rtnSeq,srcSeq+start,N-start);
		unsigned i=0;
		for(;i<(start+length-N)/N;++i)
		{
			memcpy(rtnSeq+N-start+N*i,srcSeq,N);
		}
		memcpy(rtnSeq+N-start+N*i,srcSeq,(start+length-N)%N);
	}
	delete[] srcSeq;
	return 0;
}

