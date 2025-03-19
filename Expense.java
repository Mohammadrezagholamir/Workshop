package models;
import models.*;
/*
Explanation:
- when we create an expense, we need to store some information about it.
- Expense is something that we need to make it an object.
- put those information here and use them in your code.
 */

public class Expense{
    private int value;
    private User talabkar;
    private User bedehkar;
    private int groupId;
    public Expense(int value , User talabkar , User bedehkar , int groupId){
        this.bedehkar = bedehkar;
        this.talabkar = talabkar;
        this.value = value;
        this.groupId = groupId;
    }
    public User getTalabkar(){
        return talabkar;
    }
    public User getBedehkar(){
        return bedehkar;
    }
    public int getValue(){
        return value;
    }
    public void removeValue(){
        this.value = 0;
    }
    public void minusValue(int value){
        this.value -= value;
    }
    public int getGroupId(){
        return groupId;
    }
    public void setValue(int value){
        this.value = value;
    }
    public void setBedehkar(User bedehkar){
        this.bedehkar = bedehkar;
    }
    public void setTalabkar(User talabkar){
        this.talabkar = talabkar;
    }

}
