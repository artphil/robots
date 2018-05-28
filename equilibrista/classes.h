/* classes */

class entrada
{
	int tam = 50;
	int i;
	int direcao[50];
	int *tempo[50];
public:
	void set_tam(int);
	int  get_tam();
	void reset();
	void set(int,int*);
	int  get_dir(int);
	int  get_tmp(int);
	void print();
};

class conta_tempo
{
	unsigned long tmp;
	int t_max;
public:
	conta_tempo(int max);
	bool fim();
	void reset();
	int get_tempo();
	void print();
};
