package views;
import java.util.Scanner;

import models.App;
import models.enums.*;

/*
Explanation:
- This is a view class for the App.
- our app needs a place that handle menus (:
 */

public class AppView {
    public void run(){
        Scanner scanner = new Scanner(System.in);
        do{
            App.getMenu().CheckCommand(scanner);
        } while(App.getMenu()!=Menu.ExitMenu);
    }
}
