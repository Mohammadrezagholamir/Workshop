package models;

/*
Explanation:
-record is a new feature in Java 14 that is used to create immutable classes.
-This class is used to represent the result of an operation.
- We use it to communicate between controllers and views to indicate the result of an operation.
- It is so simple just put a ... and a ... for it?(what do you think ? (: )
 */
public class Result {
    private final boolean isSuccsessfull;
    private final String message;
    public Result(boolean vazeiat , String message){
        this.message = message;
        this.isSuccsessfull = vazeiat;
    }
    public boolean getVazeiat(){
        return isSuccsessfull;
    }
    public String getMessage(){
        return message;
    }
    @Override
    public String toString(){
        return message;
    }
}
