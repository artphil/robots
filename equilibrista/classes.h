/* classes */

class entrada
{
	int tam = 50;
	int i;
	int direcao[50];
	unsigned long tempo[50];
public:
	void set_tam(int);
	int  get_tam();
	void reset();
	void set(int,unsigned long);
	int  get_dir(int);
	unsigned long  get_tmp(int);
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
	void set_max(unsigned long);
	int get_milis();
	int get_seg();
	void print();
};
