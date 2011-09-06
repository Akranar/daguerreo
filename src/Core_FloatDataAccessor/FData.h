#ifndef FDATA_H_
#define FDATA_H_

#define FDATA1(x) ((FData1 &) x)
#define FDATA2(x) ((FDATA2 &) x)
#define FDATA3(x) ((FDATA3 &) x)
#define FDATA4(x) ((FDATA4 &) x)
#define FDATA5(x) ((FDATA5 &) x)
#define FDATA6(x) ((FDATA6 &) x)
#define FDATA7(x) ((FDATA7 &) x)
#define FDATA8(x) ((FDATA8 &) x)
#define FDATA9(x) ((FDATA9 &) x)
#define FDATA10(x) ((FDATA10 &) x)
#define FDATA11(x) ((FDATA11 &) x)
#define FDATA12(x) ((FDATA12 &) x)
#define FDATA13(x) ((FDATA13 &) x)
#define FDATA14(x) ((FDATA14 &) x)
#define FDATA15(x) ((FDATA15 &) x)

class FData1
{
	float data[1];
public:
	FData1(float f0)
	{
		data[0] = f0;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData2
{
	float data[2];
public:
	FData2(float f0, float f1)
	{
		data[0] = f0, data[1] = f1;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData3
{
	float data[3];
public:
	FData3(float f0, float f1, float f2)
	{
		data[0] = f0, data[1] = f1, data[2] = f2;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData4
{
	float data[4];
public:
	FData4(float f0, float f1, float f2, float f3)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData5
{
	float data[5];
public:
	FData5(float f0, float f1, float f2, float f3, 
		float f4)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData6
{
	float data[6];
public:
	FData6(float f0, float f1, float f2, float f3, 
		float f4, float f5)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData7
{
	float data[7];
public:
	FData7(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData8
{
	float data[8];
public:
	FData8(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData9
{
	float data[9];
public:
	FData9(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData10
{
	float data[10];
public:
	FData10(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData11
{
	float data[11];
public:
	FData11(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9, float f10)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9, data[10] = f10;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData12
{
	float data[12];
public:
	FData12(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9, float f10, float f11)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9, data[10] = f10, data[11] = f11;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData13
{
	float data[13];
public:
	FData13(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9, float f10, float f11, 
		float f12)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9, data[10] = f10, data[11] = f11;
		data[12] = f12;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData14
{
	float data[14];
public:
	FData14(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9, float f10, float f11, 
		float f12, float f13)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9, data[10] = f10, data[11] = f11;
		data[12] = f12, data[13] = f13;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData15
{
	float data[15];
public:
	FData15(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9, float f10, float f11, 
		float f12, float f13, float f14)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9, data[10] = f10, data[11] = f11;
		data[12] = f12, data[13] = f13, data[14] = f14;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};

class FData16
{
	float data[16];
public:
	FData16(float f0, float f1, float f2, float f3, 
		float f4, float f5, float f6, float f7, 
		float f8, float f9, float f10, float f11, 
		float f12, float f13, float f14, float f15)
	{
		data[0] = f0, data[1] = f1, data[2] = f2, data[3] = f3;
		data[4] = f4, data[5] = f5, data[6] = f6, data[7] = f7;
		data[8] = f8, data[9] = f9, data[10] = f10, data[11] = f11;
		data[12] = f12, data[13] = f13, data[14] = f14, data[15] = f15;
	};
	float & operator[] (int index)
	{
		return data[index];
	};
};


#endif