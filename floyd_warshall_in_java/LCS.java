//Robert Thackston
//rthackston3
//I worked on this assignment alone, using only course materials.
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.IOException;
import java.lang.Math;

public class LCS {
    
    public static void main(String[] args) {
        LCS largest = new LCS("sampletext.txt", "sampletext1.txt");
        System.out.println(largest.LCSlength());
        System.out.println(largest.LCStext());
    }
    
    private int[][] comparison;
    private String sub1 = "", sub2 = "";
    
 /**
  * Constructor for Longest Common subsequence
  * @param filename1
  * @param filename2
  */
 public LCS(String filename1, String filename2) {
     try{
         Scanner scan1 = new Scanner(new File(filename1));
         Scanner scan2 = new Scanner(new File(filename2));
         while (scan1.hasNext()) {
             sub1 += scan1.nextLine();
         }
         while (scan2.hasNext()) {
             sub2 += scan2.nextLine();
         }
     } catch (FileNotFoundException exc) {
         System.out.println("File was not found");
         System.exit(0);
     } catch (IOException io) {
         io.printStackTrace();
         System.exit(0);
     }
     comparison = new int[sub1.length()+1][sub2.length()+1];
     for (int i = 0; i < sub1.length() +1; i++) {
         for (int j = 0; j < sub2.length() +1; j++) {
             if (i == 0 || j == 0)
                 comparison[i][j] = 0;
             else if (sub1.charAt(i-1) == sub2.charAt(j-1)) {
                 comparison[i][j] = comparison[i-1][j-1] + 1;
             }
             else {
                 comparison[i][j] = Math.max(comparison[i-1][j], comparison[i][j-1]);
             }
         }
     }
 }
 /**
  * traces and returns the length of a longest common subsequence.
  * @return the length of the longest common subsequence
  */
 public int LCSlength() {
     return comparison[sub1.length()][sub2.length()];
 }
 /**
  * constructs a string representation of the Longest common subsequence.
  * @return a longest common subsequence.
  */
 public String LCStext() {
     String returnVal = "";
     int i = sub1.length(), j = sub2.length();
     while (i != 0 && j != 0) {
         if (sub1.charAt(i-1) == sub2.charAt(j-1)) {
             returnVal = sub1.charAt(i-1) + returnVal;
             i--;
             j--;
         }
         else {
             if (comparison[i-1][j] >= comparison[i][j-1]) {
                 i--;
             }
             else if (comparison[i][j-1] > comparison[i-1][j]) {
                 j--;
             }
         }
     }    
     return returnVal;
 }
}
