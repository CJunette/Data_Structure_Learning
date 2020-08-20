// 004_Homework_4-19_Array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	FILE *fout;
	fout = fopen("permutation8.txt", "w");
	
	/*
	for(int a1 = 0; a1 <= 9; a1++)
	{
		for(int a2 = 0; a2 <= 8; a2++)
		{
			for(int a3 = 0; a3 <= 7; a3++)
			{
				for(int a4 = 0; a4 <= 6; a4++)
				{
					for(int a5 = 0; a5 <= 5; a5++)
					{
						for(int a6 = 0; a6 <= 4; a6++)
						{
							for(int a7 = 0; a7 <= 3; a7++)
							{
								for(int a8 = 0; a8 <= 2; a8++)
								{
									for(int a9 = 0; a9 <= 1; a9++)
									{
										if((a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) == 9)
										{
											fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, a6, a5, a4, a3, a2, a1);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	*/
	
	for(int a2 = 0; a2 <= 8; a2++)
	{
		for(int a3 = 0; a3 <= 7; a3++)
		{
			for(int a4 = 0; a4 <= 6; a4++)
			{
				for(int a5 = 0; a5 <= 5; a5++)
				{
					for(int a6 = 0; a6 <= 4; a6++)
					{
						for(int a7 = 0; a7 <= 3; a7++)
						{
							for(int a8 = 0; a8 <= 2; a8++)
							{
								for(int a9 = 0; a9 <= 1; a9++)
								{
									if((a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) == 8)
									{
										fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, a6, a5, a4, a3, a2, 0);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	/*
	for(int a3 = 0; a3 <= 7; a3++)
	{
		for(int a4 = 0; a4 <= 6; a4++)
		{
			for(int a5 = 0; a5 <= 5; a5++)
			{
				for(int a6 = 0; a6 <= 4; a6++)
				{
					for(int a7 = 0; a7 <= 3; a7++)
					{
						for(int a8 = 0; a8 <= 2; a8++)
						{
							for(int a9 = 0; a9 <= 1; a9++)
							{
								if((a3 + a4 + a5 + a6 + a7 + a8 + a9) == 7)
								{
									fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, a6, a5, a4, a3, 0, 0);
								}
							}
						}
					}
				}
			}
		}
	}
	*/
	/*
	for(int a4 = 0; a4 <= 6; a4++)
	{
		for(int a5 = 0; a5 <= 5; a5++)
		{
			for(int a6 = 0; a6 <= 4; a6++)
			{
				for(int a7 = 0; a7 <= 3; a7++)
				{
					for(int a8 = 0; a8 <= 2; a8++)
					{
						for(int a9 = 0; a9 <= 1; a9++)
						{
							if((a4 + a5 + a6 + a7 + a8 + a9) == 6)
							{
								fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, a6, a5, a4, 0, 0, 0);
							}
						}
					}
				}
			}
		}
	}
	*/
	/*
	for(int a5 = 0; a5 <= 5; a5++)
	{
		for(int a6 = 0; a6 <= 4; a6++)
		{
			for(int a7 = 0; a7 <= 3; a7++)
			{
				for(int a8 = 0; a8 <= 2; a8++)
				{
					for(int a9 = 0; a9 <= 1; a9++)
					{
						if((a5 + a6 + a7 + a8 + a9) == 5)
						{
							fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, a6, a5, 0, 0, 0, 0);
						}
					}
				}
			}
		}
	}
	*/
	/*
	for(int a6 = 0; a6 <= 4; a6++)
	{
		for(int a7 = 0; a7 <= 3; a7++)
		{
			for(int a8 = 0; a8 <= 2; a8++)
			{
				for(int a9 = 0; a9 <= 1; a9++)
				{
					if((a6 + a7 + a8 + a9) == 4)
					{
						fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, a6, 0, 0, 0, 0, 0);
					}
				}
			}
		}
	}
	*/
	/*
	for(int a7 = 0; a7 <= 3; a7++)
	{
		for(int a8 = 0; a8 <= 2; a8++)
		{
			for(int a9 = 0; a9 <= 1; a9++)
			{
				if((a7 + a8 + a9) == 3)
				{
					fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, a7, 0, 0, 0, 0, 0, 0);
				}
			}
		}
	}
	*/
	/*
	for(int a8 = 0; a8 <= 2; a8++)
	{
		for(int a9 = 0; a9 <= 1; a9++)
		{
			if((a8 + a9) == 2)
			{
				fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, a8, 0, 0, 0, 0, 0, 0, 0);
			}
		}
	}
	*/
	/*
	for(int a9 = 0; a9 <= 1; a9++)
	{
		if((a9) == 1)
		{
			fprintf(fout, "{%d, %d, %d, %d, %d, %d, %d, %d, %d}, \n", a9, 0, 0, 0, 0, 0, 0, 0, 0);

		}
	}
	*/
}