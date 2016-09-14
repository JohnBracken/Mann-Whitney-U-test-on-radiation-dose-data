//The following code describes a process for performing a
//Mann-Whitney U statistical test comparing patient radiation dose data
//from two different interventional cath labs.  This is to determine
//if one cath lab typically delivers a lower radiation dose than the
//other.  This is a statistical test that ranks the data and does not
//make the assumption that the data follows a normal distribution.


//Here are the libraries that we'll need for this code.

//Input/output stream library header file.
#include <iostream>

//File stream library header file.
#include <fstream>

//C standard libary needed to use some specific functions.
//Needed for rand() function to generate random numbers.
#include <cstdlib>

//String libary header file.
#include <string>

//Math library header file.
#include <cmath>

//Time library needed to generate random number seeds.
#include <ctime>

//Number of data points in each dataset.  Set to constant value.
#define NUMPOINTS 60

//Total number of all datapoints.  Set to constant value.
#define DOUBLE_NUM 120

//Using the namespace standard so I don't need to repeat
//calling std:: every time I use certain commands.
using namespace std;


//2D arrays to hold data from each cath lab, along with the
//group each data point belongs to.
int array2D_1[2][NUMPOINTS];
int array2D_2[2][NUMPOINTS];
int array2D_total[2][DOUBLE_NUM];
int rankarray_cath15[NUMPOINTS];
int rankarray_cath6[NUMPOINTS];
int ranksum_cath15 = 0;
int ranksum_cath6 = 0;
int Uvalue_15;
int Uvalue_6;


//Function prototype used to load in the parent data
//from both cath labs into arrays from a file.
void ReadData(int Clab15[], int Group1[], int Clab6[], int Group2[], int NumPoints);

//Function prototype used to combine two 1D arrays together into a 2D array.
void CombineArray(int array1[], int array2[], int array3[], int array4[], int Numpoints);

//Function prototype to combine the 2 2D arrays of data together.
void Concatenate_arrays(int array2D_1[NUMPOINTS][NUMPOINTS], int array2D_2[NUMPOINTS][NUMPOINTS]);

//Function prototype to sort an array of data.
void DataSort(int array2D_total[2][DOUBLE_NUM]);

//Function needed to rank all the data for each group and put into separate
//1D arrays.
void RankArrays(int array2d_total[2][DOUBLE_NUM], int rankarray_cath15[], int rankarray_cath6[]);

//Function to sum the total ranks in the total number of arrays.
void Ranksum(int rankarray_cath15[], int rankarray_cath6[]);

//Function to calculate U values.
void UValue(int ranksum_cath15, int ranksum_cath6);

//Function to return a z statistical value.
int Zvalue (int Uvalue6);





//Main function
int main()
{

//Number of total data points from each parent dataset
//(will be equal in this work).
int numPoints = 60;

//Zvalue returned.
int Zval;

//Arrays containing raw parent dose data.
int CathLab15[numPoints];
int CathLab6[numPoints];
int Group1[numPoints];
int Group2[numPoints];

//Function call to read data from files and put them in an array.
//Function call doesn't need parantheses for input arguments,
//but the prototype and function definitions need the
//parentheses for the input arguements.
ReadData(CathLab15, Group1, CathLab6, Group2,numPoints);

//Sort both arrays of data from smallest to largest with the
//appropriate function.
CombineArray(CathLab15, Group1, CathLab6, Group2, numPoints);

//Combine the 2 2D arrays of data together into a large final 2D array of all the data.
Concatenate_arrays(array2D_1, array2D_2);

//Function call to sort the data completely across both datasets using the full
//2D array of data.
DataSort(array2D_total);

//Function to rank the data from each group and place these ranks into two separate
//1D arrays.
RankArrays(array2D_total, rankarray_cath15, rankarray_cath6);

//Get the sums of the ranks from both rank arrays.
Ranksum(rankarray_cath15, rankarray_cath6);

//Calculate the U values.
UValue(ranksum_cath15,ranksum_cath6);

//Calculate a z statistical value for one of the cath labs (test statistic).
Zval = Zvalue(Uvalue_6);


//Output and display the test statistic.
cout << "The Z-value from these data is: " << Zval << endl;


    return 0;
}

//Function definition to read the data from a data file into arrays.
void ReadData(int CatLab15[], int Group1[], int CatLab6[], int Group2[], int NumPoints)
    {

    //Open up a data file input stream.
    ifstream data_file1;


    //Need a separator variable.  Since data is being read from an
    //Excel.csv file, the data columns are comma delimited.  This
    //variable acts as a placeholder to allow the 2nd column to be
    //read.
    char separator;

    //Open the .csv file containing the data and store in the file
    // input stream.  File in same directory as code, so don't need
    // path.
    data_file1.open("MannWhitneyUtest_2.csv");

    //Ignore the first line in the data, since it's just titles.
    data_file1.ignore(256, '\n');

    //Fill the data arrays with the data from the file.
    //Use a for loop to cycle through each row of data.
    for(int i=0; i<NumPoints; ++i)
    {
    //Send the data from the file into each element of the arrays.
    //Can fill both arrays simultaneously.
    data_file1 >> CatLab15[i] >> separator >> Group1[i]
         >> separator >> CatLab6[i] >> separator >> Group2[i];
    }

    //Close the data file.
data_file1.close();
    }

//Function containing the insertion sort algorithm.
void DataSort(int Clab[2][DOUBLE_NUM]){

//Value pulled from the array to sort.
int value;

//Hypothetical hole left in the array
//after pulling a value from it.
int hole;

int index;

//Start going through each element in
//the array to determine if it needs to
//be sorted.
for (int i=1; i < DOUBLE_NUM;  ++i)
    {
          //Put the array element into the
          //current value being evaluated.
          value = Clab[0][i];

          index = Clab[1][i];

          //Create the hypothetical hole in the array
          //where the value was taken from.
          hole = i;

//Compare the current value with the previous element
//in the array.
while(hole>0 && Clab[0][hole-1] >value)
    {
       //If value is smaller than the array element
       //to the left, move the left array element one
       //notch to the right
       Clab[0][hole] = Clab[0][hole-1];
       Clab[1][hole] = Clab[1][hole-1];

       //The hole will now be shifted one unit to the left.
       hole = hole -1;
    }//Keeping checking all the other elements to the left of
    //the current value until you find an element that is smaller than
    //the value being evaluated.

    //Put the value being tested into where the new hole is and move
    //on to another element with the for loop.  The for loop will test
    //all the elements in the array for sorting.
    Clab[0][hole] = value;
    Clab[1][hole] = index;

    }
}

//Function used to combine 1D arrays together into a 2D array.
void CombineArray(int array1[], int array2[], int array3[], int array4[], int NumPoints)
{

    for (int i=0; i< NumPoints; ++i)
        {
         array2D_1[0][i] = array1[i];
         array2D_1[1][i] = array2[i];
         array2D_2[0][i] = array3[i];
         array2D_2[1][i] = array4[i];
        }
}


//Function used to stack 2 2D-arrays together.  This is needed to perform the data array rank task
//that is necessary in the Mann-Whitney U test.
void Concatenate_arrays(int array2D_1[NUMPOINTS][NUMPOINTS], int array2D_2[NUMPOINTS][NUMPOINTS])
{
  for(int i=0; i<NUMPOINTS; ++i){
    array2D_total[0][i] = array2D_1[0][i];
    array2D_total[1][i] = array2D_1[1][i];
    array2D_total[0][i+NUMPOINTS] = array2D_2[0][i];
    array2D_total[1][i+NUMPOINTS] = array2D_2[1][i];
  }

}


//Function to rank the array data in order, from the smallest to the largest value.
void RankArrays(int array2D_total[2][DOUBLE_NUM], int rankarray_cath15[], int rankarray_cath6[])
{
    int index1 = -1;
    int index2 = -1;

    for (int i=0; i < DOUBLE_NUM; ++i)
        {
           if(array2D_total[1][i] == 1)
           {
               ++index1;
               rankarray_cath15[index1] = i+1;


           }
           else
                  {
                ++index2;
                rankarray_cath6[index2] = i+1;
                  }
        }
}


//Function to sum the total ranks in the total number of arrays.
void Ranksum(int rankarray_cath15[], int rankarray_cath6[])
    {

        for (int i = 0;  i < NUMPOINTS; ++i)
            {
                ranksum_cath15 += rankarray_cath15[i];
                ranksum_cath6 += rankarray_cath6[i];
            }

    }



//Can now use this function to calculate the U value of the Mann-Whitney test
//and the array rank sum for each set of data is needed.
void UValue(int ranksum_cath15, int ranksum_cath6)
{
    Uvalue_15 = NUMPOINTS*NUMPOINTS + NUMPOINTS*(NUMPOINTS+1)/2 - ranksum_cath15;
    Uvalue_6 = NUMPOINTS*NUMPOINTS + NUMPOINTS*(NUMPOINTS+1)/2 - ranksum_cath6;

}

//Now we can calculate and return the Z test statistic.
int Zvalue(int Uvalue6)
{
   int  Zval;
   int mu;
   int sigma_u;

   mu = (NUMPOINTS*NUMPOINTS)/2;
   sigma_u = sqrt((NUMPOINTS*NUMPOINTS*(2*NUMPOINTS+1))/12);

   Zval = (Uvalue6-mu)/sigma_u;
   return Zval;
}


//Function used to print array data on the screen.
void PrintArray(float Clab1[], float Clab2[], int NumPoints)
{
    for(int i=0; i<NumPoints; ++i){
 //Print out the data in the arrays to make sure they got
 //filled properly.
    cout << Clab1[i] << '\t' << Clab2[i] << '\n';
    }
}
