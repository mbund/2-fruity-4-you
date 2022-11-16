#include "FEHRandom.h"
#include "FEHLCD.h"

FEHRandom Random;

void FEHRandom::Initialize()
{
	// This function is only present to make the code compile and match the header file
}

void FEHRandom::Seed()
{
	// This function is only present to make the code compile and match the header file
}

int FEHRandom::RandInt()
{
	return rand(); // steal rand from stdlib.h
}