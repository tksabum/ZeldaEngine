#include "Guilty.h"

Guilty& Guilty::GetInstance()
{
	static Guilty instance;
	return instance;
}


Guilty::Guilty()
{

}

Guilty::~Guilty()
{

}
