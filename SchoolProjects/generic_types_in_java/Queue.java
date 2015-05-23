//Robert Thackston
public class Queue {
    // Variable: the array representation of the Queue used throughout the object.
    private Student[] studentQueue;
    // Variable: a placeholder student object used for moving objects around.
    private Student tempStudent;
    // Variable: a placeholder Student array used for moving objects around.
    private Student[] tempArray;
    // Variable: counting value for certain methods.
    private int queueSize;
    // Variable: String representation of the queue.
    private String queueString;

    public static void main(String args[]) {
        Queue queueOne = new Queue(new Student<String, Integer, String>("Tom", 902399185, "Junior"));
        Queue queueTwo = new Queue();
        
        queueOne.enqueue(new Student<String, Integer, String>("Bill", 902468514, "Freshman"));
        System.out.println(queueOne.printQueue());
        System.out.println("------------------------------");
        queueOne.dequeue();
        System.out.println(queueOne.printQueue());
        System.out.println("------------------------------");
        queueOne.enqueue(new Student<String, Integer, String>("Marsha", 902986427, "Sophomore"));
        System.out.println(queueOne.printQueue());
        System.out.println("------------------------------");
        System.out.println(queueTwo.printQueue());
        System.out.println("------------------------------");
        queueTwo.enqueue(new Student<String,Integer,String>());
        System.out.println(queueTwo.printQueue());
        System.out.println("------------------------------");
        queueTwo.dequeue();
        System.out.println(queueTwo.printQueue());
        System.out.println("------------------------------");
    }
    /** 
     * constructor for an empty Queue.  Prints a 'No Students' line if its printQueue is called.
     */
    public Queue() {
        studentQueue = new Student[0];
    }
    /**
     * Constructor for a Queue with one student.
     * Adds the student to index zero of the array.
     * 
     * @param student The student first placed into the array.
     */
    public Queue(Student student) {
        studentQueue = new Student[1];
        studentQueue[0] = student;
    }
    /**
     * Adds a student to the tail end of the array.
     * If the array has no empty spaces, enqueue creates a temporary array of size one cell greater than the original array.
     * The Student is placed into this array which is then reassigned to the original variable.
     * 
     * @param student The student object added to the array.
     */
    public void enqueue(Student student) {
        if (this.getSize() > 0 && this.getSize() == studentQueue.length){
            tempArray = new Student[this.getSize() + 1];
            for (int n = 0; n < this.getSize(); n++) {
                tempArray[n] = studentQueue[n];
            }
            tempArray[this.getSize()] = student;
            studentQueue = tempArray;
        } else if (this.getSize() == 0 && studentQueue.length == 0) {
            studentQueue = new Student[1];
            studentQueue[0] = student;
        } else {
            studentQueue[this.getSize()] = student;
        }
    }
    /**
     * Removes the Student at the head end of the array, and then moves all remaining students one cell closer to the beginning of the array to compensate.
     */
    public Student dequeue() {
        if (this.getSize() == 0){
            return null;
        }
        tempStudent = studentQueue[0];
        for (int k = 1; k < this.getSize(); k++) {
            studentQueue[k-1] = studentQueue[k];
        }
        studentQueue[this.getSize() - 1] = null;
        return tempStudent;
    }
    /**
     * Returns the size of the Queue.
     * The size is found in terms of the number of Students in the array, not the number of cells.
     */
    public int getSize() {
        queueSize = 0;
        for (int i = 0; i < studentQueue.length; i++) {
            if (studentQueue[i] != null)
                queueSize++;
        }
        return queueSize;
    }
    /**
     * Returns a string representation of all the Students in the Queue.
     * If there are no Students in the Queue, returns a string object containing 'No Students'.
     */
    public String printQueue() {
        if (this.getSize() == 0)
            queueString = "No Students.";
        if (this.getSize() != 0)
            queueString = studentQueue[0].toString();
        for (int j = 1; j < this.getSize(); j++){
            queueString += "\n" + studentQueue[j].toString();
        }
        return queueString;
    }
}