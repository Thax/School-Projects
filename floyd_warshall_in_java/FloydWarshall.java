//Robert Thackston
//rthackston3
//I worked on this assignment alone, using only course materials.
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class FloydWarshall {

 /** matrix represents the graph we want to minimize */
 private int[][] matrix;
 
 /** represents INFINITY (15 million for our purpose)*/
 private final Integer INF = 150000000;
 
 /** size of the rows and columns */
 private int matrixSize;
 
 /** represents the distance matrix */
 private int[][] distance;
 
 /** represents the predecessor matrix used in Floyd's Algorithm */
 private int[][] predecessor;
 
 public static void main(String[] args) {
     FloydWarshall floyd = new FloydWarshall("samplegraph.txt");
     floyd.doFloyds();
     System.out.println(floyd);
     System.out.println(floyd.shortestPath(0, 1));
 }
 
 /**
  * constructor
  * @param filename from where to get
  * the values of the matrix;
  */
 public FloydWarshall(String filename) {
  try {
   BufferedReader bf = new BufferedReader(new FileReader(new File(filename)));
   String line;
   int index = 0;
   matrixSize = 0;
   while( (line = bf.readLine())  != null) {
    if(matrixSize == 0) {
     matrixSize = Integer.parseInt(line);
     matrix = new int[matrixSize][matrixSize];
     distance = new int[matrixSize][matrixSize];
     predecessor = new int[matrixSize][matrixSize];
     continue;
    }
    String[] temp = line.split("\\s+");
    for(int i = 0; i < matrixSize; i++) {
     if(temp[i].equalsIgnoreCase("INF")) {
      matrix[index][i] = INF;
     } else {
      matrix[index][i] = Integer.parseInt(temp[i]);
     }
    }
    index++;
   }
  } catch (FileNotFoundException exc) {
   System.out.println("File " + filename + " was not found");
   System.exit(0);
  } catch (IOException io) {
   io.printStackTrace();
   System.exit(0);
  }
  for(int i = 0; i < matrix.length; i++) {
   for(int j = 0; j < matrix.length; j++) {
    distance[i][j] = matrix[i][j];
    predecessor[i][j] = -1;
    if(distance[i][j] > 0 && distance[i][j] < INF) {
     predecessor[i][j] = i;
    }
   }
  } 
 } /*end constructor */
 
 /** 
  * getter for Matrix
  * @return the matrix
  */
 public int[][] getMatrix() {
  return matrix;
 }
 
 /**
  * getter for Distance
  * @return the distance matrix
  */
 public int[][] getDistance() {
  return distance; 
 }
 
 /**
  * getter for Predecessor
  * @return the predecessor matrix
  */
 public int[][] getPredecessor() {
  return predecessor;
 }
 
 /**
  * perform Floyd's Algorithm
  * on the adjacency matrix and fills up
  * the distance and predecessor matrix
  */
 public void doFloyds() {
     for (int k = 0; k < matrixSize; k++) {
         for (int i = 0; i < matrixSize; i++) {
             for (int j = 0; j < matrixSize; j++) {
                 if (this.getDistance()[i][j] > (this.getDistance()[i][k] + this.getDistance()[k][j])) {
                     this.getDistance()[i][j] = this.getDistance()[i][k] + this.getDistance()[k][j];
                     this.getPredecessor()[i][j] = k;
                 }
             }
         }
     }
 } 
  

   
 /**
  * return a string representation of the shortest path 
  * between start and end
  * @param start the starting point
  * @param end the ending point
  * @return the string representation
  */
 public String shortestPath(int start, int end) {
     String result = "";
     result += shortestPathHelper(start, end, result);
     result += end;
     return result;
 }
 /**
  * helper method for shortestPath.  Put in place to allow recursive calls without reinstantiating result string.
  * @param start the starting point
  * @param end the ending point
  * @param result the result string
  * @return an intermediary string representation
  */
 public String shortestPathHelper(int start, int end, String result){
     if (start == end)
         return "Path is null.";
     if (this.getPredecessor()[start][end] == start) {
         return start + "-";
     }
     else {
         result += shortestPathHelper(start, getPredecessor()[start][end], result);
         result += shortestPathHelper(getPredecessor()[start][end], end, result);
     }
     return result;
 }
 
 /**
  * toString representation of Floyd's Graph
  * @return the string representation
  */
 public String toString() {
  String result = "The original matrix is:\n";
  for(int i = 0; i < matrix.length; i++) {
   for(int j = 0; j < matrix.length; j++) {
    if(matrix[i][j] == INF) {
     result += "INF ";
     continue;
    }
    result += matrix[i][j] + " ";
   }
   result += "\n";
  }
  result += "The Distance matrix is:\n";
  for(int i = 0; i < distance.length; i++) {
   for(int j = 0; j < distance.length; j++) {
    if(distance[i][j] == INF) {
     result += "INF ";
     continue;
    }
    result += distance[i][j] + " ";
   }
   result += "\n";
  }
  result += "The Predecessor matrix is:\n";
  for(int i = 0; i < predecessor.length; i++) {
   for(int j = 0; j < predecessor.length; j++) {
    if(predecessor[i][j] == INF) {
     result += "INF ";
     continue;
    }
    result += predecessor[i][j] + " ";
   }
   result += "\n";
  }
  return result;
 } /*end toString method */
 
} /*end class*/

