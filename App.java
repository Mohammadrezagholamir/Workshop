package models;
/*
Explanation:
- In out app, we need somewhere to keep our general data like list of users and list of groups and logged-in user etc.
- This class is the place for that.
- Put your general data here and use them in your code.
- you should put some functions here to manage your data too.
 */

import java.util.ArrayList;
import models.enums.*;
import views.AppView;

public class App {
    public static ArrayList<User> users = new ArrayList<>();
    public static ArrayList<Group> groups = new ArrayList<>();
    private static Menu currentMenu = Menu.SignUpMenu;
    private static User loggedIn = null;
    public static Menu getMenu(){
        return currentMenu;
    }
    public static void setMenu(Menu current){
        App.currentMenu = current;
    }
    public static User getLoggedIn(){
        return loggedIn;
    }
    public static void setLoggedIn(User khar){
        App.loggedIn = khar;
    }



}
