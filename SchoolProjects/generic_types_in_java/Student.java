//Robert Thackston
public class Student<N, A, G> {
    // Variables: Generic variables used for the 3 values a Student contains.
    private N name;
    private A id;
    private G grade;

    public static void main(String args[]) {
        Student<String,Integer,String> studentOne = new Student<String,Integer,String>();
        Student<String,Integer,String> studentTwo = new Student<String,Integer,String>("Tom", 902399185, "Junior");
        Student<String,String,Integer> studentThree = new Student<String,String,Integer>("Fred", "Hawkeye", 6);
        System.out.println(studentOne);
        studentOne.setStudent("Ted", 902357689, "Senior");
        System.out.println(studentOne);
        System.out.println(studentTwo);
        System.out.println(studentThree);
        studentThree.getStudent();
    }
    /**
     * empty constructor.
     * if called to print, displays null for all values.
     */
    public Student(){}
    /**
     * Student constructor
     * 
     * @param n Generic name value
     * @param a Generic id value
     * @param g Generic grade value.
     */
    public Student(N n, A a, G g){
        this.name = n;
        this.id = a;
        this.grade = g;
    }
    /**
     * Setter for the student object.
     * used to change the value of a Student object without creating a new Student.
     * 
     * @param n Generic name value
     * @param a Generic id value
     * @param g Generic grade value.
     */
    public void setStudent(N n, A a, G g) {
        this.name = n;
        this.id = a;
        this.grade = g;
    }
    /**
     * Returns an array of the values the Student contains.
     */
    public Object[] getStudent() {
        Object[] studentArray = new Object[3];
        studentArray[0] = this.name;
        studentArray[1] = this.id;
        studentArray[2] = this.grade;
        return studentArray;
    }
    /** 
     * returns a string representation of the Student object.
     */
    public String toString() {
        String studentString = "Name: " + this.name + " Id: " + this.id + " Grade: " + this.grade;
        return studentString;
    }
  
}