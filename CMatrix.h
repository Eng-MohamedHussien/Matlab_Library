#ifndef CMATRIX_H
#define CMATRIX_H
#include <iostream>
#include <cstring>
using namespace std;
class CMatrix
{
	int nR, nC;
	double** values;
public:
	CMatrix();
	~CMatrix();
	enum MI{ MI_ZEROS, MI_ONES, MI_EYE, MI_RAND, MI_VALUE };
	CMatrix(int nR, int nC, int initialization = MI_ZEROS, double initializationValue = 0.0);
	CMatrix(int nR, int nC, double first, ...);
	CMatrix(CMatrix& m);
	CMatrix(double d);
	CMatrix(string s);
	void copy(CMatrix& m);
	void copy(double d);
	void copy(string s);
	void reset();
	string getString();
	CMatrix& operator=(CMatrix& m);
	CMatrix& operator=(double d);
	CMatrix& operator=(string s);
	CMatrix& add(CMatrix& m,CMatrix& T);
	void add(CMatrix& m);
	void operator+=(CMatrix& m);
	void operator+=(double d);
	CMatrix& operator+(CMatrix& m);
	CMatrix& operator+(double d);
	CMatrix& sub(CMatrix& m,CMatrix& T);
	void sub(CMatrix& m);
	void operator-=(CMatrix& m);
	void operator-=(double d);
	CMatrix& operator-(CMatrix& m);
	CMatrix& operator-(double d);
	CMatrix& mul(CMatrix& m,CMatrix& T);
	void mul(CMatrix& m);
	void operator*=(CMatrix& m);
	void operator*=(double d);
	CMatrix& operator*(CMatrix& m);
	CMatrix& operator*(double d);
	CMatrix& div(CMatrix& m,CMatrix& T);
	void div(CMatrix& m);
	void operator/=(CMatrix& m);
	void operator/=(double d);
	CMatrix& operator/(CMatrix& m);
	CMatrix& operator/(double d);
	CMatrix& operator++(); //Pre Increment
	CMatrix& operator++(int); //Post Increment, int is not used
	CMatrix& operator--(); //Pre Increment
	CMatrix& operator--(int); //Post Increment, int is not used
	CMatrix& operator-();
	CMatrix& operator+();
	friend istream& operator >> (istream &is, CMatrix& C); //Stream
	friend ostream& operator << (ostream &os, CMatrix& C); //Stream
	void setSubMatrix(int iR, int iC, CMatrix& m);
	CMatrix& getSubMatrix(int r, int c, int nr, int nc);
	CMatrix& getCofactor(int r, int c,CMatrix& ret);
	void addColumn(CMatrix& m);
	void addRow(CMatrix& m);
	double& operator[](int i){ return values[i / nC][i%nC]; }
	double& operator()(int i){ return values[i / nC][i%nC]; }
	double& operator()(int r, int c){ return values[r][c]; }
	int getn(){ return nR*nC; };
	int getnR(){ return nR; };
	int getnC(){ return nC; };
	double& getDeterminant(double& value1);
    CMatrix& getTranspose(CMatrix& T);
	CMatrix& getInverse(CMatrix& a);
	};

#endif // CMATRIX_H
