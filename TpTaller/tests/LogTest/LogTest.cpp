#include <model/Logs/Logs.h>

int main()

{
	Logs* ml= new Logs("Error en archivo");
	//ml->Logs("ahora kapas juega mejor");
	//ml->Logs("ya fue que se rompa");
	delete ml;
	return 0;
}
