/*
* --------------------------------------------------------
*	 			Last Edit By: Abdallah Ali 30-1-2016
* --------------------------------------------------------
* Fast CheckSUM algorithm (without error correction)
* Used to confirm that data is transferred correctly
* We should use the checksum on a relatively small array
* 
* ----------------- Some Notes & solves -----------------
* 
* Note1 : The original Algorithm was designed for 8-bit Microprocessors 
* Solved: by useing 8-bit (AND Mask)
* 
* ------------------- The Algorithm ----------------------
* 
* The Check-sum algorithm (simple version)
* 1- add all the array elements together (SUM) then take the 2's complement
* 2- neglect the all Most significant byte (store only the first 1 bytes)
* 3- Now the result can be used as check-sum value 
* 
* on destination sum all the elements in the array then add the checksum 
* if the result = Zero then data is transmitted correctly
* Else the data is corrupted 
*/

#include <stdio.h>

// Function Declrations

unsigned int creatCheckSUM(unsigned int arry[],unsigned int size);
unsigned int confirmCheckSUM(unsigned int arry[], unsigned int size, unsigned int checkSUM);


int main(void)
{

	/* Creat a test Array */
	unsigned int testArry[] = { 10, 20, 50, 40, 50, 60, 70, 66, 77, 88, 99, 120};

	/* Calculate the checkSUM */
	unsigned int checkCode = creatCheckSUM(testArry, sizeof(testArry));
	
	/* print the checkSUM code*/
	printf("The check sum code = %d \n", checkCode);

	/* Now confirm the array or if you want to test corrupted array
	 *  then enter wrong number in the arryy with the same checkSUM */


	unsigned int isCorrect = confirmCheckSUM(testArry, sizeof(testArry), checkCode);

	if(isCorrect) printf("The array & CheckSUM is verfied \n");
	else printf("Bad Array \n");


	return 0;
}


unsigned int creatCheckSUM(unsigned int arry[], unsigned int size)
{

	unsigned int  sum = 0;		//8-bit  Variable to store the SUM of the Array
	unsigned int counter = 0;   // Counter to index all array elements

	/* Sum of Array elements */
	for(counter = 0; counter < size; counter++)
		sum = sum + arry[counter];	// Add all elements togther
	
	/* Calulate the 2's complement */
	sum = ~sum + 1; 

	/* Apply mask to get the least byte only from the 32 bit varibale */
	sum = sum & 0x000000FF;

	return sum;		// return the 2's complement

}

unsigned int confirmCheckSUM(unsigned int arry[], unsigned int size, unsigned int checkSUM)
{
        unsigned int sum = 0;       // Variable to store the SUM of the Array
        unsigned int counter = 0;   // Counter to index all array elements

        for(counter = 0; counter < size; counter++)
                sum = sum + arry[counter];    // Add all elements togther
	
	/* Apply mask to get the least byte only from the 32 bit varibale */
	sum = sum & 0x000000FF;
	sum = checkSUM + sum;		// Add the checkSUM to the SUM
	
	/* Again, Apply mask to get the least byte only from the 32 bit varibale */
	sum = sum & 0x000000FF;

	if(sum == 0) return 1;		// If result = Zero, then data is transferred correctly
	else return 0;
}
