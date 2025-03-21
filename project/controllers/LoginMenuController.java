package controllers;
import java.util.ArrayList;
import models.*;
import models.Result;
/*
Explanation:
- This is a controller class for the login menu Controller.
- This class will be used to implement functions that do log in menu operations.
- notice that this class should not have any input and output and just use it to implement functionalities.
 */
import models.enums.Menu;




public class LoginMenuController {
    public User findUser(String Foalni , ArrayList<User> users){
        for(User alaki : users){
            if(alaki.getUsername().equals(Foalni)){
                return alaki;
            }
        }
        return null;
    }
    public boolean isCorrectPassword(String password , String Folani , ArrayList<User> users){
        User alaki = findUser(Folani, users);
        if(alaki.getPassword().equals(password)){
            return true;
        }
        return false;
    }
    public boolean isCorrectEmail(String email , String Folani , ArrayList<User> users){
        User alaki = findUser(Folani, users);
        if(alaki.getEmail().equals(email)) return true;
        return false;
    }
    public Result login(String Username , String Password , ArrayList<User> users){
        if(findUser(Username, App.users) == null){
            return new Result(false, "username doesn't exist!");

        }
        if(!isCorrectPassword(Password, Username, App.users)){
            return new Result(false , "password is incorrect!");
        }
        User loggedUser = findUser(Username, users);
        App.setLoggedIn(loggedUser);
        App.setMenu(Menu.Dashboard);
        return new Result(true, "user logged in successfully.you are now in dashboard!");
        
    }
    public Result goToSignUp(){
        App.setMenu(Menu.SignUpMenu);
        return new Result(true, "you are now in signup menu!");
    }
    public Result ForotPassword( String email , String username){
        if(findUser(username, App.users )==null){
            return new Result(false, "username doesn't exist!");
        }
        if(!(isCorrectEmail(email, username, App.users))){
            return new Result(false, "email doesn't match!");
        }
        return new Result(true , "password : " + findUser(username, App.users).getPassword());
    }
    public Result goToLogin(){
        return new Result(true, "you are now in login menu!");
    }
}
