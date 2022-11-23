#include "ReducedConcatStringTree.h"

int LitStringHash::hashFucntion(string const key) const
{
    if (key == "") throw ("Cannot hash a null string");
    int power = 1;
    int res = 0;
    for (int i = 0; i < key.length(); i++)
    {
        res += (int)key[i] * power;
        power *= config.p;
    }
    res %= this->size;
    return res;
}
int LitStringHash::probe(std::string const key, int i) const
{
	return (int)(hashFucntion(key) + i*config.c1 + i*i*config.c2) % size; //có thể sai vì đã ép kiểu
}
int LitStringHash::search (std::string const s) const
{
	int index{};
	//LitString newS(s);
	for (int i = 0; i < size; i++)
	{
		index = probe(s, i);
		if (status[index] == NIL) return -1;

		else if (status[index] == FILLED)
            if (data[index].str == s) return index;
	}
	return -1;
}
int  LitStringHash:: insert(string const s) //không cho phép insert trùng nhau
{
	if (!data) allocateMem();

	int temp = search(s);
	if (temp != -1) return temp; //đã có chuỗi s->str, không insert nữa

	int index{};
	for (int i = 0; i < size; i++)
	{
		index = probe(s, i);
		if (status[index] !=  FILLED)
		{
			data[index] = LitString(s);
			status[index] =  FILLED;
            //return index;
			break;
		}
		if (i == size-1 && status[index] == FILLED) throw runtime_error("No possible slot");
	}
	//đến đây chắc chắn phải thêm được
	int currSize = 0;
	for (int i = 0; i < size; i++)	if (status[i] == FILLED) currSize++;
	if ( (double)currSize/size > config.lambda) rehash();
	return index;
}
int  LitStringHash::remove (string const s)
{
	if (search(s) == -1) throw ("Not found!");

	int index{};
	for (int i = 0; i < size; i++)
	{
		index = probe(s,i);
		if  (status[index] == NIL)  throw ("Not found!");
		else if ( status[index] == FILLED)
			if (data[index].str == s)
			{
				status[index] = DELETED;
				break;
			}
	}

	int currSize = 0;
	for (int i = 0; i < size; i++)	if (status[i] == FILLED) currSize++;
	if (currSize == 0) dellocateMem();
	return index; //vị trí vừa xóa
}



void LitStringHash::allocateMem()
{
	if (!data)
	{
		size = config.initSize;
		data = new LitString[size]{};
		status = new STATUS_TYPE[size]{};
		for (int i = 0; i < size; i++) status[i] = NIL;
	}
}
void LitStringHash::dellocateMem()
{
	if (data)
	{
		delete[] data;
		delete[] status;
		data = nullptr;
		status = nullptr;
		size = 0;
	}
}
void LitStringHash::rehash()
{
	auto temp = new LitString[size * config.alpha]{};
	for (int i = 0; i <size; i++) temp[i] = data[i];
	delete[] data;
	data = temp;

	auto tempst = new STATUS_TYPE [size * config.alpha]{};
	for (int i = 0; i <size; i++) tempst[i]= status[i];
	for (int i = size; i < size * config.alpha; i++) tempst[i] = NIL;
	delete[] status;
	status = tempst;

	size = size * config.alpha;
}

bool LitString:: operator==(LitString const &other)
{
	if (this->str == other.str) return true;
	return false;
}


ReducedConcatStringTree::ReducedConcatStringTree(const char * s, LitStringHash * litStringHash) : litStringHash{litStringHash}
{
	//LitStringHash::LitString litS(s);
	int index = litStringHash->insert(s);
	litStringHash->data[index];//chứa phần tử mà ta mới thêm vô
	root = new node(&(litStringHash->data[index]));
}

