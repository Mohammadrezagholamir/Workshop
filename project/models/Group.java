package models;
/*
Explanation:
- In our app, we have groups that have some information.
- Group is something that we need to make it an object because it looks like an object (:
- put those information here and use them in your code.
 */

import java.util.ArrayList;

public class Group {
    private ArrayList<User> myUserGroup = new ArrayList<>();
    private String name;
    private String type;
    private User creator;
    private int groupId;
    static int id = 0;
    private ArrayList<Expense> groupExpenses = new ArrayList<>();
    public Group(String name , String type , User creator){
        this.name = name;
        this.type = type;
        this.groupId= ++id;
        this.creator = creator;
        myUserGroup.add(creator);
        creator.addToGroup(this);

    }
    public ArrayList<User> getMyUsers(){
        return myUserGroup;
    }
    public String getName(){
        return name;
    }
    public String getType(){
        return type;

        
    }
    public User getCreator(){
        return creator;
    }
    public int getGroupId(){
        return groupId;

    }
    public void setMyUsersGroup(ArrayList<User> users){
        this.myUserGroup = users;
    }
    public String toString(){
        return "group name : " + name + "\n"
        + "id : " + groupId + "\n"
        + "type : " + type + "\n"
        + "creator : " + creator.getName();
    }
    public void showMembers(){
        for(User khar : myUserGroup){
            System.out.println(khar.getName());
        }
    }
    public void addUsers(User khar){
        myUserGroup.add(khar);
    }
    public boolean existsInMe(User user){
        if(this.myUserGroup.contains(user)){
            return true;
        }
        return false;
    }
    public void addExpenseGroup(Expense expense){
        groupExpenses.add(expense);
    }
    
}
