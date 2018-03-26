/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

#include <stdio.h>

#include <math.h>

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // if n is non-positive, return false
    if (n < 0) {
        printf("n must be positive\n");
        return false;
    }
    
    //
    int left = 0;
    int right = n - 1;
    
    // iterate through "values" to find n
    while (right >= left) {
        
        int middle = ((right + left) / 2);
        
        //look at the middle of the list
        //return true if number is found
        if (values[middle] == value) {
            return true;
        }
        
        //else if number higher, search left
        else if (values[middle] > value) {
            right = middle - 1;
        }
    
        //else if number lower, search right
        else if (values[middle] < value) {
            left = middle + 1;
        }

    } //end of while loop
    
    return false; //this was here originally
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // selection sort
    // iterate through each element in array "values"
    // start at first value of i = 0
    for (int i = 0; i < (n - 1); i++) {
        
        // assume values[0] is the minimum initially
        int min = i;
        
        // j loop is used to locate min value
        for (int j = (i + 1); j < n; j++)
        
            if (values[j] < values[min]) {
                min = j;
            }
        
        // if min is not equal to current i
        // use a temporary variable to swap
        if (min != i) {
            int tempMin = values[i];
            values[i] = values[min];
            values[min] = tempMin;
        }
            
    }
    
} //end of sort function
