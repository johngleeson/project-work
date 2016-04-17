/*
 *
 * John Gleeson - 12162213 - Mobile Communications and Security
 * Assignment 1 - Max Vector Problem
 * Submission date - 24-09-2014
 */
import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

public class a1_12162213 {

    public static void main(String args[]) throws FileNotFoundException, IOException {

        // Start a timer that is used when the calculation is completed
        long startTime = System.currentTimeMillis();

        /* Declare and initialise a new FileInputStream that accepts a filename of type String 
         in the form args passed in from the command-line */
        FileInputStream fileStream = new FileInputStream(args[0]);

        // Create a new String to store everything that is read from the BufferedReader
        String wholeFile = "";

        /* Double used to temporarily store the vector value while it is passed from an array of
         Strings to an array of Doubles */
        double stringToDouble;

        /* Create and initialise a new BufferedReader that in turn accepts a new inputStreamReader
         as input. fileStream is then passed as the input of InputStreamReader */
        try (BufferedReader buffReader = new BufferedReader(new InputStreamReader(fileStream))) {

            // While the bufferedReader still has data
            while (buffReader.ready()) {

                // Concatenate the contents of the buffer onto the wholeFile String
                wholeFile += buffReader.readLine();

            }

            // Exception handling
        } catch (Exception e) {
            System.out.println("Error:" + e);
        }

        /* Declare and initialise a new String ArrayList that is going to hold the contents of the 
         (potentially gigantic) wholeFile String by splitting the contents at a comma */
        ArrayList<String> vectorsAsStrings = new ArrayList<>(Arrays.asList(wholeFile.split(",")));

        /* Create and initialise a new ArrayList of type Double to store the vectorsAsStrings ArrayList
         There has to be a more efficient way to do this but I couldn't figure it out */
        ArrayList<Double> doubleList = new ArrayList<>();

        /* Enhanced for loop to iterate through the vectorsAsStrings ArrayList and populate 
         the doubleList ArrayList with a parsed equivalent of each vectorsAsStrings element */
        for (String singleVector : vectorsAsStrings) {

            stringToDouble = Double.parseDouble(singleVector);
            doubleList.add(stringToDouble);
        }

        // double to store the maximum sum so far as the algorithm iterates through the ArrayList
        double cumulativeSubvector = 0.0;

        // double to store a potential candidate for the maximum sum
        double tempSubvector = 0.0;

        // int to store the array index at which calculation for the cumulativeSubvector begins
        int initialIndex = 0;

        // int to store the array index at which calculation of the cumulativeSubvector ends
        int finalIndex = 0;

        // Begin for loop to iterate through the elements in the doubleList ArrayList
        for (int i = 0; i < doubleList.size(); i++) {

            /* if the tempSubvector value is 0 and the value of the element at index i 
             is <= 0, it increments the initialIndex value and continues with the rest of the for loop.
             Ensures the algorithm works if the first value is negative.
             */
            if (tempSubvector == 0 && doubleList.get(i) <= 0) {
                initialIndex = i + 1;
                continue;
            }

            // add the value of the element at the current index to the tempSubvector
            tempSubvector += doubleList.get(i);

            /* if value of tempSubvector is greater than the cumulativeSubvector value,
             the the finalIndex value is the current value of i and the tempSubvector
             value is the cumulativeSubvector
             */
            if (tempSubvector > cumulativeSubvector) {
                finalIndex = i;
                cumulativeSubvector = tempSubvector;
            }

            /* if the value of tempSubvector is negative,
             increment the initialIndex and reset the tempSubvector value
             */
            if (tempSubvector < 0) {
                initialIndex = i + 1;
                tempSubvector = 0;
            }

        }

        /* Stop the timer that was started at the beginning of the calculation
         and find the total time of the operation
         */
        long finishTime = System.currentTimeMillis();
        long elapsedTime = finishTime - startTime;

        // Output all relevent information to the user
        System.out.println("Initial index:" + initialIndex);
        System.out.println("Final index:" + finalIndex);
        System.out.printf("Max Sub Vector: %.2f\n", cumulativeSubvector);
        System.out.println("Calculation time in seconds:" + elapsedTime / 1000);

    }

}
