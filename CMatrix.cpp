#include "CMatrix.h"
#include "stdarg.h"
#include <algorithm>
CMatrix::CMatrix()
{
	nR = nC = 0;
	values = NULL;
}
CMatrix::~CMatrix()
{
	reset();
}
CMatrix::CMatrix(int nR, int nC, int initialization, double initializationValue)
{
	this->nR = nR;
	this->nC = nC;
	if ((nR*nC) == 0){ values = NULL; return; }
	values = new double*[nR];
	for (int iR = 0; iR<nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC<nC; iC++)
		{
			switch (initialization)
			{
			case MI_ZEROS: values[iR][iC] = 0; break;
			case MI_ONES: values[iR][iC] = 1; break;
			case MI_EYE: values[iR][iC] = (iR == iC) ? 1 : 0; break;
			case MI_RAND: values[iR][iC] = (rand() % 1000000) / 1000000.0; break;
			case MI_VALUE: values[iR][iC] = initializationValue; break;
			}
		}
	}
}
CMatrix::CMatrix(int nR, int nC, double first, ...)
{
	this->nR = nR;
	this->nC = nC;
	if ((nR*nC) == 0){ values = NULL; return; }
	values = new double*[nR];
	va_list va;
	va_start(va, first);
	for (int iR = 0; iR<nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC<nC; iC++)
		{
			values[iR][iC] = (iC == 0 && iR == 0) ? first : va_arg(va, double);
		}
	}
	va_end(va);
}
CMatrix::CMatrix(CMatrix& m)
{
	nR = nC = 0;
	values = NULL;
	copy(m);
}
CMatrix::CMatrix(string s)
{
	nR = nC = 0;
	values = NULL;
	copy(s);
}
void CMatrix::copy(CMatrix& m)
{
	reset();
	this->nR = m.nR;
	this->nC = m.nC;
	if ((nR*nC) == 0){ values = NULL; return; }
	values = new double*[nR];
	for (int iR = 0; iR<nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC<nC; iC++)
		{
			values[iR][iC] = m.values[iR][iC];
		}
	}
}
CMatrix::CMatrix(double d)
{
	nR = nC = 0;
	values = NULL;
	copy(d);
}
void CMatrix::copy(double d)
{
	reset();
	this->nR = 1;
	this->nC = 1;
	values = new double*[1];
	values[0] = new double[1];
	values[0][0] = d;
}
void CMatrix::copy(string s)
{
	reset();
	char* buffer = new char[s.length() + 1];
	strcpy(buffer, s.c_str());
	char* lineContext;
	char* lineSeparators = ";\r\n";
	char* line = strtok_r(buffer, lineSeparators, &lineContext);
	while (line)
	{
		CMatrix row;
		char* context;
		char* separators = " []";
		char* token = strtok_r(line, separators, &context);
		while (token)
		{
			CMatrix item (atof(token));
			row.addColumn(item);
			token = strtok_r(NULL, separators, &context);
		}
		if (row.nC>0 && (row.nC == nC || nR == 0))
			addRow(row);
		line = strtok_r(NULL, lineSeparators, &lineContext);
	}
	delete[] buffer;
}
void CMatrix::reset()
{
	if (values)
	{
		for (int i = 0; i<nR; i++)
			delete[] values[i];
		delete[] values;
	}
	nR = nC = 0;
	values = NULL;
}
string CMatrix::getString()
{
	string s;
	for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
			char buffer[50];
			snprintf(buffer, 50, "%g\t", values[iR][iC]);
			s += buffer;
		} s
			+= "\n";
	}
	return s;
}
CMatrix& CMatrix::operator=(CMatrix& m)
{
	copy(m);
	return *this;
}
CMatrix& CMatrix::operator=(double d)
{
	copy(d);
	return *this;
}
CMatrix& CMatrix::operator=(string s)
{
	copy(s);
	return *this;
}
CMatrix& CMatrix::add(CMatrix& m,CMatrix& T)
{
    for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
            T.values[iR][iC] = 0;
        }
    }
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		T.values[iR][iC]=values[iR][iC] + m.values[iR][iC];
    return T;
}
void CMatrix::add(CMatrix& m)
{
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		values[iR][iC] += m.values[iR][iC];
}
void CMatrix::operator+=(CMatrix& m)
{
	add(m);
}
void CMatrix::operator+=(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
	add(a);
}
CMatrix& CMatrix::operator+(CMatrix& m)
{
   static CMatrix T(nR,nC);
    return add(m,T);
}
CMatrix& CMatrix::operator+(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
    static CMatrix T(nR,nC);
    return add(a,T);
}
CMatrix& CMatrix::sub(CMatrix& m,CMatrix& T)
{
	for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
            T.values[iR][iC] = 0;
        }
    }
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		T.values[iR][iC]=values[iR][iC] - m.values[iR][iC];
    return T;
}
void CMatrix::sub(CMatrix& m)
{
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		values[iR][iC] -= m.values[iR][iC];
}
void CMatrix::operator-=(CMatrix& m)
{
	sub(m);
}
void CMatrix::operator-=(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
	sub(a);
}
CMatrix& CMatrix::operator-(CMatrix& m)
{
	 static CMatrix T(nR,nC);
    return sub(m,T);
}
CMatrix& CMatrix::operator-(double d)
{
	CMatrix a (nR, nC, MI_VALUE, d);
	static CMatrix T(nR,nC);
    return sub(a,T);
}
CMatrix& CMatrix::mul(CMatrix& m,CMatrix& T)
{
    for (int R = 0; R<T.nR; R++)
	{
		for (int C = 0; C<T.nC; C++)
		{
            T.values[R][C] = 0;
        }
    }
	if (nC != m.nR)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<T.nR; iR++)
	for (int iC = 0; iC<T.nC; iC++)
	{
		for (int k = 0; k<m.nR; k++)
			T.values[iR][iC] += values[iR][k] * m.values[k][iC];
	}
	return T;
}
void CMatrix::mul(CMatrix& m)
{
	if (nC != m.nR)
		throw("Invalid matrix dimension");
	CMatrix r(nR, m.nC);
	for (int iR = 0; iR<r.nR; iR++)
	for (int iC = 0; iC<r.nC; iC++)
	{
		r.values[iR][iC] = 0;
		for (int k = 0; k<m.nR; k++)
			r.values[iR][iC] += values[iR][k] * m.values[k][iC];
	}
	copy(r);
}
void CMatrix::operator*=(CMatrix& m)
{
	mul(m);
}
void CMatrix::operator*=(double d)
{
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		values[iR][iC] *= d;
}
CMatrix& CMatrix::operator*(CMatrix& m)
{
	static CMatrix T(nR,m.nC);
    return mul(m,T);
}
CMatrix& CMatrix::operator*(double d)
{
	CMatrix a (nR, nC, MI_VALUE, d);
	static CMatrix T(nC,a.nR);
    return mul(a,T);
}
CMatrix& CMatrix::operator++()
{
    CMatrix a (nR, nC, MI_VALUE, 1.0);
	add(a);
	return *this;
}
CMatrix& CMatrix::operator++(int)
{
	CMatrix a (nR, nC, MI_VALUE, 1.0);
	add(a);
	return *this;
}
CMatrix& CMatrix::operator--()
{
    CMatrix a (nR, nC, MI_VALUE, -1.0);
	add(a);
	return *this;
}
CMatrix& CMatrix::operator--(int)
{
	CMatrix a (nR, nC, MI_VALUE, -1.0);
	add(a);
	return *this;
}
CMatrix& CMatrix::operator-()
{
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		values[iR][iC] = -values[iR][iC];
	return *this;
}
CMatrix& CMatrix::operator+()
{
	return *this;
}
void CMatrix::setSubMatrix(int r, int c, CMatrix& m)
{
	if ((r + m.nR)>nR || (c + m.nC)>nC)throw("Invalid matrix dimension");
	for (int iR = 0; iR<m.nR; iR++)
	for (int iC = 0; iC<m.nC; iC++)
		values[r + iR][c + iC] = m.values[iR][iC];
}
CMatrix& CMatrix::getSubMatrix(int r, int c, int nr, int nc)
{
	if ((r + nr)>nR || (c + nc)>nC)throw("Invalid matrix dimension");
	CMatrix m(nr, nc);
	for (int iR = 0; iR<m.nR; iR++)
	for (int iC = 0; iC<m.nC; iC++)
		m.values[iR][iC] = values[r + iR][c + iC];
    *this=m;
	return *this;
}
void CMatrix::addColumn(CMatrix& m)
{
	CMatrix n(max(nR, m.nR), nC + m.nC);
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(0, nC, m);
	*this = n;
}
void CMatrix::addRow(CMatrix& m)
{
	CMatrix n(nR + m.nR, max(nC, m.nC));
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(nR, 0, m);
	*this = n;
}
CMatrix& CMatrix::getCofactor(int r, int c,CMatrix& ret )
{
	if (nR <= 1 && nC <= 1)throw("Invalid matrix dimension");
	int s=nR - 1;
	int o=nC - 1;
	CMatrix m(s ,o);
	for (int iR = 0; iR<m.nR; iR++)
	for (int iC = 0; iC<m.nC; iC++)
	{
		int sR = (iR<r) ? iR : iR + 1;
		int sC = (iC<c) ? iC : iC + 1;
		m.values[iR][iC] = values[sR][sC];
	}
	ret=m;
	return ret;
}
double& CMatrix::getDeterminant(double& value1)
{
	if (nR != nC)throw("Invalid matrix dimension");
	if (nR == 2&& nC == 2)
	{
	value1= (this->values[0][0]*this->values[1][1])-(this->values[0][1]*this->values[1][0]);
	return value1;
	}
	if (nR >=3 && nC >= 3)
	{
	value1=0;
	int m=1;
	int iC;
	double temp;
	CMatrix x;
	for ( iC = 0; iC<nC; iC++)
	{
        temp=0;
		value1 +=m * values[0][iC] * getCofactor(0, iC,x).getDeterminant(temp);
		m*=-1;
	}
	return value1;
	}
}
istream& operator >> (istream &is, CMatrix& m)
{
	string s;
	getline(is, s, ']');
	s += "]";
    CMatrix v (s);
	m = v;
	return is;
}
ostream& operator << (ostream &os, CMatrix& m)
{
	os << m.getString();
	return os;
}

CMatrix& CMatrix:: getTranspose(CMatrix& T){
    for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
            T.values[iR][iC] = 0;
        }
    }
    for(int i=0;i<nR;i++)
    {
        for(int j=0;j<nC;j++)
        {
            T.values[j][i]=this->values[i][j];
        }
    }
    return T;
}
CMatrix& CMatrix::getInverse(CMatrix& a){
for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
            a.values[iR][iC] = 0;
        }
    }
if(nC == 0||nR == 0) throw("Invalid matrix dimension");
else if(nC==1&&nR==1)
{
    CMatrix c(nR,nC);
    c.values[0][0]=1/values[0][0];
    a=c;
}
else if(nC==2&&nR==2)
    {
    CMatrix c(nR,nC);
    double d ;
    d= 1/getDeterminant(d);
    c.values[0][0]=this->values[1][1]*d;
    c.values[1][1]=this->values[0][0]*d;
    c.values[0][1]=-1*this->values[0][1]*d;
    c.values[1][0]=-1*this->values[1][0]*d;
    a=c;
    }
else{
CMatrix c(nR,nC),b,q(nR,nC);
double d;
        for(int ir=0;ir<nR;ir++)
        {
          for(int ic=0;ic<nC;ic++)
          {
            c.values[ir][ic]=getCofactor(ir, ic,b).getDeterminant(d)*pow(-1,(ir+ic));
          }
        }
        c=c.getTranspose(q);
        c*=(1/getDeterminant(d));
        a=c;
    }
    return a;
}
CMatrix& CMatrix::div(CMatrix& m,CMatrix& T)
{
    static CMatrix w(m.nR,m.nC);
    for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
            T.values[iR][iC] = 0;
        }
    }
	if(nC != m.nR)
		throw("Invalid matrix dimension");
    T=*this*m.getInverse(w);
    return T;
}
void CMatrix::div(CMatrix& m)
{
    static CMatrix w(m.nR,m.nC);
	if(nC != m.getnR())
		throw("Invalid matrix dimension");
    *this*=m.getInverse(w);
}

void CMatrix::operator/=(CMatrix& m)
{
	div(m);
}

void CMatrix::operator/=(double d)
{
	for(int iR=0;iR<nR;iR++)
		for(int iC=0;iC<nC;iC++)
			values[iR][iC] *= 1/d;
}


CMatrix& CMatrix::operator/(CMatrix& m)
{
	static CMatrix T(nR,m.nC);
    return div(m,T);
}

CMatrix& CMatrix::operator/(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
	static CMatrix T(nR,a.nC);
    return div(a,T);
}
